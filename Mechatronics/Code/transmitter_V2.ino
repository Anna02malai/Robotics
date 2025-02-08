// include libraries
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

// Let these addresses be used for the pair
uint8_t address[][6] = { "1Node", "2Node" };
bool radioNumber = 0;

int joy1_x = A0;
int joy1_y = A1;
int joy_b = 10;

// define joystick values
int y_pos = 512;
int x_pos = 512;

// declare array for motor control (4 motors + direction)
uint16_t motorcontrol[3];

void setup() 
{
  // setup serial monitor
  Serial.begin(115200);
  radio.begin();
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding!!");
    while (1) {} // Infinite loop to halt further execution
}
  // setup radio transmitter
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(motorcontrol));
  radio.openWritingPipe(address[radioNumber]);
  pinMode(joy1_x,INPUT);
  pinMode(joy1_y,INPUT);
  pinMode(joy_b,INPUT);
  digitalWrite(joy_b,HIGH);
}

void loop() {
  Serial.println("motorcontrol values: ");

  // read joystick positions
  x_pos = analogRead(joy1_x);
  y_pos = analogRead(joy1_y);

  x_pos = map(x_pos, 0, 1023, 0, 510); //Mapping the x values to (0 - 510)
  y_pos = map(y_pos, 0, 1023, 0, 510); //Mapping the y values to (0 - 510)

  if (x_pos > 240 & x_pos < 270){
    motorcontrol[0] = 255;
  }
  else{
    motorcontrol[0] = x_pos;
  }
  if (y_pos > 240 & y_pos < 270) // backward
  {
    motorcontrol[1] = 255;
  }
  else{
    motorcontrol[1] = y_pos;
  }

  //Display the x_pos and y_pos values in the serial monitor.
  Serial.print("x_pos: ");
  Serial.print(motorcontrol[0]);
  Serial.print("y_pos: ");
  Serial.print(motorcontrol[1]);

  // read button state
  int button = digitalRead(joy_b);
  motorcontrol[2] = button;

  // display button state in serial monitor
  Serial.print("button state:");
  Serial.print(button);

  // send motor control data to radio
  bool report = radio.write(&motorcontrol, sizeof(motorcontrol));  // transmit report
  if (report)
  {
    Serial.println("transmission successful");
  }
  delay(10);
    
}

