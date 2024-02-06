
#include <EEPROM.h>
#include "SerialBlueTooth.h"
#include "LongPressButton.h"
#include "index.h"
#include"KY013.h"
#include"MAX3010.h"
#include"DHT22.h"

#include "wifi.h"
 



void setup() {
  Serial.begin(9600);
  Serial.println("Initializing .... ");
  if (!wifi_init())
    BT_Init();
    setupMax3010();
}

void loop() {
  // server.handleClient();
  // setupDHT22();
  // printDHT22();
  String bpm= read_BPM();
  Serial.println(bpm);
  delay(500);
}
