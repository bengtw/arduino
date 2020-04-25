
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
#define LED_COUNT 80
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
long currentHue = 43000;

float angle_pitch_output;
float angle_roll_output;
float angle_turn_output;

int r;
int g;
int b;


float nPixelX[10];
float nPixelY[10];

float oldNpixelX[10];

float oldNpixelY[10];



int pixelX = 3;
int pixelY = 4;

int tempX = 3;
int tempY = 4;


 

MPU6050 mpu6050(Wire);

//#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
/*
      for (int i=0; i<40; i++){
        strip.setPixelColor( i, strip.ColorHSV( 0,255,100) ) ;         //  Set pixel's color (in RAM)
        }   
      strip.show();
*/

  for (int i=0;i<10;i++){
      nPixelY[i] = random(0,4);
      nPixelX[i] = random(0,7);
      oldNpixelY[i] = -1;
      oldNpixelX[i] = -1;
    }
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
  


}

void loop() {

    mpu6050.update();
    angle_pitch_output = mpu6050.getAngleX();
    angle_roll_output = mpu6050.getAngleY();
    angle_turn_output = mpu6050.getAngleZ();
 
    for (int i=0;i<10;i++){

  
      int oldX = round(nPixelX[i]);
      int oldY = round(nPixelY[i]);;
  
      placeOnStrip(oldX,oldY, -1,40);
      placeOnStrip(oldX,4-oldY, -1,0);
  
      float tOldNPixelY = nPixelY[i];
      float tOldNPixelX = nPixelX[i];
  
  
      nPixelY[i] = nPixelY[i] + (angle_roll_output / 90)*.6;
      if (nPixelY[i] > 4){
        nPixelY[i] = 4;
      }
      if (nPixelY[i] < 0){
        nPixelY[i] = 0;
      }
      
      nPixelX[i] = nPixelX[i] - (angle_pitch_output / 90)*.6;
      if (nPixelX[i] > 7){
        nPixelX[i] = 7;
      }
      if (nPixelX[i] < 0){
        nPixelX[i] = 0;
      }
  
      tempY = round(nPixelY[i]);
      tempX = round(nPixelX[i]);
  
      if (isEmptySpot(tempX, tempY)){
        pixelX = tempX;
        pixelY = tempY;
        oldNpixelX[i] = -1;
        oldNpixelY[i] = -1;
      }
      else{
        oldNpixelX[i] = tOldNPixelX;
        oldNpixelY[i] = tOldNPixelY;
        
        pixelY = oldY;
        pixelX = oldX;  
      }
  
      placeOnStrip(pixelX,pixelY, i, 40);
      placeOnStrip(pixelX,4-pixelY, i, 0);
  
  
  
    }


    //CHECK TO SEE IF SLOTS HAVE OPENED UP!
    for (int i=0;i<10;i++){
      if (oldNpixelX[i] > -1){
        tempY = round(nPixelY[i]);
        tempX = round(nPixelX[i]);
        if (isEmptySpot(tempX, tempY) ){
  
  
          int rX = round(oldNpixelX[i]);
          int rY = round(oldNpixelY[i]);
  
          placeOnStrip(rX,rY, -1, 40);
          placeOnStrip(rX,4-rY, -1, 0);
  
  
          placeOnStrip(tempX,tempY, i, 40);
          placeOnStrip(tempX,4-tempY, i, 0);
          
  
          oldNpixelX[i] = -1;
          oldNpixelY[i] = -1;
            
          }
        else{
          nPixelY[i] = oldNpixelY[i];
          nPixelX[i] = oldNpixelX[i];
          oldNpixelY[i] = -1;
          oldNpixelX[i] = -1;  
          }
      }
    }
    
    strip.show();

}// loop end


void placeOnStrip(int x, int y, int pBrightness, int displacement){

    int plotPos = x + (y*8);
    if (pBrightness>-1){
      strip.setPixelColor( plotPos + displacement , strip.Color( 0,0,(pBrightness*20)) ) ;
    }
    else{
      strip.setPixelColor( plotPos + displacement , strip.Color( 0,0,0 ) );
      }
  }

bool isEmptySpot(int x, int y){
    int plotPos = x + (y*8);

    if ( strip.getPixelColor( plotPos + 40 ) > 0){
      return false;
    }
    else{
      return true;
    }

  }  
