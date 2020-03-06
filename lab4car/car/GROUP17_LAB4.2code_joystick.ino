#include <WiFi.h>
#include <WiFiUdp.h>

int xaxis = 34;
int yaxis = 35;
int sw = 2;

const char* ssid     = "meam510"; // create the WiFi name
const char* password = "gooooooo"; // create the WiFi password


WiFiUDP UDPTestServer; // set the WiFi to UDP mode
IPAddress myIPaddress(192,168,1,174); // Yuchen Sun's IP
IPAddress ipTarget(192, 168, 1, 165); // Shallchee Shih's IP

const int UDP_PACKET_SIZE = 20; // set packet size
char udpBuffer[UDP_PACKET_SIZE]; // set the sending packet
char packetBuffer[UDP_PACKET_SIZE]; // set the receiev packet 
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

  pinMode(xaxis,INPUT); // set the xaxis as input mode
  pinMode(yaxis,INPUT); // set the yaxis as input mode
  pinMode(sw,INPUT_PULLUP); // set the switch always high

}

/*write the sending subroutine here*/
void fncUdpSend(int number1, int number2) // number1: the value of potentiometer, number2: the switch boolean
{
  Serial.println("sending data...");
  udpBuffer[0]=number1; // read the xaxis
  udpBuffer[1]=number2; // read the yaxis
  // send the packet to the other device                                          
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  // code for debugging
  Serial.println("Sent data is:");
  Serial.printf("%d\n",udpBuffer[0]);
  Serial.printf("%d\n",udpBuffer[1]);
}

void loop() {
  int joystick[2]; // read the value of joystick
  // code for debugging
  Serial.println("The x analog");
  Serial.printf("%d\n",analogRead(xaxis));
  Serial.println("The y analog");
  Serial.printf("%d\n",analogRead(yaxis));
  // get the right value from the joystick
  joystick[0] = map(analogRead(xaxis),0,4095,0,255);
  joystick[1] = map(analogRead(yaxis),0,4095,0,255);
  fncUdpSend(joystick[0],joystick[1]); // send the analog value to the car
  delay(10); // delay for working
}
