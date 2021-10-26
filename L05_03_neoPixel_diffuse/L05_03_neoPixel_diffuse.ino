/*   Project: L05_03_neoPixel
   Description: Use FOR loop and individual r/g/b to light up 16 pixels 
    with a small delay betwnn
      03 - add random color, lisght strip as rainbow, send maize & blue down strip
   Author: Ivan Boyd
   Date: 10/13/2021, 8:15AM (begin)
 */
 
 #include <Adafruit_NeoPixel.h>
 const int  PIXELPIN =   17, // Pin the NeoPix are conn'd to
            PIXELCOUNT = 16;         
        int n = 0,           // will use as pix counter in array
            red_rgb[]   = {245, 53, 10},
            green_rgb[] = {51, 190, 52},
            blue_rgb[]  = {74, 76, 136},
//            rand_rgb[]  = {rand(0,255), rand(0,255), rand(0,255)},
            rand_rgb[]  = {0,0,0},
// set rgb to color hex
            color_hex[] = {0xdf2523,0x1c8d3a,0x282aa9},  // where 1st param R, 2nd G, 3rd B
            rgb[]       = {0,1,2},
            red=0, green=1, blue=2,
            off_rgb[]   = {0,0,0},
            bri   = 0,
            randNumb  = random(0,255),
            randHex   = random(0x0000,0xFFFFFF),
            del=50   //delay 
            ;
            
Adafruit_NeoPixel pixel(PIXELCOUNT, PIXELPIN, NEO_GRB + NEO_KHZ800); // declare object
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
  pixel.begin();
  pixel.setPixelColor(n, color_hex[red]);   // Sets to my Red choice r, g, b = 0-255 
  bri = 10;
  pixel.setBrightness(bri);  
 // pixel.show();
}

void loop() {
  // Step 1, Asmnt 3 - sequence light 16 leds

  OneColorPixStream();
  clearColorPixStream();
      Serial.printf("Calling RandColorPixStreamm() n: %i PIXELCOUNT %i\n",n,PIXELCOUNT);
  RandColorPixStream();
    clearColorPixStream();
  rainbowColorPixStream();
      clearColorPixStream();


//    // Step 2, Asmnt 3 - send rand color up/back strip
//    // Do all pix's in same random color
//    for(n=0; n<PIXELCOUNT; n++) {
//    pixel.show();
//    pixel.setPixelColor(n, color_hex[randHex]);   // r, g, b = 0-255 
////    pixel.setPixelColor(n, rand_rgb[getRand(randNumb)], rand_rgb[getRand(randNumb)], rand_rgb[getRand(randNumb)]);   // r, g, b = 0-255 

    delay(del); 
  }
//  for(n=PIXELCOUNT; n>=0; n--) {          // turn off pix's in reverse order
//    pixel.show();
//    pixel.setPixelColor(n, off_rgb[0], off_rgb[1], off_rgb[2]);   // r, g, b = 0-255 
//    delay(del); 
//  }  


/* Use random() to ruturn # bet 0 & 255 for use in array to print to pix's
 *  
*/

int getRand() {
  int rando;
  rando = random(0,255);
  return rando;
}

void clearColorPixStream() {
  // Step 1, Asmnt 3 - sequence light 16 leds
    for(n=PIXELCOUNT; n>=0; n--) {
    pixel.show();
    pixel.setPixelColor(n, off_rgb[0], off_rgb[1], off_rgb[2]);   // r, g, b = 0-255 
    Serial.printf("I'm in function clearColorPixStream() n: %i PIXELCOUNT %i\n",n,PIXELCOUNT);
    delay(del); 
  }
}

void OneColorPixStream() {
  // Step 1, Asmnt 3 - sequence light 16 leds
  /*  Params: 1. cnt, pixCnt)
   */

  int cnt =0, pixCnt =  PIXELCOUNT;
  for(cnt=0; cnt<PIXELCOUNT; cnt++) {
//    pixel.show();
    pixel.setPixelColor(cnt, color_hex[blue]);   // r, g, b = 0-255
      pixel.show(); 
      delay(del); 
           Serial.printf("I'm in function OneColorPixStream() cnt: %i pixCnt %i\n",cnt,pixCnt);

  }
}

void rainbowColorPixStream() {
  // Step 1, Asmnt 3 - sequence light 16 leds
  /*  Params: 1. cnt, pixCnt)
   */
  int rainbowcolor_hex[] = {0x4B0082,0x0000FF,0x00FF00,0xFFFF00,0xff0000,0xFFA500,0x86501AF}; // where 1st param R, 2nd G, 3rd B
  int i=0, cnt =0, rainbowCnt=0, pixCnt =  PIXELCOUNT, rainbowHexCnt=0;
  for(cnt=0; cnt<PIXELCOUNT; cnt++) {                 // do this for all 16 pixels
    for(i=0;i<7;i++) {
//      (rainbowCnt=0;rainbowCnt<7; rainbowCnt++)     // for every 2 pixels show color in array then adv
        pixel.setPixelColor(cnt,rainbowcolor_hex[rainbowHexCnt]);   // r, g, b = 0-255
          pixel.show(); 
          rainbowHexCnt++;
          cnt++;
          delay(del); 
    }
  }
   
//    pixel.show();
    
           Serial.printf("I'm in function RainbowColorPixStream() cnt: %i pixCnt %i\n",cnt,pixCnt);
  }


void RandColorPixStream() {
  // Step 2, Asmnt 3 - sequence light 16 leds
  /*  Params: 1. cnt, pixCnt)
   */
  int cnt =0, pixCnt =  PIXELCOUNT;
  for(cnt=0; cnt<PIXELCOUNT; cnt++) {
  
      pixel.setPixelColor(cnt, randHex);   // r, g, b = 0-255 
      pixel.show(); 
      delay(del); 
      Serial.printf("I'm in function RandColorPixStream() cnt: %i pixCnt %i\n",cnt,pixCnt);

  }
}

// **************************left over code from pt 1
//  for(n=0; n<PIXELCOUNT; n++) {
//    pixel.show();
////    pixel.setPixelColor(n, red_rgb[0], red_rgb[1], red_rgb[2]);   // r, g, b = 0-255 
//    pixel.setPixelColor(n, color_hex[blue]);   // r, g, b = 0-255
////      pixel.setPixelColor(n, red);   // in hex
//      pixel.show(); 
//      delay(del); 

//  for(n=PIXELCOUNT; n>=0; n--) {
//    pixel.show();
//    pixel.setPixelColor(n, off_rgb[0], off_rgb[1], off_rgb[2]);   // r, g, b = 0-255 
//    delay(del); 
//  }

//  pixel.setPixelColor(n, green_rgb[0], green_rgb[1], green_rgb[2]);   // r, g, b = 0-255  
//  pixel.setPixelColor(n, blue_rgb[0], blue_rgb[1], blue_rgb[2]);   // r, g, b = 0-255
//  //
//  pixel.setPixelColor(n, rand_rgb[0], rand_rgb[1], rand_rgb[2]);   // r, g, b = 0-255
//  int cnt =0, pixCnt =  PIXELCOUNT;
//  for(cnt=0; cnt<PIXELCOUNT; cnt++) {
////    pixel.show();
//    pixel.setPixelColor(cnt, color_hex[blue]);   // r, g, b = 0-255
//      pixel.show(); 
//      delay(del); 
//           Serial.printf("I'm in function OneColorPixStream() cnt: %i pixCnt %i\n",cnt,pixCnt);
//
//  } 
