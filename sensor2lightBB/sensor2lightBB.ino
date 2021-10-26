/* program: sensor2lightBB
 * Ivan Boyd    10/26/2021
 *  Same as sensor2light but moving to the Big BreadBoard and changing
 *    pins, adding NP's, etc, to make it work.
 *  Intent of program is to combine dist-sensor and NeoPix programs and
 *  tie distance to led value intensity, ie, when object is further away
 *  the LEDs are dimmer
 *  
 *  History:
 * - Light1DiffusePIX-2pin ... sets up Adafruit libs to call both RGB & GRB 
 *    NEOPIXELS
 * - HC-SR04-DistSensor-function   ... sets up distance sensor and prints in cm
 *  From sample code on using the HC-SR04
 *  
 *  Legend: "IB" = Infinity Box   "obj" = object    "NP" = NEOPIXEL
*/
//  Set up Adafruit Environment
#include <Adafruit_NeoPixel.h>
const int RGB_PixPin = 3,   GRB_PixPin = 23, 
          RGB_PixCnt = 4,   GRB_PixCnt = 15, PIXELCOUNT = 0;      //GRB   PIXELPIN_1 now RGB_PixPin,PIXELPIN_2 = 16
    int n = 0,           // will use as pix counter in array
        red_rgb[]   = {245, 53, 10},
        green_rgb[] = {51, 190, 52},
        blue_rgb[]  = {74, 76, 136},
        pixCnt      = 0,
        off_hex     = 0x000000,
        
// set rgb to color hex
      color_hex[] = {0xdf2523,0x1c8d3a,0x282aa9},  // where 1st param R, 2nd G, 3rd B
      rgb[]       = {0,1,2},
      red=0, green=1, blue=2,
      off_rgb[]   = {0,0,0},
      bri[]       = {25,100,255},
      minNP = 2,  maxNP = 127,
      low = 3,    med = 65,     high = 127,          // for NP light intensity
      NPintensity   = 0, newNPintensity,       // var to hold mapped value from dist
      quickDel = 50,   ckDel = 1000, longDel =  3000      //delay
      ;
Adafruit_NeoPixel RGB_pixels(PIXELCOUNT, RGB_PixPin, NEO_RGB + NEO_KHZ800); // declare object
Adafruit_NeoPixel GRB_pixels(GRB_PixCnt, GRB_PixPin, NEO_GRB + NEO_KHZ800); // declare object
// pixel_1, now RGB_pixels, pixel_2 now GRB_pixels

// Trig & Echo pins
const int trigPin = 1,   echoPin = 2,   maxDist = 98, minDist = 1;      //Actual maxDist is 1198, but reduced to work with dist sensor
bool  inRange;            // use to test if something is in the range
                          // of the sensor

int duration = 0, distance = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.printf("Printer initialized\n");
  delay(500);
  RGB_pixels.begin();
  GRB_pixels.begin();
  runNPchk();   // Turns NP's OFF, then ON Low Intensity for ckDel length(3 Sec's), 
                // ditto Med & High LED intensity
  NPsOff(RGB_PixCnt,GRB_PixCnt);
  //void NPsOff(int _RGB_PixCnt, int _GRB_PixCnt) 
  inRange = !inRange;
//  minDist = 1;   // these two are constant, del theselines         
//  maxDist = 98;
  
}

void loop() {
 
  distance = getDist(trigPin, duration, distance);
  inRange = distance < maxDist && distance > minDist;
  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon

//  NPintensity = map(distance, minDist, maxDist, minNP, maxNP);
//  NPintensity = map(distance, minDist, 500, maxNP, minNP);
//    NPintensity = map(distance, 1, 500, 50, 0);
//        NPintensity = map(distance, 100, 0, 0, 127);
    NPintensity = map(distance, maxDist, 0, 0, 127);

  Serial.printf("Mapping Dist to NPs, Dist: %i NPintensity: %i minDist: %i maxDist %i minNP %i maxNP %i  \n",distance, NPintensity, minDist, maxDist, minNP, maxNP);

//  delay(3000);
  if (inRange) {
    Serial.printf("Object has entered scanning field: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
    NPsOn(NPintensity,RGB_PixCnt, GRB_PixCnt);
//    showNPmapped(NPintensity);
//    showNPlvl(distance);

  }
  else {
    NPsOff(RGB_PixCnt,GRB_PixCnt);
  }

}           // ************ END MAIN VOID LOOP *************


                  //  FUNCTIONS //

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
                                                         // n is pix# being set
  int i;
  Serial.printf("Turn NP's On n: %i GRB_PixCnt %i\n",n,GRB_PixCnt);
  Serial.printf("Turn NP's On n: %i RGB_PixCnt %i\n",n,RGB_PixCnt);
  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon

//  bri = 100;
  RGB_pixels.setBrightness(_bri); 
  GRB_pixels.setBrightness(_bri);  
  for (i = 0; i < _RGB_PixCnt; i++) {
    RGB_pixels.setPixelColor(i, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
  }
  for (i = 0; i < _GRB_PixCnt; i++) {
    GRB_pixels.setPixelColor(i, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 

  }   
  //  delay(500);
  RGB_pixels.show(); 
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
//  GRB_pixels.setPixelColor(0, 0x000000);   // Turn off pixels
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
