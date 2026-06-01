#include <ESP32Servo.h>

const int SERVO_PIN = 6;  // 按实际开发板修改
Servo myServo;

void setup() {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    Serial.print("Servo angle: ");
    Serial.println(angle);
    delay(300);
  }

  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    Serial.print("Servo angle: ");
    Serial.println(angle);
    delay(300);
  }
}
