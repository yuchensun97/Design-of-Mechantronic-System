#include <WiFi.h>
#include <WiFiUdp.h>

int analogPin = 32; // potentiometer connected to analog pin 32
int ledPin = 21; // LED connected to digital pin 21
int inPin = 33; // switch connected to P33
int val = 0; // variable to store the read value
int flag = 0; //set the Onboard LED blinking flag to 0

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
  pinMode(inPin, INPUT_PULLUP); // set the switch pin as input
  pinMode(ledPin,OUTPUT); // set the led pin as output
  pinMode(2,OUTPUT); // set the onboard led as output
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

void fncUdpSend(int number1,int number2) // number1: the value of potentiometer, number2: the switch boolean
{
  // write the sending subroutine here
  Serial.println("sending data...");
  udpBuffer[0]=(number2<<7)+number1; // if the botton is pressed, left shift number2
  // send the packet to the other device                                          
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  Serial.println("Sent data is:");
  Serial.printf("%d\n",udpBuffer[0]);
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

// create variable to use the PWM 
int currTime=0;
int prevTime=0;
int timeDiff=0;

void loop() {
  int mv; // variable to store the sending value
  int read_sw; // point to store the Recieved value
  int sw; // variable to store the switch value
  float min; // variable to store the smaller mv
  float max; // variable to store the bigger mv
  int finish=0; // the finish boolean
  sw = digitalRead(inPin)+1; // read the switch's value, 1: botton is pressed; 2: botton un-pressed
  val = analogRead(analogPin); // read the input analogPin
  mv = map(val,0,4095,100,1); // convert the analog to time in milliseconds

  currTime = millis(); // let currTime equals to the counts by timer
  timeDiff = currTime - prevTime; // calculate the time difference between two counts
  if(flag==0){  
    if(timeDiff > (mv*50)){
    digitalWrite(ledPin,LOW); // if the time differnce is larger than duty circle * mv, set the led pin low
    prevTime=millis(); // set previous time to the counts
    flag=1; // set the LED blinking flag to 1
    }
  }
  else if(flag==1){
    if(timeDiff> (mv*50)){
    digitalWrite(ledPin,HIGH); // if the time difference is lower than duty circle*mv, set the led pin HIGH
    prevTime=millis(); // set the previous time to the counts
    flag=0; // set the LED blinkg flag to 0
    }
  }

  read_sw = handleUDPServer(); // read the recieve value
  fncUdpSend(mv,sw); // send the potentiometer and switch value

  if(sw==1){
    // when the switch is pressed
    if(read_sw>=mv){
      // if the recieve value is larger than sending value
     max=read_sw; //set the max value to Recieved number
     min=mv; // set the min value to sending value
     }
    else{
      max=mv; // set the max value to the sending value
      min=read_sw; // set the min value to reading value
    }
    if ((max-min)/min <0.1){
      // if the reading & sending value is in the 10% range
      digitalWrite(2, HIGH); // light up my onboard led
      finish = 1; // set the finish flag to 1
    }
    else{
    digitalWrite(2, LOW); // pull down my onboard led
    finish = 1; // set the finish flag to 1
    for(int i = 0; i<20;i++){
      // make sure the value is sent
      fncUdpSend(mv,sw);
      }
    }
  }

  if (read_sw > 110 & read_sw < 250){
    // if the other device is pressed
    float red_sw_mv_2=read_sw - 128; // get the correct value of the other device
    if (red_sw_mv_2 >= mv){
      // if the reading value is larger than the sending value
      min = mv; // set the min value to sending value
      max = red_sw_mv_2; // set the max value to reading value
    }
    else{
      min = red_sw_mv_2; // set the min value to reading value
      max = mv; // set the max value to reading value
    }

    if ((max-min)/min <0.1){
      // if the reading & sending value is within the 10% range
      digitalWrite(2,LOW); // my onboard LED pulled down
      finish = 1; // set the finish flag to 1
    }
    else{
    digitalWrite(2,HIGH); // my onboard LED pulled down
    finish = 1; // set the finish flag to 1
    }
  }

  if (finish == 1)
  {
    delay(10000); // if the game is fished, stop working
  }
}