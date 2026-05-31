#include "aht10.h"

static i2c_master_dev_handle_t aht10_dev;
static const char *TAG = "AHT10";

static esp_err_t aht10_read_raw(uint8_t *data);

esp_err_t aht10_init(i2c_master_bus_handle_t i2c_bus) {
  i2c_device_config_t dev_cfg = {
    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    .device_address = AHT10_ADDR,
    .scl_speed_hz = I2C_MASTER_FREQ_HZ,
  };
  ESP_ERROR_CHECK_WITHOUT_ABORT(i2c_master_bus_add_device(i2c_bus, &dev_cfg, &aht10_dev));

  uint8_t init_cmd[] = {AHT10_CMD_INIT, 0x08, 0x00};
  esp_err_t ret = i2c_master_transmit(aht10_dev, init_cmd, sizeof(init_cmd), 100);
  return ret;
}

esp_err_t aht10_read_temp(float *temperature) {
  uint8_t data[6];
  esp_err_t ret = aht10_read_raw(data);
  if (ret != ESP_OK) return ret;
  uint32_t raw = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
  *temperature = ((float)raw / (1 << 20)) * 200.0f - 50.0f;
  return ESP_OK;
}

esp_err_t aht10_read_hum(float *humidity) {
  uint8_t data[6];
  esp_err_t ret = aht10_read_raw(data);
  if (ret != ESP_OK) return ret;
  uint32_t raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
  *humidity = ((float)raw / (1 << 20)) * 100.0f;
  return ESP_OK;
}

// Private functions

static esp_err_t aht10_read_raw(uint8_t *data) {
  uint8_t trigger[] = {AHT10_CMD_TRIGGER, 0x33, 0x00};
  esp_err_t ret = i2c_master_transmit(aht10_dev, trigger, sizeof(trigger), 100);
  if (ret != ESP_OK) return ret;
  return i2c_master_receive(aht10_dev, data, 6, 100);
}