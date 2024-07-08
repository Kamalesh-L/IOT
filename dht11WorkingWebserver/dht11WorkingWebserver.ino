#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

const char* ssid = "Ras";
const char* password = "12345678";

WebServer server(80);

#define DHTPIN 15     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");

  // Start the DHT sensor
  dht.begin();

  // Route for root / web page
  server.on("/", HTTP_GET, handleRoot);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();

  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature and humidity to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000); // Wait for a few seconds before reading again
}

// Handler for the root page
void handleRoot() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String webpage = "<html><head><meta http-equiv='refresh' content='2'></head><body>";
  webpage += "<h1>ESP32 DHT11 Web Server</h1>";
  webpage += "<p>Temperature: " + String(temperature) + " °C</p>";
  webpage += "<p>Humidity: " + String(humidity) + " %</p>";
  webpage += "</body></html>";

  server.send(200, "text/html", webpage);
}
