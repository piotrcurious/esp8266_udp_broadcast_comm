
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "data_packet.h"

// WiFi network name and password:
const char *ssid = "YOUR_SSID";
const char *password = "YOUR_PASSWORD";

// UDP port to send and receive packets
const int udpPort = 8888;

// UDP object
WiFiUDP udp;

// Data packet struct
data_packet_t data;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

   // Start UDP
   Serial.println("Starting UDP");
   udp.begin(udpPort);
   Serial.print("Local port: ");
   Serial.println(udp.localPort());
}

void loop() {
   // Check WiFi connection status
   if (WiFi.status() != WL_CONNECTED) {
     Serial.println("WiFi connection lost");
     WiFi.begin(ssid, password);
     return;
   }

   // Check if there is a UDP packet available
   int packetSize = udp.parsePacket();
   if (packetSize) {
     // Read the packet into a byte array
     byte buffer[DATA_PACKET_SIZE];
     udp.read(buffer, DATA_PACKET_SIZE);

     // Deserialize the byte array into a data packet struct
     DATA_PACKET_DESERIALIZE(data, buffer);

     // Print the received data packet for debugging
     Serial.println("Received data packet:");
     DATA_PACKET_PRINT(data);

     // Access some data elements as examples
     Serial.print("Temperature: ");
     Serial.println(data.temperature); // print temperature value

     Serial.print("Humidity: ");
     Serial.println(data.humidity); // print humidity value

     Serial.print("Light: ");
     Serial.println(data.light); // print light sensor value

   }
}
