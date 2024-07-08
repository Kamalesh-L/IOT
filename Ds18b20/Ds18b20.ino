#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials and server address
const char* ssid = "Ras";
const char* password = "12345678";
const char* serverAddress = "http://192.168.14.212/sendDS18B20.php"; // Update to your server

// GPIO where the DS18B20 is connected
const int oneWireBus = 4; // Change this to the pin connected to your DS18B20

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Start the DS18B20 sensor
  sensors.begin();

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
  sensors.requestTemperatures(); 
  float temperature = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Send data to server
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
