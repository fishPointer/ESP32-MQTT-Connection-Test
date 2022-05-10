#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *SSID = "Breath of the Wifi";
const char *PWD = "supervolcano55";
const char *HUTAO = "192.168.0.193";
int mqttPort = 1616;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 

void connectToWiFi();
void callback(char* topic, byte* payload, unsigned int length);
void setupMQTT();
void reconnect();

////////////////////////

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupMQTT();
}

void loop() {
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
}

////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++) {
    Serial.print(char(payload[i]));
  }
  Serial.println();
  Serial.println(length);
}

////////////////////////

void connectToWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
}

void setupMQTT() {
  mqttClient.setServer(HUTAO, mqttPort);
  mqttClient.setCallback(callback);
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "Crumble-";
      clientId += String(random(0xff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("corn");
        mqttClient.subscribe("potato");
      }
      
  }
}