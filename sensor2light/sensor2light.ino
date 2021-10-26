/* program: sensor2light
 * Ivan Boyd    10/26/2021
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
const int PIXELPIN_1 = 15, PIXELPIN_2 = 16,   PIXELCOUNT = 1; 
    int n = 0,           // will use as pix counter in array
        red_rgb[]   = {245, 53, 10},
        green_rgb[] = {51, 190, 52},
        blue_rgb[]  = {74, 76, 136},
        off_hex     = 0x000000,
        
// set rgb to color hex
      color_hex[] = {0xdf2523,0x1c8d3a,0x282aa9},  // where 1st param R, 2nd G, 3rd B
      rgb[]       = {0,1,2},
      red=0, green=1, blue=2,
      off_rgb[]   = {0,0,0},
      bri[]       = {25,100,255},
      minNP = 0,  maxNP =255,
      low = 3,    med = 75,     high = 255,          // for NP light intensity
      NPintensity   = 0, newNPintensity,       // var to hold mapped value from dist
      quickDel = 50,   ckDel = 1000, longDel =  3000      //delay
      ;
Adafruit_NeoPixel pixel_1(PIXELCOUNT, PIXELPIN_1, NEO_RGB + NEO_KHZ800); // declare object
Adafruit_NeoPixel pixel_2(PIXELCOUNT, PIXELPIN_2, NEO_GRB + NEO_KHZ800); // declare object


// Trig & Echo pins
const int trigPin = 12,   echoPin = 11,   maxDist = 1198, minDist = 1;
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
  pixel_1.begin();
  pixel_2.begin();
  runNPchk();   // Turns NP's OFF, then ON Low for ckDel(3 Sec's), 
                //ditto Med & High
  NPsOff();
  inRange = !inRange;
  
}

void loop() {
 
  distance = getDist(trigPin, duration, distance);
  inRange = distance < maxDist && distance > minDist;
  Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon

//  NPintensity = map(distance, minDist, maxDist, minNP, maxNP);
//  NPintensity = map(distance, minDist, 500, maxNP, minNP);
//    NPintensity = map(distance, 1, 500, 50, 0);
        NPintensity = map(distance, 100, 0, 0, 127);


  Serial.printf("Mapping Dist to NPs, Dist: %i NPintensity: %i minDist: %i maxDist %i minNP %i maxNP %i  \n",distance, NPintensity, minDist, maxDist, minNP, maxNP);

//  delay(3000);
  if (inRange) {
    Serial.printf("Object has entered scanning field: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon
    NPsOn(NPintensity);
//    showNPmapped(NPintensity);
//    showNPlvl(distance);

  }
  else {
    NPsOff();
  }

}           // *** END MAIN VOID LOOP ***


                  //  FUNCTIONS //

//void showNPmapped(int _NPintensity) {
//  NPsON(_NPintensity);
//}


void showNPlvl(int _distance) {
    if (_distance > 700) {
      NPsOn(low);
    }
    if (_distance > 200 && _distance < 700) {
      NPsOn(med);
    }
    if (_distance < 200) {
      NPsOn(high);
    }
}

void runNPchk() {       // Turns NP's OFF, then ON Low for ckDel(3 Sec's), ditto Med & High
    NPsOff();
    delay(1000);
    NPsOn(low);
    delay(ckDel);
    NPsOff();
    NPsOn(med);
    delay(ckDel);
    NPsOff(); 
    NPsOn(high);
    delay(ckDel);
    NPsOff();
}

void NPsOn(int _bri) {  // Turns All NP's On brightness level set a parameter
 // n is pix# being set
  Serial.printf("Turn NP's On n: %i PIXELCOUNT %i\n",n,PIXELCOUNT);
    Serial.printf("inRange: %i, Distance = %icm \n", inRange, distance);   // Print dist to Serial Mon


//  bri = 100;
  pixel_1.setBrightness(_bri); 
  pixel_2.setBrightness(_bri);  
  pixel_1.setPixelColor(0, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
  pixel_2.setPixelColor(0, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
//  delay(500);
  pixel_1.show(); 
  pixel_2.show(); 
}

void NPsOff() {         // Turns All NP's Off ()x000000
  Serial.printf("turn NP's off, n: %i PIXELCOUNT %i\n",n,PIXELCOUNT);
  pixel_1.setPixelColor(0, 0x000000);   //  Turn off pixels
  pixel_2.setPixelColor(0, 0x000000);   // Turn off pixels
  pixel_1.show(); 
  pixel_2.show(); 
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
