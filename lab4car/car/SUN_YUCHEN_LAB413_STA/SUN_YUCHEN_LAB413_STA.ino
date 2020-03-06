#include <WiFi.h>
#include <WiFiUdp.h>

int xaxis = 34;
int yaxis = 35;

const char* ssid     = "meam510"; // create the WiFi name
const char* password = "gooooooo"; // create the WiFi password


WiFiUDP UDPTestServer; // set the WiFi to UDP mode
IPAddress myIPaddress(192,168,1,174); // Yuchen Sun's IP
IPAddress ipTarget(192, 168, 1, 165); // Shallchee Shih's IP

const int UDP_PACKET_SIZE = 20; // set packet size
char udpBuffer[UDP_PACKET_SIZE]; // set the sending packet
char packetBuffer[UDP_PACKET_SIZE]; // set the receiev packet 
char receive_number[UDP_PACKET_SIZE]; // set the recieve number packet
WiFiServer server(80); 
unsigned int udpTargetPort=2801; // create the target port

void setup() {
  //put your setup code here, to run once
  // set the Wifi as Station mode
  Serial.begin(115200);
  // WiFi address configuration
  Serial.println("Connecting to ");  Serial.println(ssid); 
  WiFi.config(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
  WiFi.begin(ssid, password);
  server.begin();
  UDPTestServer.begin(udpTargetPort); // UDP begin

  while (WiFi.status() != WL_CONNECTED) // wait until the Wifi is connected
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  packetBuffer[UDP_PACKET_SIZE] = 0; // initialize the recieve packet
}

int handleUDPServer() 
{
  // write the recieve packet here
  int cb = UDPTestServer.parsePacket(); // a boolean to determine whether there is a sending packet.
  if (cb)
  { 
    // recieve the sending packet
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE); 
    receive_number[0] = packetBuffer[0];
    return receive_number[0];
  }
}

void fncUdpSend(int number1, int number2) // number1: the value of potentiometer, number2: the switch boolean
{
  // write the sending subroutine here
  Serial.println("sending data...");
  udpBuffer[0]=number1; // if the botton is pressed, left shift number2
  udpBuffer[1]=number2;
  // send the packet to the other device                                          
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  Serial.println("Sent data is:");
  Serial.printf("%d\n",udpBuffer[0]);
  Serial.printf("%d\n",udpBuffer[1]);
}

void loop() {
  int recieve_speed;  // recieve pwm
  int val_1; // read the value of potentiometer
  int val_2; 
  recieve_speed = handleUDPServer();
  val_1= map(analogRead(xaxis),0,4095,0,1023);
  val_2 = map(analogRead(yaxis),0,4095,0,1023);
  fncUdpSend(val_1,val_2);
  delay(10);
  Serial.printf("%d\n",recieve_speed);
  delay(30);
}