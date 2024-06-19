#ifndef CONTROL_PAGE_H
#define CONTROL_PAGE_H

const char CONTROL_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Motor Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
    }
    button {
      padding: 10px 20px;
      font-size: 16px;
      margin: 10px;
    }
  </style>
</head>
<body>
  <h1>Motor Control</h1>
  <button onclick="sendCommand('A')">Forward</button>
  <button onclick="sendCommand('B')">Backward</button>
  <button onclick="sendCommand('L')">Left</button>
  <button onclick="sendCommand('R')">Right</button>
  <button onclick="sendCommand('Q')">Diagonal Forward-Right</button>
  <button onclick="sendCommand('Z')">Stop</button>

  <script>
    var ws;

    function connectWebSocket() {
      ws = new WebSocket("ws://" + window.location.hostname + ":81/");
      ws.onopen = function() {
        console.log("WebSocket connected");
      };
      ws.onmessage = function(event) {
        console.log("Received message: " + event.data);
      };
      ws.onclose = function() {
        console.log("WebSocket closed");
      };
    }

    function sendCommand(command) {
      if (ws.readyState === WebSocket.OPEN) {
        ws.send(command);
        console.log("Sent command: " + command);
      } else {
        console.log("WebSocket not connected");
      }
    }

    connectWebSocket();
  </script>
</body>
</html>
)=====";

#endif
