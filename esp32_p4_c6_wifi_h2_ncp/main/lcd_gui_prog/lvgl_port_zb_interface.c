
#include "lvgl_port_zb_interface.h"
#include "ui.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
//#include "esp_ldo_regulator.h"
//#include "esp_lcd_panel_ops.h"
//#include "esp_lcd_mipi_dsi.h"
//#include "esp_lcd_dsi.h"
//#include "esp_lcd_touch_gt911.h"
//#include "ui.h"
static const char *TAG = "EXAMPLE";



void set_display_rotaition(lv_display_t * disp, lv_display_rotation_t rotation)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_disp_set_rotation(disp, rotation);
        lvgl_port_unlock();
    }
}

/************************************************** TEST ADD BUTTON FROM CB ************************************************/
void lvgl_add_button(char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_obj_t *scr = lv_scr_act();
        lv_obj_t *btn = lv_btn_create(scr);
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text_static(label, text);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 90);
        lvgl_port_unlock();
    }
}


//void zb_gui_init();
void ui_init(void);
void lvgl_main_prog_start(void)
{
    ESP_LOGI(TAG, "lvgl_main_prog_start");
    if (lvgl_port_lock(0) == true){
        ui_init();
        ESP_LOGI(TAG, "lvgl_main_prog_start ui_init");
      lvgl_port_unlock();
 
    }
}

static uint8_t selected = 0;
void print_log_to_screen(char* text, uint32_t color)
{
    /*if (lvgl_port_lock(0) == true)
    {
       uint32_t child_count = lv_obj_get_child_count(ui_Screen1_Panel2);
       if(child_count > 22)
       {
            lv_obj_t *obj_to_del = lv_obj_get_child(ui_Screen1_Panel2, 0);
            lv_obj_del(obj_to_del);
       }

    lv_obj_t* ui_Screen1_TextArea = lv_textarea_create(ui_Screen1_Panel2);
    lv_obj_set_width(ui_Screen1_TextArea, 308);
    lv_obj_set_height(ui_Screen1_TextArea, LV_SIZE_CONTENT);    /// 70
    lv_obj_set_align(ui_Screen1_TextArea, LV_ALIGN_CENTER);
    lv_textarea_set_text(ui_Screen1_TextArea, text);
    lv_textarea_set_placeholder_text(ui_Screen1_TextArea, "Placeholder...");
    lv_obj_set_style_text_color(ui_Screen1_TextArea, lv_color_hex(0x293D7A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Screen1_TextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Screen1_TextArea, &ui_font_RobotoCyrInfoLabels, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Screen1_TextArea, lv_color_hex(0xA7BFC1), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1_TextArea, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Screen1_TextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Screen1_TextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Screen1_TextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Screen1_TextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);

      
       lvgl_port_unlock();
    }*/
}

void scroll_log_container_y(lv_coord_t y)
{
    /*if (lvgl_port_lock(0) == true)
    {
       uint32_t child_count = lv_obj_get_child_count(ui_Screen1_Panel2);
       if(child_count > 0)
       {
            lv_obj_t *obj_to_scroll = lv_obj_get_child(ui_Screen1_Panel2, 1);
            lv_obj_scroll_to_y(obj_to_scroll, 1, LV_ANIM_ON);
       }
        //lv_obj_scroll_to_y(ui_Screen1_Panel2, 380, LV_ANIM_ON);
        //lv_obj_scroll_to_view(ui_Screen1_Containerlog, LV_ANIM_ON);
        lvgl_port_unlock();
    }*/
}

