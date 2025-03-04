#include <Servo.h>
#include <LiquidCrystal.h>

// Pin Definitions
const int trigPin = 6;
const int echoPin = 7;
const int photoelectricPin = 8;
const int servoPin = 9;
const int ledPin = 10;
const int buzzerPin = 13;
const int moistureAnalogPin = A0;
const int moistureDigitalPin = A1;

// Initialize LCD: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Initialize Servo
Servo myServo;

// Variables
long duration;
int distance;
int moistureLevel;
bool moistureDetected;
bool objectDetected;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Set Pin Modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(photoelectricPin, INPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(moistureAnalogPin, INPUT);
  pinMode(moistureDigitalPin, INPUT);

  // Attach Servo
  myServo.attach(servoPin);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("System Initializing");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Measure Distance using Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Read Moisture Sensor
  moistureLevel = analogRead(moistureAnalogPin);
  moistureDetected = digitalRead(moistureDigitalPin);

  // Read Photoelectric Switch
  objectDetected = digitalRead(photoelectricPin);

  // Display Distance on LCD
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm");

  // Display Moisture Level on LCD
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(moistureLevel);

  // Check for Waste Presence
  if (distance < 10) {
    digitalWrite(ledPin, HIGH); // Turn on LED
    digitalWrite(buzzerPin, HIGH); // Turn on Buzzer
    delay(500);
    digitalWrite(buzzerPin, LOW); // Turn off Buzzer

    // Determine Waste Type
    if (moistureDetected == LOW) {
      // Wet Waste Detected
      lcd.clear();
      lcd.print("Wet Waste Detected");
      myServo.write(90); // Rotate Servo to 90 degrees for wet waste
    } else {
      // Dry Waste Detected
      lcd.clear();
      lcd.print("Dry Waste Detected");
      myServo.write(45); // Rotate Servo to 45 degrees for dry waste
    }
    delay(2000); // Wait for sorting
    myServo.write(0); // Reset Servo to initial position
  } else {
    digitalWrite(ledPin, LOW); // Turn off LED
  }

  delay(500);
}
