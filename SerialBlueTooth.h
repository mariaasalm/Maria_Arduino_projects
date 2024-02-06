#include <string.h>
#include "BluetoothSerial.h"
#include <string>

BluetoothSerial SerialBT;
const static int SSIDADDRESS = 0;
const static int PASSADDRESS = 40;
const static int DVIDADDRESS = 110;
const static int SSIDSIZE = 32;
const static int PASSSIZE = 64;
const static int DVIDSIZE = 10;
String esid, epid, edid;
int BT_init();

void read_EEPROM(String &esid, String &epid, String &edid) {
  // Read eeprom for ssid and pass and dvid
  char c = 0;
  int id = SSIDADDRESS;
  do {
    c = EEPROM.read(id);
    esid += c;
    id++;
  } while ((id < SSIDADDRESS + SSIDSIZE) && (c != 0));
  id = PASSADDRESS;
  do {
    c = EEPROM.read(id);
    epid += c;
    id++;
  } while ((id < PASSADDRESS + PASSSIZE) && (c != 0));
  id = DVIDADDRESS;
  do {
    c = EEPROM.read(id);
    edid += c;
    id++;
  } while ((id < DVIDADDRESS + DVIDSIZE) && (c != 0));
}

//SSID maximum size = 32 characters
//SSID starting address = 0
void updateSSID(String &ssidName) {
  Serial.println("updateSSID("+ssidName+")....... is called.");
  int i = 0;
  if (ssidName.length()) {  // If ssidName is not empty
    do {
      EEPROM.write(SSIDADDRESS + i, ssidName[i]);
      i++;
    } while ((i < 32) && (ssidName[i] != 0));
  }
  EEPROM.write(SSIDADDRESS + i, '\0');
  EEPROM.commit();
}

//Password maximum size = 64 characters
//Password starting address = 40
void updatePASS(String &password) {
  Serial.println("updatePASS("+password+")....... is called.");
  int i = 0;
  if (password.length()) {  // If ssidName is not empty
    do {
      EEPROM.write(PASSADDRESS + i, password[i]);
      i++;
    } while ((i < PASSSIZE) && (password[i] != 0));
  }
  EEPROM.write(PASSADDRESS + i, '\0');
  EEPROM.commit();
}

//DEVID maximum size = 10 characters
//DEVID starting address = 110
void updateDVID(String &deviceID) {
  Serial.println("updateDVID("+deviceID+")....... is called.");
  int i = 0;
  if (deviceID.length()) {  // If ssidName is not empty
    do {
      EEPROM.write(DVIDADDRESS + i, deviceID[i]);
      i++;
    } while ((i < DVIDSIZE) && (deviceID[i] != 0));
  }
  EEPROM.write(DVIDADDRESS + i, '\0');
  EEPROM.commit();
}


void updateAll(String &commands)
{
  Serial.println("updateAll("+commands+")....... is called.");
  int start_command = 0;
  int end_command = commands.indexOf(";",start_command);
  delay(200);
  Serial.println("end_command = "+end_command);
  delay(200);
/*  while (end_command > 0)
  {
    String str = commands.substring(start_command, end_command);
    int index = str.indexOf(':');
    String command = (str.substring(0, index + 1)).toLowerCase();
    String value = str.substring(index + 1, str.length());
    switch(command)
    {
      case "ssid:":
        updateSSID(value);
        break;
      case "pass:":
        updatePASS(value);
        break;
      case "dvid:":
        updateDVID(value);
        break;
      case "restart:":
        ESP.restart();
        break;
      default:
        esid = epid = edid = '\0';
        read_EEPROM(esid, epid, edid);
        Serial.println("SSID:" + esid + "\tPASSWORD:" + epid + "\tDeviceID:" + edid);
    }
    start_command = end_command + 1;
    end_command = commands.indexOf(";",start_command);
  }*/
}

int WriteEEPROM() {
  int result = 0;
  if (SerialBT.available()) {
    String str = SerialBT.readString();
    int index = str.indexOf(':');
    String command = str.substring(0, index + 1);
    String value = str.substring(index + 1, str.length());
    command.toLowerCase();
    Serial.println("Command="+command+" --> value="+value );
    /*if(command.compareTo("all")){
      updateAll(value);
    } else */
    if (command.compareTo("ssid:") == 0) {
      updateSSID(value);
    } else if (command.compareTo("pass:") == 0) {
      updatePASS(value);
    } else if (command.compareTo("dvid:") == 0) {
      updateDVID(value);
    } else if (command.compareTo("restart:") == 0) {
      ESP.restart();
    } else {
      str = command + ":" + value + " is not a valid command";
      SerialBT.write(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
      Serial.println(str);
    }
    esid = epid = edid = '\0';
    read_EEPROM(esid, epid, edid);
    str = "SSID:" + esid + "\nPASSWORD:" + epid + "\nDeviceID:" + edid;
    SerialBT.write(reinterpret_cast<const uint8_t *>(str.c_str()), str.length());
    Serial.println(str);
  }
  return result;
}

int BT_Init() {
  int result = 0;
  SerialBT.begin("SAFERH_Wrist_Band");  //Bluetooth device name
  Serial.println("BT has been initialized using device name SAFERH_Wrist_Band, ssid and password can be updated using BT");
  EEPROM.begin(512);
  delay(500);
  Serial.println("Use following commands \nssid:ssidname \npass:password \ndvid:device_name \nrestart: \nprint:\n");
  while (result != 1) {
    result = WriteEEPROM();
  }
  Serial.println("\n Writing done on EEPROM:" + result);
  return result;
}