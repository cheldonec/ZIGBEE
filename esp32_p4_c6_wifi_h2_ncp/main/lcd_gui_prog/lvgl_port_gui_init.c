#include "lvgl_port_gui_init.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_memory_utils.h"

#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_ldo_regulator.h"
#include "esp_lcd_st7701.h"
#include "esp_lcd_touch_gt911.h"

static const char *TAG = "esp_draw_bit";

esp_lcd_panel_handle_t disp_panel = NULL;

static esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL;

esp_lcd_panel_io_handle_t lcd_panel_io = NULL;

static i2c_master_bus_handle_t i2c_bus_handle = NULL;

static esp_lcd_panel_io_handle_t tp_io_handle = NULL;

static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

static esp_err_t bsp_enable_dsi_phy_power(void)
{
#if MIPI_DSI_PHY_PWR_LDO_CHAN > 0
    // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
    static esp_ldo_channel_handle_t phy_pwr_chan = NULL;
    esp_ldo_channel_config_t ldo_cfg = {
        .chan_id = MIPI_DSI_PHY_PWR_LDO_CHAN,
        .voltage_mv = MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
    };
    ESP_RETURN_ON_ERROR(esp_ldo_acquire_channel(&ldo_cfg, &phy_pwr_chan), TAG, "Acquire LDO channel for DPHY failed");
    ESP_LOGI(TAG, "MIPI DSI PHY Powered on");
#endif // BSP_MIPI_DSI_PHY_PWR_LDO_CHAN > 0

    return ESP_OK;
}

static esp_err_t bsp_brightness_init(void)
{
    gpio_set_direction(LCD_BACKLIGHT, GPIO_MODE_OUTPUT);
    gpio_set_level(LCD_BACKLIGHT, 1);
    // Setup LEDC peripheral for PWM backlight control
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = LCD_BACKLIGHT,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LCD_LEDC_CH,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = 1,
        .duty = 0,
        .hpoint = 0
    };
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = 1,
        .freq_hz = 20000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&LCD_backlight_timer));
    ESP_ERROR_CHECK(ledc_channel_config(&LCD_backlight_channel));
    return ESP_OK;

}

static esp_err_t bsp_brightness_set(int brightness_percent)
{
    if (brightness_percent > 100) {
        brightness_percent = 100;
    }
    if (brightness_percent < 0) {
        brightness_percent = 0;
    }

    ESP_LOGI(TAG, "Setting LCD backlight: %d%%", brightness_percent);
    uint32_t duty_cycle = (1023 * brightness_percent) / 100; // LEDC resolution set to 10bits, thus: 100% = 1023
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, duty_cycle));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));
    return ESP_OK;
}

