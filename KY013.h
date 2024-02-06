#include <math.h>
int sensorPin = 32; // Select the input pin for the potentiometer
int sensorValue = 0; // varibale to store the value coming form the sonsor
int lastValue = 0;
int counter = 0;



void temp_ky013() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(sensorPin);
  if( sensorValue == lastValue)
  {
    Serial.println("No temperature change.");
  }
  else if(sensorValue < lastValue) {
    Serial.print("temprature rises. Value = ");
    Serial.println(sensorValue);
  }
  else if(sensorValue > lastValue) {
    if (counter == 0){
      Serial.println("Start");
    }
    Serial.print("temprature dropped. Value = ");
    Serial.println(sensorValue);
  }
  counter++;
  lastValue = sensorValue;
  delay(5000); 

}



double Thermistor(int RawADC) {
  Serial.println("RawADC = "+String(RawADC));
  double Temp;
  Temp = log(10000.0*(1024.0/(RawADC-1))); 
  Serial.println("Temp1 = "+String(Temp));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Serial.println("Temp2 = "+String(Temp));
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  Serial.println("Temp3 = "+String(Temp));
   //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return Temp;
}
 
 String read_body_temp(){

 int readVal = analogRead(sensorPin);
 Serial.println("ReadVal = "+String(readVal));  // display tempature
 double temp =  Thermistor(readVal);
 Serial.println(temp);  
 delay(500);// display tempature
 return String(temp);
 
}

