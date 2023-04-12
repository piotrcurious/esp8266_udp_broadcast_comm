
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

// define a macro to create a new type called data_element_t that consists of a type name, a type size, a variable name, a variable value and a delimiter
#define DATA_ELEMENT(type, name, value) type##_t name; uint8_t name##_size; char name##_delim;

// define a packed struct type
packed struct data_packet {
  // use the macro to create the struct elements
  DATA_ELEMENT (seqnumber, seqnumber, '0')
  DATA_ELEMENT (temperature, temperature, -10)
  DATA_ELEMENT (humidity, humidity, 50.5)
  DATA_ELEMENT (name, name, "ESP8266")
  DATA_ELEMENT (status, status, true)
};

// define a delimiter to separate each element in the buffer
#define DELIMITER ','

#endif
