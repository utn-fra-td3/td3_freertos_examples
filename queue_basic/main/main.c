#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/temperature_sensor.h"

QueueHandle_t queue_temp = NULL;

void task_sensor(void *param) {
  temperature_sensor_handle_t temp_handle = NULL;
  temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(20, 50);
  temperature_sensor_install(&temp_sensor_config, &temp_handle);
  // Enable temperature sensor
  temperature_sensor_enable(temp_handle);
  while(1) {
    // Get converted sensor data
    float tsens_out;
    temperature_sensor_get_celsius(temp_handle, &tsens_out);
    xQueueSend(queue_temp, &tsens_out, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void task_log(void *param) {
  
  while(1) {
    float temp;
    xQueueReceive(queue_temp, &temp, portMAX_DELAY);
    ESP_LOGI("task_log", "Temperatura %f", temp);
  }
}

void app_main(void)
{
  queue_temp = xQueueCreate(1, sizeof(float));
  xTaskCreate(task_sensor, "Sensor", 2048, NULL, 1, NULL);
  xTaskCreate(task_log, "Log", 2048, NULL, 1, NULL);
}