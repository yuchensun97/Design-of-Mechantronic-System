#include <Servo.h>
#include "pitches.h"

//music setup
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

Servo myservo1; // create servo object to control a servo
Servo myservo2; // create servo object to control a servo
Servo myservo3;
Servo myservo4;

bool music_flag=true;
byte mystr[10]; //Initialized variable to store recieved data
int mystr_save[10];
bool changed=0;
int flag_skip=400;

//vive setup
const byte vive = 2;
const byte vive_2 = 4;
//volatile bool interrupt_flag = false;
bool vive_flag=1;
bool vive_2_flag=0;
bool turn_flag=false;
uint32_t tperiod = 0;
uint32_t oldtime = 0;

uint32_t tperiod_2 = 0;
uint32_t oldtime_2 = 0;

uint32_t pulse_width = 0;
int sync = 0;
int x_flag = 0;
int y_flag = 0;

int sync_2 = 0;
int x_2_flag = 0;
int y_2_flag = 0;

uint32_t x = 0;
uint32_t y = 0;

uint32_t x_2 = 0;
uint32_t y_2 = 0;

int vive_exist_flag=0;
int vive_2_exist_flag=0;

//autonomous code set up
int step_count=-7;
int x_turn_right_negative_flag_count=0;
int x_turn_left_negative_flag_count=0;
int x_turn_right_slow_negative_flag_count=0;
int x_turn_left_slow_negative_flag_count=0;

int x_turn_right_positive_flag_count=0;
int x_turn_left_positive_flag_count=0;
int x_turn_right_slow_positive_flag_count=0;
int x_turn_left_slow_positive_flag_count=0;

int y_turn_right_negative_flag_count=0;
int y_turn_left_negative_flag_count=0;
int y_turn_right_slow_negative_flag_count=0;
int y_turn_left_slow_negative_flag_count=0;

int y_turn_right_positive_flag_count=0;
int y_turn_left_positive_flag_count=0;
int y_turn_right_slow_positive_flag_count=0;
int y_turn_left_slow_positive_flag_count=0;

int x_go_straight_flag_count=0;
int y_go_straight_flag_count=0;

int x_go_straight_slow_flag_count=0;
int y_go_straight_slow_flag_count=0;

int x_go_back_flag_count=0;
int y_go_back_flag_count=0;
int x_move_left_to_wall_flag_count=0;
int turn_constant=50;
int turn_slow_constant=20;
int go_straight_constant=80;
int go_straight_slow_constant=30;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1);
  
//set up servo
  myservo1.attach(9); // attaches the servo on pin 9 to the servo object
  myservo2.attach(10); // attaches the servo on pin 10 to the servo object
  myservo3.attach(11); // attaches the servo on pin 11 to the servo object
  myservo4.attach(12); // attaches the servo on pin 11 to the servo object

  //set upvive
  pinMode(vive,INPUT_PULLUP);
  pinMode(vive_2,INPUT_PULLUP);  
}

//function to measure interval
int interval()
{
  tperiod = micros()-oldtime;
  return tperiod;
}

int interval_2()
{
  tperiod_2 = micros()-oldtime_2;
  return tperiod_2;
}

// print out orignial data
//  Serial.print("0 original value is: ");  
//  Serial.println(mystr[0]); //Print data on Serial Monitor
//  Serial.print("1 original value is: ");  
//  Serial.println(mystr[1]); //Print data on Serial Monitor
//  Serial.print("2 original value is: ");  
//  Serial.println(mystr[2]); //Print data on Serial Monitor
//  Serial.print("shield value is: ");  
//  Serial.println(mystr[3]); //Print data on Serial Monitor

