#ifndef _AHT10_H_
#define _AHT10_H_

#include "driver/i2c_master.h"

#define AHT10_ADDR          0x38
#define AHT10_CMD_INIT      0xE1
#define AHT10_CMD_TRIGGER   0xAC

esp_err_t aht10_init(i2c_master_bus_handle_t i2c_bus);
esp_err_t aht10_read_temp(float *temperature);
esp_err_t aht10_read_hum(float *humidity);

#endif