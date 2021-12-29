#include "ir_pair_tube.h"
#include "drv_gpio.h"

static uint16_t ir_value = 0;

void ir_pair_tube_init(void)
{
    drv_gpio_pin_mode(IR1_DETECT_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(IR2_DETECT_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(IR3_DETECT_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(IR4_DETECT_PIN, GPIO_MODE_INPUT_NONE);
    drv_gpio_pin_mode(IR_TX_ENABLE_PIN, GPIO_MODE_OUTPUT);       //IR TX
}

void ir_tx_enable(bool enable)
{
    drv_gpio_digital_write(IR_TX_ENABLE_PIN, enable);
}

void ir_rx_update(void)
{
    int ir1_value = drv_gpio_digital_read(IR1_DETECT_PIN);
    int ir2_value = drv_gpio_digital_read(IR2_DETECT_PIN);
    int ir3_value = drv_gpio_digital_read(IR3_DETECT_PIN);
    int ir4_value = drv_gpio_digital_read(IR4_DETECT_PIN);
    if(ir1_value != 0)
    {
        ir1_value = 1;
    }
    if(ir2_value != 0)
    {
        ir2_value = 1;
    }
    if(ir3_value != 0)
    {
        ir3_value = 1;
    }
    if(ir4_value != 0)
    {
        ir4_value = 1;
    }
    ir_value = ir1_value | (ir2_value << 1) | (ir3_value << 2) | (ir4_value << 3);
}

uint16_t get_ir_data(void)
{
    return ir_value;
}