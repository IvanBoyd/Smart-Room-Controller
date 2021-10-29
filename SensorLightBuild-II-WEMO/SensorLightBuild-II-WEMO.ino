/* program: SensorLight-BME280-Timer adds Timer to temp sensor inputs. I'm not sure
 *          what I will do as ouput but a soundwave would be cool :)
 * Ivan Boyd    10/26/2021
 *  Program MidTerm Cklist:  H-W-O-3C-2M-3D-FM-XC  ... Upper Case = done, lower case = !done
 *  N-(H)All Hue ligths, N-(W) 2 Wemo, Y-(O)dynamic OLED msgs, Y-(3C)Min 3 add components,
 *  Y-(2M)2 Modes (auto, manual), N-(3D)3D Design & Print 1 part, Y-(FM)a FUSE made component,
 *  .5?-(XC)X/C use 1 component not from class (diffuse NeoPix's
 *  *Intent of program is to combine dist and temperature sensors and NeoPix programs and
 *  tie distance to LED value intensity output, ie, when object is further away
 *  the LEDs are dimmer, while meeting all obj's of the assignment.
 *  
 *  Legend: "IB" = Infinity Box   "obj" = object    "NP" = NEOPIXEL
*/
#include <Adafruit_BME280.h>                // temp, pressure & humidity
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>               // OLED display
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C   // This addr is from our (IoT) i2C scanner program

const int   RGB_PixPin  = 3,          GRB_PixPin = 23, 
            RGB_PixCnt  = 4,          GRB_PixCnt = 15, 
            PURE_RED    = 0xFF0000;  
int   minNP = 2,          maxNP = 127,
      low = 3,            med = 65,         high = 127,         // for NP light intensity
      NPintensity   = 0,  newNPintensity,                       // var to hold mapped value from dist
      quickDel = 50,      ckDel = 1000,     longDel =  3000;    // delay presets
      
Adafruit_NeoPixel RGB_pixels(RGB_PixCnt, RGB_PixPin, NEO_RGB + NEO_KHZ800); // declare object
Adafruit_NeoPixel GRB_pixels(GRB_PixCnt, GRB_PixPin, NEO_GRB + NEO_KHZ800); // declare object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Set up Sonar Sensor Environment
const int trigPin = 1,   echoPin = 2,   maxDist = 98, minDist = 1;      //Actual maxDist is 1198, but reduced to work with dist sensor
bool  inRange;            // use to test if something is in the range of the sensor
int   duration = 0,       distance = 0;

// Set up Adafruit_BME280 Environment for temp, pressure & humidity
Adafruit_BME280 bme;      // for the I2C device
float tempC,      tempF,    tempinHg,
      pressPA,    humidRH,  fiveMinAvg;
bool  bmeStatus;         
bool  eStatus;
byte  hexAddress=0x76;
      // "inHg" The barometer measures pressure in 3 different units: 
      //inches of mercury ( inHg) from 0.29 to 32.48 with a resolution of 0.01, 
      //millibars (hPa) from 10.0 to 1100
//  Time Vars
int   curr_T  = millis(),   lastSec     = millis(),   lastMin     = millis(),  
      minCnt  = 0,          i           = 0,          t           = 0,
      ttlSec  = 3,          lastSecond  = 0,          currentTime = millis(),
      lastQSec= millis();
float fiveMinTemp[5] = {0.0,0.0,0.0,0.0,0.0};
bool  first5minTemp;
//  WEMO Vars
int   wemoLavaLamp  = 0, wemoGreenFan = 1, wemoTeapot = 2, wemoWhiteFan = 3,
      wemoDelay     = 0;
bool  p_Off   = 0,              // _Off is set false
      t_On    = false,
      lightOn = false;                   // bool defaults to F or 0
// Button Var's
int       buttonState =0,     QbuttonState =0;
const int Y_BUTTONPIN = 8;
EthernetClient client;
//bool eStatus;                    //used to ensure port openned correctly
byte thisbyte;                  //used to get IP address


