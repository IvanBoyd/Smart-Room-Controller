/*
 *  Project:      Turn on HUE Lights
 *  Description:  Will load into SensorLight master program
 *  Authors:      Ivan Boyd
 *  Date:         10/28/21
 */
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <hue.h>

int       b_buttonState = 0;
const int B_BUTTON_PIN  = 5,   B_LEDPIN = 15, LED_ON = 255, LED_OFF = 0;

void setup()  {
  pinMode(B_BUTTON_PIN,INPUT_PULLUP);
  Serial.begin(9600);
  // ensure all SPI devices start off
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
//    pinMode(4, OUTPUT);
//    digitalWrite(4, HIGH);
    pinMode(B_LEDPIN, OUTPUT);
  Ethernet.begin(mac);
  delay(200);          //ensure Serial Monitor is up and running           
//  printIP();
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
  b_buttonState = digitalRead(B_BUTTON_PIN);
  if(b_buttonState) {       // Turn on light
    setHue(1,true,HueBlue,50,255);
    setHue(2,true,HueBlue,100,255);
    setHue(3,true,HueBlue,150,255);
    setHue(4,true,HueBlue,200,255);
    setHue(5,true,HueBlue,255,255);
    
//    delay(5000);
  }
  if(!b_buttonState) {
    setHue(1,false,0,0,0);
    setHue(2,false,0,0,0);
    setHue(3,false,0,0,0);
    setHue(4,false,0,0,0);
    setHue(5,false,0,0,0);

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
