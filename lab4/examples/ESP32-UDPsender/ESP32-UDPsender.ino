/*
 * sends a packet to esp32-udpreceiver 
 * 
 */
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "nintendoSwitch";
const char* password = "12Switch";

WiFiServer server(80);

IPAddress ipLocal(192, 168, 1, 174);  // change to your local esp32 IP ADDRESS

// set up sender
WiFiUDP UDPTestServer;
unsigned int udpTargetPort=2801;
const int SEND_PACKET_SIZE = 20;
char SendBuffer[SEND_PACKET_SIZE];
IPAddress ipTarget(192, 168, 1, 165); // change to your target esp32 IP ADDRESS

// set up reciever
unsigned int udpLocalPORT = 2808;
const int RECIEVE_PACKET_SIZE = 20;
byte packetBuffer[RECIEVE_PACKET_SIZE+1];

void setup() {
  Serial.begin(115200);  
  pinMode(2,OUTPUT);
  Serial.print("Connecting to ");  Serial.println(ssid);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  delay(100);
  
  WiFi.softAPConfig(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  server.begin();
  WiFi.begin(ssid, password);

  UDPTestServer.begin(udpLocalPORT); // strange bug needs to come after WiFi.begin but before connect
  
//  while(WiFi.status()!=WL_CONNECTED){
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
}

// send udp packets periodically                                                         
void loop() {
  handleUDPServer();
  delay(1);
  
  delay (2000);
  digitalWrite(2, LOW); // debugging LED pulses                                     
  delay (1000);
  digitalWrite(2, HIGH);
  fncUdpSend();
}

void handleUDPServer(){
  int cb = UDPTestServer.parsePacket(); //read data (check to see the number of the packet)
  if (cb){
    UDPTestServer.read(packetBuffer,RECIEVE_PACKET_SIZE);
    Serial.print("Message recieved:");
    Serial.printf("%s\n",packetBuffer);
  }
}

void fncUdpSend()
{
  strcpy(SendBuffer,"hello"); // send anything up to 48 char     
//  Serial.print("hello testing message") ;                           
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",SendBuffer);
  UDPTestServer.endPacket();
  Serial.println("Finished sending data");
}
