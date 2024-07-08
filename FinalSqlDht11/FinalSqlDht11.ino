#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 15     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

const char* ssid = "TP-Link_E196";
const char* password = "85221628";
const char* serverAddress = "192.168.0.100"; // IP address of your server running XAMPP
const int serverPort = 80;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000); // Give some time for the serial monitor to initialize

  pinMode(DHTPIN, INPUT);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  float temperature = dht.readTemperature(); // Read temperature from DHT sensor
  float humidity = dht.readHumidity();       // Read humidity from DHT sensor

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  sendSensorData(temperature, humidity);

  delay(5000); // Send data every 5 seconds
}

void sendSensorData(float temperature, float humidity) {
  WiFiClient client;
  
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Connection failed!");
    return;
  }

  // Construct HTTP request with sensor data
  String postData = "temperature=" + String(temperature) +
                    "&humidity=" + String(humidity);
  
  client.println("POST /dht11.php HTTP/1.1");
  client.println("Host: " + String(serverAddress));
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.print(postData);
  delay(10);

  // Print response from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  client.stop();
}