#include <LiquidCrystal.h>
#include <Servo.h>

// **Pin Definitions**
const int trigPin = 6;     // HC-SR04 Trig
const int echoPin = 7;     // HC-SR04 Echo
const int moisturePin = A1; // HW130 Analog Output
const int moistureDOPin = 8; // HW130 Digital Output
const int ledPin = 10;     // LED
const int servoPin = 9;    // Servo motor

// **Components**
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo sorterServo;

// **Constants**
const int objectDistanceThreshold = 20;  // Distance threshold in cm
const int moistureThreshold = 100;  // Threshold for moisture sensor

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(moisturePin, INPUT);
    pinMode(moistureDOPin, INPUT);
    pinMode(ledPin, OUTPUT);

    sorterServo.attach(servoPin);
    lcd.begin(16, 2);
    lcd.print("System Ready...");
    delay(1000);
    lcd.clear();
}

void loop() {
    // **Step 1: Check for Object using HC-SR04**
    long duration;
    int distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;  // Convert to cm

    if (distance > 0 && distance < objectDistanceThreshold) {  // Object detected
        digitalWrite(ledPin, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Object Detected");
        Serial.print("Object close enough.\n");

        // **Step 2: Check Moisture Level (Type of Waste)**
        int moistureValue = analogRead(moisturePin);
        bool isWetWaste = moistureValue > moistureThreshold;  // If higher, wet waste

        lcd.setCursor(0, 1);
        if (isWetWaste) {
            lcd.print("Wet Waste");
            sorterServo.write(90);  // Rotate to wet waste bin
            Serial.print("Servo 90.\n");
        } else {
            lcd.print("Dry Waste");
            sorterServo.write(0);  // Rotate to dry waste bin
            Serial.print("Servo 0.\n");
        }
        delay(2000);
    } else {
        digitalWrite(ledPin, LOW);
        sorterServo.write(45);  // Neutral position
        Serial.print("Servo 45.\n");
        lcd.clear();
    }

    delay(500);
}