void setup() {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    for(;;); // Don't proceed, loop forever
  }
    //initialize ethernet port and uSD module to off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  RGB_pixels.begin();
  GRB_pixels.begin();
  runPRTchk();        // Start print to monitor
  runBMEchk();        // check temp module BME280
  runNPchk();   // Turns NP's OFF, then ON Low Intensity for ckDel length(3 Sec's), 
                // ditto Med & High LED intensity
  NPsOff(RGB_PixCnt,GRB_PixCnt);        // Set all NeoPix's to off
  inRange = !inRange;                   // bool var for if obj has entered my region to search
  lastMin = 0;       
  first5minTemp = true;
  fiveMinAvg    = CtoF(bme.readTemperature());
  i = 0;
  Serial.printf("Starting Program:\n");

  //Start ethernet connection
  eStatus = Ethernet.begin(mac);
  if (!eStatus) {
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
    runWEMOck(wemoGreenFan, 2000);
    lightOn = false;
    // leave device  off until prog begins
    p_Off   = 0;
    ttlSec  = 3000;
    pinMode(Y_BUTTONPIN,INPUT_PULLUP); 
    buttonState=0;        
    QbuttonState=0;
    lastSec = millis();
    t_On = false;  // or 0
    lastQSec  = millis();
    delay(170);
}

