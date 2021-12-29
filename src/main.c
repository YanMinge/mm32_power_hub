#include "stdio.h"
#include "stdint.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "drv_ring_buf.h"
#include "drv_timer.h"
#include "indicator_led.h"
#include "drv_aw21036.h"
#include "drv_i2c.h"
#include "battery_check.h"
#include "power_management.h"
#include "ir_pair_tube.h"
#include "system.h"

static void hardware_init(void)
{
    power_pin_init();
    ir_pair_tube_init();
    indicator_led_init();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

int main(void)
{
    systick_init();
    uart_ringbuf_init();
    drv_uart1_init(115200);
    drv_uart_printf(UART1, "hello power hub\r\n");
    hardware_init();
    unsigned long current_time = millis();
    ir_tx_enable(true);
    power_on();
    while(1)
    {
        if(millis() - current_time >= SENSOR_UPDATE_TIME)
        {
            power_management();
            current_time = millis();
        }
        adc_update();
        ir_rx_update();
        usb_state_update();
    }
}
