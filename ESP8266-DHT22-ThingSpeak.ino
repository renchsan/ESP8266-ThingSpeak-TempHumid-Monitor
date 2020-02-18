// ThingSpeak Temp/Humidity IoT Monitor
// ESP-8266 (ESP-01)
// DHT22 Sensor (Connect VCC to 5V line)
// DHT22 Out pin is connected to Pin 2
// 5V to 3.3V Buck Converter  (Power for ESP-8266)
// Project by: https://github.com/renchtech
// Modified from: https://eryk.io/2016/04/temperature-logging-to-thingspeak-using-esp8266-and-dht22/
// Modified to use DHT 22, Farenheit degrees


#include <DHT.h>
#include <ESP8266WiFi.h>

// Thingspeak API key
String apiKey = "thingspeak-api-key";       // Enter ThingSpeak API Key

// Wifi SSID and pw
const char *ssid =  "your-ssid-name";        // Enter SSID name
const char *pass =  "your-ssid-password";    // Enter SSID password
 
const char* server = "api.thingspeak.com";
#define DHTPIN 2                             // DHT22 sensor pin (ESP-01 pin 2)
 
DHT dht(DHTPIN, DHT22,11);

WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
}
 
void loop() {
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (client.connect(server,80)) {
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(postStr.length());
     client.print("\n\n");
     client.print(postStr);
 
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" F, Humidity: ");
     Serial.print(h);
     Serial.println("%. Send to Thingspeak.");
  }
  client.stop();
 
  Serial.println("Waiting...");
  
  // delay set to send every 10 minutes
  delay(600000);
}
