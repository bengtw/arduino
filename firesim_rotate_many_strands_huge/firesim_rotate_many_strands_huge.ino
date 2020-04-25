
  /*
 *
 * This code is basic usage MPU-6050 Accelerometer and Gyroscope
 * Running this code you will get angles only.
 * The Angles at X, Y and Z are displayed on LCD1602-I2C display module
 *
 * Library and code have been taken from:
 * https://github.com/tockn/MPU6050_tockn
 *
 * Updated by Ahmad Shamshiri on July 05, 2018 at 22:19 in Ajax, Ontario, Canada
 * for Robojax.com
 * Get this code from Robojax.com
 * Watch video instruction for this code at:https://youtu.be/uIz6WIis4dc
 *
 * You will need to watch 2 videos before following the instruction in this video:
 * 1-MPU6050 Introduction video and code: https://youtu.be/uhh7ik02aDc
 * 2-LCD1602 with I2C module video and code https://youtu.be/q9YC_GVHy5A
 */
//#include <MPU6050_tockn.h>
//#include <Wire.h>
#include <Adafruit_NeoPixel.h>


#define LED_PIN 6
#define LED_COUNT 144
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


int strips = 12;
int striplength = 12;

long hueBase;

long lastrun[12];
long delaytime[12];

int speedFactor = 23;

long cMatrix[12][12];

int state = 1 ;

long speedDelay = 120;
long lastSpeedCheck = millis();
const int buttonRed = 3;
const int buttonWhite = 5;
const int buttonYellow = 7;

long buttonLastPress = millis();
long buttonDelay = 150;


int stateRed = 1; 
int stateYellow = 1; 
int stateWhite = 1; 

void setup() {
   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
  

  hueBase = 10500;

  for (int j=0;j<10;j++){
      lastrun[j] = millis();
      delaytime[j] = random(40,90);
    }


  pinMode(buttonRed, INPUT_PULLUP);  
  pinMode(buttonWhite, INPUT_PULLUP);  
  pinMode(buttonYellow, INPUT_PULLUP);  
}

void loop() {

    stateRed = digitalRead(buttonRed);
    stateWhite = digitalRead(buttonWhite);
    stateYellow = digitalRead(buttonYellow );


      if (stateRed == 0){
         
        if (millis() - buttonLastPress > buttonDelay){

        Serial.println(speedFactor);

          speedFactor--;
          buttonLastPress = millis();
        }
      
      }
      if (stateWhite == 0){
        if ((millis() - buttonDelay) > buttonLastPress){
          state = state + 1;
          buttonLastPress = millis();
          if (state > 4){
            state = 1;
          }
           Serial.println(state);
        }
              
      }
      if (stateYellow == 0){
        if (millis() - buttonLastPress > buttonDelay){

          speedFactor ++;
          if (speedFactor >=40){
            speedFactor = 40;
          }
          Serial.println(speedFactor);
          buttonLastPress = millis();
        }
      }
        
      if (speedFactor <=1){
        speedFactor = 1;
      }

    



    if (state < 4){
      
      if (state == 1){
  
          hueBase = 10500;
      }
      if (state == 2){
       
          hueBase = 35000;
      }
      if (state == 3){
          hueBase = 22000;
      }
      
      for (int y=0;y<strips;y++){
        if (millis() - lastrun[y] > delaytime[y]){
          lastrun[y] = millis();
          
          delaytime[y] = random(40-speedFactor,80 - speedFactor*2);  
          for (int x=0;x<striplength;x++){
  
            if (x < striplength -1){
                long newIndex = cMatrix[x+1][y]-(random(950,1150)) - ((15-speedFactor) * 35);
        
                if (newIndex<0){
                  newIndex=0;
                }
                
                cMatrix[x][y] = newIndex;  
                placeOnStrip(x,y,newIndex );
              
            }
            else{
              float modifier = abs(((float)strips/2)-y);
              long newIndex = random(4000,10000) - ((modifier-1)*1000);
              cMatrix[x][y] = newIndex;
              placeOnStrip(x,y,newIndex);
  
              
           }
            
          }
        }
      } 
      
  }
 
  if (state == 4){
    for (int i=0;i<144;i++){
           strip.setPixelColor( i,strip.ColorHSV(0, 255, 0)) ;

      }
    state == 5;
  } 

  strip.show();

}// loop end


void placeOnStrip(int x, int y, long indexHue){
    int dHue;
     
      if (state== 1){
        dHue = 10500 - (hueBase - indexHue);
      }

     if (state == 2){
        dHue = hueBase - indexHue;
      }
     
     if (state==3){
      dHue = hueBase-indexHue;
      }
     
     int plotPos = x + (y*striplength);

     int brightness = float(indexHue*100/10000);
     if (brightness < 0){
      brightness = 0;
     }

     strip.setPixelColor( plotPos,strip.ColorHSV(dHue, 255, brightness*2)) ;
  }

void fplaceOnStrip(int x, int y, long indexHue){
     
     int plotPos = x + (y*striplength);

     strip.setPixelColor( plotPos,strip.ColorHSV(indexHue, 255, 100)) ;
  }

  
