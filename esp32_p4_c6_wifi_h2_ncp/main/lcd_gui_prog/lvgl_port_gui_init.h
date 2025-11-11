#ifndef LVGL_PORT_GUI_INIT_H
#define LVGL_PORT_GUI_INIT_H


#include "esp_lvgl_port.h"
#include "esp_err.h"



#define LCD_BACKLIGHT     (GPIO_NUM_23)
#define LCD_RST           (GPIO_NUM_5)
#define LCD_H_RES         (480)
#define LCD_V_RES         (800)

#define I2C_CLK_SPEED_HZ  (400000)
#define I2C_PORT_NUM      (I2C_NUM_1)
#define I2C_SDA_PIN       (GPIO_NUM_7)
#define I2C_SCL_PIN       (GPIO_NUM_8)
#define LCD_TOUCH_RST     (GPIO_NUM_NC)
#define LCD_TOUCH_INT     (GPIO_NUM_NC)

#define LCD_MIPI_DSI_LANE_NUM          (2)    // 2 data lanes
#define LCD_MIPI_DSI_LANE_BITRATE_MBPS (1000) // 1Gbps

#define LCD_DRAW_BUFF_DOUBLE   (1)
#define LCD_BIGENDIAN          (0)

#define MIPI_DSI_PHY_PWR_LDO_CHAN       (3)  // LDO_VO3 is connected to VDD_MIPI_DPHY
#define MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)

#define LCD_LEDC_CH            (1)

#define LCD_480_800_PANEL_60HZ_DPI_CONFIG(px_format)  \
    {                                                    \
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,     \
        .dpi_clock_freq_mhz = 28,                        \
        .virtual_channel = 0,                            \
        .pixel_format = px_format,                       \
        .num_fbs = 2,                                    \
        .video_timing = {                                \
            .h_size = 480,                               \
            .v_size = 800,                               \
            .hsync_back_porch = 42,                      \
            .hsync_pulse_width = 12,                     \
            .hsync_front_porch = 42,                     \
            .vsync_back_porch = 8,                      \
            .vsync_pulse_width = 2,                     \
            .vsync_front_porch = 166,                     \
        },                                               \
        .flags.use_dma2d = true,                         \
    }

esp_err_t lvgl_port_gui_init(void);

#endif