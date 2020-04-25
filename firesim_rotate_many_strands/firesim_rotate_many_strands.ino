
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

long lastrun[10];
long delaytime[10];

int speedFactor = 15;


long cMatrix[10][7];

float yAcc, xAcc, zAcc;

int state = 1 ;

long speedDelay = 150;
long lastSpeedCheck = millis();

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

  for (int j=0;j<10;j++){
      lastrun[j] = millis();
      delaytime[j] = random(40,90);
    }
}

void loop() {


    mpu6050.update();

    yAcc = mpu6050.getAccY();
    xAcc = mpu6050.getAccX();
    zAcc = mpu6050.getAccZ();

    Serial.print(xAcc);
               Serial.print(" ");
    Serial.print(yAcc);
               Serial.print(" ");
    Serial.println(zAcc);
 

    if (yAcc < 0){
      state = 1;
     }
    else{
      state = 2;
      }
    if (zAcc < -0.2 && zAcc > -0.75){
      if (millis() - speedDelay > lastSpeedCheck){
      speedFactor ++;
      if (speedFactor >=30){
        speedFactor = 30;
      }
      lastSpeedCheck = millis();
      }
    }  
    if (zAcc > 0.2 && zAcc < 0.75){
      if (millis() - speedDelay > lastSpeedCheck){
      speedFactor --;
      if (speedFactor <=1){
        speedFactor = 1;
      }
      lastSpeedCheck = millis();
      }
    } 

/*
    Serial.print("X:");
    Serial.println(xAcc);
    Serial.print("Y:");
    Serial.println(yAcc);
    Serial.print("Z:");
    Serial.println(zAcc);
*/    
//    Serial.println(state);

    if (state < 3){
      
      if (state == 1){
  
          hueBase = 10500;
      }
      if (state == 2){
       
          hueBase = 35000;
      }
      
      for (int y=0;y<strips;y++){
        if (millis() - lastrun[y] > delaytime[y]){
          lastrun[y] = millis();
          
          delaytime[y] = random((50 - (speedFactor * 1.5)),(100 - (speedFactor*1.5)));  
          for (int x=0;x<striplength;x++){
  
            if (x < striplength -1){
                long newIndex = cMatrix[x+1][y]-(random(950,1150)) - ((15-speedFactor) * 30);
        
                if (newIndex<0){
                  newIndex=0;
                }
                
                cMatrix[x][y] = newIndex;  
                placeOnStrip(x,y,newIndex );
              
            }
            else{
         float modifier = abs(((float)strips/2)-y);
           long newIndex = random(4000,10000) - ((modifier-1)*2000);
              cMatrix[x][y] = newIndex;
              placeOnStrip(x,y,newIndex);
  
              
           }
            
          }
        }
      } 
      
  }
  if (state == 3){
     //STATE == 3
     delay(60);
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
           float modifier = abs(((float)strips/2)-i);
           long newIndex = random(4000,10000) - ((modifier)*1000);

           Serial.println(modifier);

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

     
     strip.setPixelColor( plotPos,strip.ColorHSV(dHue, 255, brightness*2)) ;
  }

void fplaceOnStrip(int x, int y, long indexHue){
     
     int plotPos = x + (y*striplength);

     strip.setPixelColor( plotPos,strip.ColorHSV(indexHue, 255, 100)) ;
  }

  
