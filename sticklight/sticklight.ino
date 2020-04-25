
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
#define LED_COUNT 40
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
long currentHue = 43000;

float angle_pitch_output;
float angle_roll_output;
float angle_turn_output;

int r;
int g;
int b;

MPU6050 mpu6050(Wire);

//#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
//   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
      for (int i=0; i<40; i++){
        strip.setPixelColor( i, strip.ColorHSV( 0,255,100) ) ;         //  Set pixel's color (in RAM)
        }   
      strip.show();

  
  Wire.begin();
  mpu6050.begin();
  
  mpu6050.calcGyroOffsets(true);

     for (int i=0; i<40; i++){
        strip.setPixelColor( i, strip.ColorHSV( 22000,255,100) ) ;         //  Set pixel's color (in RAM)
        }   
      strip.show();


}

void loop() {

    mpu6050.update();



    angle_pitch_output = mpu6050.getAngleX();
    angle_roll_output = mpu6050.getAngleY();
    angle_turn_output = mpu6050.getAngleZ();
    Serial.println(angle_pitch_output);


//    if (angle_pitch_output < -5 || angle_pitch_output > 5){
      currentHue = (currentHue + (int)angle_pitch_output*3) % 65535;

      for (int i=0; i<40; i++){
//        strip.setPixelColor( i, strip.ColorHSV( (currentHue + (i*1800)) & 65535,150+angle_roll_output,100) ) ;         //  Set pixel's color (in RAM)

//        strip.setPixelColor( i, strip.ColorHSV( angle_pitch_output*500,150+angle_roll_output,100) ) ;         //  Set pixel's color (in RAM)
  
          r = 125 + (angle_roll_output * 125/180);
          b = 125 + (angle_pitch_output * 125/180) ;
          g = 125 + (angle_turn_output * 125/180);
          
      strip.setPixelColor( i, strip.Color( r,g,b) ) ;         //  Set pixel's color (in RAM)
        
        }
       
      strip.show();
    //}

}// loop end