//autonomous function----------------------------------------------------------------------------------------------
void turn_right_servo(){
  mystr[0]=180;
  mystr[1]=180;
  mystr[2]=180;
  for(int i=1;i<500;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void turn_right_slow_servo(){
  mystr[0]=180;
  mystr[1]=180;
  mystr[2]=180;
  for(int i=1;i<150;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void turn_left_servo(){
  mystr[0]=0;
  mystr[1]=0;
  mystr[2]=0;
  for(int i=1;i<500;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void turn_left_slow_servo(){
  mystr[0]=0;
  mystr[1]=0;
  mystr[2]=0;
  for(int i=1;i<150;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void go_straight_servo(){
  mystr[0]=27;
  mystr[1]=88;
  mystr[2]=180;
  for(int i=1;i<3000;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void go_straight_slow_servo(){
  mystr[0]=27;
  mystr[1]=88;
  mystr[2]=180;
  for(int i=1;i<1000;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void move_left(){
  mystr[0]=95;
  mystr[1]=0;
  mystr[2]=95;
  for(int i=1;i<2000;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void go_back_servo(){
  mystr[0]=180;
  mystr[1]=88;
  mystr[2]=40;
  for(int i=1;i<1000;i++){
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
    myservo2.write(mystr[1]); // sets the servo position according to the scaled value
    myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  }
}

void y_go_back_func(int y_go_back_coord)
{
  
  if(y<(y_go_back_coord-50)){
    y_go_back_flag_count=0; 
    go_back_servo();
  }
  else{
    y_go_back_flag_count=y_go_back_flag_count+1;
    if(y_go_back_flag_count==2){
      step_count=step_count+1;
      y_go_back_flag_count=0;
    }
  }
}

void x_move_left_to_wall_func(int x_move_left_coord)
{
  if(x>(x_move_left_coord+50)){
    x_move_left_to_wall_flag_count=0; 
    move_left();
  }
  else{
    x_move_left_to_wall_flag_count=x_move_left_to_wall_flag_count+1;
    if(x_move_left_to_wall_flag_count==2){
      step_count=step_count+1;
      x_move_left_to_wall_flag_count=0;
    }
  }
}

//x_turn function------------------------------------
//------------negative-----------------------------------
void x_turn_right_negative_func()
{  
  if((y_2-y)>turn_constant){
    x_turn_right_negative_flag_count=0; 
    turn_right_servo();
  }
  else{
    x_turn_right_negative_flag_count=x_turn_right_negative_flag_count+1;
    if(x_turn_right_negative_flag_count==2){
      step_count=step_count+1;
      x_turn_right_negative_flag_count=0;
    }
  }
}

void x_turn_right_slow_negative_func()
{  
  if((y_2-y)>turn_slow_constant){
    x_turn_right_slow_negative_flag_count=0; 
    turn_right_slow_servo();
  }
  else{
    x_turn_right_slow_negative_flag_count=x_turn_right_slow_negative_flag_count+1;
    if(x_turn_right_slow_negative_flag_count==2){
      step_count=step_count+1;
      x_turn_right_slow_negative_flag_count=0;
    }
  }
}

void x_turn_left_negative_func()
{  
  if((y-y_2)>turn_constant){
    x_turn_left_negative_flag_count=0; 
    turn_left_servo();
  }
  else{
    x_turn_left_negative_flag_count=x_turn_left_negative_flag_count+1;
    if(x_turn_left_negative_flag_count==2){
      step_count=step_count+1;
      x_turn_left_negative_flag_count=0;
    }
  }
}

void x_turn_left_slow_negative_func()
{  
  if((y-y_2)>turn_slow_constant){
    x_turn_left_slow_negative_flag_count=0; 
    turn_left_slow_servo();
  }
  else{
    x_turn_left_slow_negative_flag_count=x_turn_left_slow_negative_flag_count+1;
    if(x_turn_left_slow_negative_flag_count==2){
      step_count=step_count+1;
      x_turn_left_slow_negative_flag_count=0;
    }
  }
}

//x_turn function------------------------------------
//------------positive-----------------------------------------------------------------
void x_turn_right_positive_func()
{  
  if(((y-y_2)>turn_constant)||(x_2<x)){
    x_turn_right_positive_flag_count=0; 
    turn_right_servo();
  }
  else{
    x_turn_right_positive_flag_count=x_turn_right_positive_flag_count+1;
    if(x_turn_right_positive_flag_count==2){
      step_count=step_count+1;
      x_turn_right_positive_flag_count=0;
    }
  }
}


void x_turn_right_slow_positive_func()
{  
  if((y-y_2)>turn_slow_constant){
    x_turn_right_slow_positive_flag_count=0; 
    turn_right_slow_servo();
  }
  else{
    x_turn_right_slow_positive_flag_count=x_turn_right_slow_positive_flag_count+1;
    if(x_turn_right_slow_positive_flag_count==2){
      step_count=step_count+1;
      x_turn_right_slow_positive_flag_count=0;
    }
  }
}

void x_turn_left_positive_func()
{  
  if((y_2-y)>turn_constant){
    x_turn_left_positive_flag_count=0; 
    turn_left_servo();
  }
  else{
    x_turn_left_positive_flag_count=x_turn_left_positive_flag_count+1;
    if(x_turn_left_positive_flag_count==2){
      step_count=step_count+1;
      x_turn_left_positive_flag_count=0;
    }
  }
}

void x_turn_left_slow_positive_func()
{  
  if((y_2-y)>turn_slow_constant){
    x_turn_left_slow_positive_flag_count=0; 
    turn_left_slow_servo();
  }
  else{
    x_turn_left_slow_positive_flag_count=x_turn_left_slow_positive_flag_count+1;
    if(x_turn_left_slow_positive_flag_count==2){
      step_count=step_count+1;
      x_turn_left_slow_positive_flag_count=0;
    }
  }
}


//y_turn function-----------------------------------------------
//------------negative-----------------------------------------------------------------

void y_turn_left_negative_func()
{  
  if((x_2-x)>turn_constant ){
    y_turn_left_negative_flag_count=0; 
    turn_left_servo();
  }
  else{
    y_turn_left_negative_flag_count=y_turn_left_negative_flag_count+1;
    if(y_turn_left_negative_flag_count==2){
      step_count=step_count+1;
      y_turn_left_negative_flag_count=0;
    }
  }
}

void y_turn_left_slow_negative_func()
{  
  if((x_2-x)>turn_slow_constant){
    y_turn_left_slow_negative_flag_count=0; 
    turn_left_slow_servo();
  }
  else{
    y_turn_left_slow_negative_flag_count=y_turn_left_slow_negative_flag_count+1;
    if(y_turn_left_slow_negative_flag_count==2){
      step_count=step_count+1;
      y_turn_left_slow_negative_flag_count=0;
    }
  }
}

void y_turn_right_negative_func()
{  
  if((x-x_2)>turn_constant ){
    y_turn_right_negative_flag_count=0; 
    turn_right_servo();
  }
  else{
    y_turn_right_negative_flag_count=y_turn_right_negative_flag_count+1;
    if(y_turn_right_negative_flag_count==2){
      step_count=step_count+1;
      y_turn_right_negative_flag_count=0;
    }
  }
}

void y_turn_right_slow_negative_func()
{  
  if((x-x_2)>turn_slow_constant){
    y_turn_right_slow_negative_flag_count=0; 
    turn_right_slow_servo();
  }
  else{
    y_turn_right_slow_negative_flag_count=y_turn_right_slow_negative_flag_count+1;
    if(y_turn_right_slow_negative_flag_count==2){
      step_count=step_count+1;
      y_turn_right_slow_negative_flag_count=0;
    }
  }
}

//y_turn function-----------------------------------------------
//------------positive-----------------------------------------------------------------
void y_turn_right_positive_func()
{   
  if((x_2-x)>turn_constant){
    y_turn_right_positive_flag_count=0; 
    turn_right_servo();
  }
  else{
    y_turn_right_positive_flag_count=y_turn_right_positive_flag_count+1;
    if(y_turn_right_positive_flag_count==2){
      step_count=step_count+1;
      y_turn_right_positive_flag_count=0;
    }
  }
}

void y_turn_right_slow_positive_func()
{   
  if((x_2-x)>turn_slow_constant){
    y_turn_right_slow_positive_flag_count=0; 
    turn_right_slow_servo();
  }
  else{
    y_turn_right_slow_positive_flag_count=y_turn_right_slow_positive_flag_count+1;
    if(y_turn_right_slow_positive_flag_count==2){
      step_count=step_count+1;
      y_turn_right_slow_positive_flag_count=0;
    }
  }
}

void y_turn_left_positive_func()
{  
  if((x-x_2)>turn_constant  ){
    y_turn_left_positive_flag_count=0; 
    turn_left_servo();
  }
  else{
    y_turn_left_positive_flag_count=y_turn_left_positive_flag_count+1;
    if(y_turn_left_positive_flag_count==2){
      step_count=step_count+1;
      y_turn_left_positive_flag_count=0;
    }
  }
}

void y_turn_left_slow_positive_func()
{  
  if((x-x_2)>turn_slow_constant){
    y_turn_left_slow_positive_flag_count=0; 
    turn_left_slow_servo();
  }
  else{
    y_turn_left_slow_positive_flag_count=y_turn_left_slow_positive_flag_count+1;
    if(y_turn_left_slow_positive_flag_count==2){
      step_count=step_count+1;
      y_turn_left_slow_positive_flag_count=0;
    }
  }
}

//go_straight and stop function------------------------------------------------------------------------------------------------------------------
void x_go_straight_func(int x_straight_coord)
{
  if(x<(x_straight_coord-go_straight_constant) || x >(x_straight_coord+go_straight_constant)){
    x_go_straight_flag_count=0; 
    go_straight_servo();
  }
  else{
    x_go_straight_flag_count=x_go_straight_flag_count+1;
    if(x_go_straight_flag_count==2){
      step_count=step_count+1;
      x_go_straight_flag_count=0;
    }
  }
}


void x_go_straight_slow_func(int x_straight_coord)
{
  if(x<(x_straight_coord-go_straight_slow_constant) ||x>(x_straight_coord+go_straight_slow_constant) ){
    x_go_straight_slow_flag_count=0; 
    go_straight_slow_servo();
  }
  else{
    x_go_straight_slow_flag_count=x_go_straight_slow_flag_count+1;
    if(x_go_straight_slow_flag_count==2){
      step_count=step_count+1;
      x_go_straight_slow_flag_count=0;
    }
  }
}

void y_go_straight_func(int y_straight_coord)
{
  if(y<(y_straight_coord-go_straight_constant) || y >(y_straight_coord+go_straight_constant)){
    y_go_straight_slow_flag_count=0; 
    go_straight_servo();
  }
  else{
    y_go_straight_flag_count=y_go_straight_flag_count+1;
    if(y_go_straight_flag_count==2){
      step_count=step_count+1;
      y_go_straight_flag_count=0;
    }
  }
}

void y_go_straight_slow_func(int y_straight_slow_coord)
{
  if(y<(y_straight_slow_coord-go_straight_slow_constant) || y >(y_straight_slow_coord+go_straight_slow_constant)){
    y_go_straight_slow_flag_count=0; 
    go_straight_slow_servo();
  }
  else{
    y_go_straight_slow_flag_count=y_go_straight_slow_flag_count+1;
    if(y_go_straight_slow_flag_count==2){
      step_count=step_count+1;
      y_go_straight_slow_flag_count=0;
    }
  }
}

void stop_servo(){
  mystr[0]=93;
  mystr[1]=88;
  mystr[2]=92;
}

//loop begins--------------------------------------------------------------------------------------------------------------------

void loop() {

  if(Serial2.available())
  {
    Serial2.readBytes(mystr, 6);
//    Serial.println("start");

    if(mystr[5]<200){
        for(int k=0;k<6;k++){
          mystr_save[k]=mystr[k];
        }
    }

    Serial.println(mystr[0]);
    Serial.println(mystr[1]);
    Serial.println(mystr[2]);
    Serial.println(mystr[3]);
    Serial.println(mystr[4]);
    Serial.println(mystr[5]);
     if((mystr[0]>200)&( mystr[5]<200)){
       mystr[0]=mystr_save[2];
       mystr[1]=mystr_save[3];
       mystr[2]=mystr_save[4]; 
       mystr[3]=mystr_save[5];
       mystr[4]=mystr_save[0];
       mystr[5]=mystr_save[1];   
  //           Serial.println("a");  
    }
     if((mystr[1]>200)&(mystr[0]<200)){
       mystr[0]=mystr_save[3];
       mystr[1]=mystr_save[4];
       mystr[2]=mystr_save[5]; 
       mystr[3]=mystr_save[0];
       mystr[4]=mystr_save[1];
       mystr[5]=mystr_save[2]; 
  //         Serial.println("b");     
    }
     if((mystr[2]>200)&(mystr[1]<200)){
       mystr[0]=mystr_save[4];
       mystr[1]=mystr_save[5];
       mystr[2]=mystr_save[0]; 
       mystr[3]=mystr_save[1];
       mystr[4]=mystr_save[2];
       mystr[5]=mystr_save[3];  
  //           Serial.println("c");    
    }
     if((mystr[3]>200)&(mystr[2]<200)){
       mystr[0]=mystr_save[5];
       mystr[1]=mystr_save[0];
       mystr[2]=mystr_save[1]; 
       mystr[3]=mystr_save[2];
       mystr[4]=mystr_save[3];
       mystr[5]=mystr_save[4];     
  //         Serial.println("d");
    }
  
     if((mystr[5]>200)&(mystr[4]<200)){
       mystr[0]=mystr_save[1];
       mystr[1]=mystr_save[2];
       mystr[2]=mystr_save[3]; 
       mystr[3]=mystr_save[4];
       mystr[4]=mystr_save[5];
       mystr[5]=mystr_save[6]; 
  //         Serial.println("f");    
    }
  }

int automode=mystr[4];
int health=mystr[5];

if (automode==203&health!=203){
//if (1){
  if(vive_flag==1){
    if(sync> 4){sync = 0;}
    pulse_width = pulseIn(vive,HIGH);
//    Serial.print("pulse_width  ");
//        Serial.println(pulse_width);
    if (pulse_width > 70){
//      Serial.println("sync  ");  
      sync += 1;
      oldtime = micros();
    }
//    Serial.println(pulse_width);
    if (pulse_width<40 & pulse_width> 5){
      if (sync == 3){
        x_flag = 1;
//        Serial.println("x");
        x = interval();
        sync = 0;
      }
      else if(sync == 1){
        y_flag = 1;
//        Serial.println("y");
        y = interval();
        sync = 0;
      }
    }
    if(x_flag == 1 & y_flag == 1){
      Serial.println("1");
      Serial.println(x);
//      Serial.println("interval between y_1 and sync");
      Serial.println(y);
      x_flag = 0; y_flag =0;
      vive_flag=0;
      vive_2_flag=1;
      vive_exist_flag=1;
    }
  }
  
  if(vive_2_flag==1){
    if(sync_2> 4){sync_2 = 0;}
    pulse_width = pulseIn(vive_2,HIGH);
//        Serial.print("pulse_width2  ");
//        Serial.println(pulse_width);
//    Serial.println(pulse_width);
    if (pulse_width > 70){
//      Serial.println("sync_2  ");
      sync_2 += 1;
      oldtime_2 = micros();
    }
    if (pulse_width<40 & pulse_width> 5){
      if (sync_2 == 3){
        x_2_flag = 1;
//        Serial.println("x");
        x_2 = interval_2();
        sync_2 = 0;
      }
      else if(sync_2 == 1){
        y_2_flag = 1;
//        Serial.println("y");
        y_2 = interval_2();
        sync_2 = 0;
      }
    }
    if(x_2_flag == 1 & y_2_flag == 1){
      Serial.println("2");
      Serial.println(x_2);
//      Serial.println("interval between y_2 and sync_2");
      Serial.println(y_2);
      x_2_flag = 0; y_2_flag =0;
      vive_flag=1;
      vive_2_flag=0;
      vive_2_exist_flag=1;
    }
  }

  if (vive_exist_flag==1& vive_2_exist_flag==1){

//-----------step_start--------------------------------------------------------------------------------------
    if(step_count==-7){
     if(x_2<x){
      y_turn_right_negative_func();
     }
     else{
      y_turn_left_negative_func();
     }
     }
    else if(step_count==-6){
      if(x_2<x){
      y_turn_right_slow_negative_func();
     }
     else{
      y_turn_left_slow_negative_func();
     }
      }
      
    
    else if(step_count==-5){
      y_go_straight_func(3900);
    }
    else if(step_count==-4){
      y_go_straight_slow_func(3900);
      
    }

    else if(step_count==-3){
      y_turn_left_negative_func();
     }
    else if(step_count==-2){
      y_turn_left_slow_negative_func();
      
    }
//    else if(step_count==-3){
//      y_turn_right_positive_func();
//     }
//    else if(step_count==-2){
//      y_turn_right_slow_positive_func();
//      
//    }
    else if(step_count==-1){
      y_go_straight_func(4000);
    }
    else if(step_count==0){
      y_go_straight_slow_func(4000);
      
    }
    
    else if(step_count==1){
      x_turn_left_negative_func();
    }
    else if(step_count==2){
      x_turn_left_slow_negative_func();      
    }
    
//    else if(step_count==1){
//      x_turn_right_negative_func();
//    }
//    else if(step_count==2){
//      x_turn_right_slow_negative_func();      
//    }
    
    else if(step_count==3){
      x_go_straight_func(3600);
    }
    else if(step_count==4){
       x_go_straight_slow_func(3600);   
    }

//------------going up the bridge----------------------------------
//
//    if(step_count==-7){
//     if(y_2>2){
//      x_turn_right_negative_func();
//     }
//     else{
//      x_turn_left_negative_func();
//     }
//     }
//    else if(step_count==-6){
//      if(y_2>2){
//        x_turn_right_slow_negative_func();
//      }
//      else{
//        x_turn_left_negative_func();
//      }
//      
//    }
//    else if(step_count==-5){
//      x_go_straight_func(3300);
//    }
//    else if(step_count==4){
//      x_go_straight_slow_func(3300);
//      
//    }
//
//    else if(step_count==-3){
//      y_turn_left_positive_func();
//     }
//    else if(step_count==-2){
//      y_turn_left_slow_positive_func();
//      
//    }
//    else if(step_count==-1){
//      y_go_straight_func(5500);
//    }
//    else if(step_count==0){
//      y_go_straight_slow_func(5500);
//      
//    }
//    
//    else if(step_count==1){
//      x_turn_right_negative_func();
//    }
//    else if(step_count==2){
//      x_turn_right_slow_negative_func();
//      
//    }
//    else if(step_count==3){
//      x_go_straight_func(3180);
//    }
//    else if(step_count==4){
//       x_go_straight_slow_func(3180);   
//    }
//      else if(step_count==5){
//      y_turn_right_negative_func();
//    }
//        else if(step_count==6){
//      y_turn_right_slow_negative_func();
//    }
//    else if(step_count==7){
//      y_go_straight_func(4130);
//    }
//   
//    else if(step_count==8){
//    y_go_straight_slow_func(3920);
//    }
//    else if(step_count==9){
//      x_turn_left_positive_func();
//    }
//      else if(step_count==10){
//        x_turn_left_slow_positive_func();
//  
//    }
//          else if(step_count==11){
//      x_go_straight_func(3145);
//        }
//        else if(step_count==12){
//      x_go_straight_slow_func(2945);
//        }

//-----------------------bridge_ends---------------------
    vive_exist_flag=0;
    vive_2_exist_flag=0;
//    Serial.println(mystr[0]); //Print data on Serial Monitor
//  Serial.println(mystr[1]); //Print data on Serial Monitor
//  Serial.println(mystr[2]); //Print data on Serial Monitor
Serial.println("step_count");
Serial.println(step_count);
}
else{
//if vive doesn't receive signal, the car stops
  stop_servo();

  myservo1.write(mystr[0]); // sets the servo position according to the scaled value
  myservo2.write(mystr[1]); // sets the servo position according to the scaled value
  myservo3.write(mystr[2]); // sets the servo position according to the scaled value
}
}
//------------automode ends----------------------

//music part
else if(health==203&music_flag==true){
  music_flag=0;
  stop_servo();

//    mystr[0]=180;
//  mystr[1]=180;
//  mystr[2]=180;

  myservo1.write(mystr[0]); // sets the servo position according to the scaled value
  myservo2.write(mystr[1]); // sets the servo position according to the scaled value
  myservo3.write(mystr[2]); // sets the servo position according to the scaled value

  int size = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
;
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(36, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(36);
  }
  }
  
//manual_mode
  else{
//    Serial.println("manual");
    if (mystr[0]<102 && mystr[0]>89){
    mystr[0]=93;
  }
  if (mystr[1]<92 && mystr[1]>89){
    mystr[1]=88;
  }
  if (mystr[2]<96 && mystr[2]>89){
    mystr[2]=92;
  }
    myservo1.write(mystr[0]); // sets the servo position according to the scaled value
  myservo2.write(mystr[1]); // sets the servo position according to the scaled value
  myservo3.write(mystr[2]); // sets the servo position according to the scaled value
  Serial.println("start");
  Serial.println(mystr[0]);
  Serial.println(mystr[1]);
  Serial.println(mystr[2]);
  Serial.println(mystr[3]);
  Serial.println(mystr[4]);
  Serial.println(mystr[5]);
  }
}
 
