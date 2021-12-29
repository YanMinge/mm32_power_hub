#ifndef __IR_PAIR_TUBE_H__
#define __IR_PAIR_TUBE_H__

#include "stdbool.h"
#include "stdint.h"

#define IR1_DETECT_PIN     PA_4
#define IR2_DETECT_PIN     PA_5
#define IR3_DETECT_PIN     PA_7
#define IR4_DETECT_PIN     PA_6
#define IR_TX_ENABLE_PIN   PB_2

void ir_pair_tube_init(void);
void ir_tx_enable(bool enable);
void ir_rx_update(void);
uint16_t get_ir_data(void);

#endif //__IR_PAIR_TUBE_H__