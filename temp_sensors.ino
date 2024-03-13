// Example testing sketch for various DHT humidity/temperature sensors written by ladyada
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
// What works:
// - Basic max6675 reading
// - Send max6675 reading to mqtt
// - Payload formating

// //Not done:
// - Cleanup
// - Calibration untested
// - Test on real hw

#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
// #include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "MAX6675.h"
#include <SPI.h>

// Set your Static IP address
// IPAddress staticIP(192, 168, 11, 132);
//Set your Gateway IP address
// IPAddress gateway(192, 168, 11, 254);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(210, 145, 254, 162);   //optional

// StaticJsonBuffer<200> jsonBuffer;
// StaticJsonDocument<256> jsonBuffer;
char json[100];

#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
// Replace the next variables with your SSID/Password combination
const char* ssid = "Wokwi-GUEST";
const char* password = "";

float calibrate1, calibrate2, calibrate3, calibrate4, calibrate5, calibrate6;
// SPI Bus 1 Init (Max 3 sensor di 1 bus)
int CLK = 18;
int SO = 19;
int CS1 = 5;
int CS2 = 17;
int CS3 = 16;
// SPI Bus 2 Init
 int H_CLK = 14;
 int H_SO = 12;
 int CS4 = 27;
 int CS5 = 26;
 int CS6 = 25;

 MAX6675 temp_sensor1(CLK, CS1, SO);
MAX6675 temp_sensor2(CLK, CS2, SO);
MAX6675 temp_sensor3(CLK, CS3, SO);
MAX6675 temp_sensor4(H_CLK, CS4, H_SO);
MAX6675 temp_sensor5(H_CLK, CS5, H_SO);
MAX6675 temp_sensor6(H_CLK, CS6, H_SO);

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "sg.dd-tech.id";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char temp1[50],temp2[50],temp3[50],temp4[50],temp5[50],temp6[50];

// LED Pin
const int ledPin = 5;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
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
      client.subscribe("esp32/output");
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
  if (now - lastMsg > 5000) {
    lastMsg = now;
    float temperature1 = temp_sensor1.getTemperature()+calibrate1;
        float temperature2 = temp_sensor2.getTemperature()+calibrate2;
        float temperature3 = temp_sensor3.getTemperature()+calibrate3;
        float temperature4 = temp_sensor4.getTemperature()+calibrate4;
        float temperature5 = temp_sensor5.getTemperature()+calibrate5;
        float temperature6 = temp_sensor6.getTemperature()+calibrate6;
        
        dtostrf(temperature1, 4, 2, temp1);
        dtostrf(temperature2, 4, 2, temp2);
        dtostrf(temperature3, 4, 2, temp3);
        dtostrf(temperature4, 4, 2, temp4);
        dtostrf(temperature5, 4, 2, temp5);
        dtostrf(temperature6, 4, 2, temp6);

    // Wait a few seconds between measurements.
  // delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  // Convert the value to a char array
    char humString[8];
    dtostrf(h, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);

     String Temp_json = "[{\"temp1\": " + String(temp1) + "},{\"temp2\": " + String(temp2) + "},{\"temp3\": " + String(temp3) + "},{\"temp4\": " + String(temp4) + "},{\"temp5\": " + String(temp5) + "},{\"temp6\": " + String(temp6) + "}]";
        

        client.publish("sensbey/temperature", Temp_json.c_str());

  }
  
}
