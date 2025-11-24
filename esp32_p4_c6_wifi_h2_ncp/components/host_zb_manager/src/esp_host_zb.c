/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_system.h"
#include "esp_random.h"

#include "esp_host_main.h"
#include "esp_host_zb.h"

#include "zb_config_platform.h"
#include "esp_zigbee_core.h"
#include "esp_zigbee_zcl_command.h"
#include "event_post_send.h"

static const char *TAG = "esp_host_zb.c";
typedef struct {
    esp_zb_ieee_addr_t  extendedPanId;                      /*!< The network's extended PAN identifier */
    uint16_t            panId;                              /*!< The network's PAN identifier */
    uint8_t             radioChannel;                       /*!< A radio channel */
} esp_host_zb_network_t;

/**
 * @brief Type to represent the sync event between the host and BUS.
 *
 */
typedef struct {
    uint16_t        id;                                     /*!< The frame ID */
    uint16_t        size;                                   /*!< Data size on the event */
    void            *data;                                  /*!< Data on the event */
} esp_host_zb_ctx_t;

static esp_host_zb_network_t        s_host_zb_network;
static QueueHandle_t                output_queue;           /*!< The queue handler for wait response */
static QueueHandle_t                notify_queue;           /*!< The queue handler for wait notification */
static SemaphoreHandle_t            lock_semaphore;

