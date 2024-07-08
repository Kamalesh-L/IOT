// #include <WiFi.h>
// #include <HTTPClient.h>

// const char ssid[] = "TP-Link_E196";
// const char password[] = "85221628";

// String HOST_NAME = "http://127.0.0.1"; // REPLACE WITH YOUR PC's IP ADDRESS
// String PHP_FILE_NAME   = "/insert_data.php";  //REPLACE WITH YOUR PHP FILE NAME
// String tempQuery = "?temperature=31.0";

// void setup() {
//   Serial.begin(115200); 
//   WiFi.begin(ssid, password);
//   Serial.println("Connecting");
//   while(WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.print("Connected to WiFi network with IP Address: ");
//   Serial.println(WiFi.localIP());
  
//   HTTPClient http;
//   String server = HOST_NAME + PHP_FILE_NAME + tempQuery;
//   http.begin(server); 
//   int httpCode = http.GET();

//   if(httpCode > 0) {
//     if(httpCode == HTTP_CODE_OK) {
//       String payload = http.getString();
//       Serial.println(payload);
//     } else {
//       Serial.printf("HTTP GET... code: %d\n", httpCode);
//     }
//   } else {
//     Serial.printf("HTTP GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//   }

//   http.end();
// }

// void loop() {

// }

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "TP-Link_E196";     
const char* password = "85221628";
const int potentiometerPin = 15;
void setup() {
  Serial.begin(115200);
  pinMode(potentiometerPin, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
int potValue = analogRead(potentiometerPin);

  // Print the analog value to the serial monitor
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  sendSensorData(potValue);
  delay(1000); // Delay for one second between readings  http://localhost/iot/pulsemeter/pulse_data.php?BPM=80&sp02=98
}

void sendSensorData(float potValue) {
  HTTPClient http;
  http.begin("http://192.168.0.100/insert_data.php?temperature=32.9");
  http.addHeader("Content-Type", "application/json");
String jsonBody = "{}";
  Serial.println(jsonBody);
  int httpResponseCode = http.POST(jsonBody);
  Serial.println(httpResponseCode);
  http.end();
}