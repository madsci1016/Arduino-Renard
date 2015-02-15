/*************************************************
*
*	Renard Library Example
*
*		This example will act as a pixel controller for 50 
*			ws2811 pixels connected to pin 6 on the Arduino.
*			It was tested on an Arduino Mega with a FTDI
*			adapter connected to Serial3 providing the Reanrd
*			data from Vixen. 
*
*        FYI the Vixen Renard plugin does something funny to the reset
*			line on the built in USB serial port on Arduino, which
*			is why I used an external FTDI adapter 
*
*************************************************/



#include <Renard.h>
#include <Adafruit_NeoPixel.h>


#define DATAPIN 6
#define PIXELS 50
#define CHANNELS 150 //3 per pixel

//Use the Adafruit Neopixel library to speak ws2811 to pixels
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, DATAPIN, NEO_RGB + NEO_KHZ800);
//use Renard library to receive data from the serial port
Renard renard;
//create an array to hold the brightness values for each channel
byte values[CHANNELS];

void setup() {
  //start renard library
  //begin(address of buffer, size of that buffer, serial port, baud)
  renard.begin((uint8_t*)&values, CHANNELS, &Serial3, 57600);
  //start pixel library
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop() {
  //check to see if we got a new set of values from the Renard library         
  if( renard.receive()) {
    //pass the values to the pixel library
    for(int i=0; i<PIXELS; i++)
           strip.setPixelColor(i, strip.Color(values[i*3], values[i*3+1], values[i*3+2]));
    //push the values out to the pixel strip
    strip.show();
  }
}


