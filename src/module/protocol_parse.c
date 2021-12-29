#include "protocol_parse.h"
#include "drv_ring_buf.h"
#include "drv_uart.h"
#include "indicator_led.h"
#include "battery_check.h"
#include "power_management.h"
#include "ir_pair_tube.h"
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

uint8_t device_hw_id = 0x80;

static void parse_gcode(char *gcode_buf)
{
    char * tmp;
    char * str;
    int16_t device_id;
    //get device id
    str = strtok_r(gcode_buf, " ", &tmp);
    device_id = atoi(str + 1);

    if(device_id == device_hw_id)
    {
        //get command id
        if (str != NULL )
        {
            uint16_t command;
            str = strtok_r(0, " ", &tmp);
            if((str[0]=='m') || (str[0]=='M'))
            {
                command = atoi(str+1);
            }
            else
            {
                return;
            }
            switch(command)
            {
                case 1:
                {
                    uint16_t red = 0;
                    uint16_t green = 0;
                    uint16_t blue = 0;
                    uint16_t brightness = 200;
                    uint16_t rsp = 0;
                    while(str != NULL)
                    {
                        str = strtok_r(0, " ", &tmp);
                        if((str[0]=='R') || (str[0]=='r'))
                        {
                            red = atoi(str+1);
                        }
                        else if((str[0]=='G') || (str[0]=='g'))
                        {
                            green = atoi(str+1);
                        }
                        else if((str[0]=='B') || (str[0]=='b'))
                        {
                            blue = atoi(str+1);
                        }
                        else if((str[0]=='L') || (str[0]=='l'))
                        {
                            brightness = atoi(str+1);
                        }
                        else if((str[0]=='X') || (str[0]=='x'))
                        {
                            rsp = atoi(str+1);
                        }
                    }
                    set_all_rgb_led(red, green, blue, brightness);
                    if(rsp != 0)
                    {
                        log_uart_printf(UART1, "G%d M1 E0\r\n", device_hw_id | 0xff00);
                    }
                    break;
                }
                case 2:
                {
                    uint16_t num = 0;
                    uint16_t red = 0;
                    uint16_t green = 0;
                    uint16_t blue = 0;
                    uint16_t brightness = 125;
                    uint16_t rsp = 0;
                    while(str != NULL)
                    {
                        str = strtok_r(0, " ", &tmp);
                        if((str[0]=='I') || (str[0]=='i'))
                        {
                            num = atoi(str+1);
                        }
                        if((str[0]=='R') || (str[0]=='r'))
                        {
                            red = atoi(str+1);
                        }
                        else if((str[0]=='G') || (str[0]=='g'))
                        {
                            green = atoi(str+1);
                        }
                        else if((str[0]=='B') || (str[0]=='b'))
                        {
                            blue = atoi(str+1);
                        }
                        else if((str[0]=='L') || (str[0]=='l'))
                        {
                            brightness = atoi(str+1);
                        }
                        else if((str[0]=='X') || (str[0]=='x'))
                        {
                            rsp = atoi(str+1);
                        }
                    }
                    set_single_rgb_led(num, red, green, blue, brightness);
                    if(rsp != 0)
                    {
                        log_uart_printf(UART1, "G%d M2 E0\r\n", device_hw_id | 0xff00);
                    }
                    break;
                }
                case 3:
                {
                    log_uart_printf(UART1, "G%d M3 V%d, C%d\r\n", device_hw_id | 0xff00, (int)(1000 * get_battery_voltage()), get_battery_capacity());
                    break;
                }
                case 4:
                {
                    log_uart_printf(UART1, "G%d M4 V%d\r\n", device_hw_id | 0xff00, get_ir_data());
                    break;
                }
                case 5:
                {
                    log_uart_printf(UART1, "G%d M5 U%d\r\n", device_hw_id | 0xff00, is_usb_insert());
                    break;
                }
                default:
                    break;
            }
        }
    }  
    else if(device_id == BROADCAST_ID)
    {
        //get command id
        if (str != NULL )
        {
            uint16_t command;
            str = strtok_r(0, " ", &tmp);
            if((str[0]=='m') || (str[0]=='M'))
            {
                command = atoi(str+1);
            }
            else
            {
                return;
            }
            switch(command)
            {
                case 1:
                    break;
                default:
                    break;
            }
        }
    }
}

void parse_command(void)
{
    uint8_t rx_buf[1] = {0};
    char *gcode_buf;
    uint8_t gcode_len = 0;
    uint8_t process_count = 0;
    static bool start_gcode = false;
    process_count = drv_ringbuf_count((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf);
    gcode_len = process_count;
    gcode_buf = (char *)malloc(gcode_len);
    while(process_count--)
    {
        drv_ringbuf_read((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, 1, rx_buf);
        if((rx_buf[0] == 'g') || (rx_buf[0] == 'G'))
        {
            *gcode_buf = rx_buf[0];
            start_gcode = true;
            break;
        }
    }
    if(start_gcode == true)
    {
        drv_ringbuf_read((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf, process_count, (uint8_t *)gcode_buf+1);
        parse_gcode(gcode_buf);
        drv_ringbuf_flush((RING_BUF_DEF_STRUCT*)&s_rx_ring_buf);
        start_gcode = false;
    }

            
    free(gcode_buf);
}