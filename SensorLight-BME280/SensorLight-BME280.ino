/* program: SensorLight-BME280 adds temp sensor to inputs. I'm not sure
 *          what I will do as ouput but a soundwave would be cool :)
 * Ivan Boyd    10/26/2021
 *  
 *  Intent of program is to combine dist and temperature sensors and NeoPix programs and
 *  tie distance to LED value intensity output, ie, when object is further away
 *  the LEDs are dimmer
 *  
 *  Legend: "IB" = Infinity Box   "obj" = object    "NP" = NEOPIXEL
*/
#include <Adafruit_BME280.h>                                // temp, pressure & humidity
#include <Adafruit_NeoPixel.h>
const int   RGB_PixPin  = 3,          GRB_PixPin = 23, 
            RGB_PixCnt  = 4,          GRB_PixCnt = 15, 
            PURE_RED    = 0xFF0000;  
int   minNP = 2,          maxNP = 127,
      low = 3,            med = 65,         high = 127,         // for NP light intensity
      NPintensity   = 0,  newNPintensity,                       // var to hold mapped value from dist
      quickDel = 50,      ckDel = 1000,     longDel =  3000;    // delay presets
      
Adafruit_NeoPixel RGB_pixels(RGB_PixCnt, RGB_PixPin, NEO_RGB + NEO_KHZ800); // declare object
Adafruit_NeoPixel GRB_pixels(GRB_PixCnt, GRB_PixPin, NEO_GRB + NEO_KHZ800); // declare object

// Set up Sonar Sensor Environment
const int trigPin = 1,   echoPin = 2,   maxDist = 98, minDist = 1;      //Actual maxDist is 1198, but reduced to work with dist sensor
bool  inRange;            // use to test if something is in the range of the sensor
int   duration = 0,       distance = 0;

// Set up Adafruit_BME280 Environment for temp, pressure & humidity
Adafruit_BME280 bme;      // for the I2C device
float tempC,      tempF,    tempinHg,
      pressPA,    humidRH,  fiveMinTmp;
bool  status;
byte  hexAddress=0x76;
      // "inHg" The barometer measures pressure in 3 different units: 
      //inches of mercury ( inHg) from 0.29 to 32.48 with a resolution of 0.01, 
      //millibars (hPa) from 10.0 to 1100

void setup() {
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
}

void loop() {
//   tempC   = bme.readTemperature();  //deg C

  distance  = getDist(trigPin, duration, distance);
  inRange   = distance < maxDist && distance > minDist;
  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
  NPintensity = map(distance, maxDist, 0, 0, 127);
  Serial.printf("Mapping Dist to NPs, Dist: %i NPintensity: %i minDist: %i maxDist %i minNP %i maxNP %i  \n",distance, NPintensity, minDist, maxDist, minNP, maxNP);

//  delay(3000);
  if (inRange) {
    Serial.printf("Object has entered scanning field: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
    NPsOn(NPintensity,RGB_PixCnt, GRB_PixCnt);
  }
  else {
    NPsOff(RGB_PixCnt,GRB_PixCnt);
  }
}     // ************ END MAIN VOID LOOP *************  //

      //  -----------     FUNCTIONS      -------------  //

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
  status  = bme.begin(hexAddress);
  if(status ==  false)  {
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
  Serial.printf("Turn NP's On  GRB_PixCnt %i\n",_GRB_PixCnt);
  Serial.printf("Turn NP's On  RGB_PixCnt %i\n",_RGB_PixCnt);
  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon

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
  Serial.printf("turn NP's off, RGB: %i GRB: %i\n",_RGB_PixCnt, GRB_PixCnt);
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
