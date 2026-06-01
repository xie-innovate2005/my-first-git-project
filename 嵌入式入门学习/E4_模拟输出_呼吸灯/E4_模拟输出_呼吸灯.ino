const int LED_PIN = 2;  // 按实际开发板修改
const int PWM_CHANNEL = 0;
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

void setup() {
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
}

void loop() {
  for (int duty = 0; duty <= 255; duty += 5) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(20);
  }

  for (int duty = 255; duty >= 0; duty -= 5) {
    ledcWrite(PWM_CHANNEL, duty);
    delay(20);
  }
}
