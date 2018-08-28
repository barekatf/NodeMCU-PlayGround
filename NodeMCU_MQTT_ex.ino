#include <ESP8266WiFi.h>
#include<PubSubClient.h>

const char *ssid = "---------"; // cannot be longer than 32 characters!
const char *pass = "---------";

const char *mqtt_server = "m11.cloudmqtt.com";
const int mqtt_port = 18005;
const char *mqttuser = "-------";
const char *mqttpass = "-------";

const int pin = LED_BUILTIN;

void callback(String topic, byte* payload, unsigned int length1) {
  Serial.print("message arrived[");
               Serial.print(topic);
               Serial.println("]");
  String msgg;

  for (int i = 0; i < length1; i++) {
    Serial.print((char)payload[i]);
    msgg += (char)payload[i];

  }
  if (topic == "ledcontrol")
  {
    if (msgg == "onn") {
      digitalWrite(pin, HIGH);
      Serial.print("\n up \n ");

    }
    else if (msgg == "off")
    {
      digitalWrite(pin, LOW);

      Serial.print("\n down");
    }
  }
}

WiFiClient espclient;
PubSubClient client(mqtt_server, 19405, callback, espclient);

void setup() {
  Serial.begin(115200);
  pinMode(pin, OUTPUT);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT…");
    if (client.connect("ESP8266Client", mqttuser, mqttpass )) {
      Serial.println("connected");
      client.subscribe("ledcontrol");
    } else {
      Serial.print("failed with state");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
