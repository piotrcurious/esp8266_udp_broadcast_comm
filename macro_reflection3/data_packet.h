
#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <Arduino.h>

// Define the data elements of the packet using macros
// Each element has a type, a name and a default value
#define DATA_PACKET_ELEMENTS \
  X(int16_t, temperature, 25) \
  X(uint8_t, humidity, 50) \
  X(uint16_t, light, 0) \
  X(bool, motion, false) \
  X(char[16], message, "Hello")

// Define the data packet struct using the macros
typedef struct {
  #define X(type, name, value) type name;
  DATA_PACKET_ELEMENTS
  #undef X
} data_packet_t;

// Define the size of the data packet in bytes
#define DATA_PACKET_SIZE sizeof(data_packet_t)

// Initialize the data packet with default values
#define DATA_PACKET_INIT(packet) \
  do { \
    #define X(type, name, value) packet.name = value;
    DATA_PACKET_ELEMENTS
    #undef X
  } while (0)

// Serialize the data packet into a byte array
#define DATA_PACKET_SERIALIZE(buffer, packet) \
  do { \
    uint8_t *ptr = buffer; \
    #define X(type, name, value) memcpy(ptr, &packet.name, sizeof(type)); ptr += sizeof(type);
    DATA_PACKET_ELEMENTS
    #undef X
  } while (0)

// Deserialize a byte array into a data packet
#define DATA_PACKET_DESERIALIZE(packet, buffer) \
  do { \
    uint8_t *ptr = buffer; \
    #define X(type, name, value) memcpy(&packet.name, ptr, sizeof(type)); ptr += sizeof(type);
    DATA_PACKET_ELEMENTS
    #undef X
  } while (0)

// Print the data packet for debugging
#define DATA_PACKET_PRINT(packet) \
  do { \
    #define X(type, name, value) Serial.print(#name); Serial.print(": "); Serial.println(packet.name);
    DATA_PACKET_ELEMENTS
    #undef X
  } while (0)

#endif // DATA_PACKET_H
