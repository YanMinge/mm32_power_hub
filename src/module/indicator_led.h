#ifndef __INDICATOR_LED_H__
#define __INDICATOR_LED_H__
#include "stdint.h"
#include "stdbool.h"
#include "drv_aw21036.h"

void indicator_led_init(void);
void power_indicator(uint8_t battery_capacity);
void set_single_rgb_led(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
void set_all_rgb_led(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
// void set_all_rgb_led(uint32_t rgb_color, uint8_t brightness);

#endif //__INDICATOR_LED_H__