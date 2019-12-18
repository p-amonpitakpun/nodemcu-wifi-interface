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

// configures
int inMode = 0;
int office = 1;
int game   = 1;
int isplay = 0;

void messageHandle(String msg);
void commandHandle(String cmd);
void configure(String param, int argInt);
void sendCommand(String cmd);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen);
void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen);
void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen);
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen);

void setup() {
  // setup serial
  Serial.begin(115200);
  while (!Serial) {

    // busy waiting for connection.

  }
  Serial.println("serial comm connected.");

  // setup software serial
  stSerial.begin(9600);

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
  microgear.on(MESSAGE, onMsghandler);

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, onFoundgear);

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, onLostgear);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  // initialize Microgear
  microgear.init(KEY, SECRET, ALIAS);

  // connect to NETPIE
  microgear.connect(APPID);
}

void loop() {
  /* To check if the microgear is still connected */
  Serial.print("[MCU] config ");
  Serial.print(inMode);
  Serial.print(" ");
  Serial.print(office);
  Serial.print(" ");
  Serial.println(game);
  if (microgear.connected()) {

    /* Call this method regularly otherwise the connection may be lost */
    microgear.loop();

//    sendCommand("sudo echo hello 1 2 3");
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

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[WiFi] reconnected.");
    }

  }

  delay(1000);

}

void messageHandle(String msg) {

  int delimiterIndex = msg.indexOf(':');

  if (delimiterIndex != -1) {

    String param = msg.substring(0, delimiterIndex);
    String argStr = msg.substring(delimiterIndex + 1);
    int argInt = argStr.toInt();
    configure(param, argInt);

  } else {

    commandHandle(msg);

  }

  return;

}

void commandHandle(String cmd) {
  if (cmd.equals("apply")) {

    String newCmd = "";
    newCmd.concat("config set ");
    newCmd.concat(inMode);
    newCmd.concat(" ");
    newCmd.concat(office);
    newCmd.concat(" ");
    newCmd.concat(game);

    sendCommand(newCmd);

    Serial.print("[MicroGear] apply configure change ");
    Serial.print(inMode);
    Serial.print(" ");
    Serial.print(office);
    Serial.print(" ");
    Serial.print(game);
    Serial.println(".");

  } else {

    Serial.print("[MicroGear] invalid command string ");
    Serial.println(cmd);

  }

  return;

}

void printConfig(String param, int argInt) {
    Serial.print("[MicroGear] save configure ");
    Serial.print(param);
    Serial.print(" ");
    Serial.println(argInt);
}

void configure(String param, int argInt) {

  if (param.equals("mode")) {
    
    int minRange = 0;
    int maxRange = 1;
    if (argInt >= minRange && argInt <= maxRange) {

      // TODO
      inMode = argInt;

      printConfig(param, argInt);
    } else {
      Serial.print("[MicroGear] argument out of range.");
    }
  } else if (param.equals("office")) {

    int minRange = 1;
    int maxRange = 12;
    if (argInt >= minRange && argInt <= maxRange) {

      // TODO
      office = argInt;

      printConfig(param, argInt);
    } else {
      Serial.print("[MicroGear] argument out of range.");
    }
  } else if (param.equals("game")) {

    int minRange = 1;
    int maxRange = 12;
    if (argInt >= minRange && argInt <= maxRange) {

      // TODO
      game = argInt;

      printConfig(param, argInt);
    } else {
      Serial.print("[MicroGear] argument out of range.");
    }
  } else if (param.equals("isplay")) {
    
    int minRange = 0;
    int maxRange = 1;
    if (argInt >= minRange && argInt <= maxRange) {

      // TODO
      isplay = argInt;

      String cmd = "config isplay ";
      cmd.concat(argInt);
      cmd.concat(" 0 0");
      sendCommand(cmd);
      
    } else {
      Serial.print("[MicroGear] argument out of range.");
    }
  } else {

    Serial.println("[MicroGear] invalid configure parameter");

  }

  return;

}

void sendCommand(String cmd) {

  // log debug
  Serial.print("[STM32] send to STM32 : ");
  Serial.println(cmd);

  // send the command to STM32
  stSerial.println(cmd);

  // read the respond from STM32
  String respond = "";
  while (stSerial.available() > 0) {
    char c = stSerial.read();
    if (c != '\0')
      respond.concat(c);
    else
      break;
  }

  // check the respond
  Serial.print("[STM32] respond : ");
  Serial.println(respond);

  return;

}

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("[MicroGear] READ << ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  messageHandle(String((char *) msg));
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("[MicroGear] FOUND << ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("[MicroGear] LOST << ");
  for (int i = 0; i < msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("[MicroGear] connected to NETPIE as ALIAS : ");
  Serial.println(ALIAS);
  /* Set the alias of this microgear ALIAS */
  microgear.setAlias(ALIAS);
}
