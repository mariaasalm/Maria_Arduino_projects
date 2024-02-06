

#define BUTTON_PIN       15  // GIOP21 pin connected to button
#define LED_PIN       16
#define SHORT_PRESS_TIME 1000 // 500 milliseconds


// Variables will change:
int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

void LongPB_init(){
   pinMode(LED_PIN, OUTPUT);
   pinMode(BUTTON_PIN, INPUT_PULLUP);
    lastState=digitalRead(BUTTON_PIN);
}

void LED_blink(){
  digitalWrite(LED_PIN, HIGH); // turn the LED on
  delay(500);             // wait for 500 milliseconds
  digitalWrite(LED_PIN, LOW);  // turn the LED off
  delay(500);             
}

int LongPB_setup(){
   // read the state of the switch/button:
  int bt_write=0;   
  currentState = digitalRead(BUTTON_PIN);
  if (lastState == HIGH && currentState == LOW)       // button is pressed
    pressedTime = millis();
  else if (lastState == LOW && currentState == HIGH) { // button is released
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;
    if ( pressDuration > SHORT_PRESS_TIME ){
      Serial.println("\n A long press is detected.");
      LED_blink();
      delay(100);
      // while(bt_write!=1){
      bt_write=BT_init();
      if(bt_write==1){
        Serial.print("\nEEPROM Write done SUCCESFULLY ,device restart\n");
        ESP.restart();
      }
      else
      Serial.print("\nWrite is not done\n");

    }
    else
    Serial.println("A short press is detected");
  }

  // save the the last state
  lastState = currentState;
  return bt_write;

}
