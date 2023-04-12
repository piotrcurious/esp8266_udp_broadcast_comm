
// receiver.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "data_packet.h" // include the header file

const char* ssid = "xxxxxx"; // your WiFi SSID
const char* password = "xxxxxx"; // your WiFi password

WiFiUDP Udp; // create a UDP object
unsigned int localUdpPort = 4211; // local port to listen on
char buffer [sizeof data_packet + 5]; // create a buffer with enough size plus 5 delimiters
int packetSize; // variable to store the packet size

void setup () {
  Serial.begin (115200); // initialize serial communication
  
  Serial.printf ("Connecting to %s ", ssid); // print message
  WiFi.begin (ssid, password); // connect to WiFi network
  
  while (WiFi.status () != WL_CONNECTED) { // wait until connected
    delay (500); // wait for half a second
    Serial.print ("."); // print dot
  }
  
  Serial.println (" connected"); // print message
  
  Udp.begin (localUdpPort); // start listening on local port
  Serial.printf ("Now listening at IP %s, UDP port %d\n", WiFi.localIP ().toString ().c_str (), localUdpPort); // print message
}

void loop () {
  packetSize = Udp.parsePacket (); // check if there is a packet available and get its size
  if (packetSize) { // if there is a packet
    Serial.printf ("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP ().toString ().c_str (), Udp.remotePort ()); // print message
    int len = Udp.read (buffer, sizeof buffer); // read the packet into the buffer and get the length
    if (len > 0) { // if there is data in the buffer
      buffer [len] = 0; // add a null terminator to the buffer
      
      // create an offset variable to track the position in the buffer
      int offset = 0;
      
      // use a macro to generate a loop that reads each element value from the buffer and prints it
      #define PRINT_ELEMENT(type, name, value) type##_t name; memcpy (&name, buffer + offset, sizeof name); offset += sizeof name; Serial.printf (#name ": %d\n", name); offset++; // skip the delimiter
      DATA_LIST (PRINT_ELEMENT) // use the data list macro with the print element macro to create the loop
    }
  }
}
