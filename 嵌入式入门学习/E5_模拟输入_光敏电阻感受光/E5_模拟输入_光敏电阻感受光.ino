const int LED_PIN = 2;             // 按实际开发板修改
const int LIGHT_SENSOR_PIN = 1;    // 按实际开发板修改
const int LIGHT_THRESHOLD = 1500;  // 阈值可根据环境调整

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogReadResolution(12);
}

void loop() {
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  Serial.print("Light value: ");
  Serial.println(lightValue);

  if (lightValue < LIGHT_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(200);
}
