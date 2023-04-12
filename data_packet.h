// data_packet.h
#ifndef DATA_PACKET_H
#define DATA_PACKET_H

// include other header files if needed
//#include <stdint.h>

// define a packed struct type
packed struct data_packet {
  uint8_t seqnumber; // unsigned char
  char data [500];
};

#endif
