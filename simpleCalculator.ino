/*
 * Arduino Keypad Calculator Program with I2C LCD
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for I2C LCD
#include <Keypad.h>            // Library for Keypad

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the Keymap
char keys[ROWS][COLS] = {
 {'1', '2', '3', 'A'},
 {'4', '5', '6', 'B'},
 {'7', '8', '9', 'C'},
 {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8}; // Connect keypad rows to Arduino pins
byte colPins[COLS] = {7, 6, 5, 4}; // Connect keypad columns to Arduino pins

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS); // Create the Keypad

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C address (0x27) for a 16x2 LCD

long Num1, Num2, Number;
char key, action = '=';
boolean result = false;

void setup() {
  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on the backlight
  lcd.print("Shon's");
  lcd.setCursor(0, 1);
  lcd.print("calculator");
  delay(2000);        // Display intro message
  lcd.clear();        // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("Instructions");
  lcd.setCursor(0, 1);
  lcd.print("*-cancel #-enter");
  delay(3000);        // Display intro message
  lcd.clear();        // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("A-Plus B-Minus");
  lcd.setCursor(0, 1);
  lcd.print("C-multiply D-divide");
  delay(4000);        // Display intro message
  lcd.clear();        // Clear the LCD
}

void loop() {
  key = kpd.getKey(); // Get the pressed key

  if (key != NO_KEY) { //detect pressed key
    DetectButtons();
  }

  if (result == true) { // if # is pressed
    CalculateResult();
  }

  DisplayResult(); //display final result
}

void DetectButtons() { 
  lcd.clear();

  if (key == '*') { // Cancel button
    Serial.println("Button Cancel");
    Number = Num1 = Num2 = 0;
    result = false;
  }

  if (key >= '0' && key <= '9') { // Handle digit keys
    Serial.print("Button ");
    Serial.println(key);
    if (Number == 0) {
      Number = key - '0';
    } else {
      Number = (Number * 10) + (key - '0');
    }
  }

  if (key == '#') { // Equal button
    Serial.println("Button Equal");
    Num2 = Number;
    result = true;
  }

  if (key >= 'A' && key <= 'D') { // Operation buttons
    Num1 = Number;    
    Number = 0;
    if (key == 'A') { Serial.println("Addition"); action = '+'; }
    if (key == 'B') { Serial.println("Subtraction"); action = '-'; }
    if (key == 'C') { Serial.println("Multiplication"); action = '*'; }
    if (key == 'D') { Serial.println("Division"); action = '/'; }
    delay(100);
  }
}

void CalculateResult() {
  if (action == '+') Number = Num1 + Num2;
  if (action == '-') Number = Num1 - Num2;
  if (action == '*') Number = Num1 * Num2;
  if (action == '/') Number = Num1 / Num2;
}

void DisplayResult() {
  lcd.setCursor(0, 0);
  lcd.print(Num1);
  lcd.print(action);
  lcd.print(Num2);

  if (result == true) {
    lcd.print("=");
    lcd.print(Number);
  }

  lcd.setCursor(0, 1);
  lcd.print(Number); // Display the result
}