/***************************************************** DRAW SENSORS **************************************************/
// ui_MainScreenSensorsContainer  контейнер на главном экране
// TEMPERATURE_SENSOR
ui_temperature_sensor_minimal_widget_t* draw_temperature_sensor_minimal_widget(lv_obj_t* sensor_parent_panel)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_temperature_sensor_minimal_widget_t* TempSensorWidget = NULL;
        TempSensorWidget = calloc(1,sizeof(ui_temperature_sensor_minimal_widget_t));
        //TempSensorWidget = pvPortCalloc(1,sizeof(ui_temperature_sensor_for_home_screen_t));
        if(TempSensorWidget == NULL) {
            lvgl_port_unlock();
            return NULL;}
        TempSensorWidget->SensorWidgetImage = NULL;
        TempSensorWidget->SensorWidgetSetupButton = NULL;
        TempSensorWidget->SensorWidgetSetupButtonImage = NULL;
        TempSensorWidget->temperature_sensor_name_label = NULL;
        TempSensorWidget->temperature_sensor_value_label = NULL;
        TempSensorWidget->temperature_sensor_widget = NULL;
        TempSensorWidget->temperature_sensor_room_label = NULL;
        //lv_obj_t* Container = lv_obj_create(NULL);
        TempSensorWidget->temperature_sensor_widget = NULL;
        TempSensorWidget->temperature_sensor_widget = lv_obj_create(sensor_parent_panel);
        lv_obj_remove_style_all(TempSensorWidget->temperature_sensor_widget);
        lv_obj_set_width(TempSensorWidget->temperature_sensor_widget, 400);
        lv_obj_set_height(TempSensorWidget->temperature_sensor_widget, 96);
        lv_obj_set_x(TempSensorWidget->temperature_sensor_widget, 0);
        lv_obj_set_y(TempSensorWidget->temperature_sensor_widget, -1);
        lv_obj_set_align(TempSensorWidget->temperature_sensor_widget, LV_ALIGN_CENTER);
        lv_obj_remove_flag(TempSensorWidget->temperature_sensor_widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(TempSensorWidget->temperature_sensor_widget, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(TempSensorWidget->temperature_sensor_widget, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(TempSensorWidget->temperature_sensor_widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(TempSensorWidget->temperature_sensor_widget, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(TempSensorWidget->temperature_sensor_widget, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

        TempSensorWidget->SensorWidgetImage = NULL;
        TempSensorWidget->SensorWidgetImage = lv_image_create(TempSensorWidget->temperature_sensor_widget);
        lv_image_set_src(TempSensorWidget->SensorWidgetImage, &ui_img_icons_device_thermostat_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);
        lv_obj_set_width(TempSensorWidget->SensorWidgetImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TempSensorWidget->SensorWidgetImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(TempSensorWidget->SensorWidgetImage, 6);
        lv_obj_set_y(TempSensorWidget->SensorWidgetImage, 16);
        lv_obj_remove_flag(TempSensorWidget->SensorWidgetImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        TempSensorWidget->temperature_sensor_name_label = NULL;
        TempSensorWidget->temperature_sensor_name_label = lv_label_create(TempSensorWidget->temperature_sensor_widget);
        lv_obj_set_width(TempSensorWidget->temperature_sensor_name_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TempSensorWidget->temperature_sensor_name_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(TempSensorWidget->temperature_sensor_name_label, 0);
        lv_obj_set_y(TempSensorWidget->temperature_sensor_name_label, -36);
        lv_obj_set_align(TempSensorWidget->temperature_sensor_name_label, LV_ALIGN_CENTER);
        lv_label_set_text(TempSensorWidget->temperature_sensor_name_label, "датчик температуры");
        lv_obj_set_style_text_color(TempSensorWidget->temperature_sensor_name_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(TempSensorWidget->temperature_sensor_name_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(TempSensorWidget->temperature_sensor_name_label, &ui_font_RobotoMedium14, LV_PART_MAIN | LV_STATE_DEFAULT);

        TempSensorWidget->temperature_sensor_room_label = NULL;
        TempSensorWidget->temperature_sensor_room_label = lv_label_create(TempSensorWidget->temperature_sensor_widget);
        lv_obj_set_width(TempSensorWidget->temperature_sensor_room_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TempSensorWidget->temperature_sensor_room_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(TempSensorWidget->temperature_sensor_room_label, 0);
        lv_obj_set_y(TempSensorWidget->temperature_sensor_room_label, 36);
        lv_obj_set_align(TempSensorWidget->temperature_sensor_room_label, LV_ALIGN_CENTER);
        lv_label_set_text(TempSensorWidget->temperature_sensor_room_label, "комната");
        lv_obj_set_style_text_color(TempSensorWidget->temperature_sensor_room_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(TempSensorWidget->temperature_sensor_room_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(TempSensorWidget->temperature_sensor_room_label, &ui_font_RobotoMedium12, LV_PART_MAIN | LV_STATE_DEFAULT);

        TempSensorWidget->temperature_sensor_value_label = NULL;
        TempSensorWidget->temperature_sensor_value_label = lv_label_create(TempSensorWidget->temperature_sensor_widget);
        lv_obj_set_width(TempSensorWidget->temperature_sensor_value_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TempSensorWidget->temperature_sensor_value_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(TempSensorWidget->temperature_sensor_value_label, LV_ALIGN_CENTER);
        lv_label_set_text(TempSensorWidget->temperature_sensor_value_label, "00,0 C");
        lv_obj_set_style_text_color(TempSensorWidget->temperature_sensor_value_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(TempSensorWidget->temperature_sensor_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(TempSensorWidget->temperature_sensor_value_label, &ui_font_RobotoMedium48, LV_PART_MAIN | LV_STATE_DEFAULT);

        TempSensorWidget->SensorWidgetSetupButton = NULL;
        TempSensorWidget->SensorWidgetSetupButton = lv_button_create(TempSensorWidget->temperature_sensor_widget);
        lv_obj_set_width(TempSensorWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_height(TempSensorWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_x(TempSensorWidget->SensorWidgetSetupButton, -6);
        lv_obj_set_y(TempSensorWidget->SensorWidgetSetupButton, 0);
        lv_obj_set_align(TempSensorWidget->SensorWidgetSetupButton, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(TempSensorWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(TempSensorWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(TempSensorWidget->SensorWidgetSetupButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(TempSensorWidget->SensorWidgetSetupButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(TempSensorWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(TempSensorWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        TempSensorWidget->SensorWidgetSetupButtonImage = NULL;
        TempSensorWidget->SensorWidgetSetupButtonImage = lv_image_create(TempSensorWidget->SensorWidgetSetupButton);
        lv_image_set_src(TempSensorWidget->SensorWidgetSetupButtonImage, &ui_img_icons_settings_24dp_61c6ce_fill0_wght200_grad0_opsz24_png);
        lv_obj_set_width(TempSensorWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TempSensorWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(TempSensorWidget->SensorWidgetSetupButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(TempSensorWidget->SensorWidgetSetupButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        //lv_obj_set_parent(TempSensorWidget->temperature_sensor_widget, sensor_parent_panel);
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        //lv_obj_delete(Container);
        //Container = NULL;
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        lvgl_port_unlock();
        return TempSensorWidget;
    }  else 
    {
        ESP_LOGI("LVGL port_zb_interface.c", "lvgl_port_lock failed");
        return NULL;
    }              
}

esp_err_t temperature_sensor_minimal_widget_destroy_ui(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor)
{
    if (lvgl_port_lock(0) == true)
    {
        if(ui_temperature_sensor && ui_temperature_sensor->temperature_sensor_widget) lv_obj_del(ui_temperature_sensor->temperature_sensor_widget);
        ui_temperature_sensor->temperature_sensor_name_label = NULL;
        ui_temperature_sensor->temperature_sensor_room_label = NULL;
        ui_temperature_sensor->temperature_sensor_value_label = NULL;
        ui_temperature_sensor->SensorWidgetSetupButtonImage = NULL;
        ui_temperature_sensor->SensorWidgetSetupButton = NULL;
        ui_temperature_sensor->SensorWidgetImage = NULL;
        ui_temperature_sensor->temperature_sensor_widget = NULL;
        free(ui_temperature_sensor);
        ui_temperature_sensor = NULL;
        lvgl_port_unlock();
        return ESP_OK;
    } else return ESP_FAIL;
}

esp_err_t temperature_sensor_minimal_widget_set_name_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_temperature_sensor->temperature_sensor_name_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    } else return ESP_FAIL;
}
esp_err_t temperature_sensor_minimal_widget_set_room_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_temperature_sensor->temperature_sensor_room_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    } else return ESP_FAIL;
}
esp_err_t temperature_sensor_minimal_widget_set_value_text(ui_temperature_sensor_minimal_widget_t* ui_temperature_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_temperature_sensor->temperature_sensor_value_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    } else return ESP_FAIL;
}

zb_manager_temperature_sensor_ui_t* create_new_temperature_sensor_ui(void)
{
    zb_manager_temperature_sensor_ui_t* result = calloc(1, sizeof(zb_manager_temperature_sensor_ui_t));
    if(result == NULL)
    {
        ESP_LOGI("UI", "create_new_temperature_sensor_ui: calloc error");
        return NULL;
    } 
    result->temperature_device_ep = temp_sensor_ep_create();
    result->draw_on_home_screen = true;
    result->draw_on_device_add_screen = false;
    result->draw_on_home_screen = false;
    result->temperature_sensor_minimal_widget_str = NULL;
    result->device_name = strdup("Имя не определено");
    if (result->device_name == NULL) {
        free(result);
        ESP_LOGE("UI", "Failed to allocate memory for device_name");
        return NULL;
    }
    result->room_name = strdup("Комната не определено");
    if (result->room_name == NULL) {
        free(result->device_name);
        free(result);
        ESP_LOGE("UI", "Failed to allocate memory for room_name");
        return NULL;
    }
    //result->temperature_device_ep_ui
    /*if ((result->home_screen_parent_panel!=NULL)&&result->draw_on_home_screen==true)
    {
        result->temperature_device_ep_ui_on_home_screen = draw_temperature_sensor_on_home_screen(result->home_screen_parent_panel);
    }*/
    //result->temperature_device_ep = temp_sensor_ep_create();
    return result;
}

esp_err_t draw_temperature_sensor_minimal(zb_manager_temperature_sensor_ui_t* sensor_structure, lv_obj_t* parent_obj)
{
    esp_err_t result = ESP_FAIL;
    if (sensor_structure == NULL)
    {
        ESP_LOGI("UI", "draw_temperature_sensor_minimal_widget: sensor_structure == NULL");
        result = ESP_FAIL;
        return result;
    }
    if (sensor_structure->temperature_sensor_minimal_widget_str != NULL)
    {
        ESP_LOGI("UI", "draw_temperature_sensor_minimal_widget: sensor_structure->temperature_sensor_minimal_widget_str != NULL");
        result = temperature_sensor_minimal_widget_destroy_ui(sensor_structure->temperature_sensor_minimal_widget_str);
        if (result == ESP_FAIL) return result;
    }
    
    sensor_structure->temperature_sensor_minimal_widget_str = draw_temperature_sensor_minimal_widget(parent_obj);
    if (sensor_structure->temperature_sensor_minimal_widget_str == NULL) return ESP_FAIL;
    result = temperature_sensor_minimal_widget_set_name_text(sensor_structure->temperature_sensor_minimal_widget_str, sensor_structure->device_name);
    if (result == ESP_FAIL) return result;
    result = temperature_sensor_minimal_widget_set_room_text(sensor_structure->temperature_sensor_minimal_widget_str, sensor_structure->room_name);
    if (result == ESP_FAIL) return result;
    result = ESP_OK;
    return result;
}
// Temperature sensor widget end

//HUMIDITY_SENSOR
ui_humidity_sensor_t* draw_humidity_sensor(lv_obj_t* sensor_parent_panel)
{
     if (lvgl_port_lock(0) == true)
    {
        ui_humidity_sensor_t* HumiditySensorWidget = NULL;
        HumiditySensorWidget = calloc(1,sizeof(ui_humidity_sensor_t));
        if(HumiditySensorWidget == NULL) {
            lvgl_port_unlock();
            return NULL;}

        HumiditySensorWidget->humidity_sensor_widget = NULL;
        HumiditySensorWidget->humidity_sensor_widget = lv_obj_create(sensor_parent_panel);
        lv_obj_remove_style_all(HumiditySensorWidget->humidity_sensor_widget);
        lv_obj_set_width(HumiditySensorWidget->humidity_sensor_widget, 400);
        lv_obj_set_height(HumiditySensorWidget->humidity_sensor_widget, 96);
        lv_obj_set_x(HumiditySensorWidget->humidity_sensor_widget, 0);
        lv_obj_set_y(HumiditySensorWidget->humidity_sensor_widget, -1);
        lv_obj_set_align(HumiditySensorWidget->humidity_sensor_widget, LV_ALIGN_CENTER);
        lv_obj_remove_flag(HumiditySensorWidget->humidity_sensor_widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(HumiditySensorWidget->humidity_sensor_widget, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(HumiditySensorWidget->humidity_sensor_widget, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(HumiditySensorWidget->humidity_sensor_widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(HumiditySensorWidget->humidity_sensor_widget, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(HumiditySensorWidget->humidity_sensor_widget, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    
        HumiditySensorWidget->SensorWidgetImage = NULL;
        HumiditySensorWidget->SensorWidgetImage  = lv_image_create(HumiditySensorWidget->humidity_sensor_widget);
        lv_image_set_src(HumiditySensorWidget->SensorWidgetImage , &ui_img_icons_humidity_percentage_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);
        lv_obj_set_width(HumiditySensorWidget->SensorWidgetImage , LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(HumiditySensorWidget->SensorWidgetImage , LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(HumiditySensorWidget->SensorWidgetImage , 6);
        lv_obj_set_y(HumiditySensorWidget->SensorWidgetImage , 16);
        lv_obj_remove_flag(HumiditySensorWidget->SensorWidgetImage , LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        HumiditySensorWidget->humidity_sensor_name_label = NULL;
        HumiditySensorWidget->humidity_sensor_name_label = lv_label_create(HumiditySensorWidget->humidity_sensor_widget);
        lv_obj_set_width(HumiditySensorWidget->humidity_sensor_name_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(HumiditySensorWidget->humidity_sensor_name_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(HumiditySensorWidget->humidity_sensor_name_label, 0);
        lv_obj_set_y(HumiditySensorWidget->humidity_sensor_name_label, -36);
        lv_obj_set_align(HumiditySensorWidget->humidity_sensor_name_label, LV_ALIGN_CENTER);
        lv_label_set_text(HumiditySensorWidget->humidity_sensor_name_label, "датчик влажности");
        lv_obj_set_style_text_color(HumiditySensorWidget->humidity_sensor_name_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(HumiditySensorWidget->humidity_sensor_name_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(HumiditySensorWidget->humidity_sensor_name_label, &ui_font_RobotoMedium14, LV_PART_MAIN | LV_STATE_DEFAULT);

        HumiditySensorWidget->humidity_sensor_room_label = NULL;
        HumiditySensorWidget->humidity_sensor_room_label = lv_label_create(HumiditySensorWidget->humidity_sensor_widget);
        lv_obj_set_width(HumiditySensorWidget->humidity_sensor_room_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(HumiditySensorWidget->humidity_sensor_room_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(HumiditySensorWidget->humidity_sensor_room_label, 0);
        lv_obj_set_y(HumiditySensorWidget->humidity_sensor_room_label, 36);
        lv_obj_set_align(HumiditySensorWidget->humidity_sensor_room_label, LV_ALIGN_CENTER);
        lv_label_set_text(HumiditySensorWidget->humidity_sensor_room_label, "комната");
        lv_obj_set_style_text_color(HumiditySensorWidget->humidity_sensor_room_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(HumiditySensorWidget->humidity_sensor_room_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(HumiditySensorWidget->humidity_sensor_room_label, &ui_font_RobotoBold12, LV_PART_MAIN | LV_STATE_DEFAULT);

        HumiditySensorWidget->humidity_sensor_value_label = NULL;
        HumiditySensorWidget->humidity_sensor_value_label = lv_label_create(HumiditySensorWidget->humidity_sensor_widget);
        lv_obj_set_width(HumiditySensorWidget->humidity_sensor_value_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(HumiditySensorWidget->humidity_sensor_value_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(HumiditySensorWidget->humidity_sensor_value_label, LV_ALIGN_CENTER);
        lv_label_set_text(HumiditySensorWidget->humidity_sensor_value_label, "65 %");
        lv_obj_set_style_text_color(HumiditySensorWidget->humidity_sensor_value_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(HumiditySensorWidget->humidity_sensor_value_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(HumiditySensorWidget->humidity_sensor_value_label, &ui_font_RobotoMedium48, LV_PART_MAIN | LV_STATE_DEFAULT);

        HumiditySensorWidget->SensorWidgetSetupButton = NULL;
        HumiditySensorWidget->SensorWidgetSetupButton = lv_button_create(HumiditySensorWidget->humidity_sensor_widget);
        lv_obj_set_width(HumiditySensorWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_height(HumiditySensorWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_x(HumiditySensorWidget->SensorWidgetSetupButton, -6);
        lv_obj_set_y(HumiditySensorWidget->SensorWidgetSetupButton, 0);
        lv_obj_set_align(HumiditySensorWidget->SensorWidgetSetupButton, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(HumiditySensorWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(HumiditySensorWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(HumiditySensorWidget->SensorWidgetSetupButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(HumiditySensorWidget->SensorWidgetSetupButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(HumiditySensorWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(HumiditySensorWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        HumiditySensorWidget->SensorWidgetSetupButtonImage = NULL;
        HumiditySensorWidget->SensorWidgetSetupButtonImage = lv_image_create(HumiditySensorWidget->SensorWidgetSetupButton);
        lv_image_set_src(HumiditySensorWidget->SensorWidgetSetupButtonImage, &ui_img_icons_settings_24dp_61c6ce_fill0_wght200_grad0_opsz24_png);
        lv_obj_set_width(HumiditySensorWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(HumiditySensorWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(HumiditySensorWidget->SensorWidgetSetupButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(HumiditySensorWidget->SensorWidgetSetupButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        lvgl_port_unlock();
        return HumiditySensorWidget;
    } else return NULL;

}
esp_err_t humidity_sensor_destroy_ui(ui_humidity_sensor_t* ui_humidity_sensor)
{
    if (lvgl_port_lock(0) == true)
    {
        if(ui_humidity_sensor && ui_humidity_sensor->humidity_sensor_widget) lv_obj_del(ui_humidity_sensor->humidity_sensor_widget);
        ui_humidity_sensor->humidity_sensor_name_label = NULL;
        ui_humidity_sensor->humidity_sensor_room_label = NULL;
        ui_humidity_sensor->humidity_sensor_value_label = NULL;
        ui_humidity_sensor->SensorWidgetSetupButtonImage = NULL;
        ui_humidity_sensor->SensorWidgetSetupButton = NULL;
        ui_humidity_sensor->SensorWidgetImage = NULL;
        ui_humidity_sensor->humidity_sensor_widget = NULL;
        free(ui_humidity_sensor);
        ui_humidity_sensor = NULL;
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}

esp_err_t humidity_sensor_set_name_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_humidity_sensor->humidity_sensor_name_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}
esp_err_t humidity_sensor_set_room_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_humidity_sensor->humidity_sensor_room_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}
esp_err_t humidity_sensor_set_value_text(ui_humidity_sensor_t* ui_humidity_sensor, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_humidity_sensor->humidity_sensor_value_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}
// HUmidity Sensor Widget end

// ON_OFF_LIGHT_DEVICE Widget

void ui_event_on_off_light_device_btn_click(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    ui_on_off_light_device_t* data =(ui_on_off_light_device_t *) lv_event_get_user_data(e);

    switch(code) {
        case LV_EVENT_PRESSED:
            //ESP_LOGI(TAG, "The last button event:\nLV_EVENT_PRESSED");
            break;
        case LV_EVENT_CLICKED:
            ESP_LOGI(TAG, "The last button event:\nLV_EVENT_CLICKED");
            //on_off_light_device_destroy_ui(data);
            break;
        case LV_EVENT_LONG_PRESSED:
            //lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
            break;
        case LV_EVENT_LONG_PRESSED_REPEAT:
            //lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
            break;
        default:
            break;
    }
}
ui_on_off_light_device_t* draw_on_off_light_device(lv_obj_t* sensor_parent_panel, on_off_light_device_variant_t device_variant)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_on_off_light_device_t* OnOffLightDeviceWidget = calloc(1,sizeof(ui_on_off_light_device_t));
        if(OnOffLightDeviceWidget == NULL) {
            lvgl_port_unlock();
            return NULL;}
        
        OnOffLightDeviceWidget->device_variant = device_variant;
        OnOffLightDeviceWidget->on_off_state = 0;
        OnOffLightDeviceWidget->on_off_light_device_widget = NULL;
        OnOffLightDeviceWidget->on_off_light_device_widget = lv_obj_create(sensor_parent_panel);
        lv_obj_remove_style_all(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->on_off_light_device_widget, 400);
        lv_obj_set_height(OnOffLightDeviceWidget->on_off_light_device_widget, 96);
        lv_obj_set_x(OnOffLightDeviceWidget->on_off_light_device_widget, 0);
        lv_obj_set_y(OnOffLightDeviceWidget->on_off_light_device_widget, -1);
        lv_obj_set_align(OnOffLightDeviceWidget->on_off_light_device_widget, LV_ALIGN_CENTER);
        lv_obj_remove_flag(OnOffLightDeviceWidget->on_off_light_device_widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(OnOffLightDeviceWidget->on_off_light_device_widget, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(OnOffLightDeviceWidget->on_off_light_device_widget, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(OnOffLightDeviceWidget->on_off_light_device_widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(OnOffLightDeviceWidget->on_off_light_device_widget, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(OnOffLightDeviceWidget->on_off_light_device_widget, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

        OnOffLightDeviceWidget->SensorWidgetImage = NULL;
        OnOffLightDeviceWidget->SensorWidgetImage = lv_image_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        if (device_variant == ON_OFF_LIGHT_DEVICE_BULB)
        {    
            if (OnOffLightDeviceWidget->on_off_state == 0)
            lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetImage, &ui_img_icons_lightbulb_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png); else
            if (OnOffLightDeviceWidget->on_off_state == 1)
            lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetImage, &ui_img_icons_lightbulb_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        } else
        if (device_variant == ON_OFF_LIGHT_DEVICE_RELE)
        {
           if (OnOffLightDeviceWidget->on_off_state == 0)
           lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetImage, &ui_img_icons_rele_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);else
            if (OnOffLightDeviceWidget->on_off_state == 1)
            lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetImage, &ui_img_icons_rele_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        }
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(OnOffLightDeviceWidget->SensorWidgetImage, 6);
        lv_obj_set_y(OnOffLightDeviceWidget->SensorWidgetImage, 16);
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        OnOffLightDeviceWidget->on_off_light_device_name_label = NULL;                
        OnOffLightDeviceWidget->on_off_light_device_name_label = lv_label_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->on_off_light_device_name_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->on_off_light_device_name_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(OnOffLightDeviceWidget->on_off_light_device_name_label, 0);
        lv_obj_set_y(OnOffLightDeviceWidget->on_off_light_device_name_label, -36);
        lv_obj_set_align(OnOffLightDeviceWidget->on_off_light_device_name_label, LV_ALIGN_CENTER);
        if (device_variant == ON_OFF_LIGHT_DEVICE_BULB)
            lv_label_set_text(OnOffLightDeviceWidget->on_off_light_device_name_label, "лампа"); else
        if (device_variant == ON_OFF_LIGHT_DEVICE_RELE)
           lv_label_set_text(OnOffLightDeviceWidget->on_off_light_device_name_label, "реле");
        lv_obj_set_style_text_color(OnOffLightDeviceWidget->on_off_light_device_name_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(OnOffLightDeviceWidget->on_off_light_device_name_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(OnOffLightDeviceWidget->on_off_light_device_name_label, &ui_font_RobotoMedium14, LV_PART_MAIN | LV_STATE_DEFAULT);

        OnOffLightDeviceWidget->on_off_light_device_room_label = NULL;                
        OnOffLightDeviceWidget->on_off_light_device_room_label = lv_label_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->on_off_light_device_room_label, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->on_off_light_device_room_label, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(OnOffLightDeviceWidget->on_off_light_device_room_label, 0);
        lv_obj_set_y(OnOffLightDeviceWidget->on_off_light_device_room_label, 36);
        lv_obj_set_align(OnOffLightDeviceWidget->on_off_light_device_room_label, LV_ALIGN_CENTER);
        lv_label_set_text(OnOffLightDeviceWidget->on_off_light_device_room_label, "комната");
        lv_obj_set_style_text_color(OnOffLightDeviceWidget->on_off_light_device_room_label, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(OnOffLightDeviceWidget->on_off_light_device_room_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(OnOffLightDeviceWidget->on_off_light_device_room_label, &ui_font_RobotoBold12, LV_PART_MAIN | LV_STATE_DEFAULT);

        OnOffLightDeviceWidget->SensorWidgetSetupButton = NULL;
        OnOffLightDeviceWidget->SensorWidgetSetupButton = lv_button_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetSetupButton, 36);
        lv_obj_set_x(OnOffLightDeviceWidget->SensorWidgetSetupButton, -6);
        lv_obj_set_y(OnOffLightDeviceWidget->SensorWidgetSetupButton, 0);
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetSetupButton, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(OnOffLightDeviceWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetSetupButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(OnOffLightDeviceWidget->SensorWidgetSetupButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(OnOffLightDeviceWidget->SensorWidgetSetupButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(OnOffLightDeviceWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(OnOffLightDeviceWidget->SensorWidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(OnOffLightDeviceWidget->SensorWidgetSetupButton, ui_event_on_off_light_device_btn_click, LV_EVENT_CLICKED, OnOffLightDeviceWidget);

        OnOffLightDeviceWidget->SensorWidgetSetupButtonImage = NULL;
        OnOffLightDeviceWidget->SensorWidgetSetupButtonImage = lv_image_create(OnOffLightDeviceWidget->SensorWidgetSetupButton);
        lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetSetupButtonImage, &ui_img_icons_settings_24dp_61c6ce_fill0_wght200_grad0_opsz24_png);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetSetupButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetSetupButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetSetupButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        OnOffLightDeviceWidget->SensorWidgetONButton = NULL;
        OnOffLightDeviceWidget->SensorWidgetONButton = lv_button_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetONButton, 48);
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetONButton, 48);
        lv_obj_set_x(OnOffLightDeviceWidget->SensorWidgetONButton, -32);
        lv_obj_set_y(OnOffLightDeviceWidget->SensorWidgetONButton, 0);
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetONButton, LV_ALIGN_CENTER);
        lv_obj_add_flag(OnOffLightDeviceWidget->SensorWidgetONButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetONButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(OnOffLightDeviceWidget->SensorWidgetONButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(OnOffLightDeviceWidget->SensorWidgetONButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(OnOffLightDeviceWidget->SensorWidgetONButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(OnOffLightDeviceWidget->SensorWidgetONButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(OnOffLightDeviceWidget->SensorWidgetONButton, ui_event_on_off_light_device_btn_click, LV_EVENT_CLICKED, OnOffLightDeviceWidget);  
        
        OnOffLightDeviceWidget->SensorWidgetONButtonImage = NULL;
        OnOffLightDeviceWidget->SensorWidgetONButtonImage = lv_image_create(OnOffLightDeviceWidget->SensorWidgetONButton);
        lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetONButtonImage, &ui_img_icons_power_on_circle_48dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetONButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetONButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetONButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetONButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        OnOffLightDeviceWidget->SensorWidgetOFFButton = NULL;                
        OnOffLightDeviceWidget->SensorWidgetOFFButton = lv_button_create(OnOffLightDeviceWidget->on_off_light_device_widget);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetOFFButton, 48);
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetOFFButton, 48);
        lv_obj_set_x(OnOffLightDeviceWidget->SensorWidgetOFFButton, 32);
        lv_obj_set_y(OnOffLightDeviceWidget->SensorWidgetOFFButton, 0);
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetOFFButton, LV_ALIGN_CENTER);
        lv_obj_add_flag(OnOffLightDeviceWidget->SensorWidgetOFFButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetOFFButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(OnOffLightDeviceWidget->SensorWidgetOFFButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(OnOffLightDeviceWidget->SensorWidgetOFFButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(OnOffLightDeviceWidget->SensorWidgetOFFButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(OnOffLightDeviceWidget->SensorWidgetOFFButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_add_event_cb(OnOffLightDeviceWidget->SensorWidgetOFFButton, ui_event_on_off_light_device_btn_click, LV_EVENT_CLICKED, OnOffLightDeviceWidget);

        OnOffLightDeviceWidget->SensorWidgetOFFButtonImage = NULL;
        OnOffLightDeviceWidget->SensorWidgetOFFButtonImage = lv_image_create(OnOffLightDeviceWidget->SensorWidgetOFFButton);
        lv_image_set_src(OnOffLightDeviceWidget->SensorWidgetOFFButtonImage, &ui_img_icons_power_off_circle_48dp_61c6ce_fill0_wght200_grad0_opsz48_png);
        lv_obj_set_width(OnOffLightDeviceWidget->SensorWidgetOFFButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(OnOffLightDeviceWidget->SensorWidgetOFFButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(OnOffLightDeviceWidget->SensorWidgetOFFButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(OnOffLightDeviceWidget->SensorWidgetOFFButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        
        lvgl_port_unlock();
        return OnOffLightDeviceWidget;
    } else return NULL;

}
esp_err_t on_off_light_device_destroy_ui(ui_on_off_light_device_t* ui_on_off_light_device)
{
    if (lvgl_port_lock(0) == true)
    {
        if(ui_on_off_light_device && ui_on_off_light_device->on_off_light_device_widget) lv_obj_del(ui_on_off_light_device->on_off_light_device_widget);
        ui_on_off_light_device->on_off_light_device_widget = NULL;
        ui_on_off_light_device->SensorWidgetSetupButton = NULL;
        ui_on_off_light_device->SensorWidgetSetupButtonImage = NULL;
        ui_on_off_light_device->SensorWidgetONButton = NULL;
        ui_on_off_light_device->SensorWidgetONButtonImage = NULL;
        ui_on_off_light_device->SensorWidgetOFFButton = NULL;
        ui_on_off_light_device->SensorWidgetOFFButtonImage = NULL;
        ui_on_off_light_device->on_off_light_device_name_label = NULL;
        ui_on_off_light_device->on_off_light_device_room_label = NULL;
        ui_on_off_light_device->on_off_light_device_value_label = NULL;
        free(ui_on_off_light_device);
        ui_on_off_light_device = NULL;
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}
esp_err_t on_off_light_device_set_name_text(ui_on_off_light_device_t* ui_on_off_light_device, char* text)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(ui_on_off_light_device->on_off_light_device_name_label, text);
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}
esp_err_t on_off_light_device_set_room_text(ui_on_off_light_device_t* ui_on_off_light_device, char* text)
{
        if (lvgl_port_lock(0) == true)
        {
            lv_label_set_text(ui_on_off_light_device->on_off_light_device_room_label, text);
            lvgl_port_unlock();
            return ESP_OK;
        }else return ESP_FAIL;
}
esp_err_t on_off_light_device_set_device_variant(ui_on_off_light_device_t* ui_on_off_light_device, on_off_light_device_variant_t device_variant)
{
        if (lvgl_port_lock(0) == true)
        {
            ui_on_off_light_device->device_variant = device_variant;
            if (device_variant == ON_OFF_LIGHT_DEVICE_BULB)
        {    
            if (ui_on_off_light_device->on_off_state == 0)
            lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_lightbulb_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png); else
            if (ui_on_off_light_device->on_off_state == 1)
            lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_lightbulb_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        } else
        if (device_variant == ON_OFF_LIGHT_DEVICE_RELE)
        {
           if (ui_on_off_light_device->on_off_state == 0)
           lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_rele_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);else
            if (ui_on_off_light_device->on_off_state == 1)
            lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_rele_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        }
            lvgl_port_unlock();
            return ESP_OK;
        }else return ESP_FAIL;
}
esp_err_t on_off_light_device_set_value_on_off(ui_on_off_light_device_t* ui_on_off_light_device, uint8_t on_off_state) // use when zb device report state
{
        if (lvgl_port_lock(0) == true)
        {
            ui_on_off_light_device->on_off_state = on_off_state;
            if (ui_on_off_light_device->device_variant == ON_OFF_LIGHT_DEVICE_BULB)
            {    
              if (ui_on_off_light_device->on_off_state == 0)
              lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_lightbulb_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png); else
              if (ui_on_off_light_device->on_off_state == 1)
              lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_lightbulb_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
            } else
            if (ui_on_off_light_device->device_variant == ON_OFF_LIGHT_DEVICE_RELE)
            {
              if (ui_on_off_light_device->on_off_state == 0)
              lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_rele_off_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);else
              if (ui_on_off_light_device->on_off_state == 1)
              lv_image_set_src(ui_on_off_light_device->SensorWidgetImage, &ui_img_icons_rele_on_64dp_61c6ce_fill1_wght200_grad0_opsz48_png);
            }
            lvgl_port_unlock();
            return ESP_OK;
        }else return ESP_FAIL;
}

