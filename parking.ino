/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.

*/

#include <SPI.h>
#include <UIPEthernet.h>

// Biblioteca do módulo I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ONLINE 1

#ifdef ONLINE
#include <PubSubClient.h>
#else
#include <SerialPubSubClient.h>
#endif
  
//LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Update these with values suitable for your network.
byte mac[] = {0xF0, 0x4D, 0xA2, 0xE6, 0x80, 0x68} ; // change by your arduino mac address
IPAddress ip(192, 168, 3, 71);
IPAddress subnet(255, 255, 0, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress dnServer(192, 168, 0, 2);

char server[] = "test.mosquitto.org";
unsigned int port = 1883;

// Callback function header
//void callback(char* topic, byte* payload, unsigned int length);
void quandoVierMensagemMQTT(char* topic, byte* payload, int length);

EthernetClient ethClient;
PubSubClient client(server, port, quandoVierMensagemMQTT, ethClient);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  
  Serial.println("Initialising");

  /*
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  } else {
    Serial.println("Connected using DHCP");
  }
  */

  Ethernet.begin(mac, ip, dnServer, gateway, subnet);

  Serial.println("Connected using Static IP");

  Serial.println(Ethernet.localIP());
  
  delay(1000);

  initPins();
  
  connectToMqttServer();
}

void initPins() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello CodeXP");
}

int connectToMqttServer() {
  if (client.connect("arduinoClient-66633363")) {
    client.subscribe("senai-code-xp/vagas/+");
    client.setCallback(quandoVierMensagemMQTT);
    Serial.println("Connected to MQTT Server");
    Serial.flush();
  } else {
    Serial.println("Failed to connect to MQTT Server");
    Serial.flush();
    connectToMqttServer();
  }
}

void quandoVierMensagemMQTT(char* topic, byte* payload, int length) {
  int payloadComoInt = payload[0] - '0';
  int arduinoId;

  sprintf(arduinoId, "%d", topic[20]);
  
  Serial.print("Topic: ");
  Serial.print(arduinoId);
  Serial.println();
  
  Serial.print("Payload: ");
  Serial.print(payloadComoInt);
  Serial.println();
  
  Serial.flush();
  if (String(topic) == "/luksde.araujo@gmail.com/comandoPortao") {
    //settarPortao(payloadComoInt);
  }
}


void loop() {
  client.loop();
}
