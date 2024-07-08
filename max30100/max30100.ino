#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MAX30100_PulseOximeter.h>

// Replace with your network credentials
const char* ssid = "Ras";
const char* password = "12345678";

// Server to send the data
const char* serverAddress = "http://192.168.14.212/sendMAX30100.php"; // Adjust as necessary

// Timing declarations
#define REPORTING_PERIOD_MS     1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

// Callback (optional)
void onBeatDetected()
{
    Serial.println("Beat!");
}

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

    // Initialize MAX30100 sensor
    if (!pox.begin()) {
        Serial.println("Failed to initialize pulse oximeter!");
        while (1);
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        float heartRate = pox.getHeartRate();
        float spo2 = pox.getSpO2();

        Serial.print("Heart rate:");
        Serial.print(heartRate);
        Serial.print(" bpm / SpO2:");
        Serial.print(spo2);
        Serial.println(" %");

        // Send data to server
        HTTPClient http;
        http.begin(serverAddress);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String postData = "heartRate=" + String(heartRate) + "&spo2=" + String(spo2);
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

        tsLastReport = millis();
    }
}
