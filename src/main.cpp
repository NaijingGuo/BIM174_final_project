#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <elapsedMillis.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
const char *ssid = "instaconnect";

const char *serverName = "http://192.168.0.240:8000/items/";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000; // post every 5s
const float ideal_humidity = 50;
elapsedMillis timeElapsed;
int check_interval = 500; // check sensor every 0.5s
#define DHTPIN 27         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define hum_pin 12
String readDHTTemperature()
{
  timeElapsed = 0;
  int recheck_time = 0;
  float t = dht.readTemperature(); // degree C
  while (isnan(t) && recheck_time < 30)
  {
    if (timeElapsed > check_interval)
    {
      t = dht.readTemperature();
    }
    timeElapsed = 0;
    recheck_time++;
  }
  if (isnan(t))
  {
    return "--";
  }
  else
  {
    return String(t);
  }
}

String readDHTHumidity()
{
  timeElapsed = 0;
  int recheck_time = 0;
  float t = dht.readHumidity();
  while (isnan(t) && recheck_time < 30)
  {
    if (timeElapsed > check_interval)
    {
      t = dht.readHumidity();
    }
    timeElapsed = 0;
    recheck_time++;
  }
  if (isnan(t))
  {
    return "--";
  }
  else
  {
    return String(t);
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      String temp = readDHTTemperature();
      String hum = readDHTHumidity();
      float humidity_float = hum.toFloat();
      if (humidity_float < ideal_humidity)
      {
        digitalWrite(hum_pin, HIGH);
      }
      Serial.println(temp);
      int httpResponseCode = http.POST("{\"name\":\"sensor data\",\"description\":\"DHT11\",\"temp\":\"" + temp + "\",\"humidity\":\"" + hum + "\"}");
      // int httpResponseCode = http.POST("{\"name\":\"sensor data\",\"description\":\"DHT11\",\"temp\":\"temp\",\"humidity\":\"hum\"}");
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}