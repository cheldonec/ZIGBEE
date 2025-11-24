/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

//#pragma once
#ifndef ESP_ZIGBEE_ZDO_COMMAND_H
#define ESP_ZIGBEE_ZDO_COMMAND_H
//#endif
#include "esp_err.h"
//#ifdef __cplusplus
//extern "C" {
//#endif
#include "esp_zigbee_type.h"
#include "esp_zigbee_zdo_common.h"
//#include "zb_manager_devices.h"

/* MATCH DESC REQ ZCL configuration */
#define ESP_ZB_MATCH_DESC_REQ_TIMEOUT              (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for finding */
#define ESP_ZB_MATCH_DESC_REQ_ROLE                 ESP_ZB_NWK_BROADCAST_RX_ON_WHEN_IDLE    /* find non-sleep Zigbee device, 0xFFFD */

#define ESP_ZB_IEEE_ADDR_REQ_TIMEOUT                (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for ieee address */
#define ESP_ZB_NODE_DESC_REQ_TIMEOUT                (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for node descriptor */
#define ESP_ZB_BIND_DEVICE_REQ_TIMEOUT              (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for binding */
#define ESP_ZB_ACTIVE_EP_REQ_TIMEOUT                (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for active endpoint */
#define ESP_ZB_SIMPLE_DESC_REQ_TIMEOUT              (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for simple descriptor */
#define ESP_ZB_PERMIT_JOIN_REQ_TIMEOUT              (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for permit join */
#define ESP_ZB_DEVICE_LEAVE_REQ_TIMEOUT             (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for device leave */
#define ESP_ZB_DEVICE_BIND_TABLE_REQ_TIMEOUT        (5 * ESP_ZB_TIME_ONE_SECOND)            /* timeout for device bind table request */

/** Find device callback
 *
 * @brief A ZDO match desc request callback for user to get response info.
 *
 * @note User's callback get response from the remote device that local node wants to find a particular device on endpoint.
 *
 * @param[in] zdo_status  The ZDO response status, refer to `esp_zb_zdp_status`
 * @param[in] addr  A short address of the device response, 0xFFFF - invalid address
 * @param[in] endpoint An endpoint of the device response, 0xFF - invalid endpoint
 * @param[in] user_ctx  User information context, set in `esp_zb_zdo_find_xxx()`
 *
 */
typedef void (*esp_zb_zdo_match_desc_callback_t)(esp_zb_zdp_status_t zdo_status, uint16_t addr, uint8_t endpoint, void *user_ctx);

/** Bind request callback
 *
 * @brief A ZDO bind request callback for user to get response info.
 *
 * @note user's callback get response from the remote device that local node wants to bind.
 *
 * @param[in] zdo_status The ZDO response status, refer to `esp_zb_zdp_status`
 * @param[in] user_ctx  User information context, set in `esp_zb_zdo_device_bind_req()`
 *
 */
typedef void (*esp_zb_zdo_bind_callback_t)(esp_zb_zdp_status_t zdo_status, void *user_ctx);

/** Active endpoint callback
 *
 * @brief A ZDO active endpoint request callback for user to get response info.
 *
 * @note User's callback get response from the remote device that local node wants to get active endpoint.
 *
 * @param[in] zdo_status The ZDO response status, refer to `esp_zb_zdp_status`
 * @param[in] ep_count  A number of active endpoint
 * @param[in] ep_id_list A pointer of the endpoint id list
 * @param[in] user_ctx  User information context, set in `esp_zb_zdo_active_ep_req()`
 *
 */
typedef void (*esp_zb_zdo_active_ep_callback_t)(esp_zb_zdp_status_t zdo_status, uint8_t ep_count, uint8_t *ep_id_list, void *user_ctx);

/** Simple descriptor callback
 *
 * @brief A ZDO simple descriptor request callback for user to get simple desc info.
 *
 * @note User's callback get response from the remote device that local node wants to get simple desc.
 *
 * @param[in] zdo_status The ZDO response status, refer to `esp_zb_zdp_status`
 * @param[in] simple_desc  A pointer to the simple desc @ref esp_zb_af_simple_desc_1_1_s
 * @param[in] user_ctx  User information context, set in `esp_zb_zdo_simple_desc_req()`
 *
 */
