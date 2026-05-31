#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "aht10.h"
#include "esp_log.h"

#define I2C_MASTER_SDA_IO   GPIO_NUM_5
#define I2C_MASTER_SCL_IO   GPIO_NUM_6
#define I2C_MASTER_FREQ_HZ  100000
#define I2C_PORT            I2C_NUM_0

static i2c_master_bus_handle_t i2c_bus;

static void i2c_bus_init(void) {
  i2c_master_bus_config_t bus_cfg = {
    .i2c_port = I2C_PORT,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
  };
  ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_new_master_bus(&bus_cfg, &i2c_bus));
}

static const char* TAG = "main";

void app_main(void) {
  i2c_bus_init();
  ESP_ERROR_CHECK_WITHOUT_ABORT(aht10_init(i2c_bus));
  ESP_LOGI(TAG, "AHT10 initialized");

  float temp, hum;
  while (1) {
    if (aht10_read_temp(&temp) == ESP_OK)
      ESP_LOGI(TAG, "Temperature: %.2f C", temp);

    vTaskDelay(pdMS_TO_TICKS(100));
    if (aht10_read_hum(&hum) == ESP_OK)
      ESP_LOGI(TAG, "Humidity: %.2f %%", hum);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
