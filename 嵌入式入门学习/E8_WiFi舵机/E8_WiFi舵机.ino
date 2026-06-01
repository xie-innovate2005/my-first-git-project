#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "your_wifi_name";
const char* WIFI_PASS = "your_wifi_password";

const int SERVO_PIN = 6;  // 按实际开发板修改

WebServer server(80);
Servo myServo;
int currentAngle = 90;

void handleRoot() {
  String html = "<html><body><h1>ESP32 Servo Control</h1>";
  html += "<p>Current angle: " + String(currentAngle) + "</p>";
  html += "<p>Use /set?angle=0~180 to control the servo.</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleSetAngle() {
  if (!server.hasArg("angle")) {
    server.send(400, "text/plain", "Missing angle parameter");
    return;
  }

  int angle = server.arg("angle").toInt();
  angle = constrain(angle, 0, 180);
  currentAngle = angle;
  myServo.write(currentAngle);

  String message = "Servo angle set to " + String(currentAngle);
  server.send(200, "text/plain", message);
}

void setup() {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
  myServo.write(currentAngle);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetAngle);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
