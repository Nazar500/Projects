
const int r = 11;
const int g = 10;
const int b = 9;

const uint8_t but = 2;
uint8_t prev_but = 1;

void setup() {
  pinMode(but, INPUT_PULLUP);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  Serial.begin(9600);
}

void setColor(int red, int green, int blue){
  analogWrite(r, red);
  analogWrite(g, green);
  analogWrite(b, blue);
}

void loop() {
  // update
  srand(millis());

  int but_value = digitalRead(but);

  Serial.println("But: " + String(but_value));
  
  if(but_value == 0 && prev_but){
    setColor(rand() % 256, rand() * 2 % 256, rand() * 3 % 256);
    prev_but = 0;
  }
  if(but_value == 1){
    prev_but = 1;
  }
}
