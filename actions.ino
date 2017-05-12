/*
Funciones de acciones en los puertos de salida
 */

//Función para cambiar manualmente los relevos uno a uno o todos en ciclo.
void ChangeRelaysManual()
{    
  OpenAllOuts();
  pinCounter++;
  if (pinCounter < 9) {
    switch(pinCounter){
      case 1:
        digitalWrite(RELAY1, CLOSE);
        EEPROM.write(1, 255);
        EEPROM.commit();   
      break;
      case 2:
        digitalWrite(RELAY2, CLOSE);
        EEPROM.write(2, 255);
        EEPROM.commit();   
      break;
      case 3:
        digitalWrite(RELAY3, CLOSE);
        EEPROM.write(3, 255);
        EEPROM.commit();   
      break;
      case 4:
        digitalWrite(RELAY4, CLOSE);
        EEPROM.write(4, 255);
        EEPROM.commit();   
      break;
      case 5:
        digitalWrite(RELAY5, CLOSE);
        EEPROM.write(5, 255);
        EEPROM.commit();   
      break;
      case 6:
        digitalWrite(RELAY6, CLOSE);
        EEPROM.write(6, 255);
        EEPROM.commit();   
      break;
      case 7:
        digitalWrite(RELAY7, CLOSE);
        EEPROM.write(7, 255);
        EEPROM.commit();   
      break;
      case 8:
        digitalWrite(RELAY8, CLOSE);
        EEPROM.write(8, 255);
        EEPROM.commit();   
      break;      
    }
  } else {
    pinCounter = 0;
    OpenAllOuts();
  }
  delay(5000);
  //Reset de Parámetros de RED. Queda en DHCP con puerto 23
  while(digitalRead(PINCtrl) == LOW){  
    RestoreSystem();
  }
}

//Función para abrir todos los relay
void OpenAllOuts(){
  digitalWrite(RELAY1, OPEN);
  digitalWrite(RELAY2, OPEN);
  digitalWrite(RELAY3, OPEN);
  digitalWrite(RELAY4, OPEN);
  digitalWrite(RELAY5, OPEN);
  digitalWrite(RELAY6, OPEN);
  digitalWrite(RELAY7, OPEN);
  digitalWrite(RELAY8, OPEN);
  EEPROM.write(1, 0);
  EEPROM.write(2, 0);
  EEPROM.write(3, 0);
  EEPROM.write(4, 0);
  EEPROM.write(5, 0);
  EEPROM.write(6, 0);
  EEPROM.write(7, 0);
  EEPROM.write(8, 0);
  EEPROM.commit();
}

//Función para procesar los datos del Cliente WiFi
void ProccessCommand(String command){  
  short i = 2;
  int relNum = 0;
  char relAction[1];
  relAction[0] = 'N';
  if (notRun){
    return;  
  }
  if (command[0] == '@' && command[1] == ':' && !notRun) {
    //Iteramos hasta encontrar el fin de cadena
    while (relAction[0] != '#') {
      relNum = int(command[i + 1]) - 48; //Cargamos Numero de Relay
      relAction[0] = command[i]; //Cargamos Acción para Relay: o = Open, c = Close
      i = i + 3;
      switch (relNum) {
        case 1://Relay 1
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT1\r");
            digitalWrite(RELAY1, CLOSE);
            SaveStateInMemory(1, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT1\r");
              digitalWrite(RELAY1, OPEN);
              SaveStateInMemory(1, false);
            }
          }
          break;

          case 2://Relay 2
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT2\r");
            digitalWrite(RELAY2, CLOSE);
            SaveStateInMemory(2, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT2\r");
              digitalWrite(RELAY2, OPEN);
              SaveStateInMemory(2, false);
            }
          }
          break;

        case 3://Relay 3
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT3\r");
            digitalWrite(RELAY3, CLOSE);
            SaveStateInMemory(3, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT3\r");
              digitalWrite(RELAY3, OPEN);
              SaveStateInMemory(3, false);
            }
          }
          break;

        case 4://Relay 4
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT4\r");
            digitalWrite(RELAY4, CLOSE);
            SaveStateInMemory(4, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT4\r");
              digitalWrite(RELAY4, OPEN);
              SaveStateInMemory(4, false);
            }
          }
          break;

        case 5://Relay 5
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT5\r");
            digitalWrite(RELAY5, CLOSE);
            SaveStateInMemory(5, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT5\r");
              digitalWrite(RELAY5, OPEN);
              SaveStateInMemory(5, false);
            }
          }
          break;

        case 6://Relay 6
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT6\r");
            digitalWrite(RELAY6, CLOSE);
            SaveStateInMemory(6, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT6\r");
              digitalWrite(RELAY6, OPEN);
              SaveStateInMemory(6, false);
            }
          }
          break;

        case 7://Relay 7
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT7\r");
            digitalWrite(RELAY7, CLOSE);
            SaveStateInMemory(7, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT7\r");
              digitalWrite(RELAY7, OPEN);
              SaveStateInMemory(7, false);
            }
          }
          break;

        case 8://Relay 8
          if (relAction[0] == 'c') {
            strcat(replyBuffer, "@CLOSE-OUT8\r");
            digitalWrite(RELAY8, CLOSE);
            SaveStateInMemory(8, true);
          }
          else {
            if (relAction[0] == 'o') {
              strcat(replyBuffer, "@OPEN-OUT8\r");
              digitalWrite(RELAY8, OPEN);
              SaveStateInMemory(8, false);
            }
          }
          break;
      }//Fin Case
    }//Fin While    
  }else{
    //Enviaron petición de Estados, Configuración o un Comando desconocido
    if (command == "@REQUEST") {
      bool action;
      replyBuffer[0] = '\0';
      action = digitalRead(RELAY1);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT1\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT1\r");
      }
      action = digitalRead(RELAY2);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT2\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT2\r");
      }
      action = digitalRead(RELAY3);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT3\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT3\r");
      }
      action = digitalRead(RELAY4);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT4\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT4\r");
      }
      action = digitalRead(RELAY5);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT5\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT5\r");
      }
      action = digitalRead(RELAY6);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT6\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT6\r");
      }
      action = digitalRead(RELAY7);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT7\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT7\r");
      }
      action = digitalRead(RELAY8);
      if (action) {
        strcat(replyBuffer, "@CLOSE-OUT8\r");
      }
      else {
        strcat(replyBuffer, "@OPEN-OUT8\r");
      }
      return;
    }
    else {      
      strcat(replyBuffer, "@STR-UNKNOW\r"); //Comando desconocido      
      return;
    }    
  }  
}
