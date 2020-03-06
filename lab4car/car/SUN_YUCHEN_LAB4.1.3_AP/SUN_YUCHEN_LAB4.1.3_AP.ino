
#include <WiFi.h>
#include <WiFiUdp.h>
#define RESOLUTION_BITS 8
#define RESOLUTION ((1<<RESOLUTION_BITS)-1)
#define LEDC_FREQ_HZ 5000
#define speed 0 // use first channel of 16
#define speed_ctrl_pin 22

int dir_1 = 32; // direction of motor 1, always high
int dir_2 = 33; // direction of motor 1, always low


const char* ssid     = "meam510";// create the WiFi name
const char* password = "gooooooo";// create the WiFi password


WiFiUDP UDPTestServer;// set the WiFi to UDP mode
unsigned int udpLocalPort=2801;// create the local port
unsigned int udpTargetPort=2801; // create the target port
const int packetSize = 20;// set packet size

char receiveBuffer[packetSize];// set the receive packet 
char number1[packetSize];
char receive_number[packetSize];// set the recive number packet

IPAddress ipTarget(192, 168, 1, 174); // Yuchen Sun's IP
IPAddress ipLocal(192, 168, 1, 165);  // Shallchee Shih's IP

void setup() {
   // set the Wifi as AP mode
  
  Serial.begin(115200);  
  pinMode(dir_1, OUTPUT);
  pinMode(dir_2,OUTPUT);
  ledcSetup(speed, LEDC_FREQ_HZ,RESOLUTION_BITS);
  ledcAttachPin(speed_ctrl_pin,speed);
  Serial.print("Connecting to ");  Serial.println(ssid);
  WiFi.mode(WIFI_AP);  //set as AP mode
  WiFi.softAP(ssid,password);
  delay(100);
  WiFi.softAPConfig(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  UDPTestServer.begin(udpLocalPort); // UDP begin
  Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
   

}                                                   



int UDPreceiveData() {// write the recieve packet here
  int cb = UDPTestServer.parsePacket();// determine whether there is a sending packet.
  if (cb) { // recieve the sending packet
    UDPTestServer.read(receiveBuffer, packetSize);
    receive_number[0]=receiveBuffer[0]&0xff;
    Serial.print("receive:");
    Serial.printf("%d\n",receive_number[0]);

    return receive_number[0];
  }
}

void loop() {
  int recieve_speed;  // recieve pwm
  
  recieve_speed=UDPreceiveData();
  Serial.print("result:");
  Serial.printf("%d\n",recieve_speed);       
  delay(1);
  digitalWrite(dir_1,HIGH);
  digitalWrite(dir_2,LOW);
  ledcWrite(speed,recieve_speed);
  delay(1);
}
