#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

SemaphoreHandle_t semphr = NULL;

#define SW  0

void gpio_cb(void *param) {
  BaseType_t woken;
  xSemaphoreGiveFromISR(semphr, &woken);
  if(woken) {
    portYIELD_FROM_ISR();
  }
}

void task_log(void *param) {

  while(1) {
    xSemaphoreTake(semphr, portMAX_DELAY);
    ESP_LOGI("task_log", "Ocurrio una interrupcion");
  }
}

void app_main(void)
{
  gpio_config_t config = {
    .pin_bit_mask = 1 << SW,
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_NEGEDGE
  };
  gpio_config(&config);

  // ESP_ERROR_CHECK(gpio_isr_register(gpio_cb, NULL, ESP_INTR_FLAG_LEVEL1,NULL));
  gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
  gpio_isr_handler_add(SW, gpio_cb, NULL);

  semphr = xSemaphoreCreateBinary();
  xTaskCreate(task_log, "Log", 2048, NULL, 1, NULL);
}