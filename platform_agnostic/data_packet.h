// data_packet.h
#ifndef DATA_PACKET_H
#define DATA_PACKET_H

// include other header files if needed
#include <stdint.h>
#include <stdbool.h>

// define some type aliases for convenience
typedef uint8_t seqnumber_t;
typedef int16_t temperature_t;
typedef float humidity_t;
typedef char name_t [20];
typedef bool status_t;

// define a packed struct type
packed struct data_packet {
  seqnumber_t seqnumber; // 1 byte
  temperature_t temperature; // 2 bytes
  humidity_t humidity; // 4 bytes
  name_t name; // 20 bytes
  status_t status; // 1 byte
};

#endif
