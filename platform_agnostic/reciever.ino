
// receiver.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <lwip/def.h> // include this header file for byte order conversion functions
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
      
      // convert the struct data to host byte order
      dataRecv->seqnumber = ntohs (dataRecv->seqnumber);
      dataRecv->temperature = ntohs (dataRecv->temperature);
      dataRecv->humidity = ntohl (dataRecv->humidity);
      // no need to convert the name and status fields as they are single-byte types
      
      Serial.printf ("Seqnumber: %d\n", dataRecv->seqnumber); // print the seqnumber field of the struct
      Serial.printf ("Temperature: %d\n", dataRecv->temperature); // print the temperature field of the struct
      Serial.printf ("Humidity: %f\n", dataRecv->humidity); // print the humidity field of the struct
      Serial.printf ("Name: %s\n", dataRecv->name); // print the name field of the struct
      Serial.printf ("Status: %d\n", dataRecv->status); // print the status field of the struct
    }
  }
}
