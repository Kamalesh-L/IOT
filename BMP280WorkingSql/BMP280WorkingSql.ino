#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Ras";
const char* password = "12345678";
const char* serverAddress = "http://192.168.14.212/sendBMP280.php"; // Replace with the IP address of your server running XAMPP and the path to your PHP script

Adafruit_BMP280 bmp; // BMP280 sensor object

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize BMP280 sensor
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring or try a different address!");
    while (1);
  }
}

void loop() {
  // Read sensor data
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25); // Adjusted to local forecast

  // Send sensor data to server
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "temperature=" + String(temperature) +
                    "&pressure=" + String(pressure) +
                    "&altitude=" + String(altitude);
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

  delay(2000); // Wait for 2 second before sending the next request
}
