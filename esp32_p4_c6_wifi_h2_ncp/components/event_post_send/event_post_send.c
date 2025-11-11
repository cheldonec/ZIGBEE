#include "event_post_send.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *EVENT_POST_SEND_TAG = "event_post_send_module";

bool eventLoopPost(esp_event_base_t event_base, int32_t event_id, void* event_data, size_t event_data_size, TickType_t ticks_to_wait)
{
     esp_err_t err = ESP_OK;
     ESP_LOGI(EVENT_POST_SEND_TAG, "SENDING EVENT FROM GROUP: %s MESSAGE: %d", event_base, (int)event_id);
    do {
      esp_err_t err = esp_event_post(event_base, event_id, event_data, event_data_size, ticks_to_wait == portMAX_DELAY ? EVENT_LOOP_POST_DELAY : ticks_to_wait);
      if (err != ESP_OK) {
        //ESP_LOGE(EVENT_POST_SEND_TAG, "Failed to post event to \"%s\" #%d: %li (%s)", event_base, event_id, err, esp_err_to_name(err));
        if (ticks_to_wait == portMAX_DELAY) {
          vTaskDelay(EVENT_LOOP_POST_DELAY);
        };
      };
    } while ((ticks_to_wait == portMAX_DELAY) && (err != ESP_OK));
  return err == ESP_OK;
}