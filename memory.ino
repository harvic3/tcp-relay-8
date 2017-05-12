/*
Funciones para acceso a memoria
 */

//Función para guardar los estados de las salidas en memoria
void SaveStateInMemory(short relay, bool state){
  if (state){
    EEPROM.write(relay, 255);
  }else{
    EEPROM.write(relay, 0);
  }
  EEPROM.commit();
}

//Función para recuperar estados desde la EEPROM
void GetStatesFromMemory(){
  byte state = 0;
  state = VerifyDataFromMemory(EEPROM.read(1));
  if (state){
    digitalWrite(RELAY1, CLOSE);
  }else{
    digitalWrite(RELAY1, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(2));
  if (state){
    digitalWrite(RELAY2, CLOSE);
  }else{
    digitalWrite(RELAY2, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(3));
  if (state){
    digitalWrite(RELAY3, CLOSE);
  }else{
    digitalWrite(RELAY3, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(4));
  if (state){
    digitalWrite(RELAY4, CLOSE);
  }else{
    digitalWrite(RELAY4, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(5));
  if (state){
    digitalWrite(RELAY5, CLOSE);
  }else{
    digitalWrite(RELAY5, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(6));
  if (state){
    digitalWrite(RELAY6, CLOSE);
  }else{
    digitalWrite(RELAY6, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(7));
  if (state){
    digitalWrite(RELAY7, CLOSE);
  }else{
    digitalWrite(RELAY7, OPEN);
  }
  state = VerifyDataFromMemory(EEPROM.read(8));
  if (state){
    digitalWrite(RELAY8, CLOSE);
  }else{
    digitalWrite(RELAY8, OPEN);
  }
}

//Función que comprueba si la salida está activa o inactiva
bool VerifyDataFromMemory(byte state){
  if (state == 255){
    return true;
  }else{
    return false;
  }
}
