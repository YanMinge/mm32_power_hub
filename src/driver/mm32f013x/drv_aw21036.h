#ifndef __DRV_AW21036_H__
#define __DRV_AW21036_H__

#include "drv_gpio.h"

static const uint8_t neo_pixel_gamma_table[256] = 
{
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
   13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
   20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
   30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
   42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
   58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 73, 75,
   76, 77, 78, 80, 81, 82, 84, 85, 86, 88, 89, 90, 92, 93, 94, 96,
   97, 99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,
  122,124,125,127,129,130,132,134,136,137,139,141,143,145,146,148,
  150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,
  182,184,186,188,191,193,195,197,199,202,204,206,209,211,213,215,
  218,220,223,225,227,230,232,235,237,240,242,245,247,250,252,255
};

/******************************************************
 *
 * Register List
 *
 *****************************************************/
#define RGB_NUM             10

#define AW21036_I2C_ADDRESS 0x34

#define AW21036_REG_GCR     0x00
#define AW21036_REG_BR0     0x01
#define AW21036_REG_BR1     0x02
#define AW21036_REG_BR2     0x03
#define AW21036_REG_BR3     0x04
#define AW21036_REG_BR4     0x05
#define AW21036_REG_BR5     0x06
#define AW21036_REG_BR6     0x07
#define AW21036_REG_BR7     0x08
#define AW21036_REG_BR8     0x09
#define AW21036_REG_BR9     0x0A
#define AW21036_REG_BR10    0x0B
#define AW21036_REG_BR11    0x0C
#define AW21036_REG_BR12    0x0D
#define AW21036_REG_BR13    0x0E
#define AW21036_REG_BR14    0x0F
#define AW21036_REG_BR15    0x10
#define AW21036_REG_BR16    0x11
#define AW21036_REG_BR17    0x12
#define AW21036_REG_BR18    0x13
#define AW21036_REG_BR19    0x14
#define AW21036_REG_BR20    0x15
#define AW21036_REG_BR21    0x16
#define AW21036_REG_BR22    0x17
#define AW21036_REG_BR23    0x18
#define AW21036_REG_BR24    0x19
#define AW21036_REG_BR25    0x1A
#define AW21036_REG_BR26    0x1B
#define AW21036_REG_BR27    0x1C
#define AW21036_REG_BR28    0x1D
#define AW21036_REG_BR29    0x1E
#define AW21036_REG_BR30    0x1F
#define AW21036_REG_BR31    0x20
#define AW21036_REG_BR32    0x21
#define AW21036_REG_BR33    0x22
#define AW21036_REG_BR34    0x23
#define AW21036_REG_BR35    0x24
#define AW21036_REG_UPDATE  0x49
#define AW21036_REG_COL0    0x4A
#define AW21036_REG_COL1    0x4B
#define AW21036_REG_COL2    0x4C
#define AW21036_REG_COL3    0x4D
#define AW21036_REG_COL4    0x4E
#define AW21036_REG_COL5    0x4F
#define AW21036_REG_COL6    0x50
#define AW21036_REG_COL7    0x51
#define AW21036_REG_COL8    0x52
#define AW21036_REG_COL9    0x53
#define AW21036_REG_COL10   0x54
#define AW21036_REG_COL11   0x55
#define AW21036_REG_COL12   0x56
#define AW21036_REG_COL13   0x57
#define AW21036_REG_COL14   0x58
#define AW21036_REG_COL15   0x59
#define AW21036_REG_COL16   0x5A
#define AW21036_REG_COL17   0x5B
#define AW21036_REG_COL18   0x5C
#define AW21036_REG_COL19   0x5D
#define AW21036_REG_COL20   0x5E
#define AW21036_REG_COL21   0x5F
#define AW21036_REG_COL22   0x60
#define AW21036_REG_COL23   0x61
#define AW21036_REG_COL24   0x62
#define AW21036_REG_COL25   0x63
#define AW21036_REG_COL26   0x64
#define AW21036_REG_COL27   0x65
#define AW21036_REG_COL28   0x66
#define AW21036_REG_COL29   0x67
#define AW21036_REG_COL30   0x68
#define AW21036_REG_COL31   0x69
#define AW21036_REG_COL32   0x6A
#define AW21036_REG_COL33   0x6B
#define AW21036_REG_COL34   0x6C
#define AW21036_REG_COL35   0x6D
#define AW21036_REG_GCCR    0x6E
#define AW21036_REG_PHCR    0x70
#define AW21036_REG_OSDCR   0x71
#define AW21036_REG_OSST0   0x72
#define AW21036_REG_OSST1   0x73
#define AW21036_REG_OSST2   0x74
#define AW21036_REG_OSST3   0x75
#define AW21036_REG_OSST4   0x76
#define AW21036_REG_OTCR    0x77
#define AW21036_REG_SSCR    0x78
#define AW21036_REG_UVCR    0x79
#define AW21036_REG_GCR2    0x7A
#define AW21036_REG_GCR4    0x7C
#define AW21036_REG_VER     0x7E
#define AW21036_REG_RESET   0x7F
#define AW21036_REG_WBR     0x90
#define AW21036_REG_WBG     0x91
#define AW21036_REG_WBB     0x92
#define AW21036_REG_PATCFG  0xA0
#define AW21036_REG_PATGO   0xA1
#define AW21036_REG_PATT0   0xA2
#define AW21036_REG_PATT1   0xA3
#define AW21036_REG_PATT2   0xA4
#define AW21036_REG_PATT3   0xA5
#define AW21036_REG_FADEH   0xA6
#define AW21036_REG_FADEL   0xA7
#define AW21036_REG_GCOLR   0xA8
#define AW21036_REG_GCOLG   0xA9
#define AW21036_REG_GCOLB   0xAA
#define AW21036_REG_GCFG0   0xAB
#define AW21036_REG_GCFG1   0xAC

#define HIGH              1
#define LOW               0

typedef enum
{
    LED_1 = 2,
    LED_2 = 3,
    LED_3 = 5,
    LED_4 = 1,
    LED_5 = 0,
    LED_6 = 6,
    LED_7 = 4,
    LED_8 = 9,
    LED_9 = 8,
    LED_10 = 7,
}rgb_led_num_type;

extern int8_t aw21036_init(void);
extern bool is_aw21036_initialized(void);
extern int8_t aw21036_set_rgb(rgb_led_num_type num, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
extern int8_t aw21036_set_all_rgb(uint8_t* data_buf);
extern int8_t aw21036_set_power_led(uint8_t num, uint8_t brightness);
extern int8_t aw21036_set_color_all(uint32_t rgb_color, uint8_t brightness);
extern void aw21036_set_code_color(uint32_t rgb_color, uint8_t brightness);
extern uint32_t hsv2rgb(uint16_t hue, uint8_t sat, uint8_t val);
extern uint8_t gamma8(uint8_t x);
extern uint32_t gamma32(uint32_t x);

#endif /* __DRV_AW21036_H__ */ 
