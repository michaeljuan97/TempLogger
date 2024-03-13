#include <WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "sg.dd-tech.id";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// LED Pin
const int ledPin[6] = {12,14,27,26,25,32};
const int relayPin[6] = {15,2,0,4,16,17};
const int buzzer = 5;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i <= 6; i++) {
    pinMode(ledPin[i], OUTPUT);
    pinMode(relayPin[i], OUTPUT);
  }
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
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
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output/1") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[0], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[0], LOW);
    }
  } else 
  if (String(topic) == "esp32/output/2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[1], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[1], LOW);
    }
  } else 
  if (String(topic) == "esp32/output/3") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[2], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[2], LOW);
    }
  } else
  if (String(topic) == "esp32/output/4") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[3], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[3], LOW);
    }
  } else 
  if (String(topic) == "esp32/output/5") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[4], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[4], LOW);
    }
  } else
  if (String(topic) == "esp32/output/6") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin[5], HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin[5], LOW);
    }
  }
//relay controller
  if (String(topic) == "esp32/relay/1") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[0], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[0], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
  if (String(topic) == "esp32/relay/2") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[1], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[1], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
  if (String(topic) == "esp32/relay/3") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[2], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[2], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
  if (String(topic) == "esp32/relay/4") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[3], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[3], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
  if (String(topic) == "esp32/relay/5") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[4], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[4], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
  if (String(topic) == "esp32/relay/6") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(relayPin[5], HIGH);
      digitalWrite(buzzer, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(relayPin[5], LOW);
      digitalWrite(buzzer, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output/1");
      client.subscribe("esp32/output/2");
      client.subscribe("esp32/output/3");
      client.subscribe("esp32/output/4");
      client.subscribe("esp32/output/5");
      client.subscribe("esp32/output/6");

      client.subscribe("esp32/relay/1");
      client.subscribe("esp32/relay/2");
      client.subscribe("esp32/relay/3");
      client.subscribe("esp32/relay/4");
      client.subscribe("esp32/relay/5");
      client.subscribe("esp32/relay/6");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
}
