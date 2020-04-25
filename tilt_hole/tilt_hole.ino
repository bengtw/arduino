
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
#define LED_COUNT 16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
long currentHue = 43000;


float angle_pitch_output;
float angle_roll_output;
float angle_turn_output;

long hueStart, hueStop;
int hueAdjustor;

long hueBase;


float yAcc, xAcc, zAcc;
  int red, green, blue;
  int cRed = 125;
  int cGreen = 125;
  
  int cBlue = 125;



MPU6050 mpu6050(Wire);

//#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)



  Wire.begin();
  mpu6050.begin();

   mpu6050.setGyroOffsets(-1, -5.2, -0.3);
//  mpu6050.calcGyroOffsets(true);   

  Serial.println("GyroOffsets set");
}

void loop() {

    mpu6050.update();

    yAcc = mpu6050.getAngleY();
    xAcc = mpu6050.getAngleX();
    zAcc = mpu6050.getAngleZ();

/*

    Serial.print(xAcc);
    Serial.print(",");
    Serial.print(yAcc);
    Serial.print(",");
    Serial.println(zAcc);   

    red = (127 + (127*xAcc/100)); 
    blue = (127 + (127*yAcc/100)); 
    green = (127 + (127*zAcc/100)); 
*/



red = (abs(xAcc)%180)*2.8;
blue = (abs(yAcc)%180)*2.8;
green = (abs(zAcc)%180)*2.8;
/*
    Serial.print(red);
    Serial.print(",");
    Serial.print(green);
    Serial.print(",");
    Serial.println(blue);  
*/

    if (red > cRed){
      cRed--;
      }
    else{
      cRed++;}
      
    if (blue > cBlue){
      cBlue--;
      }
    else{
      cBlue++;}

    if (green > cGreen){
      cGreen--;
      }
    else{
      cGreen++;}

  
    for (int i=0;i<16;i++){
        strip.setPixelColor( i,strip.Color(red, 0, 0)) ;
      }
  

 

  strip.show();
  delay(10);


}// loop end
