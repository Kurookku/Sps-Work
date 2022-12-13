#include <Keypad.h>
#include <TOTP.h>
#include <Servo.h>

#include <LiquidCrystal.h> // include the library for the LCD screen

// initialize the LCD screen with the appropriate pins
LiquidCrystal lcd(39, 41, 45, 47, 49, 51);



void (*resetFunc)(void) = 0;
const byte ROWS = 4;  //four rows
const byte COLS = 3;  //three columns
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};
byte rowPins[ROWS] = { 8, 7, 6, 5 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 4, 3, 2 };     //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int red = 13;
const int green = 12;
const int beepPin = 10;

char current_code[7] = {};
int index = 0;

uint8_t hmacKey[] = { 0x53, 0x50, 0x53, 0x43, 0x4C, 0x55, 0x42, 0x52, 0x4F, 0x4F, 0x4D, 0x4B, 0x45, 0x59, 0x31 };
TOTP totp = TOTP(hmacKey, 6, 30);

Servo servo;
const int servoPin = 9;

void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(beepPin, OUTPUT);
  servo.attach(servoPin);

  digitalWrite(red, HIGH);

  servo.write(0);
   // initialize the LCD screen and set the cursor to the top-left corner
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  
  lcd.print("Door status:");
  lcd.setCursor(0, 1);
  lcd.print("Locked");

  //Serial.println(totp.getCode(1669927271));
}

void loop() {

  char key = keypad.getKey();

  if (key != NO_KEY) {
    current_code[index++] = key;
      tone(beepPin, 1000, 100);
    if (key == '*' || key == '#') {
      Serial.println("Locked");
      index = 0;
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);
      servo.write(0);
      tone(beepPin, 1000);
      delay(500);

      noTone(beepPin);
      resetFunc();
// update the LCD screen to show that the door is locked
      lcd.setCursor(0, 1);
      lcd.print("Locked");      
    } else if (index > 5) {
      // password check here
      Serial.println(current_code);
      if (strcmp(current_code, "091120") == 0) {
        Serial.println(current_code);
        Serial.println("Is correct!");
        digitalWrite(red, LOW);
        digitalWrite(green, HIGH);
        servo.write(180);
        tone(beepPin, 2000);
        delay(500);
        noTone(beepPin);
        Serial.println("door Unlocked");

        lcd.setCursor(0, 1);
        lcd.print("Unlocked");
      }
    }
  }
}
