
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
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 70
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
long currentHue = 43000;

int strips = 7;
int striplength = 10;

float angle_pitch_output;
float angle_roll_output;
float angle_turn_output;

long hueStart, hueStop;
int hueAdjustor;

long hueBase;


long cMatrix[10][7];

float yAcc, xAcc, zAcc;

int state = 1 ;

MPU6050 mpu6050(Wire);

//#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(200); // Set BRIGHTNESS to about 1/5 (max = 255)
  
/*
      for (int i=0; i<40; i++){
        strip.setPixelColor( i, strip.ColorHSV( 0,255,100) ) ;         //  Set pixel's color (in RAM)
        }   
      strip.show();

    for (int i=0;i<5;i++){
      long newHue = random(1000,10500);
      cMatrix[7][i] = newHue;
      Serial.println(newHue);
      placeOnStrip(7,i,newHue);
    }

 strip.show();
/*
strip.setPixelColor( 20, strip.ColorHSV( 0,255,100) ) ;  
strip.setPixelColor( 45, strip.ColorHSV( 0,255,100) ) ;  
strip.setPixelColor( 60, strip.ColorHSV( 0,255,100) ) ;  
strip.setPixelColor( 70, strip.ColorHSV( 0,255,100) ) ;  

strip.show();
*/
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);   

  hueBase = 10500;
}

void loop() {


    mpu6050.update();

    yAcc = mpu6050.getAccY();
    xAcc = mpu6050.getAccX();
    zAcc = mpu6050.getAccZ();

    if (zAcc < 0){
      state = 1;
     }
    else{
      state = 2;
      }
    if (xAcc < -0.5){
      state = 3;
     }  
    if (xAcc > 0.5){
      state = 4;
     }  

    Serial.print("X:");
    Serial.println(xAcc);
    Serial.print("Y:");
    Serial.println(yAcc);
    Serial.print("Z:");
    Serial.println(zAcc);
    
//    Serial.println(state);

    if (state < 3){
      
      if (state == 1){
  
          hueBase = 10500;
      }
      if (state == 2){
       
          hueBase = 35000;
      }
      
      for (int y=0;y<strips;y++){
  
        for (int x=0;x<striplength-1;x++){
          long newIndex = cMatrix[x+1][y]-920;
  
          if (newIndex<0){
            newIndex=0;
          }
          
          cMatrix[x][y] = newIndex;  
          placeOnStrip(x,y,newIndex );
          
        }    
      } 
  
      for (int i=0;i<strips;i++){
        float modifier = abs(((float)strips/2)-i);
        long newIndex = random(4000,10000) - ((modifier-1)*1000);

        cMatrix[striplength-1][i] = newIndex;
        placeOnStrip(striplength-1,i,newIndex);
      }   
  }
  if (state == 3){
     //STATE == 3

     hueBase = 22000;

    for (int x=0;x<striplength;x++){
  
        for (int y=0;y<strips-1;y++){
  
          
          long newIndex = cMatrix[x][y+1]-600;
  
          if (newIndex<0){
            newIndex=0;
          }
          
          cMatrix[x][y] = newIndex;  
          placeOnStrip(x,y,newIndex );
          
        }    
      } 
  
      for (int i=0;i<striplength;i++){
        long newIndex = random(0,5000);
        cMatrix[i][strips-1] = newIndex;
        placeOnStrip(i,strips-1,newIndex);
      }
    
    }
  if (state == 4){
    for (int i=0;i<70;i++){
           strip.setPixelColor( i,strip.ColorHSV(0, 255, 0)) ;

      }
  } 

  strip.show();
  delay(60);
/*

  for (int i=0; i<70; i++){
strip.setPixelColor( i,strip.ColorHSV(i*900, 255, 100)) ;
    
    
    }

    strip.show();
*/

}// loop end


void placeOnStrip(int x, int y, long indexHue){
    int dHue;
     
      if (state== 1){
        dHue = 10500 - (hueBase - indexHue);
      }

     if (state == 2){
        x = (striplength-1) -x;
        y = (strips-1)-y;
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

/*     if (y==0){
          brightness = brightness/6;   
        }
     if (y==1){
          brightness = brightness/3;   
        }
     if (y==5){
          brightness = brightness/3;   
        }

     if (y==6){
          brightness = brightness/6;   
        }
     
*/
     strip.setPixelColor( plotPos,strip.ColorHSV(dHue, 255, brightness*2)) ;
  }

void fplaceOnStrip(int x, int y, long indexHue){
     
     int plotPos = x + (y*striplength);

     strip.setPixelColor( plotPos,strip.ColorHSV(indexHue, 255, 100)) ;
  }

  
