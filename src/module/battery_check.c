#include <stdio.h>
#include <math.h>
#include "battery_check.h"
#include "indicator_led.h"
#include "drv_gpio.h"

static float battery_voltage = 0;
static uint8_t battery_capacity = 0;
static bool is_usb_inserted = false;

static const battery_capacity_table_t battery_capacity_table[BATTERY_CAPACITY_TABLE_LEN] =
{
    { 4.17f, 100 },
    { 4.04f, 90 },
    { 3.94f, 80 },
    { 3.84f, 70 },
    { 3.74f, 60 },
    { 3.65f, 50 },
    { 3.61f, 40 },
    { 3.58f, 30 },
    { 3.55f, 20 },
    { 3.52f, 15 },
    { 3.50f, 10 },
    { 3.45f, 5  },
    { 3.30f, 0  }
};

static uint32_t battery_check_get_value_and_filter(void)
{
    uint32_t analog_battery_raw = drv_gpio_analog_read(PA_0);
    return analog_battery_raw;
}

bool is_usb_insert(void)
{
    return is_usb_inserted;
}

void usb_state_update(void)
{
    int usb_gpio_value = drv_gpio_digital_read(USB_DETECTION_PIN);
    if(usb_gpio_value == LOW)
    {
        is_usb_inserted = true;
    }
    else
    {
        is_usb_inserted = false;
    }
}

battery_charge_type battery_charge_state(void)
{
    battery_charge_type charge_state = UNCHARGE;
    int charge_gpio_value = drv_gpio_digital_read(CHARGING_DETECTION_PIN);
    if(is_usb_insert() && (charge_gpio_value == HIGH))
    {
        charge_state = CHARGE_FULL;
    }
    else if(charge_gpio_value == HIGH)
    {
        charge_state = UNCHARGE;
    }
    else
    {
        charge_state = CHARGING;
    }
    return charge_state;
}

void battery_voltage_update(void)
{
    uint32_t battery_current_ad_value = battery_check_get_value_and_filter();
    battery_voltage = (float)(battery_current_ad_value / 4095.0f * 3.3f * 1.556f);
}

void battery_capacity_update(void)
{
    for(uint8_t i = 0; i < BATTERY_CAPACITY_TABLE_LEN; i++)
    {
        if(battery_voltage > battery_capacity_table[i].battery_voltage)
        {
            if(i == 0)
            {
                battery_capacity = battery_capacity_table[i].battery_capacity;
                break;
            }
            else
            {
                battery_capacity = battery_capacity_table[i].battery_capacity + \
                       (uint8_t)round((battery_voltage - battery_capacity_table[i].battery_voltage) * \
                                      (battery_capacity_table[i - 1].battery_capacity - battery_capacity_table[i].battery_capacity) / \
                                      (battery_capacity_table[i - 1].battery_voltage - battery_capacity_table[i].battery_voltage));
                break;
            }
        }
    }
    if(battery_voltage < battery_capacity_table[BATTERY_CAPACITY_TABLE_LEN - 1].battery_voltage)
    {
        battery_capacity = 0;
    }
    power_indicator(battery_capacity);
}

float get_battery_voltage(void)
{
    return battery_voltage;
}

uint8_t get_battery_capacity(void)
{
    return battery_capacity;
}
