#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 22

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//LIGHT SENSOR
  float brightnessfactor = 0.5;
  int sensorPin = A0; // select the input pin for LDR
  int sensorValue = 0; // variable to store the value coming from the sensor


//BUTTON
  const byte interruptPin = 2;
  // the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 250;    // the debounce time; increase if the output flickers


//STATE
  int state = 3; //START WITH FIRE-ALTERNATE
   

//FIREATTEMPT
  int currentFireValue = 0;
  int currentHues[] = {0,0,0,0,0,0,0,0,0,0,0,0};
  int directions[] = {0,0,0,100,0,0,0,0,0,0,0,0};


  int hueMax = 7000; 
  float adjustor = 50;

 //FIRETWO
 

//BLUEGREENFADE
long maxHue = 39000;
long minHue = 34000;
long currentHue[12];
int h_directions[12]; 


void setup() {
  // put your setup code here, to run once:
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)


  for(int i=0; i<12; i++){
     currentHue[i] = random(minHue,maxHue);
  }


  for(int x=0; x<12; x++){

    h_directions[x] = 0;
    while ( h_directions[x] == 0){
     h_directions[x] = random(-4,4);
    }
    }
  

}

void loop() {
  // put your main code here, to run repeatedly:


 
    
    
    
    int led = 0;
    int j = 0;
      for (int i = 0; i<11; i++){
          
          currentHue[led] = currentHue[led] + h_directions[led]*20;
          if (currentHue[led] <= minHue ){
            currentHue[led] = minHue;
            h_directions[led] = - h_directions[led];
          }
          if (currentHue[led] >= maxHue ){
            currentHue[led] = maxHue;
            h_directions[led] = -h_directions[led];
          }
          strip.setPixelColor(i, strip.ColorHSV(currentHue[led], 255,100));
          led++;
      }      
      strip.show();
      delay(50);



}


void wipe(){
      for(int i=0; i<11; i = i+2) {
         strip.setPixelColor(i, strip.Color(0,0,0));
         strip.setPixelColor(21-i, strip.Color(0,0,0));
      }
    strip.show(); // Update strip with new contents
  }

 
