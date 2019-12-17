#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <MicroGear.h>

#define APPID   "SmartVibrationChair"
#define KEY      "u7tUMhQ2r2IxQRN"
#define SECRET  "yvBEiWBdJqicoHs4f0DPwrfnu"
#define ALIAS   "nodemcu"

WiFiClient client;
MicroGear microgear(client);

String ssid = "NMSI";
String pswd = "cud54153";

// SoftwareSerial Rx Tx
const byte rx = 13;
const byte tx = 15;
SoftwareSerial stSerial(rx, tx);

int timer = 0;

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("[MicroGear] connected to NETPIE as ");
    Serial.println(ALIAS);
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}

void setup() {
  // setup serial
  Serial.begin(115200);
  while (!Serial){
    // busy waiting for connection.
  }
  Serial.println("serial comm connected.");

  // setup software serial
//  stSerial.begin(115200);

  // connect to the WiFi
  Serial.print("[WiFi] connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    // busy waiting ...
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WiFi] WiFi connected.");
    Serial.print("[WiFi] IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  /* Add Event listeners */
  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE,onMsghandler);

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT,onFoundgear);

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT,onLostgear);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED,onConnected);

  // initialize Microgear
  microgear.init(KEY, SECRET, ALIAS);

  // connect to NETPIE
  microgear.connect(APPID);
}

void processPayload(String payload);

void loop() {
    /* To check if the microgear is still connected */
  if (microgear.connected()) {

      /* Call this method regularly otherwise the connection may be lost */
      microgear.loop();
  }
  else {
    while (!microgear.connected()) {
        Serial.println("[MicroGear] connection lost, reconnect...");
        microgear.connect(APPID);
        delay(1000);
    }
  }

  while (WiFi.status() != WL_CONNECTED) {
    // reconnect to the WiFi
    Serial.print("[WiFi] reconnecting to ");
    Serial.println(ssid);
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pswd);
    delay(1000);
  }
    delay(5000);
}

void processPayload(String payload) {
  // TODO processPayload
  return;
}

void commandHandle(String cmd, String param) {
  // TODO commandHandle
  String command = "";
  
  // send to STM32
  Serial.print("[STM32] send to STM32 : ");
  Serial.println(command);
//  stSerial.println(command);

  // read the respond from STM32
//  String respond = "";
//  while (stSerial.available() > 0) {
//    char c = stSerial.read();
//  }
//  if (!respond.equals("")) {
//    Serial.print("[STM32] respond : ");
//    Serial.println("respond");
//  } else {
//    Serial.println("[STM32] no respond.");
//  }
  return;
}
