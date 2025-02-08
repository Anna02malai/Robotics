

  ////////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////VARIABLES/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


int checkTime= 690;
int minForWhite=150;
int maxTimeForWhite=200;
float sensorDistance=4;   //in
float avgVel=2;   //in/sec


float basePower=50;

const int buttonPin = 6; // Pin connected to the button
int buttonState = LOW;    // Variable to store the state of the button
int lastButtonState = LOW; // Variable to store the previous state of the button
int goState = LOW;       // Variable to store the state of the robot


float angle=0; //deg
int lineCount=0;
int sense1=0;
int sense2=0;
bool whiteEnded=false;
float t1;
float t2;
unsigned long t_true;
unsigned long t;
unsigned long t_new;
unsigned long t_white;
unsigned long t_white_start;
unsigned long t_servo;

#include <Servo.h>
Servo servo_ladder;
int down_speed = 71;
int up_speed = 91; 

// include libraries
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "IRremote.h"

#define CE_PIN 25
#define CSN_PIN 24

#define SERVO_PIN 34

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };
uint16_t motorcontrol[3];
bool radioNumber = 0;

//PWM Pins to control motor Speeds
int motor1 = 4; // Left Wheel
int motor2 = 3; // Right Wheel

// motor Speed Control
float m1_a=1; // left modifier
float m2_a=1; // right modifier

const int motorPin1_1 = 26; // Left Wheel
const int motorPin1_2 = 27; // Interrupt Pins

const int motorPin2_1 = 28; //  Right Wheel
const int motorPin2_2 = 29; //  Interrupt Pins





  ////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////MAIN////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////









void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(35, INPUT);
  pinMode(motor1,OUTPUT);
  pinMode(motor2,OUTPUT);
  pinMode(buttonPin, INPUT);

  servo_ladder.attach(SERVO_PIN, 500, 2500);
  // servo_ladder.write(80);

  servo_ladder.write(81);

    // setup serial monitor
  Serial.begin(115200);
  radio.begin();
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  // set motor control pins to outputs

  pinMode(motor1,OUTPUT);  //PWM - Speed control pins
  pinMode(motor2,OUTPUT);

  pinMode(motorPin1_1, OUTPUT);
  pinMode(motorPin1_2, OUTPUT);

  pinMode(motorPin2_1, OUTPUT);
  pinMode(motorPin2_2, OUTPUT);

  
  //set up receiver
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, address[radioNumber]);
  radio.setPayloadSize(sizeof(motorcontrol));  // float datatype occupies 4 bytes
  radio.startListening();

}

void loop() { //MAIN  

  t = millis() - t_new;
  t_white=millis()-t_white_start;
  t_true=millis();
  Serial.print(sense1);
  Serial.print(", ");
  Serial.println(t_white);

  if (executeLoop()){ //run code loop

    hbridgeDrive();

    //driveStraight(); 

    detectLines();

    calcAngle();

    checkIR();

    // Serial.print(sense1);	    // Connect to pin 8 and 9, display results
    // Serial.print(",");
    // Serial.print(sense2);
    // Serial.print(",");
    // Serial.print(((abs(t1-t2))/1000)*avgVel);
    // Serial.print(",");
    // Serial.println(angle);
  } else {
    stopMotors();
    t_white_start=t_true;

  }
  delay(2);
}









  ////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////FUNCTIONS//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////









bool executeLoop(){
  buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != lastButtonState) {
    // If the button is pressed (changed from HIGH to LOW)
    if (buttonState == LOW) {
      // Toggle the LED state
      goState = !goState;
    }
    // Save the current button state
    lastButtonState = buttonState;
  }

  return (goState != LOW);
}

