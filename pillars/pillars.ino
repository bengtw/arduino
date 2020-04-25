int red_light_pin= 9;
int green_light_pin = 3;
int blue_light_pin = 6;

void setup() {
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}

void loop() {

  for (int i= 0; i<256; i++){
    RGB_color(i, 0 ,  255-i); // 
    delay(30);
  }
  for (int i= 0; i<256; i++){
    RGB_color(255-i, i , 0); // 
    delay(30);
  }
  for (int i= 0; i<256; i++){
    RGB_color(0, 255-i ,  i); // 
    delay(30);
  }

 }

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
