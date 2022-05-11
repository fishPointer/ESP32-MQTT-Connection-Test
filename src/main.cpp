#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char *SSID = "Breath of the Wifi";
const char *PWD = "supervolcano55";
const char *HUTAO = "192.168.0.193";
const char *TOPIC_CORN = "corn";
const char *TOPIC_BOOLY = "booly";
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

//Initialize some Variables for switching and payload conversion
int callbackmode = 0;
int num_payload = -1;
String str_payload;
char buffer_payload[length];
char out_msg[40];

//Saving Payload into a buffer, and String
for (int i = 0; i < length; i++) 
{
  buffer_payload[i] = char(payload[i]);
}
str_payload = String(buffer_payload);

//Will replace with enum, and integrate into the switch
if(strcmp(topic, TOPIC_CORN) == 0)
{
  callbackmode = 2;
}

if(strcmp(topic, TOPIC_BOOLY) == 0)
{
  callbackmode = 3;
}

//Primary switch statement for different functions by topic
//Master list
  switch(callbackmode)
  {
    case 1:
      Serial.println("Callback Mode 1");
      mqttClient.publish("feedback", "AAA");
      break;

    case 2:
      Serial.print("Topic detected: ");
      Serial.println(topic);
      mqttClient.publish("feedback", TOPIC_CORN);
      break;

    case 3:
      Serial.println("booly trigger");
      num_payload = str_payload.toInt();
      num_payload = 5*num_payload;
      itoa(num_payload, out_msg, 10);
      Serial.print("calc output: ");
      Serial.println(out_msg);
      mqttClient.publish("feedback", out_msg);
      break;

    //Default Functionality, read off message and topic
    default:
      Serial.print("Callback - ");
      Serial.println(topic);
      Serial.print("Message:");

      for (int i = 0; i < length; i++) 
      {
        Serial.print(char(payload[i]));
      }
      Serial.println();
  }
  
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
        mqttClient.subscribe("booly");
      }
      
  }
}