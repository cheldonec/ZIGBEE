/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "esp_host_zb.h"

#include "esp_zigbee_zcl_command.h"
#include "esp_log.h"

static const char *TAG = "ESP_ZB_ZCL_COMMAND_C";

uint8_t esp_zb_zcl_custom_cluster_cmd_req(esp_zb_zcl_custom_cluster_cmd_t *cmd_req)
{
    typedef struct {
        esp_zb_zcl_basic_cmd_t zcl_basic_cmd;                   /*!< Basic command info */
        uint8_t  address_mode;                                  /*!< APS addressing mode constants refer to esp_zb_zcl_address_mode_t */
        uint16_t profile_id;                                    /*!< Profile id */
        uint16_t cluster_id;                                    /*!< Cluster id */
        uint16_t custom_cmd_id;                                 /*!< Custom command id */
        uint8_t  direction;                                     /*!< Direction of command */
        uint8_t  type;                                          /*!< The type of attribute, which can refer to esp_zb_zcl_attr_type_t */
        uint16_t size;                                          /*!< The value size of attribute  */
    } ESP_ZNSP_ZB_PACKED_STRUCT esp_host_zb_zcl_data_t;

    uint16_t data_len = sizeof(esp_host_zb_zcl_data_t);
    uint8_t *data = NULL;
    esp_host_zb_zcl_data_t zcl_data = {
        .zcl_basic_cmd = cmd_req->zcl_basic_cmd,
        .address_mode = cmd_req->address_mode,
        .profile_id = cmd_req->profile_id,
        .cluster_id = cmd_req->cluster_id,
        .custom_cmd_id = cmd_req->custom_cmd_id,
        .direction = cmd_req->direction,
        .type = cmd_req->data.type,
        .size = 0,
    };

    switch (cmd_req->data.type) {
        case ESP_ZB_ZCL_ATTR_TYPE_NULL:
            break;
        case ESP_ZB_ZCL_ATTR_TYPE_8BIT:
        case ESP_ZB_ZCL_ATTR_TYPE_BOOL:
        case ESP_ZB_ZCL_ATTR_TYPE_8BITMAP:
        case ESP_ZB_ZCL_ATTR_TYPE_U8:
            zcl_data.size = 1;
            break;
        default:
            break;
    }

    data = calloc(1, data_len + zcl_data.size);
    memcpy(data, &zcl_data, data_len);
    if (cmd_req->data.value && zcl_data.size) {
        memcpy(data + data_len, cmd_req->data.value, zcl_data.size);
        data_len += zcl_data.size;
    }

    uint8_t output = 0;
    uint16_t outlen = sizeof(uint8_t);

    esp_host_zb_output(ESP_NCP_ZCL_WRITE_CMD, data, data_len, &output, &outlen);
    if (data) {
        free(data);
        data = NULL;
    }

    //return ESP_OK;
    return output;
}

uint8_t zm_manager_zcl_read_attr_cmd_req(esp_zb_zcl_read_attr_cmd_t *cmd_req)
{
    ESP_LOGI(TAG, "Try to Read ATTR");
    uint8_t output = 0;
    uint16_t outlen = sizeof(uint8_t);
    typedef struct {
        esp_zb_zcl_basic_cmd_t  zcl_basic_cmd;      /*!< Basic command info */
        uint8_t                 address_mode;       /*!< APS addressing mode constants refer to esp_zb_zcl_address_mode_t */
        uint16_t                cluster_id;         /*!< Cluster ID to read */
        uint8_t                 attr_number;        /*!< Number of attribute in the attr_field */
    } __attribute__ ((packed)) esp_host_zb_read_attr_t;

    esp_host_zb_read_attr_t req ={
        .zcl_basic_cmd = {
            .dst_addr_u   = cmd_req->zcl_basic_cmd.dst_addr_u,
            .dst_endpoint = cmd_req->zcl_basic_cmd.dst_endpoint,
            .src_endpoint = cmd_req->zcl_basic_cmd.src_endpoint,
        },
        .address_mode = cmd_req->address_mode,
        .cluster_id   = cmd_req->clusterID,
        .attr_number  = cmd_req->attr_number,
    };

    
    uint16_t inlen = sizeof(esp_host_zb_read_attr_t) + cmd_req->attr_number * sizeof(uint16_t);
    uint8_t  *input = calloc(1, inlen);
    if (input) {
        memcpy(input, &req, sizeof(esp_host_zb_read_attr_t));
        memcpy(input + sizeof(esp_host_zb_read_attr_t), cmd_req->attr_field,cmd_req->attr_number * sizeof(uint16_t));
        esp_host_zb_output(ESP_NCP_ZCL_ATTR_READ_CMD, input, inlen, &output, &outlen);
        
        free(input);
        input = NULL;

    }
    //return ESP_OK;
    return output;
}
