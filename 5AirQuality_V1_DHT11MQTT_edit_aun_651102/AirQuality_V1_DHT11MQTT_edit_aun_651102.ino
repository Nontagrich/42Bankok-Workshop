#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <SimpleDHT.h>          //โหลดไลบารี่ DHT11
#include <SoftwareSerial.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Wire.h>


//########## CONFIGURATION : MODIFY HERE ##########
const char* ssid = "PWFaun24";
const char* password = "PWFaun24";
const char* mqtt_server = "192.168.0.49";
const char* serverHostname = "192.168.0.49";
const String serverUsername = "admin";
const String serverPassword = "admin";
//#################################################

int DHTSENSOR = 15;             //GPIO15 (ADC2_CH3)
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์
int i = 0;
int now = 0;
int mqttRetryAttempt = 0;
int wifiRetryAttempt = 0;
long lastMsg = 0;
char msg[20];

#define AirQV1_TOPIC "AirQV1_T1" 
#define ONBOARD_LED  2;
const char led = ONBOARD_LED;

SimpleDHT11 dht11;              //ระบุรุ่นเซ็นเซอร์รุ่น DHT11
WiFiClient espClient;
PubSubClient client(espClient);


void receivedCallback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message received: ");
  //Serial.println(topic);
  //Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  }
  //Serial.println();
  /* we got '1' -> on */
  if ((char)payload[0] == '1') {
    digitalWrite(led, HIGH); //---- Write HIGH to pin IO12 (LED_BUILTIN)
  } else {
    /* we got '0' -> on */
    digitalWrite(led, LOW); //---- Write HIGH to pin IO12 (LED_BUILTIN)
  }
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (client.connect(clientId.c_str(), serverUsername.c_str(), serverPassword.c_str())) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(AirQV1_TOPIC);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(1000);
      mqttRetryAttempt++;
      if (mqttRetryAttempt > 5) {
        Serial.println("Restarting!");
        interuptReboot();
      }
    }
  }
}

void interuptReboot() {
    Serial.println("Rebooting");
    //esp_restart();
    ESP.restart();
}

void setup() {
  Serial.begin(115200);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiRetryAttempt++;
    if (wifiRetryAttempt > 5) {
       Serial.println("Restarting!");
       interuptReboot();
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1884);
  /* this receivedCallback function will be invoked
    when client received subscribed topic */
  client.setCallback(receivedCallback);
  /*start SHT31 */
  delay(1000);

  pinMode(15, INPUT); 
}

void loop() {
  dht11.read(DHTSENSOR, &temperature, &humidity, NULL);  //อ่านค่าจากเซ็นเซอร์
  while (temperature == 0) {
  dht11.read(DHTSENSOR, &temperature, &humidity, NULL);  //อ่านค่าจากเซ็นเซอร์
  Serial.println("DHT11 get failed");
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% | ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");
  
  //#################################################
  
  now = millis();
  
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming
    subscribed topic-process-invoke receivedCallback */
  client.loop();
  /* we increase counter every 3 secs
    we count until 3 secs reached to avoid blocking program if using delay()*/
  
  if (now - lastMsg > 1) {
    lastMsg = now;
    char data[90];
    String json = "{\"Temperature\":" + String(temperature) + ",\"Humidity\":" + String(humidity) + "}";
    json.toCharArray(data, (json.length() + 1));
    client.publish(AirQV1_TOPIC, data, false);
  }
  unsigned long lastMillis;
  
  //#################################################
  delay(2000);
}
