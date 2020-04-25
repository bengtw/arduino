

#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 24

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


//int directions[] = {0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0};
// int currentBrightness[] = {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0};


// CAPACITIVE STUFF!

CapacitiveSensor   cs_3_4 = CapacitiveSensor(4,3); // 1M resistor between pins 3 & 4, pin 3 is sensor pin, add a wire and or foil

int in = 3; 
int out = 4;  
int r;           
int p = LOW;    
long time = 0;       
long capdebounce = 300;



long fadeHues[] = {37000, 17000, 3000, 50000}; //BLUE, GREEN, RED

int state = 1; //SET LATER WITH RANDOM
int oldState = -1;

int numberOfAnimations = 6;               

int adjustor = 1;
int startCounter = 0;
int waitms = 250;

int currentColorIndex = 0;


//FIREDFADE

long maxHue = 41000;
long minHue = 34000;

int maxBrightness = 120;
int minBrightness = 30;

long spanValue = 6000;

int delayTime = 100;
long currentHue[12];
long currentBrightness[12];
int h_directions[12];
int b_directions[12];


const byte interruptPin = 2; //push button attached to this pin

const byte capInterruptPin = 3; //push button attached to this pin

long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 250;    // the debounce time; increase if the output flickers

// setup() function -- runs once at startup --------------------------------

void setup() {

  pinMode(4, INPUT); //CAPACITIVE SENSOR

  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(interruptPin, INPUT_PULLUP); //PUSHBUTTON!

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(interruptPin), stateChangeInterruptHandler, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(capInterruptPin), capStateChangeInterruptHandler, CHANGE);

  randomSeed(analogRead(0));

  wipe();
  reUpHues();

}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

  if (state < 5){ /// HUE FADING UP AND DOWN ON RANDOM PILLARS, FIRST FOUR STATES ARE ALL JUST SINGLE COLOR PILLAR-LIGHTS

    
    for(int i=0; i<12; i++) { // For each pixel in strip...

       //Update Hue and Brightness of pixels
       currentHue[i] = currentHue[i] - h_directions[i]*40;     
       if (currentHue[i] <= minHue){
          currentHue[i] = minHue;
          h_directions[i] = -h_directions[i];
         // h_directions[i] = random(1,10); 
        }
       if (currentHue[i] >= maxHue){
          currentHue[i] = maxHue;
          h_directions[i] = -h_directions[i];
         // h_directions[i] = random(-10,1);
        }
       currentBrightness[i] = currentBrightness[i] - b_directions[i];     
       if (currentBrightness[i] <= minBrightness){
          currentBrightness[i] = minBrightness;
          b_directions[i] = -b_directions[i]; 
        }
       if (currentBrightness[i] >= maxBrightness){
          currentBrightness[i] = maxBrightness;
          b_directions[i] = -b_directions[i];
        }

      strip.setPixelColor((i*2)+1, strip.ColorHSV(currentHue[i],255,currentBrightness[i])) ;         //  Set pixel's color (in RAM)
//      strip.setPixelColor((i*2)+1, strip.ColorHSV(currentHue[i],255,90)) ;         //  Set pixel's color (in RAM)
      }

    strip.show();                        //  Update strip to match
    delay(delayTime);                           //  Pause for a moment
  }


  if (state == 5){ ///RGB-FADE ///
    for(int i=0; i<13; i++) {
       strip.setPixelColor((i*2)+1, (Wheel((i+currentColorIndex) & 255)));
    }

    if (state != 5){ //IF WE HAVE CHANGED STATE, DON'T COMPLETE THE FADE
      wipe();
    }

    strip.show(); // Update strip with new contents
    delay(90);  // Pause for a moment

    currentColorIndex++;
    if (currentColorIndex > 256){
      currentColorIndex = 0;
    }
  }


if (state == 6){ ///OFF
      wipe();
      state == 9; //TO prevent us runnning multiple wipes.
    delay(500);  // Pause for a moment
  }

}


//HELPERS!

void reUpHues(){

 
  minHue = fadeHues[state-1];
  maxHue = fadeHues[state-1]+spanValue;
  
  for(int i=0; i<12; i++){
    currentHue[i] = random(minHue,maxHue);
    currentBrightness[i] = random(minBrightness, maxBrightness);
    h_directions[i] = 0;
    while ( h_directions[i] == 0){
      h_directions[i] = random(-5,5);
    }
    b_directions[i] = 0;
    while ( b_directions[i] == 0){
      b_directions[i] = random(-2,2);
    }

  }
 
}


void wipe(){
  for(int i=0; i<40; i++) { // Wipe all pixels
        strip.setPixelColor(i, 0, 0, 0 ) ;         //  Set pixel's color (in RAM)
        strip.show();                          //  Update strip to match
    } 

  }

//DEBOUNCED interrupthandler
void stateChangeInterruptHandler() {
  oldState = state;
  if ( (millis() - lastDebounceTime) > debounceDelay) {
    Serial.println("button triggered");
    state = state + 1;
    if (state > numberOfAnimations){
      state = 1;
    }
    if (state < 5){
      reUpHues();
      }
    lastDebounceTime = millis(); //set the current time
    Serial.println(state);
    Serial.println(fadeHues[state-1]);

  }
}


void capStateChangeInterruptHandler() {
  oldState = state;
  r = digitalRead(3);
 

  
  if (r == HIGH && p == LOW && millis() - time > capdebounce) {
    Serial.println("Cap triggered");

     
     state = state + 1;
    if (state > numberOfAnimations){
      state = 1;
    }
    time = millis();    
  }
  p = r;


}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel2(byte WheelPos) {
  float factor = 0.3;
  if(WheelPos < 85) {
   return strip.Color((WheelPos * 3)*factor, 255 - WheelPos * 3*factor, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color((255 - WheelPos * 3)*factor, 0, WheelPos * 3*factor);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3*factor, (255 - WheelPos * 3)*factor);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  float factor = 0.5;
  if(WheelPos < 85) {
   return strip.Color((WheelPos * 3 )*factor, (255 - WheelPos * 3)*factor, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color((255 - WheelPos * 3)*factor, 0, (WheelPos * 3)*factor);
  } else {
   WheelPos -= 170;
   return strip.Color(0, (WheelPos * 3)*factor, (255 - WheelPos * 3)*factor);
  }
}
