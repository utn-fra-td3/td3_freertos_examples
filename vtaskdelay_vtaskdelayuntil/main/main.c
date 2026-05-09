#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_random.h"

void task_1(void *param) {
  while(1) {
    for(uint32_t i = 0; i < esp_random(); i++);
    ESP_LOGI("task_1", "A punto de llamar a vTaskDelay por 1000 ms");
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI("task_1", "Desbloqueado");
  }
}

void task_2(void *param) {
  TickType_t timestamp = xTaskGetTickCount();
  ESP_LOGI("task_2", "timestamp @ %ld", pdTICKS_TO_MS(timestamp));
  while(1) {
    for(uint32_t i = 0; i < esp_random(); i++);
    ESP_LOGI("task_2", "A punto de llamar a vTaskDelayUntil por 1000 ms");
    vTaskDelayUntil(&timestamp, pdMS_TO_TICKS(1000));
    ESP_LOGI("task_2", "Desbloqueado");
  }
}

void app_main(void)
{
  xTaskCreate(task_1, "Tarea 1", 2048, NULL, 1, NULL);
  xTaskCreate(task_2, "Tarea 2", 2048, NULL, 1, NULL);
}