#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Replace these with your network credentials
const char* ssid = "OnePlus Nord CE 3 Lite 5G";
const char* password = "s3v2ku4x";

// Replace with your MQTT broker address
const char* mqtt_server = "192.168.108.55";

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

// Function to connect to the MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("test/topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(250);
    }
  }
}

// Callback function for when a message is received from the server
void callback(char* topic, byte* payload, unsigned int length) {
 
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Prepare the JSON object
  StaticJsonDocument<200> doc;
  doc["name"] = "Shank";
  doc["id"] = 101;
  doc["percentage"] = 95.5;
  doc["marks"] = 475;

  // Serialize JSON to a string
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  // Publish the JSON string to the MQTT topic
  client.publish("test/topic", jsonBuffer);

  // Delay before sending the next message
  delay(250);  // Send every 250 milliseconds
}
