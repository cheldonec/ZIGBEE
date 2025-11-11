/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Zigbee HA_on_off_switch Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#include "esp_check.h"
#include "string.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ha/esp_zigbee_ha_standard.h"
#include "esp_zb_host.h"
#include "lvgl_port_zb_interface.h"
#include "lvgl_port_gui_init.h"
#include "screens/ui_MainScreen.h"
#include "esp_event.h"
//#include "esp_psram.h"
typedef struct light_bulb_device_params_s {
    esp_zb_ieee_addr_t ieee_addr;
    uint8_t  endpoint;
    uint16_t short_addr;
} light_bulb_device_params_t;

static switch_func_pair_t button_func_pair[] = {
    {GPIO_INPUT_IO_TOGGLE_SWITCH, SWITCH_ONOFF_TOGGLE_CONTROL}
};

static const char *TAG = "ESP_ZB_ON_OFF_SWITCH";

static void zb_buttons_handler(switch_func_pair_t *button_func_pair)
{
    if (button_func_pair->func == SWITCH_ONOFF_TOGGLE_CONTROL) {
        /* implemented light switch toggle functionality */
        esp_zb_zcl_on_off_cmd_t cmd_req;
        cmd_req.zcl_basic_cmd.src_endpoint = HA_ONOFF_SWITCH_ENDPOINT;
        cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
        cmd_req.on_off_cmd_id = ESP_ZB_ZCL_CMD_ON_OFF_TOGGLE_ID;
        ESP_EARLY_LOGI(TAG, "Send 'on_off toggle' command");
        esp_zb_zcl_on_off_cmd_req(&cmd_req);
    }
}

static esp_err_t deferred_driver_init(void)
{
    static bool is_inited = false;
    if (!is_inited) {
        ESP_RETURN_ON_FALSE(switch_driver_init(button_func_pair, PAIR_SIZE(button_func_pair), zb_buttons_handler),
                            ESP_FAIL, TAG, "Failed to initialize switch driver");
        is_inited = true;
    }
    return is_inited ? ESP_OK : ESP_FAIL;
}

static void bdb_start_top_level_commissioning_cb(uint8_t mode_mask)
{
    ESP_RETURN_ON_FALSE(esp_zb_bdb_start_top_level_commissioning(mode_mask) == ESP_OK, , TAG, "Failed to start Zigbee bdb commissioning");
}

static void bind_cb(esp_zb_zdp_status_t zdo_status, void *user_ctx)
{
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) {
        ESP_LOGI(TAG, "Bound successfully!");
        if (user_ctx) {
            light_bulb_device_params_t *light = (light_bulb_device_params_t *)user_ctx;
            ESP_LOGI(TAG, "The light originating from address(0x%x) on endpoint(%d)", light->short_addr, light->endpoint);
            free(light);
        }
    }
}

static void user_find_cb(esp_zb_zdp_status_t zdo_status, uint16_t addr, uint8_t endpoint, void *user_ctx)
{
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) {
        ESP_LOGI(TAG, "Found light");
        esp_zb_zdo_bind_req_param_t bind_req;
        light_bulb_device_params_t *light = (light_bulb_device_params_t *)malloc(sizeof(light_bulb_device_params_t));
        light->endpoint = endpoint;
        light->short_addr = addr;
        esp_zb_ieee_address_by_short(light->short_addr, light->ieee_addr);
        esp_zb_get_long_address(bind_req.src_address);
        bind_req.src_endp = HA_ONOFF_SWITCH_ENDPOINT;
        bind_req.cluster_id = ESP_ZB_ZCL_CLUSTER_ID_ON_OFF;
        bind_req.dst_addr_mode = ESP_ZB_ZDO_BIND_DST_ADDR_MODE_64_BIT_EXTENDED;
        memcpy(bind_req.dst_address_u.addr_long, light->ieee_addr, sizeof(esp_zb_ieee_addr_t));
        bind_req.dst_endp = endpoint;
        bind_req.req_dst_addr = esp_zb_get_short_address(); /* TODO: Send bind request to self */
        ESP_LOGI(TAG, "Try to bind On/Off");
        esp_zb_zdo_device_bind_req(&bind_req, bind_cb, (void *)light);
    }
}

