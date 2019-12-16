#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

String ssid = "";
String pswd = "";

String host = "";

WiFiClient client;

HTTPClient http;

void setup() {
  // setup serial
  Serial.begin(115200);

  Serial.print("[WiFi] connecting to ");
  Serial.println(ssid);

  // connect to the WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pswd);

  while (WiFi.status() != WL_CONNECTED) {
    // busy waiting ...
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("[WiFi] WiFi connected");
    Serial.print("[WiFi] IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void processPayload(String payload);

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[HTTP] begin ...");
    if (http.begin(client, host)) {
      Serial.println("[HTTP] GET ...");
      // start connection and send HTTP header
      int httpCode = http.GET();
      if (httpCode >0) {
        Serial.printf("[HTTP] GET code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
          processPayload(payload);
        }
      } else {
        Serial.println("[HTTP] GET failed");
      }
    } else {
      Serial.println("[HTTP] Unable to connect to HOST");
    }
  } else {
    Serial.print("[WiFi] reconnecting to ");
    Serial.println(ssid);
  
    // connect to the WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pswd);
  
    while (WiFi.status() != WL_CONNECTED) {
      // busy waiting ...
      delay(500);
    }
  }

  delay(10000);
}

void processPayload(String payload) {
  // TODO processPayload
  return;
}

void commandHandle(String cmd, String param) {
  // TODO commandHandle
  return;
}
