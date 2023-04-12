#ifndef DATA_PACKET_H_
#define DATA_PACKET_H_

// Define a macro list of data elements with their types, names and initial values

#define DATA_ELEMENT_LIST(F) \
F(int16_t, temperature, -10) \
F(uint8_t, humidity, 50) \
F(float, pressure, 1013.25) \
F(bool, led, false) \
F(char, message[10], "Hello")

// Define a struct type that contains all the data elements as members

typedef struct {
  #define DECLARE_DATA_ELEMENT(type, name, value) type name;
  DATA_ELEMENT_LIST(DECLARE_DATA_ELEMENT)
} data_packet_t;

#endif
