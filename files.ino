/*
Funciones de Acceso a Archivos
 */

//Función para iniciar el sistema de archivos
bool InitializeFileSystem(){
  if (!SPIFFS.begin()) {    
    Serial.println("");
    Serial.println("Failed to mount file system!");
    return false;
  }
  Serial.println("");
  return true;
}

//Formatear File System
bool FormatFileSystem(){
  Serial.println("Formating file system. Please wait!");
  if (!SPIFFS.format()){
    Serial.println("");
    Serial.println("Error to format File System!!");
    return false;
  }
  Serial.println("File system formated!");
  return true;
}

//Función para abrir un archivo
bool OpenFile(String path, String &data){
  File configFile = SPIFFS.open(path, "r");
  if (configFile) {
    Serial.println("Loading file. Please Wait!");
    data = configFile.readStringUntil('\r');
    configFile.close();
    return true;    
  }
  configFile.close();
  return false;
}

//Función para crear un archivo
bool CreateFile(String path, String data){
  File configFile = SPIFFS.open(path, "w+");
  if (configFile) {
    Serial.println("Creating file. Please Wait!");
    configFile.println(data);
    configFile.close();
    String net = "NOT_ASSIGNED";
    net.toCharArray(netName, net.length() + 1);
    return true;      
  }
  configFile.close();
  return false;
}

//Función para guardar el archivo de configuración del sistema
bool SaveFile(String data, bool formatFS) {
  //Init File System
  if (formatFS){
    if (!FormatFileSystem()){
      return false;  
    }    
    if (!InitializeFileSystem()) {      
      return false;
    }
  }
  
  //Abrir Archivo
  if (!CreateFile(pathSettingsFile, data)){
    Serial.println("Failed to create to configuration file!!");    
  }else{
    return true;
  }    
}

//Función para Cargar la configuración Inicial
bool LoadFileConfigurations() {  
  String data;
  //Abrir Archivo
  if (!OpenFile(pathSettingsFile, data)){
    Serial.println("Failed to open configuration file!");
    Serial.println("Creating configuration file. Please Wait!");
    SaveFile(String(fileBase), true);
    return false;
  } 
    
  //Parse and get data
  size_t sizeA = data.length();
  //OJO El archivo no puede ser más grande de 255 bytes
  if (sizeA > 255){
    Serial.println("Error: the configuration file is very large. Maximum 255 characters!!");
    return false;
  }
  
  StaticJsonBuffer<255> jsonBuffer;
  char fileR[sizeA + 2];
  data.toCharArray(fileR, sizeA + 1); 
  
  JsonObject& json = jsonBuffer.parseObject(fileR);
  if (!json.success()) {
    Serial.println("Error processing configuration file!");
    Serial.println("Rewriting configuration file. Please Wait!");      
    SaveFile(String(fileBase), true);
    return false;
  }
  
  String net = json["Net"];    
  String key = json["Key"];
  net.toCharArray(netName, net.length() + 1);
  key.toCharArray(netKey, key.length() + 1);
  ip = ConvertToIpAddress(json["Ip"]);
  subNet = ConvertToIpAddress(json["SubNet"]);
  gateWay = ConvertToIpAddress(json["GateWay"]);
  ipRemote = ConvertToIpAddress(json["IpRemote"]);
  port = json["Port"];
  userSet = bool(json["UserSet"]);
  
  Serial.println("Configuration file loaded!!");
  return true;
}

//Función para Cargar la configuración Inicial
void SaveConfigurations() {  
  StaticJsonBuffer<255> jsonBuffer;  
  JsonObject& json = jsonBuffer.createObject();
  
  json["Net"] = String(netName);
  json["Key"] = String(netKey);
  json["Ip"] = ConvertIpToString(ip);
  json["SubNet"] = ConvertIpToString(subNet);
  json["GateWay"] = ConvertIpToString(gateWay);
  json["IpRemote"] = ConvertIpToString(ipRemote);
  json["Port"] = String(port);
  json["UserSet"] = "false";
  
  String data;
  json.printTo(data);
  SaveFile(data, false);
  Serial.println("Parameter(s) loaded!");
}
