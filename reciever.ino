
// receiver.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "data_packet.h" // include the header file

const char* ssid = "xxxxxx"; // your WiFi SSID
const char* password = "xxxxxx"; // your WiFi password

WiFiUDP Udp; // create a UDP object
unsigned int localUdpPort = 4211; // local port to listen on
char buffer [sizeof data_packet]; // create a buffer with enough size
int packetSize; // variable to store the packet size
struct data_packet *dataRecv; // pointer variable to store the received struct

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
      dataRecv = (struct data_packet *)&buffer [0]; // cast the buffer into a struct pointer
      Serial.printf ("Seqnumber: %d\n", dataRecv->seqnumber); // print the seqnumber field of the struct
      Serial.printf ("Data: %s\n", dataRecv->data); // print the data field of the struct
    }
  }
}
