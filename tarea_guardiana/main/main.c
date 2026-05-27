#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t logQueue = NULL;

typedef struct LogMessage {
  char* tag;
  char* message;
} LogMessage_t;

void task_log(void *param) {
  while (1) {
    LogMessage_t logMessage;
    if (xQueueReceive(logQueue, &logMessage, portMAX_DELAY) == pdPASS) {
      ESP_LOGI(logMessage.tag, "%s", logMessage.message);
    }
  }
}

void task_1(void *param) {
  while (1) {
    LogMessage_t logMessage = {
      .tag = "Task 1",
      .message = "Hello from Task 1!"
    };
    xQueueSend(logQueue, &logMessage, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void task_2(void *param) {
  while (1) {
    LogMessage_t logMessage = {
      .tag = "Task 2",
      .message = "Hello from Task 2!"
    };
    xQueueSend(logQueue, &logMessage, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void task_3(void *param) {
  while (1) {
    LogMessage_t logMessage = {
      .tag = "Task 3",
      .message = "Hello from Task 3!"
    };
    xQueueSend(logQueue, &logMessage, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void app_main(void)
{
  logQueue = xQueueCreate(10, sizeof(LogMessage_t));

  xTaskCreate(task_log, "TaskLog", 2048, NULL, 1, NULL);
  xTaskCreate(task_1, "Task1", 2048, NULL, 1, NULL);
  xTaskCreate(task_2, "Task2", 2048, NULL, 1, NULL);
  xTaskCreate(task_3, "Task3", 2048, NULL, 1, NULL);
}