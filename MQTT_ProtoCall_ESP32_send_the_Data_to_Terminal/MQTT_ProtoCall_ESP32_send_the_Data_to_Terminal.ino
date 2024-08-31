#include <WiFi.h>
#include <PubSubClient.h>

// Replace these with your mobile hotspot credentials
const char* ssid = "OnePlus Nord CE 3 Lite 5G"; // Replace with your mobile hotspot SSID
const char* password = "s3v2ku4x"; // Replace with your mobile hotspot password

// Replace with your MQTT broker address
const char* mqtt_server = "192.168.108.55";    // Example IP address for local testing, replace as needed

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.print(" | Error: ");
      switch(client.state()) {
        case -2: Serial.println("MQTT_CONNECT_FAILED"); break;
        case -3: Serial.println("MQTT_CONNECT_TIMEOUT"); break;
        case -4: Serial.println("MQTT_CONNECT_BAD_PROTOCOL"); break;
        case -5: Serial.println("MQTT_CONNECT_BAD_CLIENT_ID"); break;
        case -6: Serial.println("MQTT_CONNECT_UNAVAILABLE"); break;
        case -7: Serial.println("MQTT_CONNECT_BAD_CREDENTIALS"); break;
        case -8: Serial.println("MQTT_CONNECT_UNAUTHORIZED"); break;
        default: Serial.println("Unknown error"); break;
      }
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static unsigned long lastMessage = 0;
  if (millis() - lastMessage > 5000) {
    lastMessage = millis();
    String msg = "Hello from ESP32";
    client.publish("test/topic", msg.c_str());
  }
}
