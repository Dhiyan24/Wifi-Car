#include <WiFi.h>
#include <WebSocketsServer.h>
#include "control_motor.h"

const char* ssid = "Protosem";
const char* password = "proto123";

WebSocketsServer webSocket = WebSocketsServer(81);
WiFiServer server(80);

int M1R = 27;
int M1L = 26;
int M2R = 25;
int M2L = 14;

void forward() {
  digitalWrite(M1R, HIGH);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, HIGH);
  digitalWrite(M2L, LOW);
}

void backward() {
  digitalWrite(M1R, LOW);
  digitalWrite(M1L, HIGH);
  digitalWrite(M2R, LOW);
  digitalWrite(M2L, HIGH);
}

void left() {
  digitalWrite(M1R, HIGH);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, LOW);
  digitalWrite(M2L, LOW);
}

void right() {
  digitalWrite(M1R, LOW);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, HIGH);
  digitalWrite(M2L, LOW);
}

void diagonalForwardRight() {
  digitalWrite(M1R, HIGH);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, LOW);
  digitalWrite(M2L, HIGH);
}

void stop() {
  digitalWrite(M1R, LOW);
  digitalWrite(M1L, LOW);
  digitalWrite(M2R, LOW);
  digitalWrite(M2L, LOW);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_TEXT:
      char c = payload[0];
      Serial.write(c);

      // Control motors based on received commands
      switch (c) {
        case 'A': // Forward
          forward();
          break;
        case 'B': // Backward
          backward();
          break;
        case 'R': // Right
          right();
          break;
        case 'L': // Left
          left();
          break;
        case 'Q': // Diagonal forward-right
          diagonalForwardRight();
          break;
        case 'Z': // Stop
          stop();
          break;
      }
      break;
  }
}

void setup() {
  pinMode(M1R, OUTPUT);
  pinMode(M1L, OUTPUT);
  pinMode(M2R, OUTPUT);
  pinMode(M2L, OUTPUT);

  Serial.begin(9600);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  // Start HTTP server
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  webSocket.loop();
 // Check for incoming client connections
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Send HTTP response with the content of the web page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.print(CONTROL_PAGE);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Close the connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}
