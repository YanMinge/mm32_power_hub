#ifndef __PROTOCOL_PARSE_H__
#define __PROTOCOL_PARSE_H__
#include "stdint.h"
#include "stdbool.h"

#define CAR_MACHINE_ID     0x81
#define BROADCAST_ID       0xff

uint8_t device_hw_id;
void parse_command(void);

#endif //__PROTOCOL_PARSE_H__