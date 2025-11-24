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

esp_zb_ieee_addr_t LocalIeeeAdr; //esp_zb_get_ieee_addr(void);
/************************************************** MAIN_SCREENS_ARRAY *****************************/
ui_ScreenObject_t** ScreensMainList;
uint8_t HomeScreenIndex;
uint8_t DevicesScreenIndex;

/************************************************** MAIN DEVICES ARRAY ****************************/

//#define REMOTE_DEVICES_COUNT (32) /************* !!!! zb_manager_devices.h */
uint8_t local_RemoteDevicesCount;
device_custom_t** local_RemoteDevicesArray;

uint8_t local_DeviceAppendShedulerCount;
device_appending_sheduler_t** local_DeviceAppendShedulerArray;

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
    build_dev_simple_desc_user_ctx_t* ctx = NULL;
    ctx = (build_dev_simple_desc_user_ctx_t*)user_ctx;
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) 
    {
       
        if(ctx==NULL) ESP_LOGW(TAG, "simple_desc_cb ERROR user_ctx"); else
        {
            uint8_t i = 0;
            if (simple_desc->app_input_cluster_count >0)
            {
                for(i = 0; i < simple_desc->app_input_cluster_count; i++)
                {
                    ESP_LOGW(TAG, "FOUND INPUT Cluster 0x%04hx to dev 0x%04hx in ep 0x%02hx",*(simple_desc->app_cluster_list + i), ctx->parent_dev->short_addr, ctx->ep->ep_id);
                }
            }

            if (simple_desc->app_output_cluster_count >0)
            {
                for(int j = i; j < simple_desc->app_input_cluster_count + simple_desc->app_output_cluster_count; j++)
                {
                    ESP_LOGW(TAG, "FOUND OUTPUT Cluster 0x%04hx to dev 0x%04hx in ep 0x%02hx",*(simple_desc->app_cluster_list + j), ctx->parent_dev->short_addr, ctx->ep->ep_id);
                }
            }
            /*if(ctx->parent_dev->control_dev_annce_simple_desc_req_count > 0)
            {
                for (int i = 0; i < ctx->parent_dev->control_dev_annce_simple_desc_req_count; i++)
                {
                    if(ctx->parent_dev->control_dev_annce_simple_desc_req_array[i]->ep_id == simple_desc->endpoint)
                    {
                        ctx->parent_dev->control_dev_annce_simple_desc_req_array[i]->status = 1;
                        break;
                    }
                }
            }
            // проверяем, все ли ответы пришли
            bool is_simple_desc_compliete = true;
            if(ctx->parent_dev->control_dev_annce_simple_desc_req_count > 0)
            {
                for (int i = 0; i < ctx->parent_dev->control_dev_annce_simple_desc_req_count; i++)
                {
                    if(ctx->parent_dev->control_dev_annce_simple_desc_req_array[i]->ep_id == simple_desc->endpoint)
                    {
                        if(ctx->parent_dev->control_dev_annce_simple_desc_req_array[i]->status == 0)
                        {
                            is_simple_desc_compliete = false;
                            break;
                        }
                        
                    }
                }
            }
            if (is_simple_desc_compliete == true)
            {
                eventLoopPost(ZB_HANDLER_EVENTS, NEW_DEVICE_JOINED, ctx->parent_dev, sizeof(device_custom_t), portMAX_DELAY);
            }*/
            if(ctx!=NULL) free(ctx);
        }
    } else
     {
        if(ctx==NULL) ESP_LOGW(TAG, "simple_desc_cb ERROR user_ctx"); else
        {
            ESP_LOGW(TAG, "simple_desc_cb ERROR ZDO_STATUS %d on try counter %d", zdo_status, ctx->try_if_error_timeout);
            ctx->try_if_error_timeout = ctx->try_if_error_timeout + 1;
            esp_zb_zdo_simple_desc_req_param_t req;
            req.addr_of_interest = ctx->parent_dev->short_addr;
            req.endpoint = ctx->ep->ep_id;
            if (ctx->try_if_error_timeout < 4) zb_manager_zdo_simple_desc_req(&req, simple_desc_cb, ctx);else
            {
                ESP_LOGW(TAG, "simple_desc_cb ERROR ZDO_STATUS %d on try counter %d", zdo_status, ctx->try_if_error_timeout);
                ESP_LOGW(TAG, "simple_desc_cb ERROR on try counter Здесь надо удалить устройство и сообщить юзеру, не удалось");
                eventLoopPost(ZB_HANDLER_EVENTS, NEW_DEVICE_JOINED_FAIL, ctx->parent_dev, sizeof(device_custom_t), portMAX_DELAY);
                if(ctx!=NULL) free(ctx);
            }
        }
    }
     ESP_LOGW(TAG, "RAM control %lu", esp_get_free_heap_size());
}


