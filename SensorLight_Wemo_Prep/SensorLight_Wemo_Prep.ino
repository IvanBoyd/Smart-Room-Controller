/*
 * Project: EthernetTest
 * Description: Ensure Ethernet Port wired correctly
 * Author: Ivan Boyd
 * Date: 10/19/21
 */

 /*
  * Ethernet Port Wiring
  * 3.3V to Teensy 3.3V
  * GND to Teensy GND
  * MISO to Teensy DI (Pin 12)
  * MOSI to Teensy DO (Pin 11)
  * SCLK to Teensy SCK (Pin 13)
  * SCNn to Teensy Pin 10 (for Chip Select)
  * RSTn to Teensy Pin 9
  * INTn and NC (No Connection) not connected
  */

#include <SPI.h>
//#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
int t = 0,
    ttlSec        = 3,
    lastSecond    = 0,
    currentTime   = millis(),
    wemoLavaLamp  = 0, wemoGreenFan = 1, wemoTeapot = 2, wemoWhiteFan = 3,
    wemoDelay     = 0
    ;
// Button Var's
int       buttonState =0,     QbuttonState =0;
//const int BUTTONPIN = 22;  
const int BUTTONPIN = 17; 
int       curr_T  = millis(),     lastQSec  = millis(),
          lastSec = millis(),     lastMin   = millis();  
     
bool  p_Off   = 0,              // _Off is set false
      t_On    = false,
      lightOn = false;                   // bool defaults to F or 0
//;  //set power to ON
EthernetClient client;
bool status;   //user to ensure port openned correctly
byte thisbyte; //used to get IP address

void setup() {
  //initialize ethernet port and uSD module to off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  //Open Serial Communication and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  Serial.printf("Starting Program:\n");

  //Start ethernet connection
  status = Ethernet.begin(mac);
  if (!status) {
    Serial.printf("failed to configure Ethernet using DHCP \n");
    //no point in continueing 
    while(1);
  }
      //print your local IP address
  Serial.print("My IP address: ");
  for (thisbyte = 0; thisbyte < 3; thisbyte++) {
    //print value of each byte of the IP address
    Serial.printf("%i.",Ethernet.localIP()[thisbyte]);
    }
  Serial.printf("%i\n",Ethernet.localIP()[thisbyte]);

    t=millis()/1000.0;
    Serial.printf("Initial millis is %i \n",t);
          delay(1000); 

    runWEMOck(wemoLavaLamp, 2000);
    runWEMOck(wemoGreenFan, 5000);

    // leave device  off until prog begins
    p_Off   = 0;
    ttlSec  = 3000;
    pinMode(BUTTONPIN,INPUT_PULLUP); 
    buttonState=0;        
    QbuttonState=0;
    lastSec = millis();
    t_On = false;  // or 0
}

void loop() { 
  currentTime = millis();
  if((currentTime - lastQSec)>250) {     // Update Time, Run once/per/Quarter second
    Serial.printf(";");  
    QbuttonState = digitalRead(BUTTONPIN);
        Serial.printf("QbuttonState: %i \n",QbuttonState); 
    delay(2000);
    lastQSec = millis();
//    delay(1000);
  }

//  if((curr_T - lastSec)>1000) {                 // Update Time, Run once/per/second

  buttonState = digitalRead(BUTTONPIN);
  Serial.printf("buttonState: %i \n",buttonState); 
  delay(1000);
  if(!buttonState) {            // button is in off/0 state
     //         if(!p_Off) {    // ie, power has been turned on
     switchON(0);
     lastSecond = millis() ;
  }
     
   else {
//           Serial.printf("Power has been turned off %b \n",p_Off);
     if(!t_On) {
          // wait 10 sec's and switch off
          // run constantly
          //currentTime = millis();
          //lastSecond = millis() ;
        t_On  = true;
        Serial.printf("Timer Started %i, %i, %i \n",t_On, currentTime, lastSecond);
        
     }
     else {
//      Serial.printf("Checking timer %b, %int, %int \n",t_On, currentTime, lastSecond);       
       //currentTime = millis();
       if((currentTime - lastSecond) >5000) {
          switchOFF(0);
          t_On  = false;
        }
        // run once per second
      }
           
//    t=millis()/1000.0;
//    ttlSec = t+9000;
//    for(t=t; t<= ttlSec; t=t+3000) {   // this waits 10 seconds before powering off
//                
     }
         
      p_Off=1;
}

void runWEMOck(int _WemoDev, int _wemoDelay) {
      switchOFF(wemoLavaLamp);       // make sure device is off
    // Test device ON/OFF
    switchOFF(_WemoDev);       // make sure device is off
//    delay(_wemoDelay); 
    switchON(_WemoDev);        
    delay(_wemoDelay); 
    switchOFF(_WemoDev);
}    // leave device  off until prog begins         
