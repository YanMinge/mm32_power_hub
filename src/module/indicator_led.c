#include "indicator_led.h"
#include "battery_check.h"
#include "system.h"
#include "drv_gpio.h"
#include "drv_uart.h"

static void capacity_indicator(uint8_t battery_capacity)
{
    if(battery_capacity < 2)
    {
        aw21036_set_power_led(1, 0);
        aw21036_set_power_led(2, 0);
        aw21036_set_power_led(3, 0);
        aw21036_set_power_led(4, 0);
    }
    else if((battery_capacity >= 2) && (battery_capacity < 25))
    {
        aw21036_set_power_led(1, (battery_capacity-25) * 5);
        aw21036_set_power_led(2, 0);
        aw21036_set_power_led(3, 0);
        aw21036_set_power_led(4, 0);
    }
    else if((battery_capacity >= 25) && (battery_capacity < 50))
    {
        aw21036_set_power_led(1, 125);
        aw21036_set_power_led(2, (battery_capacity-25) * 5);
        aw21036_set_power_led(3, 0);
        aw21036_set_power_led(4, 0);
    }
    else if((battery_capacity >= 50) && (battery_capacity < 75))
    {
        aw21036_set_power_led(1, 125);
        aw21036_set_power_led(2, 125);
        aw21036_set_power_led(3, (battery_capacity-50) * 5);
        aw21036_set_power_led(4, 0);
    }
    else if(battery_capacity >= 75)
    {
        aw21036_set_power_led(1, 125);
        aw21036_set_power_led(2, 125);
        aw21036_set_power_led(3, 125);
        aw21036_set_power_led(4, (battery_capacity-75) * 5);
    }
}

static void charging_indicator(void)
{
    static int16_t time_count = 0;
    static uint8_t led_state = 0;
    time_count++;
    if(time_count > (100 / SENSOR_UPDATE_TIME))
    {
        led_state = led_state + 1;
        if(led_state == 5)
        {
            led_state = 0;
        }
        time_count = 0;
    }
    switch(led_state)
    {
        case 0:
        {
            aw21036_set_power_led(1, 0);
            aw21036_set_power_led(2, 0);
            aw21036_set_power_led(3, 0);
            aw21036_set_power_led(4, 0);
            break;
        }
        case 1:
        {
            aw21036_set_power_led(1, 0);
            aw21036_set_power_led(2, 0);
            aw21036_set_power_led(3, 0);
            aw21036_set_power_led(4, 125);
            break;
        }
        case 2:
        {
            aw21036_set_power_led(1, 0);
            aw21036_set_power_led(2, 0);
            aw21036_set_power_led(3, 125);
            aw21036_set_power_led(4, 125);
            break;
        }
        case 3:
        {
            aw21036_set_power_led(1, 0);
            aw21036_set_power_led(2, 125);
            aw21036_set_power_led(3, 125);
            aw21036_set_power_led(4, 125);
            break;
        }
        case 4:
        {
            aw21036_set_power_led(1, 125);
            aw21036_set_power_led(2, 125);
            aw21036_set_power_led(3, 125);
            aw21036_set_power_led(4, 125);
            break;
        }
    }
}

static void full_charge_indicator(void)
{
    aw21036_set_power_led(1, 125);
    aw21036_set_power_led(2, 125);
    aw21036_set_power_led(3, 125);
    aw21036_set_power_led(4, 125);
}

// static void charge_error_indicator(void)
// {

// }

void indicator_led_init(void)
{
    aw21036_init();
    aw21036_set_power_led(1, 0);
    aw21036_set_power_led(2, 0);
    aw21036_set_power_led(3, 0);
    aw21036_set_power_led(4, 0);
    set_all_rgb_led(0, 0, 0, 0);
}

void power_indicator(uint8_t battery_capacity)
{
    if(battery_charge_state() == UNCHARGE)
    {
        capacity_indicator(battery_capacity);
    }
    else if(battery_charge_state() == CHARGING)
    {
        charging_indicator();
    }
    else if(battery_charge_state() == CHARGE_FULL)
    {
        full_charge_indicator();
    }
}

void set_single_rgb_led(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    switch(num)
    {
        case 1:
            aw21036_set_rgb(LED_1, r, g, b, brightness);
            break;
        case 2:
            aw21036_set_rgb(LED_2, r, g, b, brightness);
            break;
        case 3:
            aw21036_set_rgb(LED_3, r, g, b, brightness);
            break;
        case 4:
            aw21036_set_rgb(LED_4, r, g, b, brightness);
            break;
        case 5:
            aw21036_set_rgb(LED_5, r, g, b, brightness);
            break;
        case 6:
            aw21036_set_rgb(LED_6, r, g, b, brightness);
            break;
        case 7:
            aw21036_set_rgb(LED_7, r, g, b, brightness);
            break;
        case 8:
            aw21036_set_rgb(LED_8, r, g, b, brightness);
            break;
        case 9:
            aw21036_set_rgb(LED_9, r, g, b, brightness);
            break;
        case 10:
            aw21036_set_rgb(LED_10, r, g, b, brightness);
            break;
        default:
            set_all_rgb_led(0, 0, 0, 0);
            break;
    }
}

void set_all_rgb_led(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    // uint32_t rgb_color = (r << 16 | g << 8 | b);
    // aw21036_set_color_all(rgb_color, brightness);
    for(uint8_t i = 1; i < RGB_NUM + 1; i++)
    {
        set_single_rgb_led(i, r, g, b, brightness);
    }   
}

// void set_all_rgb_led(uint32_t rgb_color, uint8_t brightness)
// {
//     aw21036_set_color_all(rgb_color, brightness);
// }