typedef void (*esp_zb_zdo_simple_desc_callback_t)(esp_zb_zdp_status_t zdo_status, esp_zb_af_simple_desc_1_1_t *simple_desc, void *user_ctx);

/**
 * @brief The Zigbee ZDO bind command struct
 *
 * @note Current implementation of the API ONLY supports 64 bit extended address's address mode. Other address mode will support later.
 *
 * @note Be aware of the one req_dst_addr is address that command send to, while dst_address is the destination of the binding entry.
 *
 * @note NOW the dst_addr_mode is default by ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED. Later SDK will support other address mode.
 *
 */
typedef struct esp_zb_zdo_bind_req_param_s {
    esp_zb_ieee_addr_t src_address;                     /*!< The IEEE address for the source */
    uint8_t src_endp;                                   /*!< The source endpoint for the binding entry */
    uint16_t cluster_id;                                /*!< The identifier of the cluster on the source device that is bound to the destination */
    uint8_t dst_addr_mode;                              /*!< The destination address mode ref to esp_zb_zdo_bind_dst_addr_mode_t*/
    esp_zb_addr_u dst_address_u;                        /*!< The destination address for the binding entry */
    uint8_t dst_endp;                                   /*!< This field shall be present only if the
                                                         * DstAddrMode field has a value of
                                                         * refer to ZB_BIND_DST_ADDR_MODE_64_BIT_EXTENDED refer to bind_dst_addr_mode
                                                         * and, if present, shall be the
                                                         * destination endpoint for the binding entry. */
    uint16_t req_dst_addr;                              /*!< Destination address of the request send to */
} esp_zb_zdo_bind_req_param_t;

/**
 * @brief The Zigbee ZDO match descrpitor command struct
 */
typedef struct esp_zb_zdo_match_desc_req_param_s {
    uint16_t dst_nwk_addr;              /*!< NWK address that request sent to */
    uint16_t addr_of_interest;          /*!< NWK address of interest */
    uint16_t profile_id;                /*!< Profile ID to be match at the destination which refers to esp_zb_af_profile_id_t */
    uint8_t num_in_clusters;            /*!< The number of input clusters provided for matching cluster server */
    uint8_t num_out_clusters;           /*!< The number of output clusters provided for matching cluster client */
    uint16_t *cluster_list;             /*!< The pointer MUST point the uint16_t object with a size equal to num_in_clusters + num_out_clusters,
                                         * which will be used to match device. */
} esp_zb_zdo_match_desc_req_param_t;

/********************* Declare functions **************************/
/* ZDO command list, more ZDO command will be supported later like node_desc, power_desc */

/**
 * @brief   Send bind device request command
 *
 * @param[in] cmd_req  Pointer to the bind request command @ref esp_zb_zdo_bind_req_param_s
 * @param[in] user_cb  A user callback that will be called if received bind response refer to esp_zb_zdo_bind_callback_t
 * @param[in] user_ctx A void pointer that contains the user defines additional information when callback trigger
 *
 */
void esp_zb_zdo_device_bind_req(esp_zb_zdo_bind_req_param_t *cmd_req, esp_zb_zdo_bind_callback_t user_cb, void *user_ctx);

/**
 * @brief   Send unbind device request command
 *
 * @param[in] cmd_req  Pointer to the bind request command @ref esp_zb_zdo_bind_req_param_s
 * @param[in] user_cb  A user callback that will be called if received bind response refer to esp_zb_zdo_bind_callback_t
 * @param[in] user_ctx A void pointer that contains the user defines additional information when callback trigger
 *
 */
void esp_zb_zdo_device_unbind_req(esp_zb_zdo_bind_req_param_t *cmd_req, esp_zb_zdo_bind_callback_t user_cb, void *user_ctx);

/**
 * @brief   Send find on-off device request command
 *
 * @param[in] cmd_req  Pointer to the find request command @ref esp_zb_zdo_match_desc_req_param_s
 * @param[in] user_cb  A user callback that will be called if received find response refer to esp_zb_zdo_match_desc_callback_t
 * @param[in] user_ctx A void pointer that contains the user defines additional information when callback trigger
 *
 */
void esp_zb_zdo_find_on_off_light(esp_zb_zdo_match_desc_req_param_t *cmd_req, esp_zb_zdo_match_desc_callback_t user_cb, void *user_ctx);



