#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "1_Hotspot"; // change to your wifi name
const char* password = "love2020"; // change to your password

const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;

const char* mqtt_Client = "6df8dd31-9065-485f-9361-1f2991420729";
const char* mqtt_username = "xC6993zTngx9mNnQ4TMCtW6hNULZFWKB";
const char* mqtt_password = "jWflrPNC#Z5JKmk*2GKKLmbx4Y$-Sm~h";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[100];

void reconnect() {
  while (!client.connected()) {
    Serial.print("Sensor MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("Netpie connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to "); 
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
}

float recieveTemp() {
  return 30 + random(100)/10;
}

float recieveHumd() {
  return 40 + random(100)/10;
}

float recieveHeight() {
  return 100 + random(50)/10;
}

int recieveLight() {
  return 40 + random(50);
}

void loop() {
    // recieve some value here
    // right now it's fake value, You have to edit it

    float temperature = recieveTemp();
    float humidity = recieveHumd();
    float height = recieveHeight();
    int light = recieveLight();

    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // format to JSON
    String data = 
    "{\"data\": {\"temperature\":" + String(temperature) + 
    ", \"humidity\":" + String(humidity) +
     ", \"height\":" + String(height) +
      ", \"light\":" + String(light) + "}}";

    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));

    // send to Gavin's Netpie
    client.publish("@shadow/data/update", msg);
    delay(2000);
}
