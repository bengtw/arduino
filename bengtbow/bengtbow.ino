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
   

//RGB-FADE COUNTER
  int currentRGBColorIndex = 0;

//FIREATTEMPT
  int currentFireValue = 0;
  int currentHues[] = {0,0,0,0,0,0,0,0,0,0,0,0};
  int directions[] = {0,0,0,100,0,0,0,0,0,0,0,0};


  int hueMax = 7000; 
  float adjustor = 50;

 //FIRETWO
 

//BLUEGREENFADE
long maxHue = 7000;
long minHue = 0;
long currentHue[12];
int h_directions[12]; 


void setup() {
  // put your setup code here, to run once:
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(175); // Set BRIGHTNESS to about 1/5 (max = 255)



  //BUTTONSTUFF
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonHandler, CHANGE);

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

  //WE WAIT WITH THE LIGHT SENSOR FOR NOW. THE HARDWARE IS THERE THOUGH!
  sensorValue = analogRead(sensorPin); // read the value from the sensor
//    Serial.println(sensorValue); //prints the values coming from the sensor on the screen

  

  //EVERYTHING OFF
  if (state == 0){
    wipe();
    delay(500);
    } 



  //STATE ONE, RGB FADE
  if (state == 1){
    
      for(int i=0; i<11; i = i+2) {
         strip.setPixelColor(i, Wheel((i+currentRGBColorIndex) & 255));
         strip.setPixelColor(21-i, Wheel((i+currentRGBColorIndex) & 255));
      }

      if (state != 1){
        wipe();  
      }

      strip.show(); // Update strip with new contents
      delay(130);  // Pause for a moment

      currentRGBColorIndex++;
      if (currentRGBColorIndex > 256){
        currentRGBColorIndex = 0;
      }
    
  }

  if (state == 2){ //ATTEMPT AT FIRE!
    int led = 0;
    int j = 0;
      for (int i = 0; i<23; i=i+2){
          
          j = i;
          currentHues[led] = currentHues[led] + directions[led];
          if (currentHues[led] > hueMax ){
            currentHues[led] = hueMax;
            directions[led] = - adjustor;
          }
          if (currentHues[led] < 10 ){
            currentHues[led] = 10;
            directions[led] = 0;
          }
          if (j>11){
            j--;
            }
          strip.setPixelColor(j, strip.ColorHSV(currentHues[led], 255,100));
          led++;
      }      
      strip.show();
      delay(50);

      if (random(20) == 1){
        directions[random(12)] = adjustor;
      }
  }

  if (state == 3){ //ATTEMPT AT ALTERNATE FIRE!
 
    
    
    
    int led = 0;
    int j = 0;
      for (int i = 0; i<23; i=i+2){
          
          j = i;
          currentHue[led] = currentHue[led] + h_directions[led]*20;
          if (currentHue[led] <= minHue ){
            currentHue[led] = minHue;
            h_directions[led] = - h_directions[led];
          }
          if (currentHue[led] >= maxHue ){
            currentHue[led] = maxHue;
            h_directions[led] = -h_directions[led];
          }
          if (j>11){
            j--;
            }
          strip.setPixelColor(j, strip.ColorHSV(currentHue[led], 255,100));
          led++;
      }      
      strip.show();
      delay(50);

  }

}


void wipe(){
      for(int i=0; i<11; i = i+2) {
         strip.setPixelColor(i, strip.Color(0,0,0));
         strip.setPixelColor(21-i, strip.Color(0,0,0));
      }
    strip.show(); // Update strip with new contents
  }

void buttonHandler() {
  if ((millis() - lastDebounceTime) > debounceDelay) {
    state++;
    if (state > 3){
      state = 0;
    }
    lastDebounceTime = millis();
  }
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
