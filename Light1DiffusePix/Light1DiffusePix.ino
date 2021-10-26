/*   Project: Smart Room Controller
   Description: Proj is to use requirements to interface w infinityi box
    Step 1: light a single Addafruit pixel w pin wiring from Addafruit

   *** NOTE: See Adaafruit "Use:" Notes to go back and forth between NEO_RGB 
             & NEO_GRB for class neo pix's and the ones I purchased, which
             could be put on diff pins
      03 - add random color, lisght strip as rainbow, send maize & blue down strip
   Author: Ivan Boyd
   Date: 10/25/2021, 11:50AM (begin)
 */
#include <Adafruit_NeoPixel.h>
const int PIXELPIN_1 = 15, PIXELCOUNT = 2; 
    int n = 0,           // will use as pix counter in array
        red_rgb[]   = {245, 53, 10},
        green_rgb[] = {51, 190, 52},
        blue_rgb[]  = {74, 76, 136},
        
// set rgb to color hex
      color_hex[] = {0xdf2523,0x1c8d3a,0x282aa9},  // where 1st param R, 2nd G, 3rd B
      rgb[]       = {0,1,2},
      red=0, green=1, blue=2,
      off_rgb[]   = {0,0,0},
      bri   = 0,
      del=50   //delay
      ;
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN_1, NEO_RGB + NEO_KHZ800); // declare object
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
  // n is pix# being set
  Serial.printf("Begin Set Up n: %i PIXELCOUNT %i\n",n,PIXELCOUNT);

  pixel.begin();
    bri = 100;
    pixel.setBrightness(bri);  

  pixel.setPixelColor(n, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
    pixel.setPixelColor(1, 0xFF0000);   // Sets to my Red choice r, g, b = 0-255 
delay(500);
// pixel.setPixelColor(n, 0x0000FF);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, 0x0000FF);   // Sets to my Red choice r, g, b = 0-255 
//
//  pixel.setPixelColor(n, color_hex[red]);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, color_hex[red]);   // Sets to my Red choice r, g, b = 0-255 
//
//  pixel.setPixelColor(n, color_hex[green]);   // Sets to my Red choice r, g, b = 0-255 
//    pixel.setPixelColor(1, color_hex[green]);   // Sets to my Red choice r, g, b = 0-255 
//  pixel.setBrightness(bri);  
  pixel.show(); 

}

void loop() {
  


}
