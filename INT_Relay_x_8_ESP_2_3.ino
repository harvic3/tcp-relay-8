/*
INT Relay x 8 con WiFi
Pines que se pueden usar
0,2,4,5,10,12,13,14,15,16
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "FS.h"
#include <EEPROM.h>

#define CLOSE     HIGH
#define OPEN      LOW
#define RELAY1    0
#define RELAY2    16
#define RELAY3    10
#define RELAY4    4
#define RELAY5    13
#define RELAY6    12
#define RELAY7    14
#define RELAY8    15
#define READY     2 //Pin para habilitar la conmutación de los Relevos. LED indicador de READY
#define PINCtrl   5  //Pin para conmutar los Relays Uno a Uno (Se activa en Cero).

//Buffers de Datos
const short bufferSize = 255; // Maximum length of Buffers
char dataBuffer[bufferSize]; //buffer para datos
char replyBuffer[bufferSize]; //bufferr para respuesta
//Default Port
const unsigned int defaultPort = 23; //Puerto de conexión por defecto

//Archivo de Configuración
char fileBase[] = "{\"Net\":\"NOT_ASSIGNED\",\"Key\":\"none\",\"Ip\":\"0.0.0.0\",\"Port\":23,\"SubNet\":\"0.0.0.0\",\"GateWay\":\"0.0.0.0\",\"IpRemote\":\"0.0.0.0\",\"UserSet\":false}";

//Address EEPROM
const unsigned short relAddress = 0; //Direccion de memoria del estado de los Relays
const unsigned short portAddress = 1; //Dirección de memoria en donde se guarda el HighByte del Puerto. El LowByte es el siguiente en memoria

//General params
const String pathSettingsFile = "/Settings.json";
bool online = false;
bool userSet = false;
bool notRun = true;
short pinCounter = 0; //Variable para la posición de On/Off del PINCtrl
unsigned long timeClock = 0; 


//Network Params
IPAddress ip(0,0,0,0);
IPAddress subNet(0,0,0,0);
IPAddress gateWay(0,0,0,0);
IPAddress ipRemote(0,0,0,0);
String mac;
unsigned int port = 0; //Puerto de conexión

//WiFi Credentials
char netName[32];
char netKey[60];

//Sockets
WiFiServer server(defaultPort);

void setup() {
  //Pines
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);
  pinMode(RELAY7, OUTPUT);
  pinMode(RELAY8, OUTPUT);
  pinMode(READY, OUTPUT);
  pinMode(PINCtrl, INPUT_PULLUP);

  //Interrupción para conmutar los relevos manualmente
  attachInterrupt(digitalPinToInterrupt(PINCtrl), ChangeRelaysManual, FALLING);

  //Disable Relays
  digitalWrite(READY, HIGH);
  
  //Init Serial comm
  Serial.begin(19200);
  while (!Serial) {
    // wait serial port initialization
  }
    
  //Inicialize EEPROM
  EEPROM.begin(16);

  //Init File System
  if (!InitializeFileSystem()) {
    return;
  }
    
  //Load Network Settings
  Serial.println("");
  Serial.println("Starting configuration!");
  if (!LoadFileConfigurations()) {
    Serial.println("Failed to set Network configuration!!");
    online = false;
    return;
  }

  //Connectar a WiFi
  ConnectToNetwork();  
}

void loop() {
  //Check for Clients
  if (online){
    WiFiClient client = server.available();
    if (client) {
      Serial.println("Client connected!!");
      while (client.connected()){
        while(!client.available()){
          delay(10);
          if (!client.remotePort() > 0){
            client.stop();
            Serial.println("Client lost!!");
            return;
          }
        }
        delay(10);  
        String request = client.readStringUntil('\r');
        if (notRun){
          return;
        }
        //Verificar que si la IP remota está configurada y es diferente del que realiza la petición, que no haga nada
        if (!(ConvertIpToString(client.remoteIP()) == ConvertIpToString(ipRemote)) && ConvertIpToString(ipRemote) != "0.0.0.0"){
          return;
        }
        //Process Command
        ProccessCommand(request);
        client.print(String(replyBuffer));
        dataBuffer[0] = '\0';
        replyBuffer[0] = '\0';
      } 
    }
  }
  
  //Datos seriales
  VerifyIfDataFromSerial();
  
  //Si red no configurada No entrar
  if (String(netName) != "NOT_ASSIGNED"){
    //Reconnectar si está desconectado
    if (WiFi.status() != WL_CONNECTED){
      Serial.println("Trying to reconnect");
      //Disconnected
      online = false;
      ConnectToNetwork();
    }
  }else if (timeClock + 5000 < millis()){
    Serial.println("Network not configured!!");
    Serial.println("Send HELP for more info!!");
    timeClock = millis();    
  }
}