static void simple_desc_cb(esp_zb_zdp_status_t zdo_status, esp_zb_af_simple_desc_1_1_t *simple_desc, void *user_ctx)
{
    ESP_LOGI(TAG, "simple_desc_cb and status %d",zdo_status);
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) 
    {
        ESP_LOGI(TAG, "*********************************************");
        uint8_t i = 0;
            if (simple_desc->app_input_cluster_count >0)
            {
                for(i = 0; i < simple_desc->app_input_cluster_count; i++)
                {
                    ESP_LOGW(TAG, "FOUND INPUT Cluster 0x%04hx  in ep 0x%02hx",*(simple_desc->app_cluster_list + i),simple_desc->endpoint);
                }
            }
         ESP_LOGI(TAG, "*********************************************");  
            if (simple_desc->app_output_cluster_count >0)
            {
                for(int j = i; j < simple_desc->app_input_cluster_count + simple_desc->app_output_cluster_count; j++)
                {
                    ESP_LOGW(TAG, "FOUND OUTPUT Cluster 0x%04hx  in ep 0x%02hx",*(simple_desc->app_cluster_list + j), simple_desc->endpoint);
                }
            }
        ESP_LOGI(TAG, "*********************************************");
    }
}

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    uint32_t *p_sg_p       = signal_struct->p_app_signal;
    esp_err_t err_status = signal_struct->esp_err_status;
    esp_zb_app_signal_type_t sig_type = *p_sg_p;
   
    esp_zb_nwk_signal_device_associated_params_t*  dev_assoc_params = NULL;
    esp_zb_zdo_signal_device_annce_params_t*       dev_annce_params = NULL;
    esp_zb_zdo_signal_device_authorized_params_t*  dev_auth_params = NULL;
    esp_zb_zdo_signal_device_update_params_t*      dev_update_params = NULL;
    esp_zb_zdo_signal_leave_indication_params_t*   leave_ind_params = NULL;
    esp_zb_zdo_device_unavailable_params_t*        dev_unavalible = NULL;
    
    switch (sig_type) {
    case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
        ESP_LOGI(TAG, "Initialize Zigbee stack");
        esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_INITIALIZATION);
        break;
    case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
    case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
        if (err_status == ESP_OK) {
            ESP_LOGI(TAG, "Deferred driver initialization %s", deferred_driver_init() ? "failed" : "successful");
            ESP_LOGI(TAG, "Start network formation");
            esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_FORMATION);
        } else {
            ESP_LOGW(TAG, "%s failed with status: %s, retrying", esp_zb_zdo_signal_to_string(sig_type),
                     esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb,
                                   ESP_ZB_BDB_MODE_INITIALIZATION, 1000);
        }
        break;
    case ESP_ZB_BDB_SIGNAL_FORMATION:
        if (err_status == ESP_OK) {
            esp_zb_ieee_addr_t extended_pan_id;
            esp_zb_get_extended_pan_id(extended_pan_id);
            ESP_LOGI(TAG, "Formed network successfully (Extended PAN ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x, PAN ID: 0x%04hx, Channel:%d, Short Address: 0x%04hx)",
                     extended_pan_id[7], extended_pan_id[6], extended_pan_id[5], extended_pan_id[4],
                     extended_pan_id[3], extended_pan_id[2], extended_pan_id[1], extended_pan_id[0],
                     esp_zb_get_pan_id(), esp_zb_get_current_channel(), esp_zb_get_short_address());
            esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
        } else {
            ESP_LOGI(TAG, "Restart network formation (status: %s)", esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb, ESP_ZB_BDB_MODE_NETWORK_FORMATION, 1000);
        }
        break;
    case ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED:
       
        if(err_status == ESP_OK)
            {
                ESP_LOGI(TAG, "ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED:");
                dev_assoc_params = (esp_zb_nwk_signal_device_associated_params_t*)esp_zb_app_signal_get_params(p_sg_p);
                
                /*sprintf(buff, "DEVICE_ASSOCIATED : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", dev_assoc_params->device_addr[7], 
                    dev_assoc_params->device_addr[6], dev_assoc_params->device_addr[5], dev_assoc_params->device_addr[4],
                    dev_assoc_params->device_addr[3], dev_assoc_params->device_addr[2], dev_assoc_params->device_addr[1], dev_assoc_params->device_addr[0]);
                ncp_header.id = ZB_MANAGER_DEV_ASSOCIATED_EVENT;
                esp_ncp_noti_input(&ncp_header, dev_assoc_params, sizeof(esp_zb_nwk_signal_device_associated_params_t));
                */
            }
        break;
    case ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE:
       
        if(err_status == ESP_OK)
            {
                ESP_LOGI(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE:");
                dev_update_params = (esp_zb_zdo_signal_device_update_params_t *)esp_zb_app_signal_get_params(p_sg_p);
                /*sprintf(buff, "DEVICE_UPDATE : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x (short: 0x%04x) (update_status: 0x%02hx)", 
                dev_update_params->long_addr[7], dev_update_params->long_addr[6], dev_update_params->long_addr[5], dev_update_params->long_addr[4],
                     dev_update_params->long_addr[3], dev_update_params->long_addr[2], dev_update_params->long_addr[1], dev_update_params->long_addr[0], 
                     dev_update_params->short_addr, dev_update_params->status);
                //zb_manager_obj.utility_functions_callbacks.print_log_to_screen_callback(buff, 0xA7BFC1);
                ESP_LOGI(TAG, "device update (short: 0x%04x) (update_status: 0x%02hx) (mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x) (status:0x%02x)", 
                dev_update_params->short_addr, dev_update_params->status, dev_update_params->long_addr[7], dev_update_params->long_addr[6], dev_update_params->long_addr[5], dev_update_params->long_addr[4],
                     dev_update_params->long_addr[3], dev_update_params->long_addr[2], dev_update_params->long_addr[1], dev_update_params->long_addr[0],dev_update_params->status);
                */
            }
        break;
    case ESP_ZB_BDB_SIGNAL_STEERING:
        if (err_status == ESP_OK) {
            ESP_LOGI(TAG, "Network steering started");
        }
        break;
    case ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE:
    if(err_status == ESP_OK)
       {
       dev_annce_params = (esp_zb_zdo_signal_device_annce_params_t *)esp_zb_app_signal_get_params(p_sg_p);
        ESP_LOGI(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE:");
        ESP_LOGI(TAG, "New device commissioned or rejoined (short: 0x%04hx)", dev_annce_params->device_short_addr);
        //esp_zb_zdo_match_desc_req_param_t  cmd_req;
        //cmd_req.dst_nwk_addr = dev_annce_params->device_short_addr;
        //cmd_req.addr_of_interest = dev_annce_params->device_short_addr;
        //esp_zb_zdo_find_on_off_light(&cmd_req, user_find_cb, NULL);
       }
        break;
    case ESP_ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED:
        if(err_status == ESP_OK)
            {    
                dev_auth_params = (esp_zb_zdo_signal_device_authorized_params_t *)esp_zb_app_signal_get_params(p_sg_p);
                ESP_LOGI(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED:");
                /*sprintf(buff, "DEVICE_AUTHORIZED : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x (short: 0x%04x) (auth_type: 0x%02x) (auth_status: 0x%02x)", 
                dev_auth_params->long_addr[7], dev_auth_params->long_addr[6], dev_auth_params->long_addr[5], dev_auth_params->long_addr[4],
                     dev_auth_params->long_addr[3], dev_auth_params->long_addr[2], dev_auth_params->long_addr[1], dev_auth_params->long_addr[0], 
                     dev_auth_params->short_addr, dev_auth_params->authorization_type, dev_auth_params->authorization_status);
                //zb_manager_obj.utility_functions_callbacks.print_log_to_screen_callback(buff, 0xA7BFC1);

                ESP_LOGI(TAG, "device authorized (short: 0x%04x) (auth_type: 0x%02x) (auth_status: 0x%02x) (mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x)", 
                dev_auth_params->short_addr, dev_auth_params->authorization_type, dev_auth_params->authorization_status, dev_auth_params->long_addr[7], dev_auth_params->long_addr[6], dev_auth_params->long_addr[5], dev_auth_params->long_addr[4],
                     dev_auth_params->long_addr[3], dev_auth_params->long_addr[2], dev_auth_params->long_addr[1], dev_auth_params->long_addr[0]);
                */
            }

        break;
    case ESP_ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS:
        if (err_status == ESP_OK) {
            if (*(uint8_t *)esp_zb_app_signal_get_params(p_sg_p)) {
                ESP_LOGI(TAG, "Network(0x%04hx) is open for %d seconds", esp_zb_get_pan_id(), *(uint8_t *)esp_zb_app_signal_get_params(p_sg_p));
                //TEST SIMPLE DESC
                esp_zb_zdo_simple_desc_req_param_t req;
                req.addr_of_interest = 0x9eec; 
                req.endpoint = 0x01;
                zb_manager_zdo_simple_desc_req(&req,simple_desc_cb,NULL);
            } else {
                ESP_LOGW(TAG, "Network(0x%04hx) closed, devices joining not allowed.", esp_zb_get_pan_id());
            }
        }
        break;
    default:
        ESP_LOGI(TAG, "ZDO signal: %s (0x%x), status: %s", esp_zb_zdo_signal_to_string(sig_type), sig_type,
                 esp_err_to_name(err_status));
        break;
    }
}

static void esp_zb_task(void *pvParameters)
{
    /* initialize Zigbee stack */
    //esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZC_CONFIG();
    //esp_zb_init(&zb_nwk_cfg);
    //zb_manager_init();
    //esp_zb_on_off_switch_cfg_t switch_cfg = ESP_ZB_DEFAULT_ON_OFF_SWITCH_CONFIG();
    //esp_zb_ep_list_t *esp_zb_on_off_switch_ep = esp_zb_on_off_switch_ep_create(HA_ONOFF_SWITCH_ENDPOINT, &switch_cfg);
    //esp_zb_device_register(esp_zb_on_off_switch_ep);
    //esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);

    //ESP_ERROR_CHECK(esp_zb_start(false));
    ESP_LOGI(TAG, "Zigbee stack started");
    esp_zb_stack_main_loop();
}

static lv_obj_t *fft_value[80];

void sensor_update_timer(lv_timer_t *timer) {
    uint8_t* i = (uint8_t*)lv_timer_get_user_data(timer);
    if (*i < 8)
    {
        //zb_manager_temperature_sensor_ui_t* ui_temperature_sensor  = create_new_temperature_sensor_ui(ui_MainScreenSensorsContainer, NULL, true, false);
        (*i)++;
    } else lv_timer_delete(timer);
}

// Callback-функция для обработки событий
static void zb_actions_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    ESP_LOGI(TAG, "Получено событие: база=%s, ID=%ld", base, id);
    
    switch (id) {
        case ATTR_REPORT_EVENT:
            ESP_LOGI(TAG, "zb_manager_report_attr_event_fn from EVENT");
            //lvgl_port_task_wake
            //zb_manager_temperature_sensor_ui_t* ui_temperature_sensor = create_new_temperature_sensor_ui(ui_MainScreenSensorsContainer, NULL, true, false);
            lv_obj_class_t* lv_obj_class = NULL;
            break;
        case ATTR_READ_RESP:
            ESP_LOGI(TAG, "zb_manager_read_attr_response_fn from EVENT");
            break;
        /*case MY_EVENT_SENSOR_DATA_READY:
            ESP_LOGI(TAG, "Данные сенсора готовы");
            break;
        case MY_EVENT_BUTTON_PRESSED:
            ESP_LOGI(TAG, "Кнопка нажата");
            break;*/
        default:
            break;
    }
}

