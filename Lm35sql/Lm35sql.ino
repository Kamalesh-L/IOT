#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Ras";
const char* password = "12345678";
const char* serverAddress = "http://192.168.14.212/sendLM35.php"; // Adjust the PHP script address accordingly

const int lm35Pin = 34; // LM35 connected to GPIO 34

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Configure analog read resolution
  analogReadResolution(12); // 12-bit resolution, 0-4095

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Read sensor data from LM35
  int sensorValue = analogRead(lm35Pin);
  float voltage = sensorValue * (3.3 / 4095.0); // Convert the value to voltage (3.3V reference)
  float temperature = (voltage - 0.5) * 100.0; // Convert voltage to temperature

  // Send sensor data to server
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "temperature=" + String(temperature);
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    String payload = http.getString(); // Get response body
    Serial.println("Response body:");
    Serial.println(payload);
  } else {
    Serial.print("Error occurred during HTTP request. Error code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error detail: ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }

  http.end();

  delay(2000); // Wait for 2 seconds before sending the next request
}