esp_err_t lvgl_port_gui_init(void)
{
    esp_err_t ret = ESP_FAIL;
    /*ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_BACKLIGHT
    };
    gpio_config(&bk_gpio_config);
    gpio_set_level(LCD_BACKLIGHT, 1);*/

   //init lvgl
   ESP_LOGI(TAG, "Init LVGL");
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 6,         /* LVGL task priority */
        .task_stack = 65536,         /* LVGL task stack size */
        #if CONFIG_IDF_TARGET_ESP32P4
        .task_affinity = 1,        /* LVGL task pinned to core (-1 is no affinity) */
        .task_stack_caps = MALLOC_CAP_SPIRAM,
        #else
        .task_affinity = -1,        /* LVGL task pinned to core (-1 is no affinity) */
        #endif
        .task_max_sleep_ms = 100,   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5        /* LVGL timer tick period in ms */
    };
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    ESP_LOGI(TAG, "Init LCD touch");
    i2c_master_bus_config_t i2c_bus_conf = {
         .i2c_port = I2C_NUM_0,
         .sda_io_num = I2C_SDA_PIN,
         .scl_io_num = I2C_SCL_PIN,
         .clk_source = I2C_CLK_SRC_DEFAULT,
         .glitch_ignore_cnt = 7,
         .intr_priority = 0,
         .trans_queue_depth = 0,
         .flags = {
             .enable_internal_pullup = 1,
         },
     };
     i2c_new_master_bus(&i2c_bus_conf, &i2c_bus_handle);

     const esp_lcd_touch_config_t tp_cfg = {
         .x_max = LCD_H_RES,
         .y_max = LCD_V_RES,
         .rst_gpio_num = LCD_TOUCH_RST, // Shared with LCD reset
         .int_gpio_num = LCD_TOUCH_INT,
         .levels = {
             .reset = 0,
             .interrupt = 0,
         },
         .flags = {
             .swap_xy = 0,
             .mirror_x = 0,
             .mirror_y = 0,
         },
     };

     esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();
     tp_io_config.scl_speed_hz = I2C_CLK_SPEED_HZ;
     esp_lcd_new_panel_io_i2c(i2c_bus_handle, &tp_io_config, &tp_io_handle);
     esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);

    ESP_LOGI(TAG, "TURN ON LCD BACKLIGHT");
    bsp_brightness_init();
    bsp_enable_dsi_phy_power();
    bsp_brightness_set(100);

    esp_lcd_dsi_bus_config_t bus_config = {
        .bus_id = 0,
        .num_data_lanes = LCD_MIPI_DSI_LANE_NUM,
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT,
        .lane_bit_rate_mbps = LCD_MIPI_DSI_LANE_BITRATE_MBPS,
    };
    esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus);

    ESP_LOGI(TAG, "Install MIPI DSI LCD control panel");
    esp_lcd_dbi_io_config_t dbi_config = {
        .virtual_channel = 0,
        .lcd_cmd_bits = 8,   // according to the LCD ILI9881C spec
        .lcd_param_bits = 8, // according to the LCD ILI9881C spec
    };
    esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &lcd_panel_io);

    esp_lcd_dpi_panel_config_t dpi_config = LCD_480_800_PANEL_60HZ_DPI_CONFIG(LCD_COLOR_PIXEL_FORMAT_RGB565);

    //dpi_config.num_fbs = CONFIG_BSP_LCD_DPI_BUFFER_NUMS;

    st7701_vendor_config_t vendor_config = {
        .mipi_config = {
            .dsi_bus = mipi_dsi_bus,
            .dpi_config = &dpi_config,
        },
        .flags = {
            .use_mipi_interface = 1,
        }
    };
    esp_lcd_panel_dev_config_t lcd_dev_config = {
        .bits_per_pixel = 16,
        .rgb_ele_order = ESP_LCD_COLOR_SPACE_RGB,
        .reset_gpio_num = LCD_RST,
        .vendor_config = &vendor_config,
    };
    esp_lcd_new_panel_st7701(lcd_panel_io, &lcd_dev_config, &disp_panel);
    
    esp_lcd_panel_reset(disp_panel);
    esp_lcd_panel_init(disp_panel);
    esp_lcd_panel_disp_on_off(disp_panel, true);

    ESP_LOGI(TAG, "Install MIPI DSI LCD TO LVGL");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_panel_io,
        .panel_handle = disp_panel,
        .control_handle = NULL,
        .buffer_size = 480 * 800 * 2,
        .double_buffer = LCD_DRAW_BUFF_DOUBLE,
        //.double_buffer = 1,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = false,
        .trans_size = 64,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = true,
            .mirror_x = false,
            .mirror_y = false,
        },
        #if LVGL_VERSION_MAJOR >= 9
        #if CONFIG_BSP_LCD_COLOR_FORMAT_RGB888
                .color_format = LV_COLOR_FORMAT_RGB888,
        #else
                .color_format = LV_COLOR_FORMAT_RGB565,
        #endif
        #endif
                .flags = {
                    .buff_dma = false,
                    .buff_spiram = true, // isused psram
                    //.buff_spiram = false, 
        #if LVGL_VERSION_MAJOR >= 9
                    .swap_bytes = (LCD_BIGENDIAN ? true : false),
        #endif
        #if CONFIG_BSP_DISPLAY_LVGL_AVOID_TEAR
                    .sw_rotate = false,                /* Avoid tearing is not supported for SW rotation */
        #else
                    .sw_rotate = true, /* Only SW rotation is supported for 90° and 270° */
        #endif
        //#if CONFIG_BSP_DISPLAY_LVGL_FULL_REFRESH
        //            .full_refresh = true,
        //#elif CONFIG_BSP_DISPLAY_LVGL_DIRECT_MODE
        //            .direct_mode = true,
        //#endif
        }
    };
    const lvgl_port_display_dsi_cfg_t dpi_cfg = {
        .flags = {
        #if CONFIG_BSP_DISPLAY_LVGL_AVOID_TEAR
            .avoid_tearing = true,
        #else
            .avoid_tearing = false,
        #endif
        }
    };
    lvgl_disp = lvgl_port_add_disp_dsi(&disp_cfg, &dpi_cfg);

    ESP_LOGI(TAG, "Install TOUCH TO LVGL");
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);
    if (lvgl_disp) {ret = ESP_OK;}
    return ret;
}