static void active_ep_cb(esp_zb_zdp_status_t zdo_status, uint8_t ep_count, uint8_t *ep_id_list, void *user_ctx)
{
    device_appending_sheduler_t* sheduler = (device_appending_sheduler_t*)(user_ctx);
    uint8_t *dev_index = NULL;
    dev_index = (uint8_t*)user_ctx;
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) 
    {
        ESP_LOGI(TAG, "Active EP callback");
       
        if(dev_index == NULL) ESP_LOGW(TAG, "Active EP callback ERROR user_ctx"); else
        {
            uint8_t index = *dev_index;
            free(dev_index);
            dev_index = NULL;
            device_custom_t* temp_pointer = NULL;
            temp_pointer = (device_custom_t*)RemoteDevicesArray[index];
            if (temp_pointer == NULL) ESP_LOGW(TAG, "Active EP callback ERROR DevFind in RemoteDevList index %d", index); else
            {
                ESP_LOGW(TAG, "Active EP callback DevFind in RemoteDevList");
                ESP_LOGW(TAG, "Active EP callback Начинаем добавлять Эндпоинты");
                ESP_LOGW(TAG, "Active EP callback Не забудь выполнить проверку, вдруг уже есть EP, то их надо очистить сперва");
                if (temp_pointer->endpoints_array != NULL && temp_pointer->endpoints_count > 0) 
                {
                    for (uint8_t i = 0; i < temp_pointer->endpoints_count; i++)
                    {
                        if (temp_pointer->endpoints_array[i] != NULL) 
                        {
                            RemoteDeviceEndpointDelete(temp_pointer->endpoints_array[i]);
                        }
                    }
                }
                if(ep_count>0)
                {
                    /*temp_pointer->control_dev_annce_simple_desc_req_count = ep_count;
                    if (temp_pointer->control_dev_annce_simple_desc_req_array != NULL)
                    {
                        free(temp_pointer->control_dev_annce_simple_desc_req_array);
                        temp_pointer->control_dev_annce_simple_desc_req_array = NULL;
                    }
                    temp_pointer->control_dev_annce_simple_desc_req_array = calloc(ep_count, sizeof(dev_annce_simple_desc_controll_t*));*/

                    temp_pointer->endpoints_count = ep_count;
                    temp_pointer->endpoints_array = calloc(temp_pointer->endpoints_count, sizeof(endpoint_custom_t*));//ep_id_list[i];
                    
                    for(int i = 0; i < ep_count; i++)
                    {
                        temp_pointer->endpoints_array[i] = RemoteDeviceEndpointCreate(ep_id_list[i]);
                        if(temp_pointer->endpoints_array[i] == NULL) ESP_LOGW(TAG, "Active EP callback ERROR Create EP");else
                        {
                            ESP_LOGW(TAG, "Active EP callback OK Create EP id %d", ep_id_list[i]);
                            endpoint_custom_t* temp_ep_pointer = (endpoint_custom_t*)temp_pointer->endpoints_array[i];
                            build_dev_simple_desc_user_ctx_t* ctx = calloc(1, sizeof(build_dev_simple_desc_user_ctx_t));
                            ctx->parent_dev= temp_pointer;
                            ctx->ep = temp_ep_pointer;
                            ctx->try_if_error_timeout = 1;
                            esp_zb_zdo_simple_desc_req_param_t req;
                            req.addr_of_interest = temp_pointer->short_addr;
                            req.endpoint = temp_ep_pointer->ep_id;
                            ESP_LOGW(TAG, "Active EP callback Send SimpleDesc to (short: 0x%04x) EP id %d", temp_pointer->short_addr, ep_id_list[i]);
                            //if (esp_zb_lock_acquire(portMAX_DELAY))
                            //{
                                //temp_pointer->control_dev_annce_simple_desc_req_array[i]->ep_id = ep_id_list[i];
                                //temp_pointer->control_dev_annce_simple_desc_req_array[i]->status = 0;
                                //temp_pointer->control_dev_annce_simple_desc_req_array[i] = calloc(1, sizeof(dev_annce_simple_desc_controll_t));
                                //temp_pointer->control_dev_annce_simple_desc_req_array[i]->ep_id = ep_id_list[i];
                                //temp_pointer->control_dev_annce_simple_desc_req_array[i]->status = 0;
                                zb_manager_zdo_simple_desc_req(&req, simple_desc_cb, ctx);
                                vTaskDelay(300/ portTICK_PERIOD_MS);
                                //esp_zb_lock_release();
                            //}
                        }
                    }
                }
            }
    }
    }else {
    ESP_LOGW(TAG, "Active EP callback failed: %d", zdo_status);
    //if (dev_index) free(dev_index); // ← Освобождение в любом случае
    }
    /*uint8_t *dev_index = NULL;
    if (user_ctx == NULL) ESP_LOGW(TAG, "Active EP callback ERROR user_ctx"); else dev_index = (uint8_t*)user_ctx;
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS)
    {        
            ESP_LOGW(TAG, "Active endpoint response: status(%d) and endpoint count(%d) index(%u) ", zdo_status, ep_count, *dev_index);
            for (int i = 0; i < ep_count; i++) {
                ESP_LOGI(TAG, "Endpoint ID List: %d", ep_id_list[i]);
            }
           
    }else 
        {
            ESP_LOGW(TAG, "Active EP req error");
            return;
        }*/
}

//***************************************************** Колбэк на таймер appending_controll_timer ***********************************************************/
void appending_controll_timeout_CB(void *arg) {
    ESP_LOGW(TAG, "appending_controll_CB! TIMEOUT");
    if (arg != NULL)
    {
            device_appending_sheduler_t* shedule = (device_appending_sheduler_t*)arg;
            esp_timer_delete(shedule->appending_controll_timer);
            shedule->appending_controll_timer = NULL;
            ESP_LOGW(TAG, "//// Надо удалить устройство и сообщить о неудаче при добавлении");
            //// Надо удалить устройство и сообщить о неудаче при добавлении
    }
}

