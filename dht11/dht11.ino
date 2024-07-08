// #include "DHT.h"

// #define DHTPIN 23
// #define DHTTYPE DHT11
// //DHTTYPE = DHT11, but there are also DHT22 and 21

// DHT dht(DHTPIN, DHTTYPE); // constructor to declare our sensor

// void setup() {
//   Serial.begin(115200);
//   dht.begin();
// }

// void loop() {
//   delay(1000);
//   // The DHT11 returns at most one measurement every 1s
//   float h = dht.readHumidity();
//   //Read the moisture content in %.
//   float t = dht.readTemperature();
//   //Read the temperature in degrees Celsius
//   float f = dht.readTemperature(true);
//   // true returns the temperature in Fahrenheit

//   if (isnan(h) || isnan(t) || isnan(f)) {
//     Serial.println("Failed reception----");
//     return;
//     //Returns an error if the ESP32 does not receive any measurements
//   }

//   Serial.print("Humidite: ");
//   Serial.print(h);
//   Serial.print("%  Temperature: ");
//   Serial.print(t);
//   Serial.print("°C, ");
//   Serial.print(f);
//   Serial.println("°F");
//   // Transmits the measurements received in the serial monitor
// }
  

// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <DHT.h>

// #define DHT11_PIN 0 // DHT11 sensor pin connected to GPIO0

// const char* ssid = "Ras";
// const char* password = "12345678";

// const char* serverUrl = "http://localhost/store_data.php"; // URL of the PHP script on your server

// DHT dht(DHT11_PIN, DHT11);

// void setup() {
//   Serial.begin(115200);
//   dht.begin();

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }

//   Serial.println("Connected to WiFi");
// }

// void loop() {
//   delay(2000); // Delay between sensor readings

//   float temperature = dht.readTemperature();
//   float humidity = dht.readHumidity();

//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("Failed to read from DHT sensor!");
//     return;
//   }

//   // Print temperature and humidity values
//   Serial.print("Temperature: ");
//   Serial.print(temperature);
//   Serial.println("°C");
//   Serial.print("Humidity: ");
//   Serial.print(humidity);
//   Serial.println("%");

//   // Create JSON payload with sensor data
//   String postData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

//   // Send HTTP POST request to server
//   HTTPClient http;
//   http.begin(serverUrl);
//   http.addHeader("Content-Type", "application/json");
//   int httpResponseCode = http.POST(postData);
  
//   if (httpResponseCode > 0) {
//     Serial.print("HTTP response code: ");
//     Serial.println(httpResponseCode);
//     String response = http.getString();
//     Serial.println(response);
//   } else {
//     Serial.println("Error sending HTTP POST request!");
//   }

//   http.end();
// }





// #include <WiFi.h>
// #include <HTTPClient.h>

// #include <DHT.h> 
// #define DHTPIN 23 //D19 
// #define DHTTYPE DHT11 
// DHT dht11(DHTPIN, DHTTYPE); 

// String URL = "http://192.168.1.100/test_data.php";

// const char* ssid = "R7"; 
// const char* password = "12345678"; 

// int temperature = 0;
// int humidity = 0;

// void setup() {
//   Serial.begin(115200);

//   dht11.begin(); 
  
//   connectWiFi();
// }

// void loop() {
//   if(WiFi.status() != WL_CONNECTED) {
//     connectWiFi();
//   }

//   Load_DHT11_Data();
//   String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  
//   HTTPClient http;
//   http.begin(URL);
//   http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
//   int httpCode = http.POST(postData);
//   String payload = "";

//   if(httpCode > 0) {
//     // file found at server
//     if(httpCode == HTTP_CODE_OK) {
//       String payload = http.getString();
//       Serial.println(payload);
//     } else {
//       // HTTP header has been send and Server response header has been handled
//       Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//     }
//   } else {
//     Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//   }
  
//   http.end();  //Close connection

//   Serial.print("URL : "); Serial.println(URL); 
//   Serial.print("Data: "); Serial.println(postData);
//   Serial.print("httpCode: "); Serial.println(httpCode);
//   Serial.print("payload : "); Serial.println(payload);
//   Serial.println("--------------------------------------------------");
//   delay(5000);
// }


// void Load_DHT11_Data() {
//   //-----------------------------------------------------------
//   temperature = dht11.readTemperature(); //Celsius
//   humidity = dht11.readHumidity();
//   //-----------------------------------------------------------
//   // Check if any reads failed.
//   if (isnan(temperature) || isnan(humidity)) {
//     Serial.println("Failed to read from DHT sensor!");
//     temperature = 0;
//     humidity = 0;
//   }
//   //-----------------------------------------------------------
//   Serial.printf("Temperature: %d °C\n", temperature);
//   Serial.printf("Humidity: %d %%\n", humidity);
// }

// void connectWiFi() {
//   WiFi.mode(WIFI_OFF);
//   delay(1000);
//   //This line hides the viewing of ESP as wifi hotspot
//   WiFi.mode(WIFI_STA);
  
//   WiFi.begin(ssid, password);
//   Serial.println("Connecting to WiFi");
  
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
    
//   Serial.print("connected to : "); Serial.println(ssid);
//   Serial.print("IP address: "); Serial.println(WiFi.localIP());
// }


#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // Change this to match your DHT sensor type

const char* ssid = "R7";
const char* password = "12345678";

// Replace with the IP address of your XAMPP server
const char* serverName = "http://localhost/insert_data.php";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
    Serial.println("HTTP Request Data: " + httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(5000); // Wait 5 seconds before reading again
}