void loop() {
  curr_T   = millis();                          // Run Constantly 
  currentTime = millis();  
//Serial.printf("QbuttonState: % i curr_T: %i lastQSec: %i  \n",QbuttonState, curr_T,lastQSec); 
//delay(1000);
  if((currentTime - lastQSec)>150) {     // Update Time, Run once/per/Quarter second
    Serial.printf("IF Qsec");  
    QbuttonState = digitalRead(Y_BUTTONPIN);
    Serial.printf("QbuttonState: %i \n",QbuttonState); 
//    delay(1000);
    if(!lightOn && QbuttonState) {      //  button was pressed and light is off
        switchON(0); 
        lightOn = true;       
    }
    delay(160);
    QbuttonState = digitalRead(Y_BUTTONPIN);
    if(lightOn && QbuttonState) {
        switchOFF(0); 
        lightOn = false;         
    }
    lastQSec = millis();
  }
if((curr_T - lastSec)>1000) {                 // Update Time, Run once/per/second
  Serial.printf(".");     lastSec = millis();     
  tempF   = CtoF(bme.readTemperature());    // read temp and call CtoF function
//    Serial.printf("Temperature by Second: %f \n",tempF);
//    delay(500);
  while(CtoF(bme.readTemperature()) > fiveMinAvg +1) {
    strobeNPs();
    delay(14);
  }
}

if((curr_T-lastMin)>60000)  {           // Update time 1x/min 
    Serial.printf("-");
    tempF   = CtoF(bme.readTemperature());
    Serial.printf("Temperature by Minute: %f \n",tempF);
    if(first5minTemp) {        // load 1st five temps to 5 min temp array to average
      fiveMinTemp[i] = CtoF(bme.readTemperature());
      Serial.printf("--- loading 1st 5 Min Temp array[%i]: %f ---\n",i,fiveMinTemp[i]);
      i++;
      }
    if(i==5 && first5minTemp) {     // 1st fiveMinTemp array full, so avg it
        i = 0;                      // i will stay 0 and not trigger this function
        first5minTemp = false;      // Do Not re-enter this if loop again
        fiveMinAvg =  (fiveMinTemp[0] + fiveMinTemp[1] +fiveMinTemp[2] 
                      + fiveMinTemp[3] + fiveMinTemp[4]) / 5;
        Serial.printf("*** FIRST Five Min Temp Average: %f ***\n",fiveMinAvg);
        delay(3000);           
     } 
   if(!first5minTemp) {
      Serial.printf("*** CYCLELING Five Min Temp Average: %f ***\n",fiveMinAvg);
       for(i=0; i<4; i++) {
          fiveMinTemp[i]= fiveMinTemp[i+1];
       }
//      fiveMinTemp[0] = fiveMinTemp[1];
//      fiveMinTemp[1] = fiveMinTemp[2];
//      fiveMinTemp[2] = fiveMinTemp[3];
//      fiveMinTemp[3] = fiveMinTemp[4]; 
      fiveMinTemp[4] = CtoF(bme.readTemperature());
      fiveMinAvg =  (fiveMinTemp[0] + fiveMinTemp[1] +fiveMinTemp[2] 
                      + fiveMinTemp[3] + fiveMinTemp[4]) / 5;
      Serial.printf("tmp1: %f tmp2: %f tmp3: %f tmp4: %f: tmp5: %f\n",fiveMinTemp[0],fiveMinTemp[1],fiveMinTemp[2] 
                      ,fiveMinTemp[3],fiveMinTemp[4]);
      delay(3000);   
     }
    lastMin = millis();
    delay(500);
    minCnt++;
  }

  distance  = getDist(trigPin, duration, distance);
  inRange   = distance < maxDist && distance > minDist;
//  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
  NPintensity = map(distance, maxDist, 0, 0, 127);
//  Serial.printf("Mapping Dist to NPs, Dist: %i NPintensity: %i minDist: %i maxDist %i minNP %i maxNP %i  \n",distance, NPintensity, minDist, maxDist, minNP, maxNP);

//  delay(3000);
  if (inRange) {
    Serial.printf("Object has entered scanning field: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
    NPsOn(NPintensity,RGB_PixCnt, GRB_PixCnt);
  }
  else {
    NPsOff(RGB_PixCnt,GRB_PixCnt);
  }

  display.clearDisplay();             //    PRINT TO OLED SSD 1306
//  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 1);
  display.printf("tempF: %f \n1st 5 Min Tmp: %i\n5 Min Avg %f",tempF, first5minTemp,fiveMinAvg);
  display.display();      // Show initial text
  delay(100);
  
}     // ************ END MAIN VOID LOOP *************  //


      //  -----------     FUNCTIONS      -------------  //

void runWEMOck(int _WemoDev, int _wemoDelay) {
      switchOFF(wemoLavaLamp);       // make sure device is off
    // Test device ON/OFF
    switchOFF(_WemoDev);       // make sure device is off
//    delay(_wemoDelay); 
    switchON(_WemoDev);        
    delay(_wemoDelay); 
    switchOFF(_WemoDev);
} 

void strobeNPs() {
//    Serial.printf("*** Just pretend that the NP's are a strobbing\n");
//    delay(3000);
    GRB_NPsOn(low, GRB_PixCnt);
//    delay(10);
    GRB_NPsOff(low, GRB_PixCnt);
}

void GRB_NPsOn(int _bri, int _GRB_PixCnt) {  // Turns All NP's On brightness level set a parameter
  int i;
//  Serial.printf("Turn NP's On  GRB_PixCnt %i\n",_GRB_PixCnt);
//  Serial.printf("Turn NP's On  RGB_PixCnt %i\n",_RGB_PixCnt);
//  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
  GRB_pixels.setBrightness(_bri);  
  for (i = 0; i < _GRB_PixCnt; i++) {
    GRB_pixels.setPixelColor(i, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
  }   
//    delay(20);
  GRB_pixels.show(); 
}

void GRB_NPsOff(int _bri, int _GRB_PixCnt) {  // Turns All NP's On brightness level set a parameter
  int i;
//  Serial.printf("Turn NP's On  GRB_PixCnt %i\n",_GRB_PixCnt);
//  Serial.printf("Turn NP's On  RGB_PixCnt %i\n",_RGB_PixCnt);
//  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
  GRB_pixels.setBrightness(_bri);  
  for (i = 0; i < _GRB_PixCnt; i++) {
    GRB_pixels.setPixelColor(i, 0x000000);   // Sets to my Red choice r, g, b = 0-255 
  }   
//    delay(20);
  GRB_pixels.show(); 
}

float CtoF(float _tempC) {     //Convert Celcius to Fahrenheit °F = (°C × 9/5) + 32//
  float _tempF;
  _tempF = (_tempC * 9/5) +32;
  return _tempF;
}

void runPRTchk() {               // Start print to monitor
  Serial.begin(9600);
  while(!Serial);
  Serial.printf("Printer initialized\n");
  delay(1000);
}

void runBMEchk()  {         // check status of BME/temp and send to print
  bmeStatus  = bme.begin(hexAddress);
  if(bmeStatus ==  false)  {
    Serial.printf("BME280 at address 0x%02X failed to start\n", hexAddress);
    delay(4000);
   }
   else {
    tempC   = bme.readTemperature();  //deg C
    tempF = CtoF(tempC);
    Serial.printf("Temperature at startup: %f \n",tempF);
    delay(4000);
   }
}
void runNPchk() {       // Turns NP's OFF, then ON Low for ckDel(3 Sec's), ditto Med & High
    NPsOff(RGB_PixCnt,GRB_PixCnt);
    delay(1000);
    NPsOn(low, RGB_PixCnt, GRB_PixCnt);
    delay(ckDel);
    NPsOff(RGB_PixCnt,GRB_PixCnt);
    NPsOn(med, RGB_PixCnt, GRB_PixCnt);
    delay(ckDel);
    NPsOff(RGB_PixCnt,GRB_PixCnt); 
    NPsOn(high, RGB_PixCnt, GRB_PixCnt);
    delay(ckDel);
    NPsOff(RGB_PixCnt,GRB_PixCnt);
}

void NPsOn(int _bri,int _RGB_PixCnt, int _GRB_PixCnt) {  // Turns All NP's On brightness level set a parameter
  int i;
//  Serial.printf("Turn NP's On  GRB_PixCnt %i\n",_GRB_PixCnt);
//  Serial.printf("Turn NP's On  RGB_PixCnt %i\n",_RGB_PixCnt);
//  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon

  RGB_pixels.setBrightness(_bri); 
  GRB_pixels.setBrightness(_bri);  
  for (i = 0; i < _RGB_PixCnt; i++) {
    RGB_pixels.setPixelColor(i, PURE_RED);   // Sets to my Red choice r, g, b = 0-255 
  }
    RGB_pixels.show(); 
  for (i = 0; i < _GRB_PixCnt; i++) {
    GRB_pixels.setPixelColor(i, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
  }   
//    delay(500);
  GRB_pixels.show(); 
}

void NPsOff(int _RGB_PixCnt, int _GRB_PixCnt) {         // Turns All NP's Off ()x000000
  int i;
//  Serial.printf("turn NP's off, RGB: %i GRB: %i\n",_RGB_PixCnt, GRB_PixCnt);
  for (i = 0; i < _RGB_PixCnt; i++) {
    RGB_pixels.setPixelColor(i, 0x000000);   //  Turn off pixels
  }
  for (i = 0; i < _GRB_PixCnt; i++) {
    GRB_pixels.setPixelColor(i, 0x000000);   //  Turn off pixels
  }
  RGB_pixels.show(); 
  GRB_pixels.show(); 
}
              
int getDist(int _trigPin,int _duration,int _distance) {
 // Clear trigPin/Set LOW
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(5);

  //trigger sensor by setting tripPin high for 10 microseconds
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  // Read the echoPin, pulseIn() returns the dur length of pulse
  //  in microseconds.
  _duration = pulseIn(echoPin, HIGH);
  // Calculate the dist:
  _distance = _duration *0.034 / 2;
  return _distance;
}
      //  -----------   END FUNCTIONS      -------------  //

      //  -----------   PROGRAM HISTORY      -------------  //
/* History, Past programs and code leading to this program: 
 *  - L10_03_BME280_SDMicro-- has a full use implementation of all of the BME280 functions
 *      including pressure, humidity and a Centigrade to Farenhite function
 *  -sensor2lightBB -- Same as sensor2light but moving to the Big BreadBoard and changing
 *    pins, adding NP's, etc, to make it work.
 * - Light1DiffusePIX-2pin ... sets up Adafruit libs to call both RGB & GRB 
 *    NEOPIXELS
 * - HC-SR04-DistSensor-function   ... sets up distance sensor and prints in cm
 *  From sample code on using the HC-SR04
*/ 
       //  ---------   END PROGRAM HISTORY      -------------  //
  
//************  OLD STUFF that prob should be ditched *******************
//void showNPmapped(int _NPintensity) {
//  NPsOn(int _bri,int _RGB_PixCnt, int _GRB_PixCnt);
//}

//void showNPlvl(int _distance) {
//    if (_distance > 700) {
//      NPsOn(low);
//    }
//    if (_distance > 200 && _distance < 700) {
//      NPsOn(med);
//    }
//    if (_distance < 200) {
//      NPsOn(high);
//    }
//}

//  NPintensity = map(distance, minDist, maxDist, minNP, maxNP);
//  NPintensity = map(distance, minDist, 500, maxNP, minNP);
//    NPintensity = map(distance, 1, 500, 50, 0);
//        NPintensity = map(distance, 100, 0, 0, 127);
