#include <Adafruit_NeoPixel.h>

#define LED_PIN    4
#define LED_COUNT 47

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
 

//BLUEGREENFADE
long maxHue = 39000;
long minHue = 31000;
long currentHue[48];
int h_directions[48]; 


void setup() {
  // put your setup code here, to run once:
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)


  for(int i=0; i<48; i++){
     currentHue[i] = random(minHue,maxHue);
  }


  for(int x=0; x<48; x++){

    h_directions[x] = 0;
    while ( h_directions[x] == 0){
     h_directions[x] = random(-14,14);
    }
    }
  

}

void loop() {
  // put your main code here, to run repeatedly:

    
    int led = 0;
      for (int i = 0; i<48; i++){
          led = i;
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
      delay(10);



}


void wipe(){
      for(int i=0; i<11; i = i+2) {
         strip.setPixelColor(i, strip.Color(0,0,0));
         strip.setPixelColor(21-i, strip.Color(0,0,0));
      }
    strip.show(); // Update strip with new contents
  }

 