/**
 * @brief Send match desc request to find matched Zigbee device
 *
 * @param[in] param    Pointer to @ref esp_zb_zdo_match_desc_req_param_s that will be used to match device
 * @param[in] user_cb  A user callback that will be called if received find response refer to esp_zb_zdo_match_desc_callback_t
 * @param[in] user_ctx A void pointer that contains the user defines additional information when callback trigger
 * @return
 *          - ESP_OK: Success in send match desc request
 *          - ESP_ERR_NO_MEM: Failed to allocate the memory for the request
 *          - ESP_ERR_INVALID_SIZE: The size of cluster list is wrong in @p param
 */
esp_err_t esp_zb_zdo_match_cluster(esp_zb_zdo_match_desc_req_param_t *param, esp_zb_zdo_match_desc_callback_t user_cb,
                                     void *user_ctx);

                                     /**
 * @brief The Zigbee ZDO active endpoint command struct
 *
 */
typedef struct esp_zb_zdo_active_ep_req_param_s {
    uint16_t   addr_of_interest;                        /*!< NWK address of interest */
} esp_zb_zdo_active_ep_req_param_t;

/**
 * @brief The Zigbee ZDO simple descriptor command struct
 *
 */
typedef struct esp_zb_zdo_simple_desc_req_param_s {
    uint16_t   addr_of_interest;                        /*!< NWK address of interest */
    uint8_t    endpoint;                                /*!< Endpoint of interest */
} esp_zb_zdo_simple_desc_req_param_t;

/**
 * @brief Stringify the Zigbee Device Object signal
 *
 * @param[in] signal A @ref esp_zb_app_signal_type_t object that expect to stringified
 * @return A string pointer of esp_zb_app_signal_type_t
 */
const char *esp_zb_zdo_signal_to_string(esp_zb_app_signal_type_t signal);


/***************************************************** ZB_MANAGER_FUNCTIONS *********************************/
/***************************************!!!!!!!!!!!!!! zb_manager_zdo_active_ep_req только заготовка */

/**
 * @brief Структура ответа на zb_manager_zdo_active_ep_req для передачи в event 
 */
typedef struct zb_manager_active_ep_resp_message_s {
        esp_zb_zdp_status_t status;
        uint8_t ep_count; 
        uint8_t *ep_list;    // Динамический массив endpoint'ов
        void*   user_ctx;
    } zb_manager_active_ep_resp_message_t;

/**
 * @brief Очищает массив ep внутри zb_manager_active_ep_resp_message_t после передачи в event, сам объект не удаляется, так как 
 * после обработки в event он будет удалён автоматически
 */
void zb_manager_free_active_ep_resp_ep_array(zb_manager_active_ep_resp_message_t *resp);

esp_err_t zb_manager_zdo_active_ep_req(esp_zb_zdo_active_ep_req_param_t *cmd_req, esp_zb_zdo_active_ep_callback_t user_cb, void *user_ctx);

/**
 * @brief Структура ответа на zb_manager_zdo_simple_desc_req для передачи в event 
 */
typedef struct {
    esp_zb_zdp_status_t status;
    esp_zb_af_simple_desc_1_1_t* simple_desc;
    void* user_ctx; // Сохраняем контекст, чтобы использовать в обработчике
} zb_manager_simple_desc_resp_message_t;

void zb_manager_free_simple_desc_resp(zb_manager_simple_desc_resp_message_t* resp);

/*
esp_zb_zdo_simple_desc_req_param_t req;
                req.addr_of_interest = 0x9eec; 
                req.endpoint = 0x01;
                zb_manager_zdo_simple_desc_req(&req,simple_desc_cb,NULL);
*/
esp_err_t zb_manager_zdo_simple_desc_req(esp_zb_zdo_simple_desc_req_param_t *cmd_req, esp_zb_zdo_simple_desc_callback_t user_cb, void *user_ctx);

/**
 * @brief Структура ответа на esp_zb_zdo_device_bind_req для передачи в event 
 */
typedef struct {
    esp_zb_zdp_status_t status;
    void* user_ctx;
} zb_manager_bind_resp_message_t;



void zb_manager_free_bind_resp(zb_manager_bind_resp_message_t* resp);
//#ifdef __cplusplus
//}
#endif
