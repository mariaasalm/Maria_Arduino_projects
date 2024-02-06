#include "DHT.h"

// DHT22 Enviroment temprature and humidity sensor.
#define DHTTYPE DHT22       // DHT 22  (AM2302), AM2321
uint8_t DHTPin = 15;  //4;         // DHT Sensor
DHT dht(DHTPin, DHTTYPE);   // Initialize DHT sensor.
float EnviromentTemperature, EnviromentHumidity;  
//void setupDHT22();
//void printDHT22();
void setupDHT22(){
  pinMode(DHTPin, INPUT);
  dht.begin();
  delay(500);
  Serial.println("DHT has been initialized .....");
  Serial.print("Environment Temp :");
  Serial.println(dht.readTemperature()); 
  Serial.print("Environment Humidity :");
  Serial.println(dht.readHumidity());
}

void printDHT22(){
  Serial.print("Temparature : ");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity : ");
  Serial.println(dht.readHumidity());
EnviromentTemperature=dht.readTemperature();
EnviromentHumidity=dht.readHumidity();
  
String data = "{ \"Temperature\":\""+ String(EnviromentTemperature) +"\", \"Humidity\":\""+ String(EnviromentHumidity) +"\"}";  

}
String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
    else  if(var == "BODYTEMPERATURE"){
    return read_body_temp();
  }
  else  if(var == "BeatsPerMin"){
    return read_BPM();
  }
  return String();
}
