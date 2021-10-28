/*
 *  Project:      HueHeader
 *  Description:  Example Codefor Hue Lights in IoT Classroom using Hue IoT LIbrary
 *  Authors:      Brian Rashap
 *  Date:         22-JUL-2020
 */
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>

int       blk_buttonState = 0;
const int BLK_BUTTON_PIN  = 5,   BLK_LEDPIN = 15, LED_ON = 255, LED_OFF = 0;

void setup()
{
  pinMode(BLK_BUTTON_PIN,INPUT_PULLUP);
  Serial.begin(9600);

  // ensure all SPI devices start off

    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
 
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);

    pinMode(BLK_LEDPIN, OUTPUT);

  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
  printIP();
  while(!Serial);
  Serial.printf("LinkStatus: %i  \n",Ethernet.linkStatus());
}

void loop() {
  /* setHue function needs 5 parameters
   *  int bulb - this is the bulb number
   *  bool activated - true for bulb on, false for off
   *  int color - Hue color from hue.h
   *  int - brightness - from 0 to 255
   *  int - saturation - from 0 to 255
   */
//  1st, check w Yellow LED from Yellow button press
//  y_buttonState = digitalRead(Y_BUTTON_PIN);
// 
//    digitalWrite(Y_LEDPIN, HIGH);
//  }
//  if(!y_buttonState) {
//    pinMode(4, OUTPUT);
//    analogWrite(Y_LEDPIN, LED_OFF);
//  }
  
  blk_buttonState = digitalRead(BLK_BUTTON_PIN);
  if(blk_buttonState) {       // Turn on light
    setHue(1,true,HueBlue,255,255);
//    delay(5000);
  }
  if(!blk_buttonState) {
  setHue(1,false,0,0,0);
  }
//  delay(5000);
}

void printIP() {
  Serial.printf("My IP address: ");
  for (byte thisByte = 0; thisByte < 3; thisByte++) {
    Serial.printf("%i.",Ethernet.localIP()[thisByte]);
  }
  Serial.printf("%i\n",Ethernet.localIP()[3]);
}