void app_main(void)
{
    //start_lvgl_interface();
    zb_manager_temperature_sensor_ui_t** temperature_sensors_list = calloc(1, TEMPERATURE_SENSORS_LIST_SIZE * sizeof(zb_manager_temperature_sensor_ui_t*));
    
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Регистрация обработчика событий
    ESP_ERROR_CHECK(esp_event_handler_register(ZB_ACTION_HANDLER_EVENTS, ESP_EVENT_ANY_ID, &zb_actions_event_handler, NULL));

    lvgl_port_gui_init();
    lvgl_main_prog_start();
    ESP_LOGI(TAG, "RAM left before create %lu", esp_get_free_heap_size());
    zb_manager_temperature_sensor_ui_t* test_sensor = create_new_temperature_sensor_ui();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after create %lu", esp_get_free_heap_size());
    draw_temperature_sensor_minimal(test_sensor, ui_MainScreenSensorsContainer);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after draw %lu", esp_get_free_heap_size());
    draw_temperature_sensor_minimal(test_sensor, ui_MainScreenSensorsContainer);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after redraw %lu", esp_get_free_heap_size());

    //ui_temperature_sensor_t* ui_temperature_sensor4 = draw_temperature_sensor_on_home_screen(ui_MainScreenSensorsContainer);
    //ui_humidity_sensor_t* ui_humidity_sensor = draw_humidity_sensor(ui_MainScreenSensorsContainer);
    //ui_on_off_light_device_t* lamp = draw_on_off_light_device(ui_MainScreenSensorsContainer,ON_OFF_LIGHT_DEVICE_BULB);
    //ui_on_off_light_device_t* rele = draw_on_off_light_device(ui_MainScreenSensorsContainer,ON_OFF_LIGHT_DEVICE_RELE);
    //vTaskDelay(3000 / portTICK_PERIOD_MS);
    //temperature_sensor_set_value_text(ui_temperature_sensor, "13.5 C");
    //humidity_sensor_set_value_text(ui_humidity_sensor, "45.5 %");
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
    //temperature_sensor_destroy_ui(ui_temperature_sensor);
    //humidity_sensor_destroy_ui(ui_humidity_sensor);
    //on_off_light_device_destroy_ui(lamp);
    //on_off_light_device_destroy_ui(rele);
    //on_off_light_device_set_value_on_off(lamp, 0);
    //on_off_light_device_set_value_on_off(rele, 1);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after delete %lu", esp_get_free_heap_size());
    
   
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    //esp_zb_stack_main_loop();
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 5, NULL);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    esp_err_t err = zb_manager_init();
    err = zb_manager_start();
    if (err == ESP_OK) 
    {
        ESP_LOGI(TAG, "Zigbee stack zb_manager_init");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Zigbee test func zb_manager_open_network");
        //zb_manager_open_network(15);
    }
    
}
