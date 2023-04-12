
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

  // Initialize data packet with default values
  DATA_PACKET_INIT(data);

}

void loop() {
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost");
    WiFi.begin(ssid, password);
    return;
  }

  // Modify some data elements as examples
  data.temperature += random(-1,2); // change temperature by -1, 0 or 1 degree
  data.humidity = random(0,101); // change humidity to a random value between 0 and 100
  data.light = analogRead(A0); // read light sensor value from analog pin A0

  // Serialize data packet into a byte array
  byte buffer[DATA_PACKET_SIZE];
  DATA_PACKET_SERIALIZE(buffer, data);

  // Send the byte array as a UDP broadcast packet
  udp.beginPacket(IPAddress(255,255,255,255), udpPort); // broadcast address
  udp.write(buffer, DATA_PACKET_SIZE);
  udp.endPacket();

  // Print the sent data packet for debugging
  Serial.println("Sent data packet:");
  DATA_PACKET_PRINT(data);

  // Wait for a second
  delay(1000);
}
