/*
Funciones para procesar datos
 */

//Función para comprobar si hay datos seriales
void VerifyIfDataFromSerial(){
  if (Serial.available()){
    delay(50);
    ReadSerialData();
  }
}

//Función para conecctar a la Red ya sea al inicio o por reconexión
void ConnectToNetwork(){
  //Apagar led de Ready
  digitalWrite(READY, HIGH);

  //Detener servidor
  server.stop();

  //Configurar Parámetros de Red si están definidos por usuario y sistema 
  if ((gateWay[0] != 0 || gateWay[1] != 0 != gateWay[2] != 0 || gateWay[3] != 0) && (subNet[0] != 0 || subNet[1] != 0 != subNet[2] != 0 || subNet[3] != 0) && (ip[0] != 0 || ip[1] != 0 != ip[2] != 0 || ip[3] != 0)){
    if (IpValid(ConvertIpToString(ip)) && IpValid(ConvertIpToString(gateWay)) && IpValid(ConvertIpToString(subNet))){
      WiFi.config(ip, gateWay, subNet);
    }
  }
     
  //Config and init Wifi Module
  WiFi.begin(netName, netKey);
  
  //Connecting WiFi
  Serial.print("\nConnecting to " + String(netName));
  while (WiFi.status() != WL_CONNECTED){    
    delay(500);
    Serial.print(".");
    if (pinCounter == 10){
      Serial.println("");
      Serial.println("\nFailed connecting to network!!");
      pinCounter = 0;
      return;
    }
    //Verify data from Serial
    VerifyIfDataFromSerial();
    pinCounter++;
  }

  pinCounter = 0;
  
  //Connected
  online = true;
  Serial.println("");
  Serial.println("Connected to NetWork!!");

  //Set Port
  if (port == 0){
    port = defaultPort;
  }

  //Reiniciar puerto del Servidor
  server.setPort(port);
  //Start Server
  server.begin();
  
  ip = WiFi.localIP();  
  subNet = WiFi.subnetMask();
  gateWay = WiFi.gatewayIP();
  mac = WiFi.macAddress();

  PrintNetParams();

  //Security params
  //Serial.println("Mac: " + mac);
  //Serial.println(String(ESP.getChipId()));
  if (mac == "18:FE:34:E1:28:A0"){    
    notRun = false;
  }

  //Iniciar Relays desde el estado en EEPROM
  GetStatesFromMemory();
  
  //Si este LED enciende, está listo
  digitalWrite(READY, LOW);
}

//Función para convertir una IP en String
String ConvertIpToString(IPAddress ipAd){
  String ipCadena = String(ipAd[0], DEC) + "." + String(ipAd[1], DEC) + "." + String(ipAd[2], DEC) + "." + String(ipAd[3], DEC);
  return ipCadena;
}

//Función para obtener una dirección Ip de una cadena
IPAddress ConvertToIpAddress(String address) {
  char charT;
  short posI = 0;
  byte numDir = 1;
  IPAddress ipT;
  for (short u = 0; u < address.length() - 1; u++) {
    charT = address.charAt(u);
    if (charT == '.') {
      switch (numDir) {
        case 1:
          ipT[0] = address.substring(posI, u).toInt();
          break;
        case 2:
          ipT[1] = address.substring(posI, u).toInt();
          break;
        case 3:
          ipT[2] = address.substring(posI, u).toInt();
          posI = u + 1;
          ipT[3] = address.substring(posI).toInt();
          u = address.length();
          break;
      }
      posI = u + 1;
      numDir++;
    }
  }
  return ipT;
}

