

#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN 6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 24

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


int directions[] = {0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0,0,0,0, 0,0,0};
int currentBrightness[] = {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0};


// CAPACITIVE STUFF!

CapacitiveSensor   cs_3_4 = CapacitiveSensor(4,3); // 1M resistor between pins 3 & 4, pin 3 is sensor pin, add a wire and or foil

int in = 3; 
int out = 4;  
int r;           
int p = LOW;    
long time = 0;       
long capdebounce = 200;



long fadeHues[] = {43650, 21845, 65500, 4500 }; //BLUE, GREEN, RED

int state = 0; //SET LATER WITH RANDOM

int numberOfAnimations = 7;               
                //  1 : BLUEPILLARS
                //  2 : GREENPILLARS
                //  3 : REDPILLARS
                //  4 : WHITEPILLARS
                //  5 : warm-yellow
                //  6 : RGB-FADE
                //  7 : FIRE!
                //  8 : off

int adjustor = 1;
int startCounter = 0;
int waitms = 250;

int baseHue = 0;

int currentColorIndex = 0;


//FIREDFADE
long maxHue = 7500;
long minHue = 2000;
int delayTime = 50;
long currentHue[12];
int h_directions[12];

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

//  attachInterrupt(digitalPinToInterrupt(interruptPin), stateChangeInterruptHandler, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(capInterruptPin), capStateChangeInterruptHandler, RISING);

  randomSeed(analogRead(0));

  wipe();



for(int i=0; i<12; i++){
  currentHue[i] = random(minHue,maxHue);
  }


for(int x=0; x<12; x++){

  h_directions[x] = 0;
  while ( h_directions[x] == 0){
    h_directions[x] = random(-4,4);
  }
  }


  state =7;  //

}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
  
  baseHue = (baseHue +  500) & 65536;

  for (int i=0; i<13; i++){
          strip.setPixelColor((i*2)+1, strip.ColorHSV(baseHue + (i*100),255,40)) ;         //  Set pixel's color (in RAM)
  }
  strip.show();
  delay(3);
  

}


//HELPERS!


void wipe(){
  for(int i=0; i<40; i++) { // Wipe all pixels
        strip.setPixelColor(i, 0, 0, 0 ) ;         //  Set pixel's color (in RAM)
        strip.show();                          //  Update strip to match
    } 

  }



void capStateChangeInterruptHandler() {
  r = digitalRead(3);
 
  if (r == HIGH && p == LOW && millis() - time > capdebounce) {
     state = state + 1;
    if (state > numberOfAnimations){
      state = 1;
    }
    time = millis();    
  }
  p = r;


}
