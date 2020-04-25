
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

/*
float angle_pitch_output;
float angle_roll_output;
float angle_turn_output;

int r;
int g;
int b;

*/
 
long lowrow[5];

long cMatrix[8][5];


//MPU6050 mpu6050(Wire);

//#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
   Serial.begin(9600);


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100); // Set BRIGHTNESS to about 1/5 (max = 255)
  
/*
      for (int i=0; i<40; i++){
        strip.setPixelColor( i, strip.ColorHSV( 0,255,100) ) ;         //  Set pixel's color (in RAM)
        }   
      strip.show();
*/

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

  Wire.begin();
  mpu6050.begin();
  
  mpu6050.calcGyroOffsets(true);
*/   


}

void loop() {
  
    for (int y=0;y<5;y++){

      for (int x=0;x<7;x++){

        
        long newHue = cMatrix[x+1][y]-1300;
        if (newHue<0){
          newHue=0;
        }
        cMatrix[x][y] = newHue;  
        placeOnStrip(x,y,newHue );
        
      }    
  } 

    for (int i=0;i<5;i++){
      long newHue = random(1000,10500);
      cMatrix[7][i] = newHue;
      placeOnStrip(7,i,newHue);
    }

  strip.show();
  delay(60);

/*    mpu6050.update();
    angle_pitch_output = mpu6050.getAngleX();
    angle_roll_output = mpu6050.getAngleY();
    angle_turn_output = mpu6050.getAngleZ();
*/ 

}// loop end


void placeOnStrip(int x, int y, long dHue){

    int plotPos = x + (y*8);
   strip.setPixelColor( plotPos,strip.ColorHSV(43000-dHue, 255, round(((float)dHue/10500)*100))) ;
//Serial.println((round(((float)dHue/10500)*100)));
    strip.setPixelColor( plotPos+40,strip.ColorHSV(dHue, 255, round(((float)dHue/10500)*100))  ) ;
  }

  
