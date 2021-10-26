/*   Project: Smart Room Controller
   Description: Proj is to use requirements to interface w infinityi box
    Step 1: light a single Addafruit pixel w pin wiring from Addafruit
===>Step 2: Because the NEO Pixels are in 2 difhis program    

   *** NOTE: See Adaafruit "Use:" Notes to go back and forth between NEO_RGB 
             & NEO_GRB for class neo pix's and the ones I purchased, which
             could be put on diff pins
      03 - add random color, lisght strip as rainbow, send maize & blue down strip
   Author: Ivan Boyd
   Date: 10/25/2021, 11:50AM (begin)
   legend: IB = Infinity Box, NP = NeoPixel
 */
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
      low = 3, med = 75, high = 255,
      del=50,   ckDel = 2000      //delay
      ;
Adafruit_NeoPixel pixel_1(PIXELCOUNT, PIXELPIN_1, NEO_RGB + NEO_KHZ800); // declare object
Adafruit_NeoPixel pixel_2(PIXELCOUNT, PIXELPIN_2, NEO_GRB + NEO_KHZ800); // declare object

/* Argument 1 = # of pix's
 *          2 = GP10 pin #
 *          3 = pix type flags, add togther:
 * Use:
 *    NEO_GRB   Pix's are wired for GRB bitstr (most NeoPix prod's)
 *    other stuff from
 */ 

void setup() {
  Serial.begin(9600);    //set up a monitor
  while(!Serial);
  runNPchk();

}


void loop() {
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
  pixel_1.begin();
  pixel_2.begin();
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


//  *** OLD STUFF THAT MIGHT BE USEFULL
// pixel.setPixelColor(n, 0x0000FF);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, 0x0000FF);   // Sets to my Red choice r, g, b = 0-255 
//
//  pixel.setPixelColor(n, color_hex[red]);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, color_hex[red]);   // Sets to my Red choice r, g, b = 0-255 
//
//  pixel.setPixelColor(n, color_hex[green]);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, color_hex[green]);   // Sets to my Red choice r, g, b = 0-255 
//  pixel.setBrightness(bri);  
