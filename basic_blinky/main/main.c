#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"

#if CONFIG_LED_STRIP == 1
#include "led_strip.h"
// Estructura para manejar la tira de LED
static led_strip_handle_t s_led_strip;
#else
#include "driver/gpio.h"
#endif

#define TAG "main"

/** 
 * @brief Configura el LED de acuerdo al tipo elegido en el menuconfig
 * @return ESP_OK la configuracion fue exitosa
 */
esp_err_t led_configuration(void) {

  esp_err_t ret = ESP_OK;

#if CONFIG_LED_STRIP == 1
  ESP_LOGI(TAG, "Ejemplo de blinky con LED configurable");
  led_strip_config_t strip_config = {
    .strip_gpio_num = CONFIG_LED_GPIO,
    .max_leds = 1
  };
  led_strip_rmt_config_t rmt_config = {
    .resolution_hz = 10 * 1000 * 1000,  // 10 MHz
    .flags.with_dma = false
  };
  ret = led_strip_new_rmt_device(&strip_config, &rmt_config, &s_led_strip);
  if(ret != ESP_OK) { return ret; }
  ret = led_strip_clear(s_led_strip);
  if(ret != ESP_OK) { return ret;}
#else
  ret = gpio_reset_pin(CONFIG_LED_GPIO);
  if(ret != ESP_OK) { return ret; }
  ret = gpio_set_direction(CONFIG_LED_GPIO, GPIO_MODE_OUTPUT);
  if(ret != ESP_OK) { return ret; }
#endif
  return ret;
}

esp_err_t led_toggle(void) {
  
  static bool s_led_state = false;
  esp_err_t ret = ESP_OK;

#if CONFIG_LED_STRIP == 1
  uint32_t led_value = s_led_state? 255 : 0;
  ret = led_strip_set_pixel(s_led_strip, 0, led_value, led_value, led_value);
  if(ret != ESP_OK) { return ret; }
  ret = led_strip_refresh(s_led_strip);
  if(ret != ESP_OK) { return ret; }
#else
  ret = gpio_set_level(CONFIG_LED_GPIO, !s_led_state);
  if(ret != ESP_OK) { return ret; }
#endif

  s_led_state = !s_led_state;
  return ret;
}

void app_main(void) {

  esp_err_t ret = ESP_OK;
  ret = led_configuration();
  if(ret != ESP_OK) {
    ESP_LOGE(TAG, "%s", esp_err_to_name(ret));
  }

  while(1) {
    // Conmuta el LED independiente del tipo
    ret = led_toggle();
    if(ret != ESP_OK) {
      ESP_LOGE(TAG, "%s", esp_err_to_name(ret));
    }
    vTaskDelay(pdMS_TO_TICKS(CONFIG_BLINKY_DELAY_MS));
  }
}