#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

#include "drv_gpio.h"
#include "drv_uart.h"

#define uart_printf(...)          log_uart_printf(UART1, __VA_ARGS__)
// #define uart_printf(...)

#define I2C_MASTER_SCL_IO            PA_11
#define I2C_MASTER_SDA_IO            PA_12
#define I2C_SCL_AFMODE               GPIO_AF_5
#define I2C_SDA_AFMODE               GPIO_AF_5

#define I2C_MASTER_NUM               I2C1              /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ           200000            /*!< I2C master clock frequency */

void i2c_master_init(void);
bool is_i2c_initialized(void);
int8_t i2c_master_read_reg(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *data);
int8_t i2c_master_read_mem(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t size);
int8_t i2c_master_write_reg(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t data);
int8_t i2c_master_write_mem(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t size);

#endif
