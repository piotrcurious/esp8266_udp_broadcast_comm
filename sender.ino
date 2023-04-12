
// sender.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "data_packet.h" // include the header file

const char* ssid = "xxxxxx"; // your WiFi SSID
const char* password = "xxxxxx"; // your WiFi password

WiFiUDP Udp; // create a UDP object
unsigned int localUdpPort = 4210; // local port to listen on
IPAddress broadcastIp (192, 168, 50, 255); // broadcast IP address of your network
unsigned int remoteUdpPort = 4211; // remote port to send to

struct data_packet packettosend; // create a struct variable
char buffer [sizeof packettosend]; // create a buffer with enough size

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
  
  packettosend.seqnumber = '0'; // assign a value to the seqnumber field of the struct
  packettosend.data = "some binary data"; // assign a value to the data field of the struct
}

void loop () {
  memcpy (buffer, &packettosend, sizeof packettosend); // copy the struct data into the buffer
  Udp.beginPacket (broadcastIp, remoteUdpPort); // start a UDP packet to the broadcast IP and port
  Udp.write (buffer, sizeof buffer); // write the buffer data to the packet
  Udp.endPacket (); // end and send the packet
  
  delay (1000); // wait for one second
}