static void StartUpdateDevices(device_appending_sheduler_t* shedule)
{
    if(shedule!= NULL)
    {
        shedule->tuya_magic_status = 0;
        shedule->tuya_magic_read_attr_cmd_param.address_mode = ESP_ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
        shedule->tuya_magic_read_attr_cmd_param.zcl_basic_cmd.src_endpoint = 1;
        shedule->tuya_magic_read_attr_cmd_param.zcl_basic_cmd.dst_endpoint = 1;
        shedule->tuya_magic_read_attr_cmd_param.zcl_basic_cmd.dst_addr_u.addr_short = shedule->appending_device->short_addr;
        shedule->tuya_magic_read_attr_cmd_param.clusterID = ESP_ZB_ZCL_CLUSTER_ID_BASIC;
        uint16_t attributes[] = {
                ESP_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, ESP_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID,ESP_ZB_ZCL_ATTR_BASIC_APPLICATION_VERSION_ID,
                ESP_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, ESP_ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, 0xfffe,
        };
        shedule->tuya_magic_read_attr_cmd_param.attr_number = sizeof(attributes) / sizeof(attributes[0]);
        shedule->tuya_magic_read_attr_cmd_param.attr_field = calloc(1,sizeof(attributes[0]) * shedule->tuya_magic_read_attr_cmd_param.attr_number);
        memcpy(shedule->tuya_magic_read_attr_cmd_param.attr_field, attributes, sizeof(uint16_t) * shedule->tuya_magic_read_attr_cmd_param.attr_number);
        shedule->tuya_magic_req_tsn = 0xff;

        // запускаем контрольный таймер
        if (shedule->appending_controll_timer != NULL) esp_timer_delete(shedule->appending_controll_timer);
        const esp_timer_create_args_t appending_controll_timer_args = {
            .callback = &appending_controll_timeout_CB,
            .name = "one-shot",
            .arg = shedule,
        };
        ESP_ERROR_CHECK(esp_timer_create(&appending_controll_timer_args, &shedule->appending_controll_timer));
        ESP_ERROR_CHECK(esp_timer_start_once(shedule->appending_controll_timer, 5000 * 1000)); // запускаем на 5 секунд
        ESP_LOGW(TAG, "shedule->appending_controll_timer stated");
        // отправляем запрос
        shedule->tuya_magic_req_tsn = zm_manager_zcl_read_attr_cmd_req(&shedule->tuya_magic_read_attr_cmd_param);
        if(shedule->tuya_magic_req_tsn == 0xff)
            {
                ESP_LOGW(TAG, "ERROR SENDING TUYA_MAGIC Break!!!");
                local_DeviceAppendShedulerArray[shedule->index_in_sheduler_array] = NULL;
                free(shedule);
                shedule = NULL;
                return;
            } else 
            {
                ESP_LOGW(TAG, "TUYA_MAGIC SEND OK with tsn %d", shedule->tuya_magic_req_tsn);
                shedule->tuya_magic_status = 1;
                // Формируем запрос active_ep_req
                shedule->active_ep_req.addr_of_interest = shedule->appending_device->short_addr;
                shedule->active_ep_req_status = 0;
            }
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
                //  создаём устройство !!! Проверено
                // 1. ищем по списку
                bool device_found = false;         // если будет найдено, то меняем статус is_in_build_status, а значит после dev_annce надо будет его опросить как новое
                for (int i = 0; i < local_RemoteDevicesCount; i++)
                {
                    if (local_RemoteDevicesArray[i]!=NULL) 
                    {
                        if (ieee_addr_compare(&local_RemoteDevicesArray[i]->ieee_addr, &dev_assoc_params->device_addr) == 0)
                        {
                            device_found = true;
                            ESP_LOGW(TAG, "ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED  device found in RemoteDevList FLAG Update set enable!!!");
                            local_RemoteDevicesArray[i]->is_in_build_status = true; // переопределяем статус добавления и очищаем
                            if (local_RemoteDevicesArray[i]->endpoints_count > 0)
                            {
                                for (int j = 0; j < local_RemoteDevicesArray[i]->endpoints_count; j++)
                                {
                                    RemoteDeviceEndpointDelete(local_RemoteDevicesArray[i]->endpoints_array[j]);
                                }
                                local_RemoteDevicesArray[i]->endpoints_count = 0;
                            }
                            break;
                        }
                    }
                }
                // 2 если не нашли, то надо найти свободную ячейку и создать новое
                if (device_found == false)
                {
                    uint8_t free_index = REMOTE_DEVICES_COUNT + 1; // несуществующий индекс
                    for(int i = 0; i < REMOTE_DEVICES_COUNT; i++)
                    {  
                        if (local_RemoteDevicesArray[i] == NULL)
                        {
                            free_index = i;
                            break;
                        }
                    }
                    if (free_index < REMOTE_DEVICES_COUNT + 1)
                    {
                        esp_zb_ieee_addr_t temp_ieee;
                        memcpy(temp_ieee, dev_assoc_params->device_addr, sizeof(esp_zb_ieee_addr_t));
                        local_RemoteDevicesArray[free_index] = RemoteDeviceCreate(temp_ieee);
                        if (local_RemoteDevicesArray[free_index] != NULL) 
                        {
                            local_RemoteDevicesArray[free_index]->is_in_build_status = true;
                            local_RemoteDevicesArray[free_index]->endpoints_count = 0;
                            memset(local_RemoteDevicesArray[free_index]->friendly_name,0,sizeof(local_RemoteDevicesArray[free_index]->friendly_name));
                            sprintf(local_RemoteDevicesArray[free_index]->friendly_name, "Устройство [ %d ]", free_index + 1);

                            ESP_LOGI(TAG, "Создано пустое устройство под индексом %d (mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x)", free_index, dev_assoc_params->device_addr[7], 
                            dev_assoc_params->device_addr[6], dev_assoc_params->device_addr[5], dev_assoc_params->device_addr[4],
                            dev_assoc_params->device_addr[3], dev_assoc_params->device_addr[2], dev_assoc_params->device_addr[1], dev_assoc_params->device_addr[0]);
                        }
                    } else ESP_LOGW(TAG, "ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED Список устройств занят, для добавления надо расширить список");
                }
            }
        break;
    case ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE:
       
        if(err_status == ESP_OK)
            {
                ESP_LOGI(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE:");
                dev_update_params = (esp_zb_zdo_signal_device_update_params_t *)esp_zb_app_signal_get_params(p_sg_p);
                // Обновляем короткий адрес
                      // 1. ищем по списку
                bool device_found = false;         // если будет найдено, то меняем статус is_in_build_status, а значит после dev_annce надо будет его опросить как новое
                bool need_full_update = false;
                for (int i = 0; i < local_RemoteDevicesCount; i++)
                {
                    if (local_RemoteDevicesArray[i]!=NULL) 
                    {
                        if (ieee_addr_compare(&local_RemoteDevicesArray[i]->ieee_addr, &dev_update_params->long_addr) == 0)
                        {
                            device_found = true;
                            ESP_LOGW(TAG, "ESP_ZB_NWK_SIGNAL_DEVICE_UPDATE  device found and SHORT_ADDR Update!!!");
                            //zb_manager_obj.RemoteDevicesArray[i]->is_in_build_status = true; // меняем статус на переподключение
                            local_RemoteDevicesArray[i]->short_addr = dev_update_params->short_addr;
                            if (local_RemoteDevicesArray[i]->is_in_build_status == 1)
                            {
                                need_full_update = true;
                                // Send TuyaMagicPacket
                                device_appending_sheduler_t* appending_ctx = calloc(1, sizeof(device_appending_sheduler_t));

                                if (appending_ctx != NULL)
                                {
                                    appending_ctx->index_in_sheduler_array = 0xff;
                                    appending_ctx->appending_device = local_RemoteDevicesArray[i];
                                    // устанавливаем индекс задачи
                                    for(int i = 0; i < local_DeviceAppendShedulerCount; i++)
                                    {
                                        if (local_DeviceAppendShedulerArray[i] == NULL) 
                                        {
                                            local_DeviceAppendShedulerArray[i] = appending_ctx;
                                            appending_ctx->index_in_sheduler_array = i;
                                            break;
                                        }
                                    }
                                    if(appending_ctx->index_in_sheduler_array == 0xff) 
                                    {
                                        ESP_LOGW(TAG, "DeviceAppendShedulerArray is full!!!");
                                        free(appending_ctx);
                                        appending_ctx = NULL;
                                        break;
                                    }
                                    //Запускаем шедулер
                                    appending_ctx->tuya_magic_try_count = 1;
                                    StartUpdateDevices(appending_ctx);
                                    
                                }
                            }
                            //memcpy(temp_ieee, zb_manager_obj.RemoteDevicesArray[i], 8);
                            break;
                        }
                    }
                }
                // начинаем процесс добавления устройства
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
        // если устройство в стадии построения, то надо его опросить как новое
                // 1. ищем по списку
                bool device_found = false;         // если будет найдено, то проверяем статус is_in_build_status, и запускаем esp_zb_zdo_active_ep_req
                device_custom_t*  temp_pointer = NULL;
                uint8_t* index = NULL;
                index = calloc(1, sizeof(uint8_t));
                *index = REMOTE_DEVICES_COUNT + 1; // несуществующий индекс
                for (int i = 0; i < RemoteDevicesCount; i++)
                {
                    if (RemoteDevicesArray[i]!=NULL) 
                    {
                        if (ieee_addr_compare(&RemoteDevicesArray[i]->ieee_addr, &dev_annce_params->ieee_addr) == 0)
                        {
                            
                            if (RemoteDevicesArray[i]->is_in_build_status == 1)
                            {
                                // обновляем short_addr
                                RemoteDevicesArray[i]->short_addr = dev_annce_params->device_short_addr;
                                ESP_LOGW(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE  device found in RemoteDevList with status is_in_build_status!!! (short: 0x%04x)", RemoteDevicesArray[i]->short_addr); 
                                //temp_pointer = (device_custom_t*)zb_manager_obj.RemoteDevicesArray[i];
                                *index = i;
                                device_found = true;
                                break;
                            }
        
                        }
                    }
                }
                if (device_found==true)
                {
                    
                    esp_zb_zdo_active_ep_req_param_t req;
                    req.addr_of_interest = dev_annce_params->device_short_addr;
                    ESP_LOGW(TAG, "ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE  Start esp_zb_zdo_active_ep_req devindex(%u)", *index);
                    //zb_manager_zdo_active_ep_req(&req, active_ep_cb, (void*)index); // отправляем в active_ep_cb индекс устройства в списке, чтобы не искать потом
                }
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
                ESP_LOGW(TAG, "Network(0x%04hx) is open for %d seconds", esp_zb_get_pan_id(), *(uint8_t *)esp_zb_app_signal_get_params(p_sg_p));
                eventLoopPost(ZB_HANDLER_EVENTS, NETWORK_IS_OPEN, NULL, 0, portMAX_DELAY);

            } else {
                ESP_LOGW(TAG, "Network(0x%04hx) closed, devices joining not allowed.", esp_zb_get_pan_id());
                eventLoopPost(ZB_HANDLER_EVENTS, NETWORK_IS_CLOSE, NULL, 0, portMAX_DELAY);
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
static void zb_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    ESP_LOGI(TAG, "Получено событие: база=%s, ID=%ld", base, id);
    device_custom_t* dev_info = NULL;
    zb_manager_cmd_read_attr_resp_message_t* read_cmd_resp_msg = NULL;
    zb_manager_active_ep_resp_message_t*     active_ep_req_resp_msg = NULL;
    zb_manager_simple_desc_resp_message_t*   simple_desc_resp = NULL;
    zb_manager_bind_resp_message_t*          bind_resp = NULL;
    switch (id) {
        case ATTR_REPORT_EVENT:
            ESP_LOGW(TAG, "zb_manager_report_attr_event_fn from EVENT ATTR_REPORT_EVENT");
            //lvgl_port_task_wake
            //zb_manager_temperature_sensor_ui_t* ui_temperature_sensor = create_new_temperature_sensor_ui(ui_MainScreenSensorsContainer, NULL, true, false);
            lv_obj_class_t* lv_obj_class = NULL;
            break;
        case ATTR_READ_RESP:
            read_cmd_resp_msg = (zb_manager_cmd_read_attr_resp_message_t*)event_data;
            
            if (read_cmd_resp_msg != NULL)
            {
                if(read_cmd_resp_msg->info.status == ESP_ZB_ZCL_STATUS_SUCCESS)
                {
                    ESP_LOGW(TAG, "zb_manager_read_attr_response_fn from EVENT ATTR_READ_RESP from short 0x%4x and tsn %d", read_cmd_resp_msg->info.src_address.u.short_addr, read_cmd_resp_msg->info.header.tsn);
                    for(int i = 0; i<read_cmd_resp_msg->attr_count;i++)
                    {
                        zb_manager_cmd_read_attr_t attr = read_cmd_resp_msg->attr_arr[i];
                        ESP_LOGW(TAG, "zb_manager_read_attr_response_fn attr_id 0x%x attr_type 0x%x attr_len %d",
                            attr.attr_id, attr.attr_type, attr.attr_len);

                        //ESP_LOG_BUFFER_HEX_LEVEL(TAG, attr.attr_value, attr.attr_len, ESP_LOG_DEBUG);     
                    }
                    // Ищем sheduler по короткому адресу, если есть, то продолжаем добавление
                    for (int j = 0; j < local_DeviceAppendShedulerCount; j++)
                    {
                        if (local_DeviceAppendShedulerArray[j]!=NULL) 
                        {
                            device_appending_sheduler_t* sheduler = (device_appending_sheduler_t*)local_DeviceAppendShedulerArray[j]; //
                            if(sheduler != NULL)
                            {
                                if (sheduler->appending_device != NULL)
                                {
                                    if (sheduler->appending_device->short_addr == read_cmd_resp_msg->info.src_address.u.short_addr)
                                    {
                                        if (sheduler->appending_device->is_in_build_status == 1)
                                        {
                                            ESP_LOGW(TAG, "TUYA_MAGIC response with TSN %d",read_cmd_resp_msg->info.header.tsn);
                                            ESP_LOGW(TAG, "zb_manager_read_attr_response_fn device found in RemoteDevList with status is_in_build_status!!! (short: 0x%04x)", sheduler->appending_device->short_addr); 
                                            ESP_LOGW(TAG, "Продолжаем добавление устройства переходим к active_ep_req");
                                            sheduler->tuya_magic_resp_tsn = read_cmd_resp_msg->info.header.tsn;
                                            sheduler->tuya_magic_status = 2;
                                            sheduler->active_ep_req_status = 1;
                                            sheduler->active_ep_req.addr_of_interest = sheduler->appending_device->short_addr;
                                            // сохраняем атрибуты Basic Cluster в устройство
                                            if (sheduler->appending_device->server_BasicClusterObj == NULL)
                                            {
                                                sheduler->appending_device->server_BasicClusterObj = calloc(1,sizeof(zigbee_manager_basic_cluster_t));
                                                zigbee_manager_basic_cluster_t cl = ZIGBEE_BASIC_CLUSTER_DEFAULT_INIT();
                                                memcpy(sheduler->appending_device->server_BasicClusterObj, &cl, sizeof(zigbee_manager_basic_cluster_t));
                                                ESP_LOGI(TAG, "  Basic Cluster added to device");
                                            }
                                            for(int i = 0; i<read_cmd_resp_msg->attr_count;i++)
                                            {
                                                zb_manager_cmd_read_attr_t attr = read_cmd_resp_msg->attr_arr[i];
                                                /*ESP_LOGW(TAG, "zb_manager_read_attr_response_fn attr_id 0x%x attr_type 0x%x attr_len %d",
                                                attr.attr_id, attr.attr_type, attr.attr_len);*/
                                                zb_manager_basic_cluster_update_attribute(sheduler->appending_device->server_BasicClusterObj, attr.attr_id, attr.attr_value);
                                            //ESP_LOG_BUFFER_HEX_LEVEL(TAG, attr.attr_value, attr.attr_len, ESP_LOG_DEBUG);     
                                            }
                                            ESP_ERROR_CHECK(esp_timer_restart(sheduler->appending_controll_timer, 5000 * 1000));
                                            ESP_LOGW(TAG, "Продолжаем добавление устройства таймер перезапущен");
                                            // отправляем запрос на получение активных эндпоинтов
                                            //!!!!!!!!!!! Внимание zb_manager_zdo_active_ep_req отправляется без CB
                                            //так как результат вернётся через esp_post_message, и там обработается
                                            //при добавлении устройства важно не нарушать последовательность запросов и ответов и последовательные обращения к local_DeviceAppendShedulerArray
                                            zb_manager_zdo_active_ep_req(&sheduler->active_ep_req, NULL, sheduler);
                                            vTaskDelay(300/ portTICK_PERIOD_MS);
                                            ESP_LOGW(TAG, "Продолжаем добавление устройства zb_manager_zdo_active_ep_req отправлен");
                                        }
                                        break;
                                    }
                                }
                            } //if(sheduler != NULL)
                        }
                    }
                    /************ ! Чистит только массив с атрибутами, сама структура удаляется из обработчика так как передача происходит чере esp_event_post */
                    zb_manager_free_read_attr_resp_attr_array(read_cmd_resp_msg);
                } else 
                {
                    ESP_LOGW(TAG, "zb_manager_read_attr_response_fn from EVENT ATTR_READ_RESP with error %d", read_cmd_resp_msg->info.status);
                }
            }
            break;
        case ACTIVE_EP_RESP:
            active_ep_req_resp_msg = (zb_manager_active_ep_resp_message_t *)event_data;
            if (active_ep_req_resp_msg != NULL)
            {
                if (active_ep_req_resp_msg->status == ESP_ZB_ZDP_STATUS_SUCCESS)
                {
                    ESP_LOGI(TAG, "Device has %d endpoints:",active_ep_req_resp_msg->ep_count);
                    if(active_ep_req_resp_msg->ep_count > 0)
                    {
                        for (int i = 0; i < active_ep_req_resp_msg->ep_count; i++) {
                        ESP_LOGI(TAG, "  EP %d", active_ep_req_resp_msg->ep_list[i]);
                        }
                    }
                    device_appending_sheduler_t* sheduler = (device_appending_sheduler_t*)active_ep_req_resp_msg->user_ctx;
                    if (sheduler == NULL) {
                        ESP_LOGI(TAG, "sheduler is NULL");
                        // ✅ Освобождаем память
                        zb_manager_free_active_ep_resp_ep_array(active_ep_req_resp_msg);
                        break;
                    }
                    if (sheduler->appending_device->is_in_build_status == 1)
                    {
                        /**********************   ставим статус выполнения active_ep */
                        ESP_LOGW(TAG, "Sheduler found in local_DeviceAppendShedulerArray with status is_in_build_status!!! (short: 0x%04x)", sheduler->appending_device->short_addr); 
                        sheduler->active_ep_req_status = 2;
                        // готовим simple_desc_req
                        sheduler->simple_desc_req_count = active_ep_req_resp_msg->ep_count;
                        sheduler->simple_desc_req_list = NULL;
                        sheduler->simple_desc_req_simple_desc_req_list_status = NULL;
                        sheduler->simple_desc_req_list = calloc(sheduler->simple_desc_req_count, sizeof(esp_zb_zdo_simple_desc_req_param_t));
                        if (sheduler->simple_desc_req_list == NULL) 
                        {
                            zb_manager_free_active_ep_resp_ep_array(active_ep_req_resp_msg);
                            break;
                        }
                        sheduler->simple_desc_req_simple_desc_req_list_status = calloc(sheduler->simple_desc_req_count, sizeof(uint8_t));
                        if (sheduler->simple_desc_req_simple_desc_req_list_status == NULL) 
                        {
                            if (sheduler->simple_desc_req_list != NULL) 
                            {
                                free(sheduler->simple_desc_req_list);
                                sheduler->simple_desc_req_list = NULL;
                            }
                            zb_manager_free_active_ep_resp_ep_array(active_ep_req_resp_msg);
                            break;
                        }
                        // перезапускаем таймер
                        ESP_ERROR_CHECK(esp_timer_restart(sheduler->appending_controll_timer, 5000 * 1000));
                       
                        for (int i = 0; i < sheduler->simple_desc_req_count; i++) 
                        {
                            sheduler->simple_desc_req_list[i].addr_of_interest = sheduler->appending_device->short_addr;
                            sheduler->simple_desc_req_list[i].endpoint = active_ep_req_resp_msg->ep_list[i];
                            sheduler->simple_desc_req_simple_desc_req_list_status[i] = 1;
                            if(zb_manager_zdo_simple_desc_req(&sheduler->simple_desc_req_list[i], NULL, sheduler)==ESP_OK)
                            {
                                ESP_LOGW(TAG, "Simple desc req sent index %d", i); // "Simple desc req sent index %d"
                                vTaskDelay(300/ portTICK_PERIOD_MS);
                            }
                        }
                    }
                    // ✅ Освобождаем память
                    zb_manager_free_active_ep_resp_ep_array(active_ep_req_resp_msg);
                } else {
                    ESP_LOGW(TAG, "Active EP failed: status=0x%02x", active_ep_req_resp_msg->status);
                }
            } // (active_ep_req_resp_msg != NULL)
            break;

        case SIMPLE_DESC_RESP:
            simple_desc_resp = (zb_manager_simple_desc_resp_message_t *)event_data;
            if (simple_desc_resp != NULL)
            {
                if (simple_desc_resp->status == ESP_ZB_ZDP_STATUS_SUCCESS && simple_desc_resp->simple_desc) 
                {
                    esp_zb_af_simple_desc_1_1_t *desc = simple_desc_resp->simple_desc;
                    ESP_LOGI(TAG, "Simple Desc OK: EP=%d, Profile=0x%04x, Device=0x%04x",
                        desc->endpoint, desc->app_profile_id, desc->app_device_id);
                    
                    device_appending_sheduler_t* sheduler = (device_appending_sheduler_t*)simple_desc_resp->user_ctx;
                    // находим запрос simple_desc и помечаем его выполненным
                    for (int i = 0; i < sheduler->simple_desc_req_count; i++) 
                    {
                            if (sheduler->simple_desc_req_list[i].endpoint == desc->endpoint) 
                            {
                                sheduler->simple_desc_req_simple_desc_req_list_status[i] = 2;
                                break;
                            }
                    }
                    // добавляем эндпоинт в уструйство
                    if (sheduler->appending_device->is_in_build_status == 1)
                    {
                        endpoint_custom_t* temp_endpoint = NULL;
                        if (sheduler->appending_device->endpoints_array == NULL) 
                        {
                            sheduler->appending_device->endpoints_array = calloc(1, sizeof(endpoint_custom_t*));
                            sheduler->appending_device->endpoints_count = sheduler->appending_device->endpoints_count + 1;
                            sheduler->appending_device->endpoints_array[sheduler->appending_device->endpoints_count-1] = RemoteDeviceEndpointCreate(desc->endpoint); // [sheduler->appending_device->endpoints_count-1
                            temp_endpoint = (endpoint_custom_t*)sheduler->appending_device->endpoints_array[sheduler->appending_device->endpoints_count-1];
                        } else 
                        {
                            sheduler->appending_device->endpoints_count = sheduler->appending_device->endpoints_count + 1;
                            sheduler->appending_device->endpoints_array = realloc(sheduler->appending_device->endpoints_array, sheduler->appending_device->endpoints_count * sizeof(endpoint_custom_t*));
                            sheduler->appending_device->endpoints_array[sheduler->appending_device->endpoints_count-1] = RemoteDeviceEndpointCreate(desc->endpoint);
                            temp_endpoint = (endpoint_custom_t*)sheduler->appending_device->endpoints_array[sheduler->appending_device->endpoints_count-1];
                        }
                        if (temp_endpoint != NULL)
                        {
                            temp_endpoint->ep_id = desc->endpoint;
                            temp_endpoint->is_use_on_device = 1;
                            memset(temp_endpoint->friendly_name,0,sizeof(temp_endpoint->friendly_name));
                            sprintf(temp_endpoint->friendly_name, "[0x%4x] [0x%02x]",sheduler->appending_device->short_addr, desc->endpoint);
                            temp_endpoint->deviceId = desc->app_device_id;
                            // добавляем кластеры из доступных
                            // на данный момент только basic, identify, temperature
                            uint16_t *clusters = desc->app_cluster_list;
                            for (int i = 0; i < desc->app_input_cluster_count; i++) 
                            {
                                ESP_LOGI(TAG, "  Input Cluster: 0x%04x", clusters[i]);
                                esp_zb_zdo_bind_req_param_t *bind_req = NULL;
                                if (clusters[i] == 0x0000) 
                                {
                                    temp_endpoint->is_use_basic_cluster = 1;
                                    temp_endpoint->server_BasicClusterObj = calloc(1,sizeof(zigbee_manager_basic_cluster_t));
                                    zigbee_manager_basic_cluster_t cl = ZIGBEE_BASIC_CLUSTER_DEFAULT_INIT();
                                    memcpy(temp_endpoint->server_BasicClusterObj, &cl, sizeof(zigbee_manager_basic_cluster_t));
                                    ESP_LOGI(TAG, "  Basic Cluster added");
                                } else
                                if (clusters[i] == 0x0003) 
                                {
                                    temp_endpoint->is_use_identify_cluster = 1;
                                    temp_endpoint->server_IdentifyClusterObj = calloc(1,sizeof(zb_manager_identify_cluster_t));
                                    zb_manager_identify_cluster_t cl = ZIGBEE_IDENTIFY_CLUSTER_DEFAULT_INIT();
                                    memcpy(temp_endpoint->server_IdentifyClusterObj, &cl, sizeof(zb_manager_identify_cluster_t));
                                    ESP_LOGI(TAG, "  Identify Cluster added");
                                }else
                                if (clusters[i] == 0x0402) 
                                {
                                    temp_endpoint->is_use_temperature_measurement_cluster = 1;
                                    temp_endpoint->server_TemperatureMeasurementClusterObj = calloc(1,sizeof(zb_manager_temperature_measurement_cluster_t));
                                    zb_manager_temperature_measurement_cluster_t cl = ZIGBEE_TEMP_MEASUREMENT_CLUSTER_DEFAULT_INIT();
                                    memcpy(temp_endpoint->server_TemperatureMeasurementClusterObj, &cl, sizeof(zb_manager_temperature_measurement_cluster_t));
                                    ESP_LOGI(TAG, "  Temperature Measurement Cluster added");
                                    ESP_LOGI(TAG, "  Create BIND_REQ");
                                    bind_req = calloc(1, sizeof(esp_zb_zdo_bind_req_param_t));
                                    bind_req->cluster_id = 0x0402;
                                    bind_req->dst_addr_mode = ESP_ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
                                    memcpy(bind_req->dst_address_u.addr_long, LocalIeeeAdr, sizeof(esp_zb_ieee_addr_t));
                                    memcpy(bind_req->src_address, sheduler->appending_device->ieee_addr, sizeof(esp_zb_ieee_addr_t));
                                    bind_req->src_endp = desc->endpoint;
                                    bind_req->dst_endp = 0x01;
                                    bind_req->req_dst_addr = sheduler->appending_device->short_addr;
                                }/*else
                                if (clusters[i] == 0x0405) 
                                {
                                    temp_endpoint-> = 1;
                                    temp_endpoint->server_TemperatureMeasurementClusterObj = calloc(1,sizeof(zb_manager_temperature_measurement_cluster_t));
                                    zb_manager_temperature_measurement_cluster_t cl = ZIGBEE_TEMP_MEASUREMENT_CLUSTER_DEFAULT_INIT();
                                    memcpy(temp_endpoint->server_TemperatureMeasurementClusterObj, &cl, sizeof(zb_manager_temperature_measurement_cluster_t));
                                    ESP_LOGI(TAG, "  Temperature Measurement Cluster added");
                                    ESP_LOGI(TAG, "  Create BIND_REQ");
                                    bind_req = calloc(1, sizeof(esp_zb_zdo_bind_req_param_t));
                                    bind_req->cluster_id = 0x0402;
                                    bind_req->dst_addr_mode = ESP_ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
                                    memcpy(bind_req->dst_address_u.addr_long, LocalIeeeAdr, sizeof(esp_zb_ieee_addr_t));
                                    memcpy(bind_req->src_address, sheduler->appending_device->ieee_addr, sizeof(esp_zb_ieee_addr_t));
                                    bind_req->src_endp = desc->endpoint;
                                    bind_req->dst_endp = 0x01;
                                    bind_req->req_dst_addr = sheduler->appending_device->short_addr;
                                }*/
                                if (bind_req != NULL) 
                                {
                                    ESP_LOGW(TAG, "SEND Bind req epid %d, clid 0x%4x" , desc->endpoint, clusters[i]);
                                    esp_zb_zdo_device_bind_req(bind_req, NULL, bind_req);
                                    // перезапускаем таймер
                                    ESP_ERROR_CHECK(esp_timer_restart(sheduler->appending_controll_timer, 5000 * 1000));
                                }
                                    
                                    //2. координатор биндит устройство
                                    /*sheduler->bind_req_count = sheduler->bind_req_count + 1;
                                    sheduler->bind_req_list = realloc(sheduler->bind_req_list, sheduler->bind_req_count * sizeof(esp_zb_zdo_bind_req_param_t));
                                    sheduler->bind_req_list_status = realloc(sheduler->bind_req_list_status, sheduler->bind_req_count * sizeof(uint8_t));
                                    sheduler->bind_req_list[sheduler->bind_req_count-1].cluster_id = clusters[i];
                                    sheduler->bind_req_list[sheduler->bind_req_count-1].dst_addr_mode = ESP_ZB_APS_ADDR_MODE_64_ENDP_PRESENT;
                                    memcpy(sheduler->bind_req_list[sheduler->bind_req_count-1].dst_address_u.addr_long, sheduler->appending_device->ieee_addr, sizeof(esp_zb_ieee_addr_t));
                                    memcpy(sheduler->bind_req_list[sheduler->bind_req_count-1].src_address, LocalIeeeAdr, sizeof(esp_zb_ieee_addr_t));
                                    sheduler->bind_req_list[sheduler->bind_req_count-1].dst_endp = desc->endpoint;
                                    sheduler->bind_req_list[sheduler->bind_req_count-1].src_endp = 0x01; // на координаторе всегда 0x01
                                    sheduler->bind_req_list[sheduler->bind_req_count-1].req_dst_addr = 0x0000;
                                    sheduler->bind_req_list_status[sheduler->bind_req_count-1] = 0;*/
                            }
                            for (int i = 0; i < desc->app_output_cluster_count; i++) 
                            {
                                ESP_LOGI(TAG, "  Output Cluster: 0x%04x", clusters[desc->app_input_cluster_count + i]);
                            }
                                ESP_LOGI(TAG, "Endpoint 0x%02x added to device with short_addr 0x%04x", desc->endpoint, sheduler->appending_device->short_addr);
                            }
                            // перезапускаем таймер
                            ESP_ERROR_CHECK(esp_timer_restart(sheduler->appending_controll_timer, 5000 * 1000));
                        

                        // проверяем все статусы, если 2 то считаем что все запросы выполнены
                        bool all_simple_desc_req_done = true;
                        for (int i = 0; i < sheduler->simple_desc_req_count; i++) 
                        {
                            if (sheduler->simple_desc_req_simple_desc_req_list_status[i] != 2) 
                            {
                                all_simple_desc_req_done = false;
                                break;
                            }
                        }
                        if (all_simple_desc_req_done) 
                        {
                            sheduler->appending_device->is_in_build_status = 1;
                            
                            /******** Надо отправить инфу в gui */
                            ESP_LOGW(TAG, "All simple desc req done Не забудь отправить инфу в gui");
                            sheduler->appending_device->is_in_build_status = 2;
                            ESP_ERROR_CHECK(esp_timer_stop(sheduler->appending_controll_timer));
                            ESP_ERROR_CHECK(esp_timer_delete(sheduler->appending_controll_timer));
                            ui_ScreenObject_t* device_screen = (ui_ScreenObject_t*)ScreensMainList[DevicesScreenIndex];
                            ui_device_for_devices_screen_widget_create(device_screen->main_panel_obj_pointer->main_panel_for_devices_widget_show_screen->on_create_elements_array[0], 
                                sheduler->appending_device, DevicesScreenIndex);
                            //очищаем планировщик
                            uint8_t index = 0xff;
                            for (int i = 0; i < local_DeviceAppendShedulerCount; i++)
                            {
                                if(local_DeviceAppendShedulerArray[i] == sheduler) index = i; 
                                break;
                            } 
                            if (index != 0xff) 
                            {
                                zb_manager_delete_appending_sheduler(sheduler);
                                local_DeviceAppendShedulerArray[index] = NULL;
                            }
                            
                        }
                    }
            
                } else 
                {
                    ESP_LOGW(TAG, "Simple Desc failed: status=0x%02x", simple_desc_resp->status);
                }
                // ✅ Освобождаем память
                zb_manager_free_simple_desc_resp(simple_desc_resp);
            } else ESP_LOGW(TAG, "Simple Desc resp is NULL");
            break;
        case BIND_RESP: 
            bind_resp = (zb_manager_bind_resp_message_t*)event_data;
            if (bind_resp!=NULL) 
            {
                if (bind_resp->status == ESP_ZB_ZDP_STATUS_SUCCESS) 
                {
                    esp_zb_zdo_bind_req_param_t *bind_req_ctx = NULL;
                    bind_req_ctx = (esp_zb_zdo_bind_req_param_t*)bind_resp->user_ctx;
                    if(bind_req_ctx!=NULL)
                    {
                        if (bind_req_ctx->cluster_id == 0x0402) 
                        {
                            ESP_LOGW(TAG, "BIND_REQ for TEMP_MEASUREMENT DEVICE (0x0402) Забиндило нас dev_short_addr 0x%04x", bind_req_ctx->req_dst_addr); 
                        }
                        free(bind_req_ctx);
                        bind_req_ctx = NULL;
                    }else ESP_LOGW(TAG, "BIND_REQ ctx is NULL");
                }else 
                {
                    ESP_LOGW(TAG, "Bind Resp failed: status=0x%02x", bind_resp->status);
                }

            // ✅ Освобождаем память
            zb_manager_free_bind_resp(bind_resp);
            } else ESP_LOGW(TAG, "Bind Resp is NULL");
            break;
        case NETWORK_IS_OPEN:
            ESP_LOGI(TAG, "Network is open from event");
            MainPanelForDevicesWidget_network_state_img_set(ScreensMainList[DevicesScreenIndex]->main_panel_obj_pointer->buttons_for_devices_widget_show_screen,1);
            break;
        case NETWORK_IS_CLOSE:
        ESP_LOGI(TAG, "Network is close from event");
            MainPanelForDevicesWidget_network_state_img_set(ScreensMainList[DevicesScreenIndex]->main_panel_obj_pointer->buttons_for_devices_widget_show_screen,0);
            break;
        case NEW_DEVICE_JOINED:
            dev_info = (device_custom_t*)event_data;
            ESP_LOGW(TAG, "New device joined from event 0x%4x", dev_info->short_addr); // 
            break;
        case NEW_DEVICE_JOINED_FAIL:
            dev_info = (device_custom_t*)event_data;
            ESP_LOGW(TAG, "New device joined fail from eventt 0x%4x", dev_info->short_addr); 
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

static void ui_screen_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    ESP_LOGI(TAG, "Получено событие: база=%s, ID=%ld", base, id);
    switch (id) {
    case UI_SCREEN_TP_HOME_CLICK:  // нажатие на кнопку "Домой"
        ui_ScreenObjectLoadScreen(ScreensMainList[0]);
        break;
    case UI_SCREEN_TP_DEVICES_CLICK:  // нажатие на кнопку "Устройства"
        ui_ScreenObjectLoadScreen(ScreensMainList[1]);
        break;
    case UI_SCREEN_MP_NETWORK_OPEN_CLICK:
        zb_manager_open_network(180);
        break;
    case UI_SCREEN_MP_NETWORK_CLOSE_CLICK:
        zb_manager_close_network();
        break;
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
    ESP_ERROR_CHECK(esp_event_handler_register(ZB_HANDLER_EVENTS, ESP_EVENT_ANY_ID, &zb_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(UI_SCREEN_EVENTS, ESP_EVENT_ANY_ID, &ui_screen_event_handler, NULL));

    // создаём список устройств
    //#define REMOTE_DEVICES_COUNT (32) /************* !!!! zb_manager_devices.h */

    //#define REMOTE_DEVICES_COUNT (32) /************* !!!! zb_manager_devices.h */
    zb_manager_devices_init();
    local_RemoteDevicesCount = RemoteDevicesCount; //!!!! zb_manager_devices.h */
    local_RemoteDevicesArray = RemoteDevicesArray;
    local_DeviceAppendShedulerCount = DeviceAppendShedulerCount;
    local_DeviceAppendShedulerArray = DeviceAppendShedulerArray;

    lvgl_port_gui_init();
    /*lvgl_main_prog_start();
    ESP_LOGI(TAG, "RAM left before create %lu", esp_get_free_heap_size());
    zb_manager_temperature_sensor_ui_t* test_sensor = create_new_temperature_sensor_ui();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after create %lu", esp_get_free_heap_size());
    draw_temperature_sensor_minimal(test_sensor, ui_MainScreenSensorsContainer);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after draw %lu", esp_get_free_heap_size());
    draw_temperature_sensor_minimal(test_sensor, ui_MainScreenSensorsContainer);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after redraw %lu", esp_get_free_heap_size());*/
    
    // создаём список экранов
    ScreensMainList = (ui_ScreenObject_t**)calloc(2, sizeof(ui_ScreenObject_t*)); // ДОМ УСТРОЙСТВА
    ScreensMainList[0] =  ui_ScreenObjectCreate("ДОМ", 0, SCREEN_FOR_SENSORS_WIDGET_SHOW);
    HomeScreenIndex = 0;
    ScreensMainList[1] =  ui_ScreenObjectCreate("УСТРОЙСТВА", 1, SCREEN_FOR_DEVICES_WIDGET_SHOW);
    DevicesScreenIndex = 1;
    //ui_ScreenObject_t* screen2_obj =  ui_ScreenObjectCreate("Главный экран2", 1);
    //ui_ScreenObjectSetScreenName(screen1_obj, "bjkfvnjkfdvnjfkdvnkdfjv");
    ui_ScreenObject_t* home_screen_obj = (ui_ScreenObject_t*)ScreensMainList[HomeScreenIndex];
    ui_ScreenObject_t* devices_screen_obj = (ui_ScreenObject_t*)ScreensMainList[DevicesScreenIndex];
    ui_ScreenObjectLoadScreen(home_screen_obj);

    draw_temperature_sensor_minimal_widget(home_screen_obj->main_panel_obj_pointer->main_panel_for_sensor_widget_show_screen->on_create_elements_array[0]);

    ui_device_for_devices_screen_widget_create(devices_screen_obj->main_panel_obj_pointer->main_panel_for_devices_widget_show_screen->on_create_elements_array[0], NULL, DevicesScreenIndex);
    //lv_obj_t* btn = lv_btn_create(screen2_obj->on_create_elements_array[0]);
    //lv_obj_set_parent(btn, screen1_obj->on_create_elements_array[0]);
    //ui_ScreenTopPanelObject_t* top_panel = ScreenTopPanelObjectCreate();
    //if (top_panel==NULL) ESP_LOGI(TAG, "top_panel is NULL");
    //lv_obj_set_parent(top_panel->on_create_elements_array[0], screen1_obj->on_create_elements_array[0]);
    
    //ui_ScreenMainPanelObject_t* main_panel = ScreenMainPanelObjectCreate();
    //if (ui_ScreenObjectSetTopPanel(screen1_obj, top_panel) == ESP_FAIL) ESP_LOGI(TAG, "ui_ScreenObjectSetTopPanel FAIL");
    //ui_ScreenObjectSetMainPanel(screen1_obj, main_panel);
    //ui_ScreenObjectLoadScreen(screen1_obj);
    //ui_MainScreen_screen_init();
    //ui_RoomsScreen_screen_init();
    //ui____initial_actions0 = lv_obj_create(NULL);
    

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
    //vTaskDelay(3000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "RAM left after delete %lu", esp_get_free_heap_size());
    ESP_ERROR_CHECK(nvs_flash_init());
   
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };
   
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    //esp_zb_stack_main_loop();
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 10, NULL);
    memset(LocalIeeeAdr,0,8);
    esp_err_t err = zb_manager_init();
    if (err == ESP_OK) ESP_LOGI(TAG, "Zigbee stack zb_manager_init"); else
    {
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        err = zb_manager_init();
    }
    //vTaskDelay(3000 / portTICK_PERIOD_MS);
    err = zb_manager_start();
    if (err == ESP_OK) 
    {
        ESP_LOGI(TAG, "Zigbee stack zb_manager_init");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        //ESP_LOGI(TAG, "Zigbee test func zb_manager_open_network");
        esp_zb_get_long_address(LocalIeeeAdr);
        ESP_LOGI(TAG,  "LocalIeeeAdr: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X", LocalIeeeAdr[0], LocalIeeeAdr[1], LocalIeeeAdr[2],
                LocalIeeeAdr[3], LocalIeeeAdr[4], LocalIeeeAdr[5], LocalIeeeAdr[6], LocalIeeeAdr[7]);
        //zb_manager_open_network(15);
    }
    
}
