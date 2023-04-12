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

// use a macro to define a list of all the struct elements and their values
#define DATA_LIST DATA_ELEMENT (seqnumber, seqnumber, '0') DATA_ELEMENT (temperature, temperature, -10) DATA_ELEMENT (humidity, humidity, 50.5) DATA_ELEMENT (name, name, "ESP8266") DATA_ELEMENT (status, status, true)

// use the macro to initialize the struct variable
struct data_packet packettosend = {DATA_LIST};
char buffer [sizeof packettosend + 5]; // create a buffer with enough size plus 5 delimiters

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
  // create a pointer to the struct data
  char *ptr = (char *)&packettosend;
  
  // create an offset variable to track the position in the buffer
  int offset = 0;
  
  // use a macro to generate a loop that copies each element value into the buffer and updates the offset
  #define COPY_ELEMENT(type, name, value) memcpy (buffer + offset, &packettosend.name, sizeof packettosend.name); offset += sizeof packettosend.name; buffer [offset] = packettosend.name##_delim; offset++;
  DATA_LIST (COPY_ELEMENT) // use the data list macro with the copy element macro to create the loop
  
  Udp.beginPacket (broadcastIp, remoteUdpPort); // start a UDP packet to the broadcast IP and port
  Udp.write (buffer, offset); // write the buffer data to the packet with the correct size
  Udp.endPacket (); // end and send the packet
  
  delay (1000); // wait for one second
}
