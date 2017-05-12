/*
Funciones de comandos seriales
 */

//Información de ayuda
void SendHelpData(){
  Serial.println(String("\nAVAILABLE COMMANDS LIST \n") +
    String("  setSSID       Request command to set the network name.\n") +
    String("  setPASS       Request command to set the network password.\n") +
    String("  seeIp         See the IP address of device.\n") +
    String("  setIP         Request command to set the IP address to device.\n") +
    String("  seeIpRemote   See the IP address of the remote host.\n") +
    String("  setIPRemote   Request command to set the IP address of the remote host.\n") +
    String("  seePort       See the communication Port of device.\n") +
    String("  setPort       Request command to set the communication Port of device.\n") +
    String("  seeNetParams  See the network parameters of device.\n") +
    String("  seeWiFiState  See the network parameters of device.\n") +
    String("  reboot        Reboot the device.\n") +
    String("  RESTORE       Request command to RESTORE the system configuration.\n"));
}

//Información de parámetros
void PrintNetParams(){
  Serial.println("\nSystem ready with \n" + 
     String("Ip: " + ConvertIpToString(ip)) + 
     String("\nSubnet Mask: " + ConvertIpToString(subNet)) + 
     String("\nGateway: " + ConvertIpToString(gateWay)) + 
     String("\nPort: " + String(port))
   );
}

//Función para capturar el parámetro como comando de configuración
String ReadSerialParam(String message){
  Serial.flush();
  Serial.println(String(message));
  while (Serial.available() == 0){
    delay(10);
  }
  delay(50);
  int dataLenght = Serial.available();
  char tBuffer[dataLenght + 1];
  for (short i = 0; i < dataLenght; i++){
    tBuffer[i] = Serial.read();
  }
  
  for (short x = 0; x <= dataLenght; x++){      
    if (tBuffer[x] == '\r' || tBuffer[x] == '\n'){
      tBuffer[x] = '\0';
      x = dataLenght + 1;
    }
  }
  return String(tBuffer);
}

//Ver algún parámetro de la configuración
void SeeParameter(String param, String value){
  Serial.println("\nThe " + param + " is " + value);
}

//Función para recibir el password de la red WiFi
String SetDataTypeString(String message){
  String param;
  param = ReadSerialParam(message);
  Serial.println("Received " + param);
  return param;
}

//Función para asignar un parámetro de tipo Numero
bool SetPort(){
  String param;
  param = ReadSerialParam("\nPlease send the number of communication Port!");
  for (byte x = 0; x < param.length(); x++){
    if (!isDigit(param.charAt(x))){
      Serial.println("Received parameter is not an integer!");
      return false;
    }
  }
  port = param.toInt();
  Serial.println("Received " + param);
  return true;
}

//Función para asignar un parámetro de tipo IP
IPAddress SetDataTypeIpAdress(String message){
  String param;
  param = ReadSerialParam(message);
  if (!IpValid(param)){
    return ip;
  }
  IPAddress ipT = ConvertToIpAddress(param);
  Serial.println("Received " + param);
  return ipT;
}

//Función para restaurar el archivo de configuración
void RestoreSystem(){
  bool flag = false;
  String param;
  while (!flag){
    param = ReadSerialParam("\nDo you want to restore the system? YES/NO");
    param.toUpperCase();
    if (param == "YES"){
      FormatFileSystem();
      String data = String(fileBase);
      CreateFile(pathSettingsFile, data);
      OpenFile(pathSettingsFile, data);
      Serial.println("\nSystem restored!!. Please reboot!!");
      flag = true;
    }else if (param == "NO"){
      Serial.println("Restore canceled!!");
      flag = true;
    }
  }  
}

//Función para validar si es Ip una cadena
bool IpValid(String ipAd){
  byte dots = 0;
  byte digits = 0;
  char field[3];
  field[0] = '\0';
  for (byte x = 0; x < ipAd.length(); x++){
    if (ipAd.charAt(x) != '.'){
      if (!isDigit(ipAd.charAt(x))){
        Serial.println("Received parameter is not a valid ip!");
        return false;
      }
      field[digits] = ipAd.charAt(x);
      digits++;      
    }else{
      if (digits > 3 || digits == 0){
        Serial.println("Received parameter is not a valid Ip!");
        return false;
      }
      if (String(field).toInt() > 255 || String(field).toInt() < 0){
        Serial.println("Received parameter is not a valid Ip!");
        return false;
      }
      dots++;      
      digits = 0;
      field[0] = '\0';
    }
  }
  if (dots != 3){
    return false;
  }
  return true;
}