//Función para capturar los datos seriales
void ReadSerialData(){
  short dataLenght = Serial.available();
  char tBuffer[dataLenght + 1];
  tBuffer[0] = '\0';
  for (short i = 0; i < dataLenght; i++){
    tBuffer[i] = Serial.read();
  }
  //Verificar si es para recibir varios paquetes
  if (tBuffer[0] == '@' && tBuffer[1] == '#' && tBuffer[2] == '@' && tBuffer[3] == ':'){    
    //Se van a recibir varios paquetes
    short packages = String(tBuffer[4]).toInt();
    int sizeData = (String(tBuffer).substring(6, dataLenght - 1)).toInt();
    //Confirmar cuántos y el tamaño de lo que se va a recibir
    Serial.println("OK-" + String(packages) + "-" + String(sizeData));   
    //Leer paquetes
    short counter = 0;
    bool flag = true;
    while (flag){
      if (Serial.available() > 0){
        delay(100);
        dataLenght = Serial.available();
        tBuffer[0] = '\0';
        for (short i = 0; i < dataLenght; i++){
          tBuffer[i] = Serial.read();
        }
        tBuffer[dataLenght - 1] = '\0';
        strcat(dataBuffer, tBuffer);
        //Controlar variables y banderas
        counter++;
        //Responder al emisor
        Serial.println("GO-" + String(counter) + "-" + String(dataLenght));
        //Verificar para ver si deben enviar de nuevo
        if(counter == packages){
          bool verify = false;          
          //Enviar data para verificar con el Emisor
          Serial.println(String(dataBuffer));
          //Esperar para verificar la respuesta
          while (!verify){
            if (Serial.available() > 0){
              delay(10);
              dataLenght = Serial.available();
              tBuffer[0] = '\0';
              for (int i = 0; i < dataLenght; i++){
                tBuffer[i] = Serial.read();
              }
              tBuffer[dataLenght - 1] = '\0';
              //Si la respuesta es positiva terminar transferencia, de lo contrario volver a empezar.
              if (String(tBuffer) == "OKPASS"){
                //Resetear banderass de los dos ciclos While para salir
                flag = false;
                verify = true;
                Serial.println("OKEND");
              }else if (String(tBuffer) == "NOPASS"){
                //Volver contador a cero y bandera de verificación
                counter = 0;         
                verify = true;
                dataBuffer[0] = '\0';
                Serial.println("OK-" + String(packages) + "-" + String(sizeData));
              }
            }
          }           
        }        
      }    
    }
    //Aquí ya se puede procesar el archivo JSON
    SaveFile(dataBuffer, false);
    dataBuffer[0] = '\0';
  }else{
    //Aquí otros comandos Seriales RESET, RESTART, ETC
    //Limpiamos la cadena de basura   
    for (short x = 0; x <= dataLenght; x++){      
      if (tBuffer[x] == '\r' || tBuffer[x] == '\n'){
        tBuffer[x] = '\0';
        x = dataLenght + 1;
      }
    }
    String command = String(tBuffer);    
    command.toUpperCase();
    if(command == "SETSSID"){ 
      String param = SetDataTypeString("\nPlease send the SSID for the network!");
      param.toCharArray(netName, param.length() + 1);
      SaveConfigurations();
      Serial.println("\nPlease reboot device!!");
    }else if(command == "SETPASS"){
      String param = SetDataTypeString("\nPlease send the PASSWORD for the network!");
      param.toCharArray(netKey, param.length() + 1);
      SaveConfigurations();
      Serial.println("\nPlease reboot device!!");
    }else if(command == "SEEIPREMOTE"){
      SeeParameter("Ip of remote client", ConvertIpToString(ipRemote));
    }else if(command == "SETIPREMOTE"){
      ipRemote = SetDataTypeIpAdress("\nPlease send the IP for the server!");
      SaveConfigurations();
    }else if(command == "SEEIP"){
      SeeParameter("IP of device", ConvertIpToString(ip));
    }else if(command == "SETIP"){
      ip = SetDataTypeIpAdress("\nPlease send the IP for this device!");
      SaveConfigurations();
      Serial.println("\nPlease reboot device!!");
    }else if(command == "SEEPORT"){
      SeeParameter("communication Port of device", String(port));
    }else if(command == "SETPORT"){
      if (SetPort()){
        SaveConfigurations();
        Serial.println("\nPlease reboot device!!");        
      }
    }else if(command == "SEENETPARAMS"){
      PrintNetParams();
    }else if(command == "RESTORE"){
      RestoreSystem();
    }else if(command == "REBOOT"){
      Serial.println("\nRebooting device!");
      ESP.reset();
    }else if(command == "SEEWIFISTATE"){
      Serial.println("\nWiFi state is: ");
      WiFi.printDiag(Serial);
    }else{
      SendHelpData();      
    }
    Serial.flush();
  }
}
