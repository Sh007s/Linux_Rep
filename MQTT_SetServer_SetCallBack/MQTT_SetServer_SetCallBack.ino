#include <WiFi.h>
#include <PubSubClient.h>

// Replace these with your network credentials
const char* ssid = "OnePlus Nord CE 3 Lite 5G";
const char* password = "s3v2ku4x";

// Replace with your MQTT broker address
const char* mqtt_server = "192.168.108.55";

// Declare the global msg variable
String msg = "Hello from ESP32";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Set the callback function
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
      if (client.subscribe("test/topic")) {
        Serial.println("Subscribed to test/topic");
      } else {
        Serial.println("Subscription failed");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
    String messageTemp;

    // Build the message string from the byte array
    for (int i = 0; i < length; i++) {
        messageTemp += (char)message[i];  // Append each character to messageTemp
    }

    // Debug print for the received message
    Serial.print("Received message: ");
    Serial.println(messageTemp);

    // Convert global msg to char array for comparison
    char msgArray[msg.length() + 1];
    msg.toCharArray(msgArray, msg.length() + 1);

    // Convert messageTemp to char array for comparison
    char str[messageTemp.length() + 1];
    messageTemp.toCharArray(str, messageTemp.length() + 1);

    // Implement string comparison logic
    int len1 = strlen(msgArray);
    int len2 = strlen(str);
    int i = 0, j = 0, k = 0;
    char matchedTopic[100];

    while (i < len2) {
        if (str[i] == msgArray[j]) {
            matchedTopic[k] = str[i];
            k++;
            j++;
            if (j == len1) {  // If all characters of msg have been matched
                break;
            }
        } else {
            j = 0;  // Reset j if characters don't match
            k = 0;  // Reset k to start over in matchedTopic array
        }
        i++;
    }

    matchedTopic[k] = '\0';  // Null-terminate the matchedTopic string

    // Check if the matchedTopic is empty (no match found)
    if (strlen(matchedTopic) == 0) {
   //     Serial.println("No match found, printing original message:");
        Serial.println(messageTemp);  // Print original message if no match
    } else {
     //   Serial.println("Matched string:");
       // Serial.println(matchedTopic);  // Print the matched string if found
    }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Check if data is available on the Serial Monitor
  if (Serial.available() > 0) {
    // Read the input from the Serial Monitor
    msg = Serial.readStringUntil('\n'); // Read until newline character

    // Print the updated msg for confirmation
    Serial.print("Updated msg: ");
    Serial.println(msg);
  }

  static unsigned long lastMessage = 0;
  if (millis() - lastMessage > 5000) {
    lastMessage = millis();
    client.publish("test/topic", msg.c_str());  // Use global msg for publishing
  }
}
