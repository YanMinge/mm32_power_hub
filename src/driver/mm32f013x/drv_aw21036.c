#include <stdio.h>
#include <math.h>
#include "drv_i2c.h"
#include "drv_timer.h"
#include "drv_aw21036.h"

#define REG_WB_DEFAULT_VALUE  0x3F

static bool aw21036_initialized = false;

int8_t aw21036_init(void)
{
    int8_t ret = 0;
    if(aw21036_initialized == true)
    {
        return ret;
    }
    if(!is_i2c_initialized())
    {
        i2c_master_init();
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_RESET, 0x00);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_RESET reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_GCR, 0x01);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_GCR reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_GCCR, 0xff);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_GCCR reg error!\r\n");
    }

    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_OTCR, 0xc0);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_OTCR reg error!\r\n");
    }

    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_WBR, REG_WB_DEFAULT_VALUE);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_WBR reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_WBG, REG_WB_DEFAULT_VALUE);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_WBG reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_WBB, REG_WB_DEFAULT_VALUE);
    delay_us(20);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_WBB reg error!\r\n");
    }
    // uint8_t version;
    // i2c_master_read_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_VER, &version);
    // uart_printf("aw21036_initialized OK! version:0x%x\r\n",version);
    aw21036_initialized = true;
    return ret;
}

bool is_aw21036_initialized(void)
{
    return aw21036_initialized;
}

int8_t aw21036_set_rgb(rgb_led_num_type num, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    int8_t ret = 0;
    if(!is_aw21036_initialized())
    {
        aw21036_init();
    }
    uint8_t brightness_temp = gamma8(brightness);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL0 + (3 * num), r);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_COL0 reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL1 + (3 * num), g);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_COL1 reg error!\r\n");
    }  
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL2 + (3 * num), b);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_COL3 reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR0 + (3 * num), brightness_temp);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_BR0 reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR1 + (3 * num), brightness_temp);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_BR1 reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR2 + (3 * num), brightness_temp);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_BR2 reg error!\r\n");
    }
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_UPDATE, 0x00);
    if(ret != 0)
    {
        uart_printf("write AW21036_REG_UPDATE reg error!\r\n");
    }
    return ret;
}

int8_t aw21036_set_all_rgb(uint8_t* data_buf)
{
    int8_t ret = 0;
    uint8_t rgb_buf[RGB_NUM * 3] = {0};
    uint8_t bri_buf[RGB_NUM * 3] = {0};
    if(!is_aw21036_initialized())
    {
        aw21036_init();
    }
    for(uint8_t i = 0; i < RGB_NUM; i++)
    {
        rgb_buf[i*3 + 0] = data_buf[i*4 + 0];
        rgb_buf[i*3 + 1] = data_buf[i*4 + 1];
        rgb_buf[i*3 + 2] = data_buf[i*4 + 2];
        bri_buf[i*3 + 0] = gamma8(data_buf[i*4 + 3]);
        bri_buf[i*3 + 1] = gamma8(data_buf[i*4 + 3]);
        bri_buf[i*3 + 2] = gamma8(data_buf[i*4 + 3]);
    }
    ret = i2c_master_write_mem(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL0, rgb_buf, RGB_NUM * 3);
    ret = i2c_master_write_mem(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR0, bri_buf, RGB_NUM * 3);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_UPDATE, 0x00);
    return ret;
}

int8_t aw21036_set_power_led(uint8_t num, uint8_t brightness)
{
    int8_t ret = 0;
    if(!is_aw21036_initialized())
    {
        aw21036_init();
    }
    uint8_t brightness_temp = gamma8(brightness);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL29 + num, 0xff);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR29 + num, brightness_temp);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_UPDATE, 0x00);
    return ret;
}

int8_t aw21036_set_color_all(uint32_t rgb_color, uint8_t brightness)
{
    int8_t ret = 0;
    uint8_t rgb_buf[RGB_NUM * 3] = {0};
    uint8_t bri_buf[RGB_NUM * 3] = {0};
    uint16_t R,G,B;
    R = rgb_color >> 16;
    G = (rgb_color >> 8) & 0xff;
    B = rgb_color & 0xff;
    if(!is_aw21036_initialized())
    {
        aw21036_init();
    }
    uint8_t brightness_temp = gamma8(brightness);
    for(uint8_t i = 0; i < RGB_NUM; i++)
    {
        rgb_buf[i*3 + 0] = R;
        rgb_buf[i*3 + 1] = G;
        rgb_buf[i*3 + 2] = B;
        bri_buf[i*3 + 0] = brightness_temp;
        bri_buf[i*3 + 1] = brightness_temp;
        bri_buf[i*3 + 2] = brightness_temp;
    }
    ret = i2c_master_write_mem(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_COL0, rgb_buf, RGB_NUM * 3);
    ret = i2c_master_write_mem(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_BR0, bri_buf, RGB_NUM * 3);
    ret = i2c_master_write_reg(I2C_MASTER_NUM, AW21036_I2C_ADDRESS, AW21036_REG_UPDATE, 0x00);
    return ret;
}

uint32_t hsv2rgb(uint16_t hue, uint8_t sat, uint8_t val)
{
    uint8_t r, g, b;
    hue = (hue * 1530L + 32768) / 65536;
    if(hue < 510)
    {
        b = 0;
        if(hue < 255)
        {
            r = 255;
            g = hue;
        }
        else
        {
            r = 510 - hue;
            g = 255;
        }
    }
    else if(hue < 1020)
    {
        r = 0;
        if(hue < 765)
        {
            g = 255;
            b = hue - 510;
        }
        else
        {
            g = 1020 - hue;
            b = 255;
        }
    }
    else if(hue < 1530)
    {
        g = 0;
        if(hue < 1275)
        {
            r = hue - 1020;
            b = 255;
        }
        else
        {
            r = 255;
            b = 1530 - hue;
        }
    }
    else
    {
        r = 255;
        g = b = 0;
    }
    uint32_t v1 = 1 + val;
    uint32_t s1 = 1 + sat;
    uint32_t s2 = 255 - sat;
    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
            (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
            (((((b * s1) >> 8) + s2) * v1)           >> 8);
}

uint8_t gamma8(uint8_t x)
{
    return neo_pixel_gamma_table[x];
}

uint32_t gamma32(uint32_t x)
{
    uint8_t *y = (uint8_t *)&x;
    for(uint8_t i = 0; i < 4; i++)
    {
        y[i] = gamma8(y[i]);
    }
    return x;
}

void aw21036_set_code_color(uint32_t rgb_color, uint8_t brightness)
{
    uint16_t r, g, b;
    r = rgb_color >> 16;
    g = (rgb_color >> 8) & 0xff;
    b = rgb_color & 0xff;
    for(uint8_t i = 0; i < RGB_NUM; i++)
    {
        aw21036_set_rgb(i, r, g, b, brightness);
    }
}

