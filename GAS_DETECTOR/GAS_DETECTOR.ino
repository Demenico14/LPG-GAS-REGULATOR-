#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
bool servoRotated = false; // flag to track servo rotation

// Gas sensor pin
const int gasSensorPin = A0;

// Fan control pins
const int fanPin = 13; // PWM pin for fan speed control

// Buzzer pin
const int buzzerPin = 10;

// LED pins
const int greenLEDPin = 7;
const int redLEDPin = 5;

// Gas threshold value
const int gasThreshold = 200; // Change this threshold as needed

// Initialize the LCD with the I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize serial communication
  myservo.attach(7);  // attaches the servo on pin 7 to the servo object
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  pinMode(fanPin, OUTPUT); // Set fan pin as output
  pinMode(buzzerPin, OUTPUT); // Set buzzer pin as output
  pinMode(greenLEDPin, OUTPUT); // Set green LED pin as output
  pinMode(redLEDPin, OUTPUT); // Set red LED pin as output

  

  delay(100);

  // Initialize the LCD
  
  lcd.begin();
  lcd.clear();
}

void loop() {
  // Read the gas sensor value
  
  int gasValue = analogRead(gasSensorPin);

  // Print the gas value to serial monitor
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Print the gas value to LCD
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(" NORMAL");
  lcd.setCursor(0, 1);
  lcd.print("Gas Value: ");
  lcd.print(gasValue);

  // Check if gas value is above threshold
  if (gasValue > gasThreshold && !servoRotated) {
    // If gas value is above threshold and servo hasn't rotated yet, rotate servo
    for (pos = 0; pos <= 180; pos += 1) {
      myservo.write(pos);  // tell servo to go to position in variable 'pos'
      delay(15);           // waits 15 ms for the servo to reach the position
    }
    servoRotated = true; // Set flag to indicate servo rotation

    analogWrite(fanPin, 255); // Start the fan at maximum speed
    lcd.setCursor(2, 0);
    Serial.println("Fan ON");

    // Turn on red LED and buzzer
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
    tone(buzzerPin, 1000); // Start buzzing

    lcd.print("Gas Value HIGH: ");
    lcd.print(gasValue);

  } else if (gasValue <= gasThreshold && servoRotated) {
    // If gas value is below threshold and servo has rotated, reset servo position
    for (pos = 180; pos >= 0; pos -= 1) {
      myservo.write(pos);  // tell servo to go to position in variable 'pos'
      delay(15);           // waits 15 ms for the servo to reach the position
    }
    servoRotated = false; // Reset flag

    analogWrite(fanPin, 0); // Stop the fan
    Serial.println("Fan OFF");

    // Turn on green LED and turn off buzzer
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    noTone(buzzerPin); // Stop buzzing
  }

  // Add some delay to prevent rapid readings
  delay(1000); // Adjust delay as needed
}
