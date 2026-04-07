#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Seeed_Arduino_SSCMA.h>
#include <ESP32Time.h>

// Defining constant variables
SSCMA AI;
const char* ssid = "dannywoo";
const char* password = "dannywoo";
const char* mqtt_serv = "192.168.45.1";
long lastMsg = 0;

// Defining objects for networking
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  delay(100);
  Serial.begin(115200);
  while (!Serial);

  Serial.print("Connecting to: ");
  Serial.print(ssid);
  Serial.println("");
  
  // Wi-Fi setup and configuration
  WiFi.setSleep(false);
  WiFi.setMinSecurity(WIFI_AUTH_WPA_PSK);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected, IP: ");
  Serial.print(WiFi.localIP());

  // MQTT Setup
  client.setServer(mqtt_serv, 1883);
  
  // Start AI mMdel
  Serial.println("");
  Serial.print("Starting rat detection model");
  AI.begin();
}

void loop() {
  // Reconnect to MQTT broker
  if (!client.connected()) {
    reconnect();
  }

  // Publishes on Rat Detection
  if (!AI.invoke() && AI.boxes()[0].target == 0) {
    Serial.print("score=");
    Serial.print(AI.boxes()[0].score);
    Serial.println("");

    // Sends MQTT message
    StaticJsonDocument<80> doc;
    char output[200];
    doc["Confidence"] = AI.boxes()[0].score;
    serializeJson(doc, output);
    Serial.println(output);
    client.publish("/dannywoo/test", output);
    delay(5000);    // Cooldown for false positives
  }
}

// Function to connect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.println("");
    Serial.print("MQTT not connected... Trying to connect");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("");
      Serial.print("Connected!");
    } else {
      Serial.println("");
      Serial.print("Failed, code=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}