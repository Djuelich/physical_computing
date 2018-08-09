#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#define PIN 6
#define NUM_LEDS 60


// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

const int buttonPinLed = 2;
const int buttonPinServo = 4;

int buttonStateLed = 0; 
int buttoncurrentLed = 0;

int buttonStateServo = 0; 
int buttoncurrentServo = 0;

Servo servoA;
Servo servoB;
int pos = 50;


void setup() {
  Serial.begin(9600);
  // initialize the button pin as a input:
  pinMode(buttonPinLed, INPUT);
  pinMode(buttonPinServo, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  servoA.attach(9);
  servoB.attach(10);
  servoA.write(50);
  servoB.write(50);
}


void loop() {
  // Get current button state.
  buttonStateLed = digitalRead(buttonPinLed);

  if (buttonStateLed == HIGH) {  
    buttoncurrentLed++;
  }   


  switch(buttoncurrentLed % 2){
  case 0: 
    FadeInOut(0x00, 0x00, 0xff);    // Blue/breathing
    Serial.println("Fade");
    break;
  case 1: 
    Strobe(0x00, 0x00, 0xff, 10, 50, 50);  // Blue
    rainbowCycleStrobe(0,10,50,50); //Strobe in all colors
    Serial.println("Strobe");
    break;
  }

  /** Testarea, multiple methods not in use. maybe later
   * 
   * FadeInOut(0x00, 0x00, 0xff); 
   * Serial.println("pushed");
   * }
   * else {
   * Strobe(0x00, 0x00, 0xff, 10, 50, 50);
   * Serial.println("not_pushed"); 
   * }
   * colorWipe(strip.Color(0, 0, 255), 50); // Blue;
   * FadeInOut(0x00, 0x00, 0xff);
   * theaterChase(strip.Color(0, 0, 127), 50); // Blue
   * theaterChaseRainbow(500);
   * Strobe(0x00, 0x00, 0xff, 10, 50, 50);
   * 
   **/

  buttonStateServo = digitalRead(buttonPinServo);

  if (buttonStateServo == HIGH) {  
    buttoncurrentServo++;
  }   


  switch(buttoncurrentServo % 2){
  case 0: 
    for(int pos = 80;pos < 150; pos++)
    {
      servoA.write(pos);
      servoB.write(252 - pos);
      delay(25);
    }
    Serial.println("Up");
    break;
  case 1: 
    for(int pos = 150;pos > 80; pos--)
    {
      servoA.write(pos);
      servoB.write(252 - pos);
      delay(25);

    }
    Serial.println("down");
    break;
  }
  /**
   * for(int pos = 80;pos < 150; pos++)
   * {
   * servoA.write(pos);
   * servoB.write(252 - pos);
   * delay(25);
   * }
   * for(int pos = 150;pos > 80; pos--)
   * {
   * servoA.write(pos);
   * servoB.write(252 - pos);
   * delay(25);
   * 
   * }
   **/
}

//method for using Adafruit and FASTLED
void showStrip() {    
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

//methode for setting pixelcolor
void setPixel(int Pixel, byte red, byte green, byte blue) { 
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

//breathing color
void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;

  for(float k = 20; k < 256; k=k+0.75) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }

  for(float k = 255; k >= 20; k=k-0.75) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {


  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//method for party-Strobe
void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){

  for(int j = 0; j < StrobeCount; j++) {

    setAll(red,green,blue);

    showStrip();

    delay(FlashDelay);

    setAll(0,0,0);

    showStrip();

    delay(FlashDelay);

  }

  delay(EndPause);

}

//party-Strobe in all colors
void rainbowCycleStrobe(uint8_t wait,int StrobeCount, int FlashDelay, int EndPause ) {
  uint16_t i, j;
  for(int a = 0; a < StrobeCount; a++) {
    for(j=0; j<256*1; j += 10) { // 1 cycles of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) &
          255));
      }
      strip.show();
      delay(FlashDelay);
      setAll(0,0,0);

      showStrip();

      delay(FlashDelay);
    }
    delay(EndPause);
  }
}

/**  button test
 * 
 * void startShow(int i) {
 * switch(i % 2){
 * case 0: //FadeInOut(0x00, 0x00, 0xff);    // Black/off
 * Serial.println("Fade");
 * break;
 * case 1: //Strobe(0x00, 0x00, 0xff, 10, 50, 50);  // Red
 * Serial.println("Strobe");
 * break;
 * 
 * }
 * }
 * 
 **/

