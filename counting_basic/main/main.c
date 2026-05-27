#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

static const char *TAG = "main";

SemaphoreHandle_t semphr;

void gpio_cb(void *param) {
  BaseType_t woken;
  xSemaphoreGiveFromISR(semphr, &woken);
}

void task(void *param) {
  while(1) {
    ESP_LOGI(TAG, "Se contaron %d pulsos", uxSemaphoreGetCount(semphr));
    xQueueReset(semphr);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void)
{
  gpio_config_t config = {
    .pin_bit_mask = 1 << 0, // GPIO0
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE
  };
  gpio_config(&config);

  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  gpio_isr_handler_add(0, gpio_cb, NULL);

  semphr = xSemaphoreCreateCounting(100, 0);
  if(semphr == NULL) {
    ESP_LOGE(TAG, "Failed to create semaphore");
    return;
  }
  // Creacion de tarea
  xTaskCreate(task, "task", 2048, NULL, 1, NULL);
}