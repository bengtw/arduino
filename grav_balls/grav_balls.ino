
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


float nPixelX = 3;
float nPixelY = 4;

int pixelX = 3;
int pixelY = 4;


 

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
strip.setPixelColor( 20, strip.ColorHSV( 0,255,100) ) ;  
strip.setPixelColor( 40, strip.ColorHSV( 0,255,100) ) ;  

strip.show();

  Wire.begin();
  mpu6050.begin();
  
  mpu6050.calcGyroOffsets(true);
  


}

void loop() {

    mpu6050.update();



    angle_pitch_output = mpu6050.getAngleX();
    angle_roll_output = mpu6050.getAngleY();
    angle_turn_output = mpu6050.getAngleZ();

    int oldX = pixelX;
    int oldY = pixelY;

    placeOnStrip(oldX,oldY, 0,40);
    placeOnStrip(oldX,4-oldY, 0,0);


    nPixelY = nPixelY + angle_roll_output / 90;
    if (nPixelY > 4){
      nPixelY = 4;
    }
    if (nPixelY < 0){
      nPixelY = 0;
    }
    nPixelX = nPixelX - angle_pitch_output / 90;
    if (nPixelX > 7){
      nPixelX = 7;
    }
    if (nPixelX < 0){
      nPixelX = 0;
    }

    pixelY = round(nPixelY);

    pixelX = round(nPixelX);

    Serial.print(pixelX);
    Serial.print(",");
    Serial.print(pixelY);
    Serial.print(",");
    Serial.print(angle_pitch_output);
    Serial.print(",");
    Serial.println(angle_roll_output);

    placeOnStrip(pixelX,pixelY, 1, 40);
    placeOnStrip(pixelX,4-pixelY, 1, 0);



    strip.show();
    delay(20);

}// loop end


void placeOnStrip(int x, int y, int pBrightness, int displacement){

    int plotPos = x + (y*8);
    strip.setPixelColor( plotPos + displacement , strip.Color( 0,0,pBrightness*100) ) ;
  }
