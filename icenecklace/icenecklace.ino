// A basic everyday NeoPixel strip test program.


#include <Adafruit_NeoPixel.h>

// Which pin on the A;duino is connected to the NeoPixels?


// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 40

// Declare our NeoPixel strip object:

Adafruit_NeoPixel strips[5] = {
  Adafruit_NeoPixel(7, 2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(10, 3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(13, 4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(10, 5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(7, 6, NEO_GRB + NEO_KHZ800)
  };




int usedPins[] = {0, 2, 4, 6, 16, 18, 20, 22, 32, 34, 36, 38};
int directions[] = {0,0,0,0,0,1,0,0,0,0,0,0};

//PRIMEFADES
int currentBrightness[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
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
  for (int i=0; i<5; i++){
    strips[i].begin();
    strips[i].show();
    strips[i].setBrightness(50);
    }



  randomSeed(analogRead(0));


for(int i=0; i<12; i++){
  currentHue[i] = random(minHue,maxHue);
  }


for(int x=0; x<12; x++){

  h_directions[x] = 0;
  while ( h_directions[x] == 0){
    h_directions[x] = random(-4,4);
  }
  }


  wipe();

  state = 6;  //


  fillstrip();

}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

    for (int s=0; s<5; s++){

      for(int i=0; i<13; i++) { 
        int green=(strips[s].getPixelColor(i) >>  8);
        int blue = (strips[s].getPixelColor(i) );
        green = green + random(-10,11);
        if (green <= 60){
          green = 60;
          }
        if (green>115){
          green = 115;
          }  
        strips[s].setPixelColor(i,20,green, 120 ) ;         //  Set pixel's color (in RAM)
      } 
        strips[s].show();
        delay(30);
    }



}


//HELPERS!


void fillstrip(){
    for (int s=0; s<5; s++){

      for(int i=0; i<13; i++) { // Wipe all pixels
        strips[s].setPixelColor(i, 20,90, 120 ) ;         //  Set pixel's color (in RAM)
        strips[s].show();                          //  Update strip to match
      } 
    }
  }


void wipe(){
  for(int i=0; i<40; i++) { // Wipe all pixels
        strips[0].setPixelColor(i, 0, 0, 0 ) ;         //  Set pixel's color (in RAM)
        strips[0].show();                          //  Update strip to match
    } 

  }
