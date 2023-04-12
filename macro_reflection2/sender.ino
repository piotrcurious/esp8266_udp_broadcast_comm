
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

// Initialize the data_packet_t object with some values
#define INIT_DATA_ELEMENT(type, name, value) data.name = value;
DATA_ELEMENT_LIST(INIT_DATA_ELEMENT)

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
    // Serialize the data_packet_t object into a byte array
    byte packet[sizeof(data_packet_t)];
    memcpy(packet, &data, sizeof(data_packet_t));

    // Send the byte array as a UDP broadcast packet
    udp.beginPacket(IPAddress(255,255,255,255), udpPort);
    udp.write(packet, sizeof(data_packet_t));
    udp.endPacket();

    // Print a message to serial monitor
    Serial.println("Packet sent");

    // Increment the data values for demonstration purposes
    #define INCR_DATA_ELEMENT(type, name, value) data.name++;
    DATA_ELEMENT_LIST(INCR_DATA_ELEMENT)
    
    // Wait for a second before sending another packet
    delay(1000);
  }
  else {
    // Reconnect to WiFi network if disconnected
    Serial.println("WiFi disconnected");
    WiFi.begin(ssid, password);
  }
}
