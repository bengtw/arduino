
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 80

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

int usedPins[] = {0, 2, 4, 6, 16, 18, 20, 22, 32, 34, 36, 38};
int directions[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};

//PRIMEFADES
int currentBrightness[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long fadeHues[] = {43650, 21845, 65500 }; //BLUE, GREEN, RED

//BLUEGREENFADE
long maxHue = 43000;
long minHue = 33000;
long currentHue[12];
int h_directions[12];


int state = 0; //SET LATER WITH RANDOM

int numberOfAnimations = 6;
//  1 : BLUEPILLARS
//  2 : GREENPILLARS
//  3 : REDPILLARS
//  4 : WHITEPILLARS
//  5 : RGB-FADE
//  6 : green-blue fade

int adjustor = 1;
int startCounter = 0;
int waitms = 250;

int currentColorIndex = 0;

const byte interruptPin = 2; //push button attached to this pin


long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 250;    // the debounce time; increase if the output flickers

// setup() function -- runs once at startup --------------------------------

void setup() {
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(interruptPin, INPUT_PULLUP); //PUSHBUTTON!

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(interruptPin), stateChangeInterruptHandler, CHANGE);
  randomSeed(analogRead(0));


  for (int i = 0; i < 12; i++) {
    currentHue[i] = random(minHue, maxHue);
  }


  for (int x = 0; x < 12; x++) {

    h_directions[x] = 0;
    while ( h_directions[x] == 0) {
      h_directions[x] = random(-4, 4);
    }
  }


  wipe();

  state = 6;  //



}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

  if (state < 5) { /// HUE FADING UP AND DOWN ON RANDOM PILLARS
    for (int i = 0; i < 12; i++) { // For each pixel in strip...
      if (state > 4) { //IF WE HAVE CHANGED STATE, DON'T COMPLETE THE LOOP
        break;
      }
      //Get current Brightness-value of pixel
      currentBrightness[i] = currentBrightness[i] + directions[i];
      if (currentBrightness[i] >= 155) {
        currentBrightness[i] = 155;
        directions[i] = -adjustor; //START GOING BACK DOWN TO ZERO!
      }
      if (currentBrightness[i] <= 5) {
        currentBrightness[i] = 5;
        directions[i] = 0;
      }
      int saturation = 0;
      if (state < 4) {
        saturation = 255;
      }
      //      strip.setPixelColor(usedPins[i], strip.gamma32(strip.ColorHSV(fadeHues[state-1],saturation,currentBrightness[i]))) ;         //  Set pixel's color (in RAM)
      strip.setPixelColor(usedPins[i], strip.ColorHSV(fadeHues[state - 1], saturation, currentBrightness[i])) ;     //  Set pixel's color (in RAM)
      strip.setPixelColor(usedPins[i]+40, strip.ColorHSV(fadeHues[state - 1], saturation, currentBrightness[i])) ;     //  Set pixel's color (in RAM)
    }

    strip.show();                        //  Update strip to match
    delay(60);                           //  Pause for a moment

    startCounter++;

    if ( random(50) == 1) {
      startCounter = 0;
      directions[random(12)] = adjustor;
    }
  }

  if (state == 5) { ///RGB-FADE ///
    for (int i = 0; i < 12; i++) {
      strip.setPixelColor(usedPins[i], (Wheel((i + currentColorIndex) & 255)));
    }

    if (state != 5) { //IF WE HAVE CHANGED STATE, DON'T COMPLETE THE FADE
      wipe();
    }

    strip.show(); // Update strip with new contents
    delay(100);  // Pause for a moment

    currentColorIndex++;
    if (currentColorIndex > 256) {
      currentColorIndex = 0;
    }
  }



  if (state  == 6) { /// BLUE/LIGHTBLUE FADE
    for (int i = 0; i < 12; i++) { // For each pixel in strip...
      if (state != 6) { //IF WE HAVE CHANGED STATE, DON'T COMPLETE THE LOOP
        break;
      }
      //Get current Brightness-value of pixel
      currentHue[i] = currentHue[i] - h_directions[i] * 20;
      if (currentHue[i] <= minHue) {
        currentHue[i] = minHue;
        h_directions[i] = -h_directions[i]; //START GOING BACK UP TO BLUE!
      }
      if (currentHue[i] >= maxHue) {
        currentHue[i] = maxHue;
        h_directions[i] = -h_directions[i];
      }
      strip.setPixelColor(usedPins[i], strip.ColorHSV(currentHue[i], 255, 40)) ;       //  Set pixel's color (in RAM)
      strip.setPixelColor(usedPins[i]+40, strip.ColorHSV(currentHue[i], 255, 40)) ;       //  Set pixel's color (in RAM)
    }

    strip.show();                        //  Update strip to match
    delay(40);                           //  Pause for a moment

  }

}


//HELPERS!


void wipe() {
  for (int i = 0; i < 40; i++) { // Wipe all pixels
    strip.setPixelColor(i, 0, 0, 0 ) ;         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }

}

//DEBOUNCED interrupthandler
void stateChangeInterruptHandler() {

  if ( (millis() - lastDebounceTime) > debounceDelay) {
    state = state + 1;
    if (state > numberOfAnimations) {
      state = 1;
    }
    lastDebounceTime = millis(); //set the current time
    Serial.println(state);
    Serial.println(fadeHues[state]);

  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel2(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  float factor = 0.3;
  if (WheelPos < 85) {
    return strip.Color((WheelPos * 3 ) * factor, (255 - WheelPos * 3) * factor, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color((255 - WheelPos * 3) * factor, 0, (WheelPos * 3) * factor);
  } else {
    WheelPos -= 170;
    return strip.Color(0, (WheelPos * 3) * factor, (255 - WheelPos * 3) * factor);
  }
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
