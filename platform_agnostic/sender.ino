//Sure, I can try to modify the code as you requested. Here is how I would do it:
//
//- To include type definitions in the data_packet header file, I would use the `typedef` keyword to create aliases for the data types that I want to use in the struct. For example, I could use `typedef uint8_t seqnumber_t` to define a type called `seqnumber_t` that is an alias for `uint8_t`. Then I would use the new type names in the struct definition instead of the original ones. This way, I can ensure that the struct fields have the same size and format across different platforms.
//- To cast the data to the correct format when sending and receiving, I would use the `htons`, `htonl`, `ntohs` and `ntohl` functions to convert the data between host byte order and network byte order. These functions are defined in the `<lwip/def.h>` header file for ESP8266. Host byte order is the byte order used by the platform that runs the code, while network byte order is a standard byte order used for network communication. By converting the data between these two byte orders, I can avoid problems caused by different endianness of different platforms. Endianness is the order of bytes in a multi-byte data type, such as big-endian (most significant byte first) or little-endian (least significant byte first).
//- To include a few more example data types into the struct, I would add some fields with different sizes and formats, such as `int16_t`, `float`, `char` and `bool`. Then I would assign some values to them in the sender code and print them in the receiver code.
//

// sender.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <lwip/def.h> // include this header file for byte order conversion functions
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
  packettosend.temperature = -10; // assign a value to the temperature field of the struct
  packettosend.humidity = 50.5; // assign a value to the humidity field of the struct
  packettosend.name = "ESP8266"; // assign a value to the name field of the struct
  packettosend.status = true; // assign a value to the status field of the struct
  
}

void loop () {
  // convert the struct data to network byte order
  packettosend.seqnumber = htons (packettosend.seqnumber);
  packettosend.temperature = htons (packettosend.temperature);
  packettosend.humidity = htonl (packettosend.humidity);
  // no need to convert the name and status fields as they are single-byte types
  
  memcpy (buffer, &packettosend, sizeof packettosend); // copy the struct data into the buffer
  Udp.beginPacket (broadcastIp, remoteUdpPort); // start a UDP packet to the broadcast IP and port
  Udp.write (buffer, sizeof buffer); // write the buffer data to the packet
  Udp.endPacket (); // end and send the packet
  
  delay (1000); // wait for one second
}
