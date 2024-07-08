#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi credentials and server address
const char* ssid = "Ras";
const char* password = "12345678";
const char* serverAddress = "http://192.168.14.212/sendMPU60501.php"; // Update to your server

Adafruit_MPU6050 mpu;

// On-board LED pins
const int greenLedPin = 25; // GPIO pin for green LED
const int redLedPin = 27;   // GPIO pin for red LED

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

  // Initialize MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");

  // Initialize LED pins
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print the values on Serial Monitor
  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(", ");
  Serial.print("Accel Y: "); Serial.print(a.acceleration.y); Serial.print(", ");
  Serial.print("Accel Z: "); Serial.print(a.acceleration.z); Serial.println(" m/s^2");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x); Serial.print(", ");
  Serial.print("Gyro Y: "); Serial.print(g.gyro.y); Serial.print(", ");
  Serial.print("Gyro Z: "); Serial.print(g.gyro.z); Serial.println(" rad/s");

  // Check gyroscopic data for movement direction
  if (g.gyro.y > 0.0) {
    Serial.println("Clockwise movement detected");
    digitalWrite(greenLedPin, HIGH); // Turn on green LED
    digitalWrite(redLedPin, LOW);    // Turn off red LED
  }
  // } else if (g.gyro.y < 0.0) {
  //   Serial.println("Anti-clockwise movement detected");
  //   digitalWrite(greenLedPin, LOW);  // Turn off green LED
  //   digitalWrite(redLedPin, HIGH);  // Turn on red LED
  // }
   else {
     Serial.println("ANti-Clockwise movement detected");
    digitalWrite(greenLedPin, LOW);  // Turn off green LED
    digitalWrite(redLedPin, HIGH);    // Turn off red LED
  }

  // Send data to server
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "accelX=" + String(a.acceleration.x) + 
                    "&accelY=" + String(a.acceleration.y) + 
                    "&accelZ=" + String(a.acceleration.z) +
                    "&gyroX=" + String(g.gyro.x) + 
                    "&gyroY=" + String(g.gyro.y) + 
                    "&gyroZ=" + String(g.gyro.z);
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    String payload = http.getString(); 
    Serial.println("Response body:");
    Serial.println(payload);
  } else {
    Serial.print("Error occurred during HTTP request. Error code: ");
    Serial.println(httpResponseCode);
    Serial.print("Error detail: ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }

  http.end();

  delay(2000); // Wait for 2 seconds before sending the next data
}
