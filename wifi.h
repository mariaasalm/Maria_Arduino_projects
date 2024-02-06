#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <AsyncUDP.h>

#define CONECTION_WAIT_TIME 10000

String ssid_g = "PTCL-OMER";  // Enter SSID here
String password_g = "pakistan3535";  //Enter Password here
const char *ssid, *password;
unsigned long tryTime  = 0;
// IPAddress ipad, mask, brod;
IPAddress server_address; 
uint16_t server_port = 55600;
uint16_t register_port = 55600;
bool registered = false;
AsyncUDP udp;
AsyncUDPMessage udpMsg;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup_Wifi(String esid, String epid);
void Connect_wifi();
void handle_OnConnect() {
//  server.send(200, "text/plain", "Connection Found ... ");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
   server.on("/bodytemp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read_body_temp().c_str());
  });
   server.on("/bpm", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", read_BPM().c_str());
  });
  




}
// void handle_NotFound() {
//  server.send(404, "text/plain", "Not found");
// }

int wifi_Setup(String& esid, String& epid, String& dvid) {
  Serial.println("wifi_Setup(String& esid, String& epid, String& dvid) ... \nesid="+esid+" - epid="+epid+" - dvid="+dvid);
  WiFi.begin(esid.c_str(), epid.c_str());
  while ((WiFi.status() != WL_CONNECTED) && (tryTime < CONECTION_WAIT_TIME)) {
    tryTime = millis();
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Unable to connect to ssid="+esid+" using password="+epid);
    return 0;
  }
     
  Serial.println("MAC Address: "+WiFi.macAddress());
  Serial.println("WiFi connected..!\nIP: "+WiFi.localIP().toString());
  Serial.println("Subnet mask: "+WiFi.subnetMask().toString());
  Serial.println("Broadcast address: "+WiFi.broadcastIP().toString());
  String msg = "DVID:"+dvid+";IPIAD:"+WiFi.localIP().toString()+";";

  // server.on("/", handle_OnConnect);
  // server.onNotFound(handle_NotFound);
  handle_OnConnect();
  server.begin();
  Serial.println("HTTP server started");
  
  udpMsg.write(reinterpret_cast<const uint8_t *>(msg.c_str()), msg.length());
  Serial.println("Message is developed -- {"+msg+"}");
  // Serial.println("udp.listen(register_port) = "+udp.listen(register_port));
  // Serial.println("udp.listen(register_port) = "+udp.listenIP().toString());
  if(udp.listen(register_port)){
    udp.onPacket([](AsyncUDPPacket packet){
      Serial.print("UDP Packet Type: ");
      Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
      Serial.print(", From: ");
      Serial.print(packet.remoteIP());
      Serial.print(":");
      Serial.print(packet.remotePort());
      Serial.print(", To: ");
      Serial.print(packet.localIP());
      Serial.print(":");
      Serial.print(packet.localPort());
      Serial.print(", Length: ");
      Serial.print(packet.length());
      Serial.print(", Data: ");
      Serial.write(packet.data(), packet.length());
      Serial.println();
      String s(packet.data(), packet.length());
      if(s.equals("RHC_ACCEPTED"))
      {
        registered = true;
        server_address = packet.remoteIP();
        server_port = packet.remotePort();
        //reply to the client
        //packet.printf("Got %u bytes of data", packet.length());
        Serial.println("Node Accepted ..... ");
      }
    }); 
    while(!registered){
      Serial.println("Registeration message is sent...... ");
      udp.sendTo(udpMsg, WiFi.broadcastIP(), register_port);
      delay(2000);  
    }
    Serial.println("After broadcasting ...... ");
  }
  return 1;
}

int wifi_init() {
  esid = epid = edid = '\0';
  EEPROM.begin(512);
  read_EEPROM(esid, epid, edid);
  EEPROM.end(); 
  Serial.println("Reading EEPROM {ssid=" + esid + "--password=" + epid + "--deviceID=" + edid + ")");
  return wifi_Setup(esid, epid, edid);
}

