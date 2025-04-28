#include <LiquidCrystal_I2C.h>
#include <Wire.h>

template<typename T>
struct pair
{
  T x, y;

  pair(T xx=0, T yy=0) {
    x = xx; y = yy;
  }
};

// constexpr uint8_t size_x = 16;
// constexpr uint8_t size_y = 2;
// constexpr uint8_t size_char_x = 5;
// constexpr uint8_t size_char_y = 8;
// constexpr uint8_t size_in_pixel_x = size_x * size_char_x;
// constexpr uint8_t size_in_pixel_y = size_y * size_char_y;

pair<uint8_t> size = { 16, 2 };
pair<uint8_t> size_char = { 5, 8 };
pair<int8_t> size_in_pixel = { size.x * size_char.x, size.y * size_char.y };

const uint16_t posSize = size_in_pixel.x * size_in_pixel.y;

LiquidCrystal_I2C lcd(0x3F, size.x, size.y);
const int16_t speed = 100;
const uint16_t FPS = 10;
uint16_t dt = 0;
unsigned int prev_time = 0;

const int16_t joy_x_pin = A1;
const int16_t joy_y_pin = A0;
const int16_t joy_button = 2;

uint8_t but_b = 1;

byte types[7] = { B10000, B01000, B00100, B00010, B00001, B11111, B00000 };

// snake
pair<int8_t>* pos = nullptr;
pair<int8_t> food;
uint16_t score = 1;

void setup() {
  // joystick
  pinMode(joy_button, INPUT_PULLUP);
  pinMode(joy_x_pin, INPUT);
  pinMode(joy_y_pin, INPUT);

  // lcd
  lcd.init();
  lcd.backlight();

  for (int i = 0; i < 6; i++) {
    byte pixel[8];
    for (int j = 0; j < 8; j++) {
      pixel[j] = types[i];
    }
    lcd.createChar(i, pixel);
  }

  // output
  Serial.begin(9600);

  // snake
  pos = new pair<int8_t>[posSize];
  place_food();
  pos[0] = {0, 0};
}

void place_food(){
  food.x = random(size_in_pixel.x);
  food.y = random(size_in_pixel.y);
}

void move_snake(int16_t dx, int16_t dy){
  // move
  // for(uint8_t i = score - 1; i > 0; i--){
  //   pos[i] = pos[i-1];
  // }

  // plus<int8_t>(pos[0].x, dx, size_in_pixel.x);
  // plus<int8_t>(pos[0].y, dy, size_in_pixel.y);

}

void draw_c_char(uint8_t x, uint8_t y, uint8_t xx) {
  lcd.setCursor(x, y);
  lcd.write(xx);
}

void draw_pixel(int8_t x, int8_t y) {
  int x_lcd = x / size_char.x;
  int y_lcd = y / size_char.y;

  int xx = x % size_char.x;
  int yy = y % size_char.y;

  byte pix[8];
  for (int i = 0; i < 8; i++) {
    pix[i] = (yy == i) ? types[xx] : types[6];
  }
  lcd.createChar(7, pix);
  lcd.setCursor(x_lcd, y_lcd);
  lcd.write(7);
}

template<typename T>
void plus(T& var, T value, T max_value) {
  if (var + value >= max_value) {
    var = 0;
  } else if (var + value < 0) {
    var = max_value - 1;
  } else {
    var += value;
  }
}

void loop() {
  // general
  dt = millis() - prev_time;
  Serial.print("FPS: ");
  Serial.println(1000 / dt);
  prev_time = millis();

  // joystick
  int16_t joy_x = analogRead(joy_x_pin);
  int16_t joy_y = analogRead(joy_y_pin);

  int8_t xValue = map(joy_x, 0, 1023, -speed, speed);
  int8_t yValue = map(joy_y, 0, 1023, -speed, speed);

  uint8_t button_value = digitalRead(joy_button);

  Serial.print("X: ");
  Serial.print(xValue);

  Serial.print(" Y: ");
  Serial.print(yValue);

  Serial.print(" Button: ");
  Serial.println(button_value);

  // Serial.print("GL_X: ");
  // Serial.print(gl_x);
  // Serial.print(" GL_Y: ");
  // Serial.println(gl_y);

  // checks
  // if (!button_value && but_b != button_value) {
  //   plus(gl_xx, 1, 6);
  // }
  // but_b = button_value;


  // snake
  move_snake(1, 0);

  // lcd
  lcd.clear();
  for (uint8_t i = 0; i < score; i++) {
    draw_pixel(pos[i].x, pos[i].y);
  }
  //draw_pixel(food.x, food.y);

  if (FPS > 0)
    delay(1000 / FPS);
}
