
#include <stdio.h>
#include "drv_i2c.h"
#include "drv_common_def.h"
#include "hal_conf.h"

static bool i2c_initialized = false;

static void i2c_gpio_config(void)
{
    uint32_t port_num, pin_num;
    GPIO_InitTypeDef GPIO_InitStructure;

    port_num = I2C_MASTER_SCL_IO >> 4;
    pin_num = BIT(I2C_MASTER_SCL_IO & 0x0f);

    GPIO_InitStructure.GPIO_Pin  = pin_num;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    //Keep the bus free which means SCK & SDA is high
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

    if(port_num == PORTA)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, (I2C_MASTER_SCL_IO & 0x0f), I2C_SCL_AFMODE);
    }
    else
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOB, (I2C_MASTER_SCL_IO & 0x0f), I2C_SCL_AFMODE);
    }

    port_num = I2C_MASTER_SDA_IO >> 4;
    pin_num = BIT(I2C_MASTER_SDA_IO & 0x0f);


    GPIO_InitStructure.GPIO_Pin  = pin_num;
    if(port_num == PORTA)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, (I2C_MASTER_SDA_IO & 0x0f), I2C_SDA_AFMODE);
    }
    else
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOB, (I2C_MASTER_SDA_IO & 0x0f), I2C_SDA_AFMODE);
    }
}

static void i2c_tx_empty_check(I2C_TypeDef* i2c_num)
{
    while(1)
    {
        if((i2c_num->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY))  // send  buffer area null
        {
            break;
        }
    }
}

static void i2c_rx_full_check(I2C_TypeDef* i2c_num)
{
    i2c_num->IC_DATA_CMD = I2C_DATA_CMD_CMD;  // read  data  instructions  ,Canregister be omitted
    while(1)
    {
        // uart_printf("IC_RAW_INTR_STAT:%d\r\n", i2c_num->IC_RAW_INTR_STAT);
        if((i2c_num->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_RX_FULL))  // receive  buffer area register  null
        {
            break;
        }
    }
}

static void i2c_tx_byte(I2C_TypeDef* i2c_num, uint8_t data)
{
    i2c_num->IC_DATA_CMD = data;  // send  data
    i2c_tx_empty_check(i2c_num);
}

static uint8_t i2c_rx_byte(I2C_TypeDef* i2c_num)
{
    uint8_t temp;
    i2c_rx_full_check(i2c_num);
    temp = i2c_num->IC_DATA_CMD;
    return temp;
}

static ITStatus i2c_get_it_status(I2C_TypeDef* i2c_num, uint32_t it)
{
    return ((i2c_num->IC_RAW_INTR_STAT & it) ? SET : RESET);
}

void i2c_master_init(void)
{
    I2C_TypeDef* i2c_master_port = I2C_MASTER_NUM;
    if(i2c_initialized == true)
    {
        return;
    }
    I2C_InitTypeDef I2C_InitStructure;
    i2c_gpio_config();
    //Enable I2C clock state
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_StructInit(&I2C_InitStructure);
    //Configure I2C as master mode
    I2C_InitStructure.Mode = I2C_CR_MASTER;
    I2C_InitStructure.OwnAddress = 0x34;
    I2C_InitStructure.Speed = I2C_CR_FAST;   //I2C_CR_STD  I2C_CR_FAST
    I2C_InitStructure.ClockSpeed = I2C_MASTER_FREQ_HZ;

    //Initializes the i2c_master_port peripheral according to the specified
    I2C_Init(i2c_master_port, &I2C_InitStructure);
    I2C1->IC_RX_TL=0x0;    //Rx FIFO  
    I2C1->IC_TX_TL=0x0;    //Tx FIFO 
    I2C1->IC_INTR_MASK=0;  //disable all
    I2C_Cmd(i2c_master_port, ENABLE);
    i2c_initialized = true;
}

bool is_i2c_initialized(void)
{
    return i2c_initialized;
}

