#include <WiFi.h>
#include <WiFiUdp.h>

int xaxis = 34;
int yaxis = 35;
int xaxis_rotate = 36;
int yaxis_rotate = 39;
int shield = 32;
int weapon=33;

int sw = 2;
//change xaxis and yaxis by changing the wires
const char* ssid     = "mariokart8"; // create the WiFi name
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
  pinMode(xaxis_rotate,INPUT); // set the xaxis as input mode
  pinMode(yaxis_rotate,INPUT); // set the yaxis as input mode
  pinMode(shield,INPUT); 
  pinMode(sw,INPUT_PULLUP); // set the switch always high

}

/*write the sending subroutine here*/
//receive the data
void fncUdpSend(int number1, int number2,int number3,int number4,int number5) // number1: the value of potentiometer, number2: the switch boolean
{
  Serial.println("sending data...");
  udpBuffer[0]=number1; 
  udpBuffer[1]=number2; 
  udpBuffer[2]=number3;
  udpBuffer[3]=number4; 
  udpBuffer[4]=number5;
  
  // send the packet to the other device                                          
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  
  // code for debugging
  Serial.println("Sent data is:");
  Serial.printf("%d\n",udpBuffer[0]);
  Serial.printf("%d\n",udpBuffer[1]);
  Serial.printf("%d\n",udpBuffer[2]);
  Serial.printf("%d\n",udpBuffer[3]);
  Serial.printf("%d\n",udpBuffer[4]);
}

void loop() {
  byte joystick[5]; // read the value of joystick
  int w1_val, w2_val, w3_val;
  float w1=0,w2=0,w3=0;
  int xaxis_val,yaxis_val;
//  Serial.print("The x analog");
//  Serial.println(analogRead(xaxis));
//  Serial.print("The y analog");
//  Serial.println(analogRead(yaxis));
  xaxis_val=analogRead(xaxis);
  yaxis_val=analogRead(yaxis);
  xaxis_val=map(xaxis_val,0,4095,-512,512);
  yaxis_val=map(yaxis_val,0,4095,-512,512);

  w1=0.7*xaxis_val;
  w2=-0.3*xaxis_val+float(1/sqrt(3))*yaxis_val;
  w3=-0.3*xaxis_val-float(1/sqrt(3))*yaxis_val;
  //map to servo's range
  w1_val= map(w1,-551,511,0,180);
  w2_val= map(w2,-551,511,0,180);
  w3_val= map(w3,-551,511,0,180);
  
  int xaxis_rotate_val=map(analogRead(xaxis_rotate),0,4095,0,180);
  Serial.print("The x_rotate analog");
  Serial.println(analogRead(xaxis_rotate));
  if(abs(xaxis_rotate_val-90)>10){
    w1_val= xaxis_rotate_val;
    w2_val= xaxis_rotate_val;
    w3_val= xaxis_rotate_val;
  }
      //sheild
  int shield_val=(analogRead(shield));
  Serial.print("The shield analog");
  Serial.println(shield_val);
  shield_val=map(shield_val,0,4095,0,190);

  joystick[0]=w3_val;
  joystick[1]=w1_val;
  joystick[2]=w2_val;
  joystick[3]=shield_val;
  // get the right value from the joystick

  fncUdpSend(joystick[0],joystick[1],joystick[2],joystick[3],joystick[4]); // send the analog value to the car
  delay(1); // delay for working
}
