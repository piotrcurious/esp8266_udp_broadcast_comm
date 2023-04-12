
// Include the necessary libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "data_packet.h"

// Define the WiFi credentials and UDP port
const char* ssid = "your_ssid";
const char* password = "your_password";
const int udpPort = 8888;

// Create a WiFiUDP object
WiFiUDP udp;

// Create a data_packet_t object
data_packet_t data;

// Setup function
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print the IP address and start UDP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  udp.begin(udpPort);
}

// Loop function
void loop() {
  // Check if WiFi is still connected
  if (WiFi.status() == WL_CONNECTED) {
    // Check if there is any UDP packet available
    int packetSize = udp.parsePacket();
    if (packetSize) {
      // Read the UDP packet into a byte array
      byte packet[packetSize];
      udp.read(packet, packetSize);

      // Cast the byte array into a data_packet_t object
      memcpy(&data, packet, sizeof(data_packet_t));

      // Print the data values to serial monitor
      #define PRINT_DATA_ELEMENT(type, name, value) Serial.print(#name); Serial.print(": "); Serial.println(data.name);
      DATA_ELEMENT_LIST(PRINT_DATA_ELEMENT)
      
      // Print a separator line for readability
      Serial.println("-----------------");
    }
  }
  else {
    // Reconnect to WiFi network if disconnected
    Serial.println("WiFi disconnected");
    WiFi.begin(ssid, password);
  }
}
