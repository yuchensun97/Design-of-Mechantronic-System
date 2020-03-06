int analogPin_1 = 33; // LED connected to digital pin 17
int analogPin_2 = 34; // potentiometer connected to analog pin 34
int val_1 = 0; // variable to store the read value
int val_2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(analogPin_1,INPUT);
  pinMode(analogPin_2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int mv_1; 
  int mv_2;
  val_1 = analogRead(analogPin_1); // read the input pin
  val_2 = analogRead(analogPin_2); // 
  mv_1 = map(val_1,0,4095,0,1023);
  mv_2 = map(val_2,0,4095,1023,0);
  Serial.println("The x is:");
  Serial.printf("%d\n",mv_1);
  Serial.println("The y is:");
  Serial.printf("%d\n",mv_2);
  delay(30);
}
