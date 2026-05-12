#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "wifi_name";
const char* password = "wifi_code";

const char* serverUrl = "http://192.168..:8001/api/leds"; // Change to your PC IP & port


// GPIO pins for each LED
const int LED1_PIN = 14;
const int LED2_PIN = 26;
const int LED3_PIN = 25;
const int LED_WIFI = 32;

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);

  // Start with LEDs off
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_WIFI, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(LED_WIFI, LOW);
    delay(250);
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Received: " + payload);

      // Parse JSON manually (simple parsing)
      bool led1On = payload.indexOf("\"led1\":\"on\"") > 0;
      bool led2On = payload.indexOf("\"led2\":\"on\"") > 0;
      bool led3On = payload.indexOf("\"led3\":\"on\"") > 0;

      digitalWrite(LED1_PIN, led1On ? HIGH : LOW);
      digitalWrite(LED2_PIN, led2On ? HIGH : LOW);
      digitalWrite(LED3_PIN, led3On ? HIGH : LOW);

      Serial.print("LED1: ");
      Serial.println(led1On ? "ON" : "OFF");
      Serial.print("LED2: ");
      Serial.println(led2On ? "ON" : "OFF");
      Serial.print("LED3: ");
      Serial.println(led3On ? "ON" : "OFF");

    } else {
      Serial.println("HTTP GET failed");
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(1000); // Wait 5 seconds before next check
}