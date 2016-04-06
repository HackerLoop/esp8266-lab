#include <Arduino.h>
#include <Hash.h>

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "FastAndSerious";
const char* password = "42FastAndSerious42";

WebSocketsServer webSocket = WebSocketsServer(80);

#define MAX_JSON_SIZE 1024
char json[MAX_JSON_SIZE];

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {  
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected");
      break;
    case WStype_TEXT:
      Serial.println("Text received");
      if (length > MAX_JSON_SIZE - 1) {
        return;
      }
      strncpy(json, (char *)payload, length);
      Serial.println(json);
      break;
    case WStype_BIN:
      Serial.println("Bin received");
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
