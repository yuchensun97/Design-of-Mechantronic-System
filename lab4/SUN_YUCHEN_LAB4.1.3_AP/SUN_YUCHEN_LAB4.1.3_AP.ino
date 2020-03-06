
#include <WiFi.h>
#include <WiFiUdp.h>
#define ANALOG_PIN A4// potentiometer connected to A4

const char* ssid     = "meam510";// create the WiFi name
const char* password = "gooooooo";// create the WiFi password

int inPin = 33;  // LED connected to digital 33
float val = 0;      // variable to store the read value
int led = 2;
int flag=0;//set the Onboard LED blinking flag to 0
int previous=0;
WiFiUDP UDPTestServer;// set the WiFi to UDP mode
unsigned int udpLocalPort=2801;// create the local port
unsigned int udpTargetPort=2801; // create the target port
const int packetSize = 20;// set packet size
char sendBuffer[packetSize];// set the sending packet
char receiveBuffer[packetSize];// set the receive packet 
char number1[packetSize];
char receive_number[packetSize];// set the recive number packet

IPAddress ipTarget(192, 168, 1, 174); // Yuchen Sun's IP
IPAddress ipLocal(192, 168, 1, 165);  // Shallchee Shih's IP

void setup() {
   // set the Wifi as AP mode
  pinMode(led, OUTPUT);// set the led pin as output
  pinMode(21, OUTPUT);// set as output
  Serial.begin(115200);  
  pinMode(inPin, INPUT_PULLUP);    // sets the digital pin 7 as input
  digitalWrite(21, HIGH);//set LED to high
  Serial.print("Connecting to ");  Serial.println(ssid);
  WiFi.mode(WIFI_AP);  //set as AP mode
  WiFi.softAP(ssid,password);
  delay(100);
  WiFi.softAPConfig(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  UDPTestServer.begin(udpLocalPort); // UDP begin
  Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
}                                                   

void UDPsendData(int number1,int number2)// number1: the value of potentiometer, number2: the switch
{
  Serial.println("sending data...");   
  sendBuffer[0]=(number2<<7)+number1;// if the botton is pressed, left shift number2                   
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",sendBuffer);
  UDPTestServer.endPacket();
  Serial.printf("%d\n",sendBuffer[0]);   
}

int UDPreceiveData() {// write the recieve packet here
  int cb = UDPTestServer.parsePacket();// determine whether there is a sending packet.
  if (cb) { // recieve the sending packet
    UDPTestServer.read(receiveBuffer, packetSize);
    receive_number[0]=receiveBuffer[0]&0xff;
    Serial.print("receive:");
    Serial.printf("%d\n",receive_number[0]);
    Serial.println("End of receiving");
    return receive_number[0];
  }
}

void loop() {
  float period_of_on,period_of_blinking,send_number;
  int result;
  float max=0;
  float min=0;
  int finish=0;
  period_of_on = map(analogRead(ANALOG_PIN),0,4095,50,5000);// convert the analog to time in milliseconds
  period_of_blinking=period_of_on*2;
  send_number=period_of_blinking/100;
   millis();
   if(flag==0){
   if((millis()-previous)>period_of_on){
    digitalWrite(21, LOW);// if the time differnce is larger than half the blinking period, set the led pin high
    previous=millis();// set previous time to the counts
    flag=1;// set the LED blinking flag to 1
    }
   }
   else if (flag==1){
   if((millis()-previous)>period_of_on){
    digitalWrite(21, HIGH);// if the time differnce is larger than half the blinking period, set the led pin low
    previous=millis();// set previous time to the counts
    flag=0;// set the LED blinkg flag to 0
    }
   }
    
   val = digitalRead(inPin)+1;   // read the switch's value, 1: botton is pressed; 2: botton un-pressed
  delay(1);
  result=UDPreceiveData();
   Serial.print("result:");
    Serial.printf("%d\n",result);       
  delay(1);

if(val==1){// when the switch is pressed
    if(send_number>=result){// if the recieve value is smaller than sending value
    max=send_number;// set the max value to the sending value
    min=result;}// set the min value to recieve value
    else{
      max=result;//set the max value to Recieved number
      min=send_number;// set the min value to sending value
  }
  if ((max-min)/min <0.1){ // if the reading & sending value is in the 10% range
      digitalWrite(2, HIGH);// light up my onboard led
      finish=1;// set the finish flag to 1
    }
    else{
    digitalWrite(2, LOW);// pull down my onboard led
    finish=1;// set the finish flag to 1
    int i=0;
    while(i<20){ // make sure the value is sent
    UDPsendData(send_number,val);
    i=i+1;
    }
    }    
}

  UDPsendData(send_number,val);// send the potentiometer and switch value
  if(result>110&result<250){// if the other device is pressed
    float result_2=result-128;// get the correct value of the other device
  if(result_2>=val){// if the reading value is larger than the sending value
    max=result_2;// set the max value to reading value
    min=val;}// set the min value to sending value
    else{
      max=val;// set the max value to reading value
      min=result_2; // set the min value to reading value
  }
  if ((max-min)/min <0.1){// if the reading & sending value is within the 10% range
      digitalWrite(2, LOW);finish=1;// my onboard LED pulled down,set the finish flag to 1
    }
    else{
    digitalWrite(2, HIGH);finish=1;}// my onboard LED pulled down,set the finish flag to 1

  }

  delay(1);
  if(finish==1){
    delay(10000);// if the game is fished, stop working
  }
}
