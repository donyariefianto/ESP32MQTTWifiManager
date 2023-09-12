#include <WiFiManager.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

const int mqttPort = 1883;
const char* mqttUser = "3ny9m4";
const char* mqttPassword = "3ny9m4.iop2023";
const char* mqttServer = "103.87.149.19";
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "sensors";

void setup() {
    Serial.begin(115200);
    Serial.println();
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    WiFi.mode(WIFI_STA); 
    Serial.begin(115200);
    
    WiFiManager wm;

    bool res;
    res = wm.autoConnect("Nano","n12345678");

    if(!res) {
        Serial.println("Failed to connect wifi");
        ESP.restart();
    } 
    else {
        Serial.println("wifi connected...yeey :)");
    }
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (!client.connected()) {
      reconnect();
  }
  client.loop();
  // client.publish(topic, msg);
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),mqttUser,mqttPassword)) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}