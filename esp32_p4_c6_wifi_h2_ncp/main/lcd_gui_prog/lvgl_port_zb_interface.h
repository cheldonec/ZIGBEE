#ifndef LVGL_PORT_ZB_I_H
#define LVGL_PORT_ZB_I_H
//#include "Kconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
//#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"

#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "sdkconfig.h"

#include "ha/esp_zigbee_ha_standard.h"

#include "esp_timer.h"
#include "driver/ledc.h"
#include "esp_lvgl_port.h"
#include "zb_manager_devices.h"
#include "zb_gui_components/ui_ScreenComponent.h"
#include "zb_gui_components/ui_PanelComponent.h"

//void start_lvgl_interface(void);
void set_display_rotaition(lv_display_t * disp, lv_display_rotation_t rotation);
void print_log_to_screen(char* text, uint32_t color);

void scroll_log_container_y(lv_coord_t y);

/************************************************* tests end *********************************************/
void lvgl_add_button(char* text);

/************************************************Square_Proj ******************************************* */
void lvgl_main_prog_start(void); // declared in ui.h
//void zb_gui_init(void); // declared in ui.h




/***************************************************** DRAW SENSORS **************************************************/
// ui_MainScreenSensorsContainer  контейнер на главном экране
// TEMPERATURE_SENSOR
typedef struct ui_temperature_sensor_for_home_screen_s {
        lv_obj_t* temperature_sensor_widget;
        lv_obj_t* temperature_sensor_name_label;
        lv_obj_t* temperature_sensor_room_label;
        lv_obj_t* temperature_sensor_value_label;
        lv_obj_t* SensorWidgetImage;
        lv_obj_t* SensorWidgetSetupButton;
        lv_obj_t* SensorWidgetSetupButtonImage;
} ui_temperature_sensor_minimal_widget_t;

ui_temperature_sensor_minimal_widget_t* draw_temperature_sensor_minimal_widget(lv_obj_t* sensor_parent_panel);
esp_err_t temperature_sensor_minimal_widget_destroy_ui(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor);
esp_err_t temperature_sensor_minimal_widget_set_name_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text);
esp_err_t temperature_sensor_minimal_widget_set_room_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text);
esp_err_t temperature_sensor_minimal_widget_set_value_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text);

typedef struct zb_manager_temperature_sensor_ui_s{
    zb_manager_temperature_sensor_ep_t*         temperature_device_ep;
    ui_temperature_sensor_minimal_widget_t*     temperature_sensor_minimal_widget_str;
    //ui_temperature_sensor_t*                    temperature_device_ep_ui_on_dev_add_screen;
    bool                                        draw_on_home_screen;
    bool                                        draw_on_device_add_screen;
    char*                                        device_name;
    char*                                        room_name;
}zb_manager_temperature_sensor_ui_t;

zb_manager_temperature_sensor_ui_t* create_new_temperature_sensor_ui(void);
esp_err_t draw_temperature_sensor_minimal(zb_manager_temperature_sensor_ui_t* sensor_structure, lv_obj_t* parent_obj);

#define TEMPERATURE_SENSORS_LIST_SIZE 20
//extern zb_manager_temperature_sensor_ui_t* temperature_sensors_list[TEMPERATURE_SENSORS_LIST_SIZE];
// TEMPERATURE_SENSOR END

// HUMIDITY_SENSOR
typedef struct ui_humidity_sensor_s {
        lv_obj_t* humidity_sensor_widget;
        lv_obj_t* humidity_sensor_name_label;
        lv_obj_t* humidity_sensor_room_label;
        lv_obj_t* humidity_sensor_value_label;
        lv_obj_t* SensorWidgetImage;
        lv_obj_t* SensorWidgetSetupButton;
        lv_obj_t* SensorWidgetSetupButtonImage;
} ui_humidity_sensor_t;

ui_humidity_sensor_t* draw_humidity_sensor(lv_obj_t* sensor_parent_panel);
esp_err_t humidity_sensor_destroy_ui(ui_humidity_sensor_t* ui_humidity_sensor);
esp_err_t humidity_sensor_set_name_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text);
esp_err_t humidity_sensor_set_room_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text);
esp_err_t humidity_sensor_set_value_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text);
// HUMIDITY_SENSOR END

// ON/OFF LIGHT DEVICE  (light bulb, rele)
typedef enum {
        ON_OFF_LIGHT_DEVICE_BULB,
        ON_OFF_LIGHT_DEVICE_RELE,
}on_off_light_device_variant_t;

typedef struct ui_on_off_light_device_s {
        lv_obj_t* on_off_light_device_widget;
        lv_obj_t* on_off_light_device_name_label;
        lv_obj_t* on_off_light_device_room_label;
        lv_obj_t* on_off_light_device_value_label;
        lv_obj_t* SensorWidgetImage;
        lv_obj_t* SensorWidgetSetupButton;
        lv_obj_t* SensorWidgetSetupButtonImage;
        lv_obj_t* SensorWidgetONButton;
        lv_obj_t* SensorWidgetONButtonImage;
        lv_obj_t* SensorWidgetOFFButton;
        lv_obj_t* SensorWidgetOFFButtonImage;
        uint8_t on_off_state;
        on_off_light_device_variant_t device_variant;
        esp_zb_64bit_addr_t device_addr;
}ui_on_off_light_device_t;

ui_on_off_light_device_t* draw_on_off_light_device(lv_obj_t* sensor_parent_panel, on_off_light_device_variant_t device_variant);
esp_err_t on_off_light_device_destroy_ui(ui_on_off_light_device_t* ui_on_off_light_device);
esp_err_t on_off_light_device_set_name_text(ui_on_off_light_device_t* ui_on_off_light_device, char* text);
esp_err_t on_off_light_device_set_room_text(ui_on_off_light_device_t* ui_on_off_light_device, char* text);
esp_err_t on_off_light_device_set_device_variant(ui_on_off_light_device_t* ui_on_off_light_device, on_off_light_device_variant_t device_variant);
esp_err_t on_off_light_device_set_value_on_off(ui_on_off_light_device_t* ui_on_off_light_device, uint8_t on_off_state); // use when zb device report state

void ui_event_on_off_light_device_btn_click(lv_event_t * e);
// ON/OFF LIGHT DEVICE  (light bulb, rele) end

/***************************************************** DRAW JOINED DEVICES **************************************************/


#endif