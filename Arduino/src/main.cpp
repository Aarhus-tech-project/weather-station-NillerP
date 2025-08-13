#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

const char *ssid = "h4prog";
const char *password = "1234567890";
const char *mqtt_server = "192.168.113.11";
const int mqtt_port = 1883;
const char *mqtt_user = "vejrbruger";
const char *mqtt_pass = "Datait2025!";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

Adafruit_BME280 bme;

void setup_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  delay(1000);
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void reconnect_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT broker... ");
    if (client.connect("Weather", mqtt_user, mqtt_pass))
    {
      Serial.println("connected!");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  delay(100);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  Serial.println("BME280 test med I2C");
  bool status = bme.begin(0x76);
  if (!status)
  {
    Serial.println("Could not find BME280 sensor!");
    while (1)
      ;
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop();

  float temperatur = bme.readTemperature();
  float luftfugtighed = bme.readHumidity();
  float tryk = bme.readPressure() / 100.0F;

  if (isnan(temperatur) || isnan(luftfugtighed) || isnan(tryk))
  {
    Serial.println("Failed to read from BME280 sensor!");
    return;
  }
  // Opret JSON objekt
  StaticJsonDocument<200> doc;

  doc["temperatur"] = temperatur;
  doc["luftfugtighed"] = luftfugtighed;
  doc["tryk"] = tryk;
  char buffer[200];
  size_t n = serializeJson(doc, buffer);

  // Send JSON til MQTT
  client.publish("vejr/data", buffer, n);

  Serial.print("Published: ");
  Serial.println(buffer);

  delay(5000);
}
