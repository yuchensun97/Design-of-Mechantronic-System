#define RESOLUTION_BITS 8
#define RESOLUTION ((1<<RESOLUTION_BITS)-1)
#define LEDC_FREQ_HZ 5000
#define speed 0 // use first channel of 16
#define speed_ctrl_pin 22

int dir_1 = 32; // direction of motor 1, always high
int dir_2 = 33; // direction of motor 1, always low
// int speed = 0; // speed, use PWM
// int speed_ctrl_pin = 21;

void setup(){
    // input your setup code here
    pinMode(dir_1, OUTPUT);
    pinMode(dir_2,OUTPUT);
    ledcSetup(speed, LEDC_FREQ_HZ,RESOLUTION_BITS);
    ledcAttachPin(speed_ctrl_pin,speed);
}

void loop(){
    // input your main code here
    digitalWrite(dir_1, HIGH);
    digitalWrite(dir_2,LOW);
    for (int i = 128; i < 255; i++){
        ledcWrite(speed, i);
        delay(30);
    }
    for(int i = 255; i>128; i--){
        ledcWrite(speed,i);
        delay(30);
    }
}