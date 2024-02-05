
#include <Servo.h>

Servo myServo;

int curPos = 0;  // variable to store the servo position
int speed = 10;
int targetPos;
unsigned long lastMoveTime = 0; // Tijdstempel voor de laatste beweging

struct SensorData {
  int deg;
  int speed;
  bool isValid;
};

void setup() {
  myServo.attach(9);   // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); 
}

void loop() {
  readInput();
  moveServo();

}

void moveServo() {
  // Vergelijk de huidige positie met de doelpositie
  if (curPos != targetPos) {
    // Controleer of het tijd is om de servo te verplaatsen op basis van de snelheid
    if (millis() - lastMoveTime >= speed) {
      // Bepaal de richting waarin de servo moet bewegen
      int direction = (targetPos > curPos) ? 1 : -1;

      // Bereken de nieuwe positie van de servo
      curPos += direction;

      // Stuur de nieuwe positie naar de servo
      myServo.write(curPos);

      // Update de tijd voor de laatste beweging
      lastMoveTime = millis();
    }
  }
}

void readInput(){
  if (Serial.available() > 0) {
    String str = Serial.readString();
    str.trim();
    SensorData data = extractValues(str);
    if (data.isValid) {
      Serial.println("ok");
      targetPos = data.deg;
      speed = data.speed;

    } else {
      Serial.println("Invalid instruction");
    }
  }
}

SensorData extractValues(String input) {
  SensorData result;

  // Find the positions of "deg:" and "speed:"
  int degIndex = input.indexOf("deg:");
  int speedIndex = input.indexOf("speed:");

  // Check if the keys exist in the string
  if (degIndex != -1 && speedIndex != -1) {
    // Extract the substrings containing the values
    String degSubstring = input.substring(degIndex + 4, speedIndex);
    String speedSubstring = input.substring(speedIndex + 6);

    // Convert the substrings to integers
    int degValue = degSubstring.toInt();
    int speedValue = speedSubstring.toInt();

    // Check if the values are within the valid range
    if (degValue >= 0 && degValue <= 180 && speedValue >= 0 && speedValue <= 100) {
      result.isValid = true;
      result.deg = degValue;
      result.speed = speedValue;
      return result;
    }
  }
  // bad input
  result.isValid = false;
  result.deg = -1;
  result.speed = -1;
  return result;
}