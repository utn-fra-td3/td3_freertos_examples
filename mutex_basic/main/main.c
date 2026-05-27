#include <stdio.h>
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mutex = NULL;

void task1(void *params) {
  while(1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    ESP_LOGI("TASK1", "Task 1 tomo el mutex");
    for(int i = 0; i < 5; i++) {
      ESP_LOGI("TASK1", "Task 1 esta trabajando...");
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    ESP_LOGI("TASK1", "Task 1 libera el mutex");
    xSemaphoreGive(mutex);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void task2(void *params) {
  while(1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    ESP_LOGI("TASK2", "Task 2 tomo el mutex");
    for(int i = 0; i < 3; i++) {
      ESP_LOGI("TASK2", "Task 2 esta trabajando...");
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    ESP_LOGI("TASK2", "Task 2 libera el mutex");
    xSemaphoreGive(mutex);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void app_main(void) {

  mutex = xSemaphoreCreateMutex();
  if(mutex == NULL) {
    ESP_LOGE("MAIN", "Error al crear el mutex");
    return;
  }
  xTaskCreate(task1, "Task 1", 2048, NULL, 1, NULL);
  xTaskCreate(task2, "Task 2", 2048, NULL, 1, NULL);
  // xTaskCreatePinnedToCore(task1, "Task 1", 2048, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(task2, "Task 2", 2048, NULL, 1, NULL, 1);
}