void hbridgeDrive(){
  
  uint8_t pipe;
  if (radio.available(&pipe)) // is there a payload? get the pipe number that recieved it
  {   
    uint16_t bytes = radio.getPayloadSize();  // get the size of the payload
    radio.read(&motorcontrol, bytes);             // fetch payload from FIFO
    //Serial.print(F("Received "));
    //Serial.print(bytes);  // print the size of the payload
    //Serial.print(F(" bytes on pipe "));
    //Serial.print(pipe);  // print the pipe number
    //Serial.println(F(": "));
    //Serial.println(motorcontrol[0]);  
    //Serial.println(motorcontrol[1]);
    //Serial.println(motorcontrol[2]); 

delay(10);

    // drive motors    - check which value corresponds to which corner
      analogWrite(motor1,248); //front left
      analogWrite(motor2,255);

    //direction control

    // Forward Direction when (Y_pos > 255)
    if (motorcontrol[1]>255){   

      // analogWrite(motor1,235); // Left Wheel
      // analogWrite(motor2,180); //Right left

      digitalWrite(motorPin1_1, HIGH); 
      digitalWrite(motorPin1_2, LOW);

      digitalWrite(motorPin2_1, HIGH); 
      digitalWrite(motorPin2_2, LOW);
    } 

    //Backward Direction when (Y_pos < 255)
    else if(motorcontrol[1]<255){ 

      // analogWrite(motor1,240); //Left wheel
      // analogWrite(motor2,220);

      digitalWrite(motorPin1_1, LOW);
      digitalWrite(motorPin1_2, HIGH);

      digitalWrite(motorPin2_1, LOW);
      digitalWrite(motorPin2_2, HIGH);
    }

    // Rightside wheel Backwards, Left side wheel Forward - to make the robot face right
    else if(motorcontrol[0]>255){    //(X_pos > 255) Right Turn

      // analogWrite(motor1,200); //front left
      // analogWrite(motor2,200);

      digitalWrite(motorPin1_1, LOW);
      digitalWrite(motorPin1_2, HIGH);    

      digitalWrite(motorPin2_1, HIGH);
      digitalWrite(motorPin2_2, LOW);
    }

    // Rightside wheel Forward, Left side wheel Backwards - to make the robot face left
    else if(motorcontrol[0]<255){     //(X_pos < 255) Left Turn 

      // analogWrite(motor1,200); 
      // analogWrite(motor2,200);

      digitalWrite(motorPin1_1, HIGH);
      digitalWrite(motorPin1_2, LOW);

      digitalWrite(motorPin2_1, LOW);
      digitalWrite(motorPin2_2, HIGH);
    }

    //Stay Stable No movement 
    else{
      digitalWrite(motorPin1_1, LOW);
      digitalWrite(motorPin1_2, LOW);

      digitalWrite(motorPin2_1, LOW);
      digitalWrite(motorPin2_2, LOW);
    }
        // ladder 
    if(motorcontrol[2]==0){
        t_servo=t_true;
    } else {
        if (t_true-t_servo<500){
        servo_ladder.write(down_speed);
        }
        else if (t_true-t_servo<1500){
        servo_ladder.write(81);
        }
        else if (t_true-t_servo<2000){
        servo_ladder.write(up_speed);
        }
        else if (t_true-t_servo<3000){
        servo_ladder.write(81);
        }
    }

  }

}

void driveStraight(){
  //motor
  // analogWrite(motor1,basePower*m1_a); //front right
  // analogWrite(motor2,basePower*m2_a); //back right
}

long RCTime(int sensorIn){
   long duration = 0;
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   delay(1);                      // Wait 1ms
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   while(digitalRead(sensorIn)){  // Wait for pin to go LOW
      duration++;
   }
   return duration;
}

void checkIR(){
  if(digitalRead(35)==LOW){
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}

void detectLines(){
  //line detection
  digitalWrite(10, LOW);  //blue       set LED pins to LOW
  digitalWrite(11, LOW);  //red
  digitalWrite(12, LOW);  //green
  sense1=RCTime(8); //Check QTI sensor values
  sense2=RCTime(9);
  if (sense1>minForWhite && t<checkTime){ //if sensor 1 detects black, and a line has not recently been crossed, update the relative time t1
    t1=t_true;
  }
  if (sense2>minForWhite && t<checkTime){ //if sensor 2 detects black, and a line has not recently been crossed, update the relative time t2
    t2=t_true;
  }
  if (sense1<minForWhite){ //if white is detected
    if (t>checkTime){ //if a line has not recently been crossed
      whiteEnded=false; //the line is identified
      t_new=t+t_new; //the time since crossing the line is updated
      lineCount=1; //a line is counted
    } else {
      if (whiteEnded){//if the end of the line has been already reached, this is a second line
        lineCount=2; //count two lines
        t_new=t+t_new; //time since crossing the line is updated
      } else {
        t_new=t+t_new; //time since crossing the line is updated
      }
    }
  } else { //if black is detected
    t_white_start=t_true; //time of starting to cross the line is updated
    whiteEnded=true; //a line has ended
    if (t>checkTime){ //if done searching for type of line
      if (lineCount==3){ //if out of bounds
        digitalWrite(12, HIGH); //light green LED
      }
      if (lineCount==2){ //if double line
        digitalWrite(10, HIGH); //light blue LED
      } 
      if (lineCount==1) { //if single line
        digitalWrite(11,HIGH); //light red LED
      }
      lineCount=0;//set lines to zero
    }
  }
  if (t_white>maxTimeForWhite){//if the line has not ended after a maximum time, turn on the green out of bounds LED and set the lines to 3
      digitalWrite(12, HIGH);
      lineCount=3;
  }
}

void calcAngle(){  
  if (sense1<minForWhite && sense2<minForWhite){
    angle =( atan2 (((abs(t1-t2))/1000)*avgVel, sensorDistance) * 180/3.14159265 );
  }
}

void stopMotors(){
  analogWrite(motor1,0); //front right
  analogWrite(motor2,0); //back right 
}

