/*   Project: L05_01_neoPixel
   Description: Use FOR loop and individual r/g/b to light up 16 pixels 
    with a small delay betwnn
   Author: Ivan Boyd
   Date: 10/12/2021, 2:38AM (begin)
 */
 
 #include <Adafruit_NeoPixel.h>
 const int  PIXELPIN =   17, // Pin the NeoPix are conn'd to
            PIXELCOUNT = 16;         
        int n = 0,           // will use as pix counter in array
            red_rgb[]   = {245, 53, 10},
            green_rgb[] = {51, 190, 52},
            blue_rgb[]  = {74, 76, 136},
            off_rgb[]   = {0, 0, 0},
            bri = 0     
            ;
            
            //this color is #f5350a
            //green_rgb[]
 
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800); // declare object
/* Argument 1 = # of pix's
 *          2 = GP10 pin #
 *          3 = pix type flags, add togther:
 * Use:
 *    NEO_GRB   Pix's are wired for GRB bitstr (most NeoPix prod's)
 *    other stuff from
 */

void setup() {  
  // n is pix# being set
  pixel.begin();
  pixel.setPixelColor(n, red_rgb[0], red_rgb[1], red_rgb[2]);   // r, g, b = 0-255 
  pixel.setPixelColor(n, green_rgb[0], green_rgb[1], green_rgb[2]);   // r, g, b = 0-255  
  pixel.setPixelColor(n, blue_rgb[0], blue_rgb[1], blue_rgb[2]);   // r, g, b = 0-255
  bri = 10;
  pixel.setBrightness(bri);  
 // pixel.show();

}

void loop() {
  // put your main code here, to run repeatedly:
  for(n=0; n<PIXELCOUNT; n++) {
    pixel.show();
    pixel.setPixelColor(n, red_rgb[0], red_rgb[1], red_rgb[2]);   // r, g, b = 0-255 
    delay(250); 
  }
  for(n=PIXELCOUNT; n>=0; n--) {
    pixel.show();
    pixel.setPixelColor(n, off_rgb[0], off_rgb[1], off_rgb[2]);   // r, g, b = 0-255 
    delay(250); 
  }
  
}
