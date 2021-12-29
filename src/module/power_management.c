#include "power_management.h"
#include "ir_pair_tube.h"
#include "indicator_led.h"
#include "battery_check.h"
#include "drv_timer.h"
#include "system.h"

static bool power_on_state = true;
static bool usb_insert_state = false;

static void power_keep(bool keep_power)
{
    drv_gpio_digital_write(POWER_HOLD_PIN, keep_power);
}

void power_off(void)
{
    power_on_state = false;
    power_keep(false);
}

void power_on(void)
{
    power_on_state = true;
    power_keep(true);
}

void power_pin_init(void)
{
    drv_gpio_pin_mode(POWER_SW_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(POWER_HOLD_PIN, GPIO_MODE_OUTPUT);

    drv_gpio_pin_mode(POWER_VOLTAGE_DETECTION_PIN, GPIO_MODE_INPUT_ANALOG);
    drv_gpio_pin_mode(HARDWARE_VERSION_DETECTION_PIN, GPIO_MODE_INPUT_ANALOG);
    drv_gpio_pin_mode(POWER_CURRENT_DETECTION_PIN, GPIO_MODE_INPUT_ANALOG);
    drv_gpio_pin_mode(CHARGING_DETECTION_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(USB_DETECTION_PIN, GPIO_MODE_INPUT_NONE);
}

bool get_power_pin_state(void)
{
    return !(drv_gpio_digital_read(POWER_SW_PIN));
}

void power_off_detect(void)
{
    static uint16_t power_off_key_down_count = 0;
    static uint16_t power_off_key_up_count = 0;
    if((power_on_state == true) && (get_power_pin_state() == true))
    {
        power_off_key_down_count++;
        if(power_off_key_down_count > (500 / SENSOR_UPDATE_TIME))
        {
            power_off_key_down_count = 0;
            power_off_key_up_count = 0;
            while(get_power_pin_state() == true);
            drv_uart_printf(UART1, "power_off 1\r\n");

            //tx set to low
            drv_gpio_pin_mode(PA_9, GPIO_MODE_OUTPUT);
            drv_gpio_digital_write(PA_9, LOW);
            delay_ms(5000);
            power_off();
            delay_ms(100);
        }
    }
    else
    {
        power_off_key_up_count++;
        if(power_off_key_up_count > 3)
        {
            power_off_key_down_count = 0;
            power_off_key_up_count = 0;
        }
    }
}

void power_management(void)
{
    // if(is_usb_insert())
    // {
    //     drv_uart_printf(UART1, "is_usb_insert\r\n");
    //     usb_insert_state = true;
    //     set_all_rgb_led(0, 0, 0, 0);
    //     ir_tx_enable(false);
    // }
    // if((usb_insert_state == true) && !is_usb_insert())
    // {
    //     drv_uart_printf(UART1, "power_off 2\r\n");
    //     power_off();
    //     delay_ms(100);
    // }
    power_off_detect();
    battery_voltage_update();
    battery_capacity_update();
    battery_charge_state();
}
