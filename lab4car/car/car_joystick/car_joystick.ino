#include <WiFi.h>
#include <WiFiUdp.h>
#define ANALOG_PIN A4// potentiometer connected to A4
#define RESOLUTION_BITS 8
#define RESOLUTION ((1<<RESOLUTION_BITS)-1)
#define LEDC_FREQ_HZ 5000
#define speed_1 0 // use first channel of 16
#define speed_2 1 // use first channel of 16
#define speed_ctrl_pin 22
#define speed_ctrl_pin_2 21

int dir_1 = 32; // direction of motor
int dir_2 = 33; // direction of motor
const char* ssid     = "meam510";// create the WiFi name
const char* password = "gooooooo";// create the WiFi password

WiFiUDP UDPTestServer;// set the WiFi to UDP mode
unsigned int udpLocalPort=2801;// create the local port
unsigned int udpTargetPort=2801; // create the target port
const int packetSize = 10;// set packet size
char sendBuffer[packetSize];// set the sending packet
char receiveBuffer[packetSize];// set the receive packet 
char number1[packetSize]; 
char receive_number[packetSize];// set the recive number packet
int xaxis; // variable for x axis value
int yaxis; // variable for y axis value 
int right; // speed of right wheel
int left; // speed of left wheel
IPAddress ipTarget(192, 168, 1, 174); // Yuchen Sun's IP
IPAddress ipLocal(192, 168, 1, 165);  // Shallchee Shih's IP

void setup() {
  // set the Wifi as AP mode
  Serial.begin(115200);  
  Serial.print("Connecting to ");  Serial.println(ssid);
  WiFi.mode(WIFI_AP);  //set as AP mode
  WiFi.softAP(ssid,password);
  delay(100);
  WiFi.softAPConfig(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  UDPTestServer.begin(udpLocalPort); // UDP begin
  Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
  // pin set up
  pinMode(dir_1, OUTPUT);
  pinMode(dir_2,OUTPUT);
  ledcSetup(speed_1, LEDC_FREQ_HZ,RESOLUTION_BITS);
  ledcSetup(speed_2, LEDC_FREQ_HZ,RESOLUTION_BITS);
  ledcAttachPin(speed_ctrl_pin,speed_1);
  ledcAttachPin(speed_ctrl_pin_2,speed_2);
}                                                   

void UDPreceiveData() {// write the recieve packet here
  int cb = UDPTestServer.parsePacket();// determine whether there is a sending packet.
  if (cb) { // recieve the sending packet
    UDPTestServer.read(receiveBuffer, packetSize);
    xaxis=receiveBuffer[0]&0b1111111111;
    yaxis=receiveBuffer[1]&0b1111111111;
    Serial.print("receive:");
    Serial.printf("%d\n",receive_number[0]);
    Serial.printf("%d\n", receive_number[1]);
    Serial.println("End of receiving");
  }
}

void loop() {
  delay(1);
  UDPreceiveData();
  delay(10);
  Serial.println("xaxis");
  Serial.printf("%d\n",xaxis);
  Serial.println("yaxis");
  Serial.printf("%d\n",yaxis);
  // Y axis used for forward and backward control
  if(yaxis < 470){
    //Set the motors backward
    digitalWrite(dir_1,LOW);
    digitalWrite(dir_2,HIGH);
    // Convert the yaxis value to speed
    right = map(yaxis,470,0,0,255);
    left = map(yaxis,470,0,0,255);
  }
  else if(yaxis > 550){
    //Set the motors forward
    digitalWrite(dir_1,HIGH);
    digitalWrite(dir_2,LOW);
    // Convert the yaxis value to speed
    right = map(yaxis,550,1023,0,255);
    left =  map(yaxis,550,1023,0,255);
  }
  else{
    right = 0;
    left = 0;
  }

  // X-axis used for left and right control
  if (xaxis < 470){
    // Convert the x axis value to speed
    int xMapped = map(xaxis,470,0,0,255);
    // move left - differential speed control
    left = left - xMapped;
    right = right - xMapped;
    // confined range
    if (left <0){
      left = 0;
    }
    if (right > 255){
      right = 255;
    }
  }
  if (xaxis > 550){
    // Convert the x axis value to speed
    int xMapped = map(xaxis,550,1023,0,255);
    // move right - differential speed control
    left = left + xMapped;
    right = right - xMapped;
    if (left > 255){
      left = 255;
    }
    if (right < 0){
      right = 0;
    }
  }

  // prevent buzzing at low speed
  if (left < 128){
    left = 0;
  }
  if (right <128){
    right = 0;
  }

  // send pwm signal to motors
  ledcWrite(speed_1,right);
  ledcWrite(speed_2,left);

  /* for debugging*/
  Serial.println("The right motor speed is:");
  Serial.printf("%d\n",right);
  Serial.println("The left motor speed is:");
  Serial.printf("%d\n",left);

  delay(1);
}