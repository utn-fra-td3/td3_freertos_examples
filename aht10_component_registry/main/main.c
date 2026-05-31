#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <aht.h>
#include <string.h>
#include <esp_err.h>
#include <esp_log.h>

#define ADDR 0x38
#define AHT_TYPE AHT_TYPE_AHT1x
#define APP_CPU_NUM PRO_CPU_NUM

static const char *TAG = "aht-example";

void task(void *pvParameters)
{
  aht_t dev = { 0 };
  dev.mode = AHT_MODE_NORMAL;
  dev.type = AHT_TYPE;

  aht_init_desc(&dev, ADDR, 0, 5, 6);
  aht_init(&dev);

  bool calibrated;
  ESP_ERROR_CHECK(aht_get_status(&dev, NULL, &calibrated));
  if (calibrated)
      ESP_LOGI(TAG, "Sensor calibrated");
  else
      ESP_LOGW(TAG, "Sensor not calibrated!");

  float temperature, humidity;

  while (1)
  {
    esp_err_t res = aht_get_data(&dev, &temperature, &humidity);
    if (res == ESP_OK)
        ESP_LOGI(TAG, "Temperature: %.1f°C, Humidity: %.2f%%", temperature, humidity);
    else
        ESP_LOGE(TAG, "Error reading data: %d (%s)", res, esp_err_to_name(res));

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void app_main()
{
  ESP_ERROR_CHECK(i2cdev_init());
  xTaskCreatePinnedToCore(task, TAG, configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}
