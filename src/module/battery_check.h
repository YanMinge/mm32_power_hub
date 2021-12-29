#ifndef __BATTERY_CHECK_H__
#define __BATTERY_CHECK_H__

#include "drv_gpio.h"

#define POWER_VOLTAGE_DETECTION_PIN     PA_0
#define HARDWARE_VERSION_DETECTION_PIN  PA_1
#define POWER_CURRENT_DETECTION_PIN     PB_0
#define POWER_SW_PIN                    PB_4
#define POWER_HOLD_PIN                  PB_8
#define USB_DETECTION_PIN               PB_7
#define CHARGING_DETECTION_PIN          PB_6

#define BATTERY_CAPACITY_TABLE_LEN (13)

typedef struct
{
  float battery_voltage;
  uint8_t battery_capacity;
}battery_capacity_table_t;

typedef enum
{
    UNCHARGE       = 0,
    CHARGE_FULL    = 1,
    CHARGING       = 2,
    CHARGE_ERROR   = 3,
}battery_charge_type;

bool is_usb_insert(void);
void usb_state_update(void);
battery_charge_type battery_charge_state(void);
extern void battery_voltage_update(void);
extern void battery_capacity_update(void);
extern float get_battery_voltage(void);
extern uint8_t get_battery_capacity(void);
#endif /* __BATTERY_CHECK_H__ */ 