int8_t i2c_master_read_reg(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *data)
{
    int8_t res = 0;
    uint16_t overTime = 3000;
    // uart_printf("i2c_master_read_reg\r\n");

    i2c_num->IC_TAR = slaver_addr;
    i2c_num->IC_ENABLE |= I2C_ENR_ENABLE;

    i2c_tx_byte(i2c_num, reg_addr);  //tx memory addr
    *data = i2c_rx_byte(i2c_num);

    i2c_num->IC_ENABLE |= I2C_ENR_ABORT;
    while (i2c_num->IC_ENABLE & I2C_ENR_ABORT)
    {
        if (overTime-- == 0)
        {
            uart_printf("i2c_master_read_reg timeout!\r\n");
            res = -1;
            break;
        }
    }
    i2c_num->IC_CLR_TX_ABRT;
    while((i2c_get_it_status(i2c_num, ((uint16_t)I2C_ISR_STOP))) == 0);
    return res;
}

int8_t i2c_master_read_mem(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t size)
{
    int8_t res = 0;
    uint16_t i;
    uint16_t overTime = 3000;

    i2c_num->IC_TAR = slaver_addr;
    i2c_num->IC_ENABLE |= I2C_ENR_ENABLE;

    i2c_tx_byte(i2c_num, reg_addr);  //tx memory addr

    for(i = 0; i < size; i++)
    {
        buffer[i] = i2c_rx_byte(i2c_num);
    }

    i2c_num->IC_ENABLE |= I2C_ENR_ABORT;
    while (i2c_num->IC_ENABLE & I2C_ENR_ABORT)
    {
        if (overTime-- == 0)
        {
            res = -1;
            uart_printf("i2c_master_read_mem timeout!\r\n");
            break;
        }
    }
    i2c_num->IC_CLR_TX_ABRT;
    while((i2c_get_it_status(i2c_num, ((uint16_t)I2C_ISR_STOP))) == 0);
    return res;
}

int8_t i2c_master_write_reg(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t data)
{
    int8_t res = 0;
    uint16_t overTime = 3000;

    i2c_num->IC_ENABLE &= ~I2C_ENR_ENABLE;
    i2c_num->IC_TAR = slaver_addr;
    i2c_num->IC_ENABLE |= I2C_ENR_ENABLE;

    i2c_tx_byte(i2c_num, reg_addr);  //tx memory addr

    i2c_tx_byte(i2c_num, data);      //tx data

    i2c_num->IC_ENABLE |= I2C_ENR_ABORT;
    while (i2c_num->IC_ENABLE & I2C_ENR_ABORT)
    {
        if (overTime-- == 0)
        {
            res = -1;
            uart_printf("i2c_master_write_reg timeout!\r\n");
            break;
        }
    }
    i2c_num->IC_CLR_TX_ABRT;
    while((i2c_get_it_status(i2c_num, ((uint16_t)I2C_ISR_STOP))) == 0);
    return res;
}

int8_t i2c_master_write_mem(I2C_TypeDef* i2c_num, uint8_t slaver_addr, uint8_t reg_addr, uint8_t *buffer, uint16_t size)
{
    int8_t res = 0;
    uint8_t temp;
    uint8_t* p;
    uint16_t i;
    uint16_t overTime = 3000;
    // uart_printf("i2c_master_write_mem\r\n");
    p = buffer;
    i2c_num->IC_ENABLE &= ~I2C_ENR_ENABLE;
    i2c_num->IC_TAR = slaver_addr;
    i2c_num->IC_ENABLE |= I2C_ENR_ENABLE;
    temp = reg_addr;
    i2c_tx_byte(i2c_num, temp);     //tx memory addr
    for(i = 0; i < size; i++)
    {
        temp = *p;
        p++;
        i2c_tx_byte(i2c_num, temp); //tx data
    }
    i2c_num->IC_ENABLE |= I2C_ENR_ABORT;
    while (i2c_num->IC_ENABLE & I2C_ENR_ABORT)
    {
        if (overTime-- == 0)
        {
            res = -1;
            uart_printf("i2c_master_write_mem timeout!\r\n");
            break;
        }
    }
    i2c_num->IC_CLR_TX_ABRT;
    while((i2c_get_it_status(i2c_num, ((uint16_t)I2C_ISR_STOP))) == 0);
    return res;
}
