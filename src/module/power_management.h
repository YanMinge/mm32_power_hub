#ifndef __POWER_MANAGEMENT_H__
#define __POWER_MANAGEMENT_H__

#include "stdbool.h"

void power_off(void);
void power_on(void);
void power_pin_init(void);
bool get_power_pin_state(void);
void power_management(void);

#endif //__POWER_MANAGEMENT_H__