static esp_err_t esp_host_zb_form_network_fn(const uint8_t *input, uint16_t inlen)
{
    typedef struct {
        esp_zb_ieee_addr_t  extendedPanId;                  /*!< The network's extended PAN identifier */
        uint16_t            panId;                          /*!< The network's PAN identifier */
        uint8_t             radioChannel;                   /*!< A radio channel */
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_form_network_t;

    esp_zb_form_network_t *form_network = (esp_zb_form_network_t *)input;
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_BDB_SIGNAL_FORMATION,
        .msg = NULL,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    memcpy(s_host_zb_network.extendedPanId, form_network->extendedPanId, sizeof(esp_zb_ieee_addr_t));
    s_host_zb_network.panId = form_network->panId;
    s_host_zb_network.radioChannel = form_network->radioChannel;

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t esp_host_zb_joining_network_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}


static esp_err_t esp_host_zb_permit_joining_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t esp_host_zb_leave_network_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_ZDO_SIGNAL_LEAVE,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t esp_host_zb_set_bind_fn(const uint8_t *input, uint16_t inlen)
{
    typedef struct {
        esp_zb_zdp_status_t    zdo_status;
        esp_zb_user_cb_t       bind_usr;                   /*!< A ZDO bind desc request callback */
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_bind_desc_t;

    esp_zb_zdo_bind_desc_t *zdo_bind_desc = (esp_zb_zdo_bind_desc_t *)input;

    // 1. Вызываем user_cb
    if (zdo_bind_desc->bind_usr.user_cb) {
        esp_zb_zdo_bind_callback_t zdo_bind_desc_callback = (esp_zb_zdo_bind_callback_t)zdo_bind_desc->bind_usr.user_cb;
        zdo_bind_desc_callback(zdo_bind_desc->zdo_status, (void *)zdo_bind_desc->bind_usr.user_ctx);
    }

    // 2. Формируем структуру для eventLoopPost
    zb_manager_bind_resp_message_t* resp_msg = calloc(1, sizeof(zb_manager_bind_resp_message_t));
    if (!resp_msg) {
        ESP_LOGE(TAG, "Failed to allocate resp_msg for BIND_RESP");
        return ESP_ERR_NO_MEM;
    }

    resp_msg->status = zdo_bind_desc->zdo_status;
    resp_msg->user_ctx = (void*)zdo_bind_desc->bind_usr.user_ctx;

    // 3. Отправляем в event loop
    bool post_ok = eventLoopPost(ZB_HANDLER_EVENTS, BIND_RESP, resp_msg, sizeof(zb_manager_bind_resp_message_t), portMAX_DELAY);
    if (!post_ok) {
        ESP_LOGE(TAG, "Failed to post BIND_RESP event");
        zb_manager_free_bind_resp(resp_msg); // ничего не очищает пока!!!!!!!!! добавлена для стильности
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "BIND_RESP posted: status=0x%02x", resp_msg->status);
    return ESP_OK;

    return ESP_OK;
}

static esp_err_t esp_host_zb_set_unbind_fn(const uint8_t *input, uint16_t inlen)
{
    typedef struct {
        esp_zb_zdp_status_t    zdo_status;
        esp_zb_user_cb_t       bind_usr;                   /*!< A ZDO bind desc request callback */
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_unbind_desc_t;

    esp_zb_zdo_unbind_desc_t *zdo_bind_desc = (esp_zb_zdo_unbind_desc_t *)input;
    if (zdo_bind_desc->bind_usr.user_cb) {
        esp_zb_zdo_bind_callback_t zdo_bind_desc_callback = (esp_zb_zdo_bind_callback_t)zdo_bind_desc->bind_usr.user_cb;
        zdo_bind_desc_callback(zdo_bind_desc->zdo_status, (void *)zdo_bind_desc->bind_usr.user_ctx);
    }

    return ESP_OK;
}

static esp_err_t esp_host_zb_find_match_fn(const uint8_t *input, uint16_t inlen)
{
    typedef struct {
        esp_zb_zdp_status_t zdo_status;
        uint16_t            addr;
        uint8_t             endpoint;
        esp_zb_user_cb_t    find_usr;
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_match_desc_t;

    esp_zb_zdo_match_desc_t *zdo_match_desc = (esp_zb_zdo_match_desc_t *)input;

    if (zdo_match_desc->find_usr.user_cb) {
        esp_zb_zdo_match_desc_callback_t zdo_match_desc_callback = (esp_zb_zdo_match_desc_callback_t)zdo_match_desc->find_usr.user_cb;
        zdo_match_desc_callback(zdo_match_desc->zdo_status, zdo_match_desc->addr, zdo_match_desc->endpoint, (void *)zdo_match_desc->find_usr.user_ctx);
    }

    return ESP_OK;
}

static esp_err_t zb_manager_report_attr_event_fn(const uint8_t *input, uint16_t inlen)
{
    /*typedef struct {
        esp_zb_zdp_status_t zdo_status;
        uint16_t            addr;
        uint8_t             endpoint;
        esp_zb_user_cb_t    find_usr;
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_match_desc_t;

    esp_zb_zdo_match_desc_t *zdo_match_desc = (esp_zb_zdo_match_desc_t *)input;

    if (zdo_match_desc->find_usr.user_cb) {
        esp_zb_zdo_match_desc_callback_t zdo_match_desc_callback = (esp_zb_zdo_match_desc_callback_t)zdo_match_desc->find_usr.user_cb;
        zdo_match_desc_callback(zdo_match_desc->zdo_status, zdo_match_desc->addr, zdo_match_desc->endpoint, (void *)zdo_match_desc->find_usr.user_ctx);
    }*/
    ESP_LOGI(TAG, "zb_manager_report_attr_event_fn");
    eventLoopPost(ZB_HANDLER_EVENTS, ATTR_REPORT_EVENT, NULL, 0, portMAX_DELAY);
    //MY_EVENT_WIFI_CONNECTED
    return ESP_OK;
}


/*static esp_err_t zb_manager_read_attr_resp_fn(const uint8_t *input, uint16_t inlen)
{
   zb_manager_cmd_read_attr_resp_message_t* resp_msg = NULL;
   resp_msg = calloc(1,inlen);
   memcpy(resp_msg, input, sizeof(esp_zb_zcl_cmd_info_t));
   uint8_t attr_count = *(uint8_t*)(input + sizeof(esp_zb_zcl_cmd_info_t));
   resp_msg->attr_count = attr_count;
   ESP_LOGW(TAG, "zb_manager_read_attr_resp_fn attr_count %d", attr_count);

   uint8_t* pointer = (uint8_t*)(input + sizeof(esp_zb_zcl_cmd_info_t) + sizeof(uint8_t));
   zb_manager_cmd_read_attr_t* attr_arr = NULL;
   attr_arr = calloc(1, attr_count * sizeof(zb_manager_cmd_read_attr_t));

   for (uint8_t i = 0; i < attr_count; i++)
   {
        attr_arr[i].attr_id  = *((uint16_t*)pointer);
        pointer = pointer + sizeof(uint16_t);

        attr_arr[i].attr_type = *((esp_zb_zcl_attr_type_t*)pointer);
        pointer = pointer + sizeof(esp_zb_zcl_attr_type_t);

        attr_arr[i].attr_len = *(uint8_t*)(pointer);
        pointer = pointer + sizeof(uint8_t);

        attr_arr[i].attr_value = calloc(1, attr_arr[i].attr_len);
        memcpy(attr_arr[i].attr_value, pointer, attr_arr[i].attr_len);
        pointer = pointer + attr_arr[i].attr_len;
        //ESP_LOGW(TAG, "zb_manager_read_attr_response_fn inlen %d short 0x%4x attr_index 0x%x attr_id 0x%x attr_type 0x%x attr_len %d", inlen, cmd_info->src_address.u.short_addr, i, attr_arr[i].attr_id, attr_arr[i].attr_type, attr_arr[i].attr_len);
   }
   resp_msg->attr_arr = attr_arr;
   eventLoopPost(ZB_HANDLER_EVENTS, ATTR_READ_RESP, resp_msg, inlen, portMAX_DELAY);
    return ESP_OK;
}*/



static esp_err_t zb_manager_dev_annce_event_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t zb_manager_dev_assoc_event_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_NWK_SIGNAL_DEVICE_ASSOCIATED,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t zb_manager_dev_update_event_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_ZDO_SIGNAL_DEVICE_UPDATE,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t zb_manager_dev_auth_event_fn(const uint8_t *input, uint16_t inlen)
{
    esp_zb_app_signal_msg_t signal_msg = {
        .signal = ESP_ZB_ZDO_SIGNAL_DEVICE_AUTHORIZED,
        .msg = (const char *)input,
    };

    esp_zb_app_signal_t app_signal = {
        .p_app_signal = (uint32_t *)&signal_msg,
        .esp_err_status = ESP_OK,
    };

    esp_zb_app_signal_handler(&app_signal);

    return ESP_OK;
}

static esp_err_t zb_manager_simple_desc_resp_fn(const uint8_t *input, uint16_t inlen)
{
    ESP_LOGW(TAG, "zb_manager_simple_desc_resp_fn");
    typedef struct {
        esp_zb_zdp_status_t zdo_status;
        esp_zb_user_cb_t    find_usr;
        esp_zb_af_simple_desc_1_1_t simple_desc; 
    } ESP_ZNSP_ZB_PACKED_STRUCT zb_manager_simple_desc_resp_pack_t;

    zb_manager_simple_desc_resp_pack_t *pkg  = (zb_manager_simple_desc_resp_pack_t *)input;
    // 1. Вызываем callback (user_cb) с user_ctx
    if (pkg ->find_usr.user_cb) {
        esp_zb_zdo_simple_desc_callback_t zdo_simple_desc_callback = (esp_zb_zdo_simple_desc_callback_t)pkg ->find_usr.user_cb;
        zdo_simple_desc_callback(pkg ->zdo_status, (esp_zb_af_simple_desc_1_1_t*)(&pkg ->simple_desc), (void *)pkg ->find_usr.user_ctx);
    }

     // 2. Формируем структуру для отправки в eventLoopPost
     //определяем размер esp_zb_af_simple_desc_1_1_t
    size_t simple_desc_size = sizeof(esp_zb_af_simple_desc_1_1_t) + 
        (pkg->simple_desc.app_input_cluster_count + pkg->simple_desc.app_output_cluster_count) * sizeof(uint16_t);
    // 3. Выделяем память под simple_desc
        esp_zb_af_simple_desc_1_1_t* desc_copy = NULL;
    desc_copy = calloc(1, simple_desc_size);
    if (!desc_copy) {
        ESP_LOGE(TAG, "Failed to allocate memory for simple_desc copy");
        return ESP_ERR_NO_MEM;
    }
    // 4. Копируем всю структуру (включая кластеры)
    memcpy(desc_copy, &pkg->simple_desc, simple_desc_size);

    // 5. Выделяем основную структуру для event loop
    zb_manager_simple_desc_resp_message_t* resp_msg = NULL;
    resp_msg = calloc(1, sizeof(zb_manager_simple_desc_resp_message_t));
    if (!resp_msg) {
        ESP_LOGE(TAG, "Failed to allocate resp_msg");
        free(desc_copy);
        return ESP_ERR_NO_MEM;
    }
    resp_msg->status = pkg->zdo_status;
    resp_msg->simple_desc = desc_copy;
    resp_msg->user_ctx = (void*)pkg->find_usr.user_ctx;

    // 6. Отправляем в event loop
    bool post_ok = eventLoopPost(ZB_HANDLER_EVENTS, SIMPLE_DESC_RESP, resp_msg, sizeof(zb_manager_simple_desc_resp_message_t), portMAX_DELAY);
    if (!post_ok) {
        ESP_LOGE(TAG, "Failed to post SIMPLE_DESC_RESP event");
        zb_manager_free_simple_desc_resp(resp_msg);
        free(resp_msg);
        resp_msg = NULL;
        return ESP_FAIL;
    }

    return ESP_OK;
}

/*static esp_err_t  zb_manager_active_ep_resp_fn(const uint8_t *input, uint16_t inlen)
{
    ESP_LOGI(TAG, "zb_manager_active_ep_resp_fn");
    //ESP_LOG_BUFFER_HEX_LEVEL(TAG, input, inlen, ESP_LOG_INFO);
    typedef struct {
        esp_zb_zdp_status_t zdo_status;
        uint8_t             ep_count;
        esp_zb_user_cb_t    find_usr;
        //uint8_t*            ep_id_list; 
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_active_ep_t;
   
    uint16_t outlen = inlen;// - sizeof(esp_zb_zdo_active_ep_t); // add ep_id_list
    uint8_t *output = calloc(1, outlen);
    if (output){
        memcpy(output, input, outlen);
        esp_zb_zdo_active_ep_t *zdo_active_ep = (esp_zb_zdo_active_ep_t *)output;
        //ESP_LOGI(TAG, "status: (%d), ep_count: (%d)", zdo_active_ep->zdo_status, zdo_active_ep->ep_count);
        if(zdo_active_ep->find_usr.user_cb){
            esp_zb_zdo_active_ep_callback_t active_ep_cb = (esp_zb_zdo_active_ep_callback_t)zdo_active_ep->find_usr.user_cb;
            active_ep_cb(zdo_active_ep->zdo_status, zdo_active_ep->ep_count, output + sizeof(esp_zb_zdo_active_ep_t), (void*)zdo_active_ep->find_usr.user_ctx);
        }
    free(output);
    output = NULL;
    }
    return ESP_OK;
}*/

/**
 * @brief Обработчик Active EP Response
 * 
 * Парсит входящий ZDO-ответ на запрос
 * esp_err_t zb_manager_zdo_active_ep_req(esp_zb_zdo_active_ep_req_param_t *cmd_req, esp_zb_zdo_active_ep_callback_t user_cb, void *user_ctx);
 * который подразумевает вызов CB
 * при обработке сперва вызывается CB + void *user_ctx
 * потом ответ отправляется в event loop zb_manager_active_ep_resp_message_t + void *user_ctx
 */
static esp_err_t zb_manager_active_ep_resp_fn(const uint8_t *input, uint16_t inlen)
{
    ESP_LOGI(TAG, "zb_manager_active_ep_resp_fn: inlen=%d", inlen);
    
    // структура входящих данных
    typedef struct {
        esp_zb_zdp_status_t zdo_status;
        uint8_t             ep_count;
        esp_zb_user_cb_t    find_usr;
        //uint8_t*            ep_id_list; 
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_zb_zdo_active_ep_t;

    // 1. готовим вызов CB
    uint16_t outlen = inlen;// - sizeof(esp_zb_zdo_active_ep_t); // add ep_id_list
    uint8_t *output_for_cb = calloc(1, outlen);
    if (output_for_cb){
        memcpy(output_for_cb, input, outlen);
        esp_zb_zdo_active_ep_t *zdo_active_ep = (esp_zb_zdo_active_ep_t *)output_for_cb;
        //вызываем CB
        if(zdo_active_ep->find_usr.user_cb){
            esp_zb_zdo_active_ep_callback_t active_ep_cb = (esp_zb_zdo_active_ep_callback_t)zdo_active_ep->find_usr.user_cb;
            active_ep_cb(zdo_active_ep->zdo_status, zdo_active_ep->ep_count, output_for_cb + sizeof(esp_zb_zdo_active_ep_t), (void*)zdo_active_ep->find_usr.user_ctx);
        }
    free(output_for_cb);
    output_for_cb = NULL;
    }
    
    //2. готовим ответ в event loop

    // 1. Выделяем основную структуру
    zb_manager_active_ep_resp_message_t *resp_msg = calloc(1, sizeof(zb_manager_active_ep_resp_message_t));
    if (!resp_msg) {
        ESP_LOGE(TAG, "Failed to allocate resp_msg");
        return ESP_ERR_NO_MEM;
    }
    esp_zb_zdo_active_ep_t *zdo_active_ep = (esp_zb_zdo_active_ep_t *)input;
    resp_msg->status = *(esp_zb_zdp_status_t*)zdo_active_ep;
    resp_msg->ep_count = zdo_active_ep->ep_count;
    resp_msg->user_ctx = (void*)zdo_active_ep->find_usr.user_ctx;

    const uint8_t *ptr = input + sizeof(esp_zb_zdo_active_ep_t);
    
    // 3. Выделяем массив endpoint'ов
    resp_msg->ep_list = calloc(resp_msg->ep_count, sizeof(uint8_t));
    if (!resp_msg->ep_list) {
        ESP_LOGE(TAG, "Failed to allocate ep_list");
        free(resp_msg);
        resp_msg = NULL;
        return ESP_ERR_NO_MEM;
    }

    // 4. Копируем endpoint'ы
    memcpy(resp_msg->ep_list, input + sizeof(esp_zb_zdo_active_ep_t), resp_msg->ep_count);
    ptr += resp_msg->ep_count;
    
    // 5. Логируем (опционально)
    ESP_LOGI(TAG, "Active EP Response: status=0x%02x, count=%d,", resp_msg->status, resp_msg->ep_count);
    //ESP_LOG_BUFFER_HEX_LEVEL(TAG, resp_msg->ep_list, resp_msg->ep_count, ESP_LOG_INFO);

    // 6. ✅ Отправляем в event loop
    bool post_ok = eventLoopPost(ZB_HANDLER_EVENTS, ACTIVE_EP_RESP, resp_msg, sizeof(zb_manager_active_ep_resp_message_t), portMAX_DELAY);
    if (!post_ok) {
        ESP_LOGE(TAG, "Failed to post ACTIVE_EP_RESP_EVENT");
        // Освобождаем при ошибке
        zb_manager_free_active_ep_resp_ep_array(resp_msg);
        free(resp_msg);
        resp_msg = NULL;
        return ESP_FAIL;
    }

    return ESP_OK;
}

static esp_err_t zb_manager_read_attr_resp_fn(const uint8_t *input, uint16_t inlen)
{
    ESP_LOGI(TAG, "zb_manager_read_attr_resp_fn: inlen=%d", inlen);

    // 1. Выделяем основную структуру
    zb_manager_cmd_read_attr_resp_message_t* resp_msg = NULL;
    resp_msg = calloc(1, sizeof(zb_manager_cmd_read_attr_resp_message_t));
    if (!resp_msg) {
        ESP_LOGE(TAG, "Failed to allocate resp_msg");
        return ESP_ERR_NO_MEM;
    }

    // 2. Копируем общую информацию (esp_zb_zcl_cmd_info_t)
    esp_zb_zcl_cmd_info_t* cmd_info = (esp_zb_zcl_cmd_info_t*)input;
    memcpy(&resp_msg->info, cmd_info, sizeof(esp_zb_zcl_cmd_info_t));

    // 3. Читаем attr_count
    uint8_t attr_count = *(uint8_t*)(input + sizeof(esp_zb_zcl_cmd_info_t));
    resp_msg->attr_count = attr_count;
    ESP_LOGW(TAG, "Attribute count: %d", attr_count);

    // 4. Выделяем массив атрибутов
    resp_msg->attr_arr = calloc(attr_count, sizeof(zb_manager_cmd_read_attr_t));
    if (!resp_msg->attr_arr) {
        ESP_LOGE(TAG, "Failed to allocate attr_arr");
        free(resp_msg);
        return ESP_ERR_NO_MEM;
    }

    // 5. Парсим каждый атрибут
    uint8_t* pointer = (uint8_t*)(input + sizeof(esp_zb_zcl_cmd_info_t) + sizeof(uint8_t));

    for (uint8_t i = 0; i < attr_count; i++) {
        zb_manager_cmd_read_attr_t* attr = &resp_msg->attr_arr[i];

        attr->attr_id  = *((uint16_t*)pointer);
        pointer += sizeof(uint16_t);

        attr->attr_type = *((esp_zb_zcl_attr_type_t*)pointer);
        pointer += sizeof(esp_zb_zcl_attr_type_t);

        attr->attr_len = *(uint8_t*)pointer;
        pointer += sizeof(uint8_t);

        // 6. Выделяем память под значение
        if (attr->attr_len > 0) {
            attr->attr_value = calloc(1, attr->attr_len);
            if (!attr->attr_value) {
                ESP_LOGE(TAG, "Failed to allocate attr_value for attr_id=0x%04x", attr->attr_id);
                // Освобождаем всё, что уже выделено
                for (int j = 0; j < i; j++) {
                    free(resp_msg->attr_arr[j].attr_value);
                }
                free(resp_msg->attr_arr);
                free(resp_msg);
                return ESP_ERR_NO_MEM;
            }
            memcpy(attr->attr_value, pointer, attr->attr_len);
            pointer += attr->attr_len;
        } else {
            attr->attr_value = NULL;
        }

        ESP_LOG_BUFFER_HEX_LEVEL(TAG, attr->attr_value, attr->attr_len, ESP_LOG_DEBUG);
    }

    // 7. ✅ Передаём структуру в event loop
    bool post_err = eventLoopPost(ZB_HANDLER_EVENTS, ATTR_READ_RESP, resp_msg, inlen, portMAX_DELAY);
    if (post_err != true) {
        ESP_LOGE(TAG, "eventLoopPost failed: %s", esp_err_to_name(post_err));
        // Освобождаем при ошибке
        zb_manager_free_read_attr_resp_attr_array(resp_msg);
        free(resp_msg);
        resp_msg = NULL;
        return ESP_FAIL;
    }

    return ESP_OK;
}

static const esp_host_zb_func_t host_zb_func_table[] = {
    {ESP_NCP_NETWORK_FORMNETWORK, esp_host_zb_form_network_fn},
    {ESP_NCP_NETWORK_JOINNETWORK, esp_host_zb_joining_network_fn},
    {ESP_NCP_NETWORK_PERMIT_JOINING, esp_host_zb_permit_joining_fn},
    {ESP_NCP_NETWORK_LEAVENETWORK, esp_host_zb_leave_network_fn},
    {ESP_NCP_ZDO_BIND_SET, esp_host_zb_set_bind_fn},
    {ESP_NCP_ZDO_UNBIND_SET, esp_host_zb_set_unbind_fn},
    {ESP_NCP_ZDO_FIND_MATCH, esp_host_zb_find_match_fn},
    {ESP_NCP_ZCL_ATTR_REPORT_EVENT, zb_manager_report_attr_event_fn},
    {ESP_NCP_ZCL_ATTR_READ_RESP, zb_manager_read_attr_resp_fn},
    {ZB_MANAGER_DEV_ANNCE_EVENT, zb_manager_dev_annce_event_fn},
    {ZB_MANAGER_DEV_ASSOCIATED_EVENT, zb_manager_dev_assoc_event_fn},
    {ZB_MANAGER_DEV_UPDATE_EVENT, zb_manager_dev_update_event_fn},
    {ZB_MANAGER_DEV_AUTH_EVENT, zb_manager_dev_auth_event_fn},
    {ZB_MANAGER_ACTIVE_EP_RESP, zb_manager_active_ep_resp_fn},
    {ZB_MANAGER_SIMPLE_DESC_RESP, zb_manager_simple_desc_resp_fn},
};

esp_err_t esp_host_zb_input(esp_host_header_t *host_header, const void *buffer, uint16_t len)
{
    QueueHandle_t queue = (host_header->flags.type == ESP_ZNSP_TYPE_NOTIFY) ? notify_queue : output_queue;
    BaseType_t ret = 0;
    esp_host_zb_ctx_t host_ctx = {
        .id = host_header->id,
        .size = len,
    };

    if (buffer) {
        host_ctx.data = calloc(1, len);
        memcpy(host_ctx.data, buffer, len);
    }

    if (xPortInIsrContext() == pdTRUE) {
        ret = xQueueSendFromISR(queue, &host_ctx, NULL);
    } else {
        ret = xQueueSend(queue, &host_ctx, 0);
    }
    return (ret == pdTRUE) ? ESP_OK : ESP_FAIL ;
}

esp_err_t esp_host_zb_output(uint16_t id, const void *buffer, uint16_t len, void *output, uint16_t *outlen)
{
    esp_host_header_t data_header = {
        .id = id,
        .sn = esp_random() % 0xFF,
        .len = len,
        .flags = {
            .version = 0,
        }
    };
    data_header.flags.type = ESP_ZNSP_TYPE_REQUEST;

    xSemaphoreTakeRecursive(lock_semaphore, portMAX_DELAY);
    esp_host_frame_output(&data_header, buffer, len);

    esp_host_zb_ctx_t host_ctx;
    xQueueReceive(output_queue, &host_ctx, portMAX_DELAY);
    if (host_ctx.data) {
        if ((host_ctx.id == id)) {
            if (output) {
                memcpy(output, host_ctx.data, host_ctx.size);
            }

            if (outlen) {
                *outlen = host_ctx.size;
            }
        }

        free(host_ctx.data);
        host_ctx.data = NULL;
    }
    xSemaphoreGiveRecursive(lock_semaphore);

    return  ESP_OK;
}

void *esp_zb_app_signal_get_params(uint32_t *signal_p)
{
    esp_zb_app_signal_msg_t *app_signal_msg = (esp_zb_app_signal_msg_t *)signal_p;

    return app_signal_msg ? (void *)app_signal_msg->msg : (void *)app_signal_msg;
}

void esp_zb_stack_main_loop(void)
{
    esp_host_zb_ctx_t host_ctx;
    while (1) {
       if (xQueueReceive(notify_queue, &host_ctx, pdMS_TO_TICKS(100)) != pdTRUE) {
            continue;
       }

       for (int i = 0; i <= sizeof(host_zb_func_table) / sizeof(host_zb_func_table[0]); i ++) {
            if (host_ctx.id != host_zb_func_table[i].id) {
                continue;
            }

            host_zb_func_table[i].set_func(host_ctx.data, host_ctx.size);
            break;
        }

        if (host_ctx.data) {
            free(host_ctx.data);
            host_ctx.data = NULL;
        }
    }
}

void esp_zb_main_loop_iteration(void)
{
    esp_zb_stack_main_loop();
}

esp_err_t esp_zb_device_register(esp_zb_ep_list_t *ep_list)
{
    return ESP_OK;
}

esp_err_t esp_zb_platform_config(esp_zb_platform_config_t *config)
{
    ESP_ERROR_CHECK(esp_host_init(config->host_config.host_mode));
    ESP_ERROR_CHECK(esp_host_start());

    output_queue = xQueueCreate(HOST_EVENT_QUEUE_LEN, sizeof(esp_host_zb_ctx_t));
    notify_queue = xQueueCreate(HOST_EVENT_QUEUE_LEN, sizeof(esp_host_zb_ctx_t));
    lock_semaphore = xSemaphoreCreateRecursiveMutex();

    return ESP_OK;
}