#include <WiFi.h>
#include <ThingSpeak.h>
#include "DHT.h"

const char* WIFI_Name = "Wokwi-GUEST";
const char* WIFI_Pass = "";

const int channelNumber = 3560208;
const char* apiKey = "6SR1HN6S98EBB3CS";

const char* universityID = "12217362";

#define DHT_PIN 15
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

WiFiClient clientObject;

void setup() {

  Serial.begin(115200);

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_Name, WIFI_Pass, 6);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }

  Serial.println("WiFi Connected");
  Serial.println(universityID);

  ThingSpeak.begin(clientObject);
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Sensor reading failed");
    delay(20000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int status = ThingSpeak.writeFields(channelNumber, apiKey);

  if (status == 200)
  {
    Serial.println("Data sent successfully");
  }
  else
  {
    Serial.println("Error sending data");
  }

  delay(20000);
}