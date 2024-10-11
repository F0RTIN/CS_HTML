#include <WiFi.h>
#include <WebSocketsServer.h>

// Constants
const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";
const int potentiometerPin = 35;  // Pin à laquelle le potentiomètre est connecté
const int buzzPin = 32;
const int ledPin = 33;
int potValue = 0;
String data = "";
String buzz = "";
String led = "";
WebSocketsServer webSocket = WebSocketsServer(81);

// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {

  // Figure out the type of WebSocket event
  switch(type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;

    // Handle incoming text message
    case WStype_TEXT:
      Serial.printf("[%u] Text: %s\n", num, payload);
      data = String((char*)payload);  // Store the received message in the data variable

      // Extract the command from the message (assuming format like "buzz:ON" or "led:OFF")
      int colonIndex = data.indexOf(':');

      // Check if the colon exists in the string
      if (colonIndex != -1) {
        // Extract the part before and after the colon
        String command = data.substring(0, colonIndex);  // e.g., "buzz" or "led"
        String value = data.substring(colonIndex + 1);   // e.g., "ON" or "OFF"

        // Debugging: Print the command and value
        Serial.printf("Command: %s, Value: %s\n", command.c_str(), value.c_str());

        // Check if the message is for the buzz or led command
        if (command == "buzz") {
          buzz = value;  // Store the value for the buzzer (ON/OFF)
        } else if (command == "led") {
          led = value;   // Store the value for the LED (ON/OFF)
        }
      }
      break;
  }
}

void setup() {

  // Start Serial port
  Serial.begin(115200);

  pinMode(potentiometerPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Connect to access point
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
  
  // Read the potentiometer value and send it to all WebSocket clients
  potValue = analogRead(potentiometerPin);
  String potValueStr = String(potValue);
  webSocket.broadcastTXT(potValueStr);  

  // Control the buzzer based on received command
  if (buzz == "ON") {
    digitalWrite(buzzPin, HIGH);  
  } else if (buzz == "OFF") {
    digitalWrite(buzzPin, LOW);  
  }

  // Control the LED based on received command
  if (led == "ON") {
    digitalWrite(ledPin, HIGH);  
  } else if (led == "OFF") {
    digitalWrite(ledPin, LOW);  
  }

  delay(200);
}
