int inPin = 16; //switch connected to P16
int outPin = 17; // LED connected to P17
int val = 0; // variable to store the read value

void setup() {
  // put your setup code here, to run once:
  pinMode(inPin, INPUT); // set digital pin 7 as input
  pinMode(outPin,OUTPUT); //initialize digital pin LED_BUILTIN as an outpuy
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(inPin); // read the input pin
  digitalWrite(outPin,val); // turn the LED off when the switch is pressed                                                                                                                                                                                                                                            
}