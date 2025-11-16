#include "../ui.h"
#include "esp_lvgl_port.h"
#include "ui_PanelComponent.h"

/*************************************************************** ui_TopPanelForSensorsWidget_buttons_t; ************************************************/
ui_TopPanelWidget_buttons_t* TopPanelWidget_buttonsCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_TopPanelWidget_buttons_t* btns_obj = NULL;
        btns_obj = calloc(1, sizeof(ui_TopPanelWidget_buttons_t));
        if (btns_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }
        btns_obj->on_create_elements_count = 0;
        btns_obj->on_create_elements_array = NULL;
        btns_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;
        lv_obj_t* ui_HomeButton = NULL;
        ui_HomeButton = lv_button_create(parent);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)calloc(btns_obj->on_create_elements_count, sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_HomeButton;
        btns_obj->button_home_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_HomeButton, 102);
        lv_obj_set_height(ui_HomeButton, 98);
        lv_obj_set_x(ui_HomeButton, -144);
        lv_obj_set_y(ui_HomeButton, 121);
        lv_obj_set_align(ui_HomeButton, LV_ALIGN_CENTER);
        lv_obj_add_flag(ui_HomeButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_HomeButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_HomeButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_HomeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_HomeButton, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_HomeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_HomeBtnImage = NULL;
        ui_HomeBtnImage = lv_image_create(ui_HomeButton);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_HomeBtnImage;
        lv_image_set_src(ui_HomeBtnImage, &ui_img_icons_home_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);
        lv_obj_set_width(ui_HomeBtnImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_HomeBtnImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_HomeBtnImage, LV_ALIGN_TOP_MID);
        lv_obj_remove_flag(ui_HomeBtnImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_HomeBtnImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_HomeBtnImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_HomeBtnLabel = NULL;
        ui_HomeBtnLabel = lv_label_create(ui_HomeButton);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_HomeBtnLabel;
        lv_obj_set_width(ui_HomeBtnLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_HomeBtnLabel, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(ui_HomeBtnLabel, 0);
        lv_obj_set_y(ui_HomeBtnLabel, -9);
        lv_obj_set_align(ui_HomeBtnLabel, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(ui_HomeBtnLabel, "дом");
        lv_obj_set_style_text_color(ui_HomeBtnLabel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui_HomeBtnLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui_HomeBtnLabel, &ui_font_RobotoBold16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_HomeBtnLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_HomeBtnLabel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_DevicesBtn = NULL;
        ui_DevicesBtn = lv_button_create(parent);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_DevicesBtn;
        btns_obj->button_devices_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_DevicesBtn, 102);
        lv_obj_set_height(ui_DevicesBtn, 98);
        lv_obj_set_x(ui_DevicesBtn, -144);
        lv_obj_set_y(ui_DevicesBtn, 121);
        lv_obj_set_align(ui_DevicesBtn, LV_ALIGN_CENTER);
        lv_obj_add_flag(ui_DevicesBtn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_DevicesBtn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_DevicesBtn, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_DevicesBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_DevicesBtn, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_DevicesBtn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_DevicesBtn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_DevicesBtnImage = NULL;
        ui_DevicesBtnImage = lv_image_create(ui_DevicesBtn);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_DevicesBtnImage;
        lv_image_set_src(ui_DevicesBtnImage, &ui_img_icons_devices_64dp_61c6ce_fill0_wght200_grad0_opsz48_png);
        lv_obj_set_width(ui_DevicesBtnImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_DevicesBtnImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_DevicesBtnImage, LV_ALIGN_TOP_MID);
        lv_obj_remove_flag(ui_DevicesBtnImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_DevicesBtnImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_DevicesBtnImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT); 
        
        lv_obj_t* ui_DevicesBtnLabel = NULL;
        ui_DevicesBtnLabel = lv_label_create(ui_DevicesBtn);
        lv_obj_set_width(ui_DevicesBtnLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_DevicesBtnLabel, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(ui_DevicesBtnLabel, 0);
        lv_obj_set_y(ui_DevicesBtnLabel, -9);
        lv_obj_set_align(ui_DevicesBtnLabel, LV_ALIGN_BOTTOM_MID);
        lv_label_set_text(ui_DevicesBtnLabel, "устройства");
        lv_obj_set_style_text_color(ui_DevicesBtnLabel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui_DevicesBtnLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui_DevicesBtnLabel, &ui_font_RobotoBold16, LV_PART_MAIN | LV_STATE_DEFAULT);
        lvgl_port_unlock();
        return btns_obj;
    } else return NULL;
}

esp_err_t ui_TopPanelWidget_buttonsDelete(ui_TopPanelWidget_buttons_t* buttons_obj)
{
    if (lvgl_port_lock(0) == true)
    {
        if (buttons_obj->on_create_elements_array != NULL)
        {
            for (uint8_t i = 0; i < buttons_obj->on_create_elements_count; i++)
            { 
                lv_obj_del(buttons_obj->on_create_elements_array[i]);
                buttons_obj->on_create_elements_array[i] = NULL;
            }
            free(buttons_obj->on_create_elements_array);
            buttons_obj->on_create_elements_array = NULL;
        }
            lvgl_port_unlock();
            return ESP_OK;
    } else return ESP_FAIL;

}

/*************************************************************** ui_MainPanelForSensorsWidget_buttons_t; ************************************************/
ui_MainPanelForSensorsWidget_buttonsContainer_t* MainPanelForSensorsWidget_buttonsContainerCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_MainPanelForSensorsWidget_buttonsContainer_t* btns_obj = NULL;
        btns_obj = calloc(1, sizeof(ui_MainPanelForSensorsWidget_buttonsContainer_t));
        if (btns_obj == NULL) 
        {
                lvgl_port_unlock();
                return NULL;
        }
        btns_obj->on_create_elements_count = 0;
        btns_obj->on_create_elements_array = NULL;
        btns_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;
        lv_obj_t* ui_Container = NULL;
        ui_Container = lv_obj_create(parent);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)calloc(btns_obj->on_create_elements_count, sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_Container;
        lv_obj_remove_style_all(ui_Container);
        lv_obj_set_width(ui_Container, 450);
        lv_obj_set_height(ui_Container, 80);
        lv_obj_set_align(ui_Container, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_Container, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_pad_left(ui_Container, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_Container, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(ui_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(ui_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonAddWidget = NULL;
        ui_ButtonAddWidget = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonAddWidget;
        btns_obj->button_add_widget_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonAddWidget, 48);
        lv_obj_set_height(ui_ButtonAddWidget, 48);
        lv_obj_set_align(ui_ButtonAddWidget, LV_ALIGN_LEFT_MID);
        lv_obj_add_flag(ui_ButtonAddWidget, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonAddWidget, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonAddWidget, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonAddWidget, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonAddWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonAddWidget, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonAddWidget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonAddWidget, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonAddWidget, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonAddWidget, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonAddWidget, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonAddWidgetImage = NULL; 
        ui_ButtonAddWidgetImage = lv_image_create(ui_ButtonAddWidget);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonAddWidgetImage;
        lv_image_set_src(ui_ButtonAddWidgetImage, &ui_img_icons_add_widget_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonAddWidgetImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonAddWidgetImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonAddWidgetImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonAddWidgetImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonAddWidgetImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonAddWidgetImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSwitchAllOn = NULL;
        ui_ButtonSwitchAllOn = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSwitchAllOn;
        btns_obj->button_switch_all_on_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonSwitchAllOn, 48);
        lv_obj_set_height(ui_ButtonSwitchAllOn, 48);
        lv_obj_set_x(ui_ButtonSwitchAllOn, 96);
        lv_obj_set_y(ui_ButtonSwitchAllOn, 0);
        lv_obj_set_align(ui_ButtonSwitchAllOn, LV_ALIGN_LEFT_MID);
        lv_obj_add_flag(ui_ButtonSwitchAllOn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonSwitchAllOn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonSwitchAllOn, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonSwitchAllOn, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSwitchAllOn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonSwitchAllOn, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonSwitchAllOn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonSwitchAllOn, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonSwitchAllOn, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonSwitchAllOn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonSwitchAllOn, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSwitchAllOnImage = NULL;
        ui_ButtonSwitchAllOnImage = lv_image_create(ui_ButtonSwitchAllOn);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSwitchAllOnImage;
        lv_image_set_src(ui_ButtonSwitchAllOnImage, &ui_img_icons_power_on_circle_36dp_61c6ce_fill1_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonSwitchAllOnImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonSwitchAllOnImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonSwitchAllOnImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonSwitchAllOnImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonSwitchAllOnImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSwitchAllOnImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSwitchAllOff = NULL;
        ui_ButtonSwitchAllOff = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSwitchAllOff;
        btns_obj->button_switch_all_off_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonSwitchAllOff, 48);
        lv_obj_set_height(ui_ButtonSwitchAllOff, 48);
        lv_obj_set_x(ui_ButtonSwitchAllOff, -96);
        lv_obj_set_y(ui_ButtonSwitchAllOff, 0);
        lv_obj_set_align(ui_ButtonSwitchAllOff, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(ui_ButtonSwitchAllOff, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonSwitchAllOff, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonSwitchAllOff, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonSwitchAllOff, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSwitchAllOff, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonSwitchAllOff, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonSwitchAllOff, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonSwitchAllOff, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonSwitchAllOff, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonSwitchAllOff, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonSwitchAllOff, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSwitchAllOffImage = NULL;
        ui_ButtonSwitchAllOffImage = lv_image_create(ui_ButtonSwitchAllOff);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSwitchAllOffImage;
        lv_image_set_src(ui_ButtonSwitchAllOffImage, &ui_img_icons_power_on_circle_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonSwitchAllOffImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonSwitchAllOffImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonSwitchAllOffImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonSwitchAllOffImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonSwitchAllOffImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSwitchAllOffImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSetup = NULL;
        ui_ButtonSetup = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSetup;
        btns_obj->button_settings_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonSetup, 48);
        lv_obj_set_height(ui_ButtonSetup, 48);
        lv_obj_set_align(ui_ButtonSetup, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(ui_ButtonSetup, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonSetup, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonSetup, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonSetup, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSetup, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonSetup, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonSetup, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonSetup, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonSetup, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonSetup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonSetup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSetupImage = NULL;
        ui_ButtonSetupImage = lv_image_create(ui_ButtonSetup);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSetupImage;
        lv_image_set_src(ui_ButtonSetupImage, &ui_img_icons_settings_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonSetupImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonSetupImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonSetupImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonSetupImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonSetupImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSetupImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lvgl_port_unlock();
        return btns_obj;
    } else return NULL;
}
esp_err_t ui_MainPanelForSensorsWidget_buttonsContainerDelete(ui_MainPanelForSensorsWidget_buttonsContainer_t* buttons_obj)
{
    if (lvgl_port_lock(0) == true)
        {
           lv_obj_delete(buttons_obj->on_create_elements_array[0]);
            for (int i = 0; i < buttons_obj->on_create_elements_count; i++)
            {
                free(buttons_obj->on_create_elements_array[i]);
                buttons_obj->on_create_elements_array[i] = NULL;
            }
            lvgl_port_unlock();
            return ESP_OK;
        } else return ESP_FAIL;
    
}

/*************************************************************** ui_MainPanelForSensorsMainSensorsPanel_t; ********************************************/
ui_MainPanelForSensorsMainSensorsPanel_t* MainPanelForSensorsMainSensorsPanelCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_MainPanelForSensorsMainSensorsPanel_t* panel_obj = NULL;
        panel_obj = calloc(1, sizeof(ui_MainPanelForSensorsMainSensorsPanel_t));
        if (panel_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }
        //panel_obj->buttons_for_sensor_widget_show_screen = NULL;
        panel_obj->on_create_elements_array = NULL;
        panel_obj->on_create_elements_count = 0;
        panel_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;

        lv_obj_t* ui_SensorsContainer = NULL;
        ui_SensorsContainer = lv_obj_create(parent);
        panel_obj->on_create_elements_count = panel_obj->on_create_elements_count + 1;
        
        panel_obj->on_create_elements_array = (lv_obj_t**)calloc(panel_obj->on_create_elements_count, sizeof(lv_obj_t*));    
        panel_obj->on_create_elements_array[panel_obj->on_create_elements_count-1] = ui_SensorsContainer;
        lv_obj_set_width(ui_SensorsContainer, 462);
        lv_obj_set_height(ui_SensorsContainer, 514);    /// 564LV_SIZE_CONTENT
        lv_obj_set_x(ui_SensorsContainer, 0);
        lv_obj_set_y(ui_SensorsContainer, 58);
        lv_obj_set_align(ui_SensorsContainer, LV_ALIGN_TOP_MID);
        lv_obj_set_flex_flow(ui_SensorsContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(ui_SensorsContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        //lv_obj_remove_flag(ui_SensorsContainer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_scroll_dir(ui_SensorsContainer, LV_DIR_VER);
        lv_obj_set_scrollbar_mode(ui_SensorsContainer, LV_SCROLLBAR_MODE_OFF);
        /*lv_obj_set_style_radius(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_SensorsContainer, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_SensorsContainer, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_SensorsContainer, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);*/
    lv_obj_set_style_radius(ui_SensorsContainer, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SensorsContainer, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_SensorsContainer, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_SensorsContainer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_SensorsContainer, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    /************* TEST FILL Container */
    for (int i = 0; i < 20; i++)
    {
        lv_obj_t* test_obj = lv_button_create(ui_SensorsContainer);
            lv_obj_set_width(test_obj, LV_SIZE_CONTENT);
            lv_obj_set_height(test_obj, LV_SIZE_CONTENT);
    }
        lv_obj_set_style_pad_left(ui_SensorsContainer, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(ui_SensorsContainer, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(ui_SensorsContainer, lv_color_hex(0x2F5B7C), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_SensorsContainer, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_SensorsContainer, lv_color_hex(0x61C6CE),
                                    LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_SensorsContainer, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_SensorsContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_SensorsContainer, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(ui_SensorsContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_SensorsContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_SensorsContainer, 6, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_SensorsContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

        
        //panel_obj->buttons_for_sensor_widget_show_screen = MainPanelForSensorsWidget_buttonsContainerCreate(panel_obj->on_create_elements_array[0], panel_obj->parentScreenGlobalIndex);

        lvgl_port_unlock();
        return panel_obj;
    } else return NULL;
}

/*************************************************************** ui_MainPanelForSensorsWidget_buttons_t; ************************************************/
ui_MainPanelForDevicesWidget_buttonsContainer_t* MainPanelForDevicesWidget_buttonsContainerCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_MainPanelForDevicesWidget_buttonsContainer_t* btns_obj = NULL;
        btns_obj = calloc(1, sizeof(ui_MainPanelForDevicesWidget_buttonsContainer_t));
        if (btns_obj == NULL) 
        {
                lvgl_port_unlock();
                return NULL;
        }
        btns_obj->on_create_elements_count = 0;
        btns_obj->on_create_elements_array = NULL;
        btns_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;
        lv_obj_t* ui_Container = NULL;
        ui_Container = lv_obj_create(parent);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)calloc(btns_obj->on_create_elements_count, sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_Container;
        lv_obj_remove_style_all(ui_Container);
        lv_obj_set_width(ui_Container, 450);
        lv_obj_set_height(ui_Container, 80);
        lv_obj_set_align(ui_Container, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_Container, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_pad_left(ui_Container, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_Container, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(ui_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(ui_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ImageNetState = NULL;
        ui_ImageNetState = lv_image_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ImageNetState;
        btns_obj->img_network_state_index = btns_obj->on_create_elements_count-1;
        /*lv_image_set_src(ui_ImageNetState, &ui_img_icons_net_is_open_48dp_61c6ce_fill1_wght200_grad0_opsz48_png);
        lv_obj_set_width(ui_ImageNetState, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ImageNetState, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ImageNetState, LV_ALIGN_LEFT_MID);
        lv_obj_remove_flag(ui_ImageNetState, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ImageNetState, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ImageNetState, 255, LV_PART_MAIN | LV_STATE_DEFAULT);*/
        lv_image_set_src(ui_ImageNetState, &ui_img_icons_net_is_open_48dp_61c6ce_fill1_wght200_grad0_opsz48_png);
    lv_obj_set_width(ui_ImageNetState, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageNetState, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_ImageNetState, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(ui_ImageNetState, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                       LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_ImageNetState, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_ImageNetState, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_ImageNetState, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_shadow_color(ui_ImageNetState, lv_color_hex(0xE5DC18), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_ImageNetState, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_ImageNetState, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui_ImageNetState, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui_ImageNetState, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui_ImageNetState, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonOpenNetwork = NULL;
        ui_ButtonOpenNetwork = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonOpenNetwork;
        btns_obj->button_open_network_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonOpenNetwork, 48);
        lv_obj_set_height(ui_ButtonOpenNetwork, 48);
        lv_obj_set_x(ui_ButtonOpenNetwork, 96);
        lv_obj_set_y(ui_ButtonOpenNetwork, 0);
        lv_obj_set_align(ui_ButtonOpenNetwork, LV_ALIGN_LEFT_MID);
        lv_obj_add_flag(ui_ButtonOpenNetwork, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonOpenNetwork, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonOpenNetwork, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonOpenNetwork, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonOpenNetwork, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonOpenNetwork, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonOpenNetwork, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonOpenNetwork, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonOpenNetwork, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonOpenNetwork, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonOpenNetwork, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonOpenNetworkImage = NULL;
        ui_ButtonOpenNetworkImage = lv_image_create(ui_ButtonOpenNetwork);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonOpenNetworkImage;
        lv_image_set_src(ui_ButtonOpenNetworkImage, &ui_img_icons_lock_open_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonOpenNetworkImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonOpenNetworkImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonOpenNetworkImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonOpenNetworkImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonOpenNetworkImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonOpenNetworkImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonCloseNetwork = NULL;
        ui_ButtonCloseNetwork = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonCloseNetwork;
        btns_obj->button_close_network_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonCloseNetwork, 48);
        lv_obj_set_height(ui_ButtonCloseNetwork, 48);
        lv_obj_set_x(ui_ButtonCloseNetwork, -96);
        lv_obj_set_y(ui_ButtonCloseNetwork, 0);
        lv_obj_set_align(ui_ButtonCloseNetwork, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(ui_ButtonCloseNetwork, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonCloseNetwork, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonCloseNetwork, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonCloseNetwork, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonCloseNetwork, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonCloseNetwork, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonCloseNetwork, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonCloseNetwork, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonCloseNetwork, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonCloseNetwork, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonCloseNetwork, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonCloseNetworkfImage = NULL;
        ui_ButtonCloseNetworkfImage = lv_image_create(ui_ButtonCloseNetwork);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonCloseNetworkfImage;
        lv_image_set_src(ui_ButtonCloseNetworkfImage, &ui_img_icons_lock_close_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonCloseNetworkfImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonCloseNetworkfImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonCloseNetworkfImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonCloseNetworkfImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonCloseNetworkfImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonCloseNetworkfImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSetup = NULL;
        ui_ButtonSetup = lv_button_create(ui_Container);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSetup;
        btns_obj->button_settings_index = btns_obj->on_create_elements_count-1;
        lv_obj_set_width(ui_ButtonSetup, 48);
        lv_obj_set_height(ui_ButtonSetup, 48);
        lv_obj_set_align(ui_ButtonSetup, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(ui_ButtonSetup, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(ui_ButtonSetup, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(ui_ButtonSetup, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_ButtonSetup, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSetup, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_ButtonSetup, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_ButtonSetup, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_ButtonSetup, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_ButtonSetup, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui_ButtonSetup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(ui_ButtonSetup, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* ui_ButtonSetupImage = NULL;
        ui_ButtonSetupImage = lv_image_create(ui_ButtonSetup);
        btns_obj->on_create_elements_count = btns_obj->on_create_elements_count + 1;
        btns_obj->on_create_elements_array = (lv_obj_t**)realloc(btns_obj->on_create_elements_array, btns_obj->on_create_elements_count * sizeof(lv_obj_t*));
        btns_obj->on_create_elements_array[btns_obj->on_create_elements_count-1] = ui_ButtonSetupImage;
        lv_image_set_src(ui_ButtonSetupImage, &ui_img_icons_settings_36dp_61c6ce_fill0_wght200_grad0_opsz40_png);
        lv_obj_set_width(ui_ButtonSetupImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(ui_ButtonSetupImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(ui_ButtonSetupImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(ui_ButtonSetupImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(ui_ButtonSetupImage, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_ButtonSetupImage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        lvgl_port_unlock();
        // ставим иконку в выключеном состоянии
        //MainPanelForDevicesWidget_buttonsContainer_set_state(btns_obj, 0);
        return btns_obj;
    } else return NULL;
}

esp_err_t MainPanelForDevicesWidget_buttonsContainer_set_state(ui_MainPanelForDevicesWidget_buttonsContainer_t* container, uint8_t state)
{
    if (lvgl_port_lock(0) == true)
    {
        if (state == 1)
        {
            lv_image_set_src(container->on_create_elements_array[container->img_network_state_index], &ui_img_icons_net_is_open_48dp_61c6ce_fill1_wght200_grad0_opsz48_png);
            lv_obj_set_style_shadow_color(container->on_create_elements_array[container->img_network_state_index], lv_color_hex(0xE5DC18), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_opa(container->on_create_elements_array[container->img_network_state_index], 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(container->on_create_elements_array[container->img_network_state_index], 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_spread(container->on_create_elements_array[container->img_network_state_index], 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_offset_x(container->on_create_elements_array[container->img_network_state_index], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_offset_y(container->on_create_elements_array[container->img_network_state_index], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }else 
        {
            lv_image_set_src(container->on_create_elements_array[container->img_network_state_index], &ui_img_icons_net_is_close_48dp_61c6ce_fill0_wght200_grad0_opsz48_png);
            lv_obj_set_style_shadow_width(container->on_create_elements_array[container->img_network_state_index], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_spread(container->on_create_elements_array[container->img_network_state_index], 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        lvgl_port_unlock();
        return ESP_OK;
    }else return ESP_FAIL;
}

esp_err_t ui_MainPanelForDevicesWidget_buttonsContainerDelete(ui_MainPanelForDevicesWidget_buttonsContainer_t* buttons_obj)
{
    if (lvgl_port_lock(0) == true)
        {
           lv_obj_delete(buttons_obj->on_create_elements_array[0]);
            for (int i = 0; i < buttons_obj->on_create_elements_count; i++)
            {
                free(buttons_obj->on_create_elements_array[i]);
                buttons_obj->on_create_elements_array[i] = NULL;
            }
            lvgl_port_unlock();
            return ESP_OK;
        } else return ESP_FAIL;
}

/*************************************************************** ui_MainPanelForDevicesMainSensorsPanel_t; ********************************************/
ui_MainPanelForDevicesMainSensorsPanel_t* MainPanelForDevicesMainSensorsPanelCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
     if (lvgl_port_lock(0) == true)
    {
        ui_MainPanelForDevicesMainSensorsPanel_t* panel_obj = NULL;
        panel_obj = calloc(1, sizeof(ui_MainPanelForDevicesMainSensorsPanel_t));
        if (panel_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }
        //panel_obj->buttons_for_sensor_widget_show_screen = NULL;
        panel_obj->on_create_elements_array = NULL;
        panel_obj->on_create_elements_count = 0;
        panel_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;

        lv_obj_t* ui_DevicessContainer = NULL;
        ui_DevicessContainer = lv_obj_create(parent);
        panel_obj->on_create_elements_count = panel_obj->on_create_elements_count + 1;
        
        panel_obj->on_create_elements_array = (lv_obj_t**)calloc(panel_obj->on_create_elements_count, sizeof(lv_obj_t*));    
        panel_obj->on_create_elements_array[panel_obj->on_create_elements_count-1] = ui_DevicessContainer;
        lv_obj_set_width(ui_DevicessContainer, 462);
        lv_obj_set_height(ui_DevicessContainer, 514);    /// 564LV_SIZE_CONTENT
        lv_obj_set_x(ui_DevicessContainer, 0);
        lv_obj_set_y(ui_DevicessContainer, 58);
        lv_obj_set_align(ui_DevicessContainer, LV_ALIGN_TOP_MID);
        lv_obj_set_flex_flow(ui_DevicessContainer, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(ui_DevicessContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        //lv_obj_remove_flag(ui_SensorsContainer, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_scroll_dir(ui_DevicessContainer, LV_DIR_VER);
        lv_obj_set_scrollbar_mode(ui_DevicessContainer, LV_SCROLLBAR_MODE_OFF);
        /*lv_obj_set_style_radius(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(ui_SensorsContainer, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_SensorsContainer, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_SensorsContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_SensorsContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_SensorsContainer, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);*/
    lv_obj_set_style_radius(ui_DevicessContainer, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_DevicessContainer, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_DevicessContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_DevicessContainer, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_DevicessContainer, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_DevicessContainer, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_DevicessContainer, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    /************* TEST FILL Container */
    for (int i = 0; i < 2; i++)
    {
        lv_obj_t* test_obj = lv_button_create(ui_DevicessContainer);
            lv_obj_set_width(test_obj, LV_SIZE_CONTENT);
            lv_obj_set_height(test_obj, LV_SIZE_CONTENT);
    }
        lv_obj_set_style_pad_left(ui_DevicessContainer, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_DevicessContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_DevicessContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_DevicessContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(ui_DevicessContainer, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(ui_DevicessContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(ui_DevicessContainer, lv_color_hex(0x2F5B7C), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(ui_DevicessContainer, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_DevicessContainer, lv_color_hex(0x61C6CE),
                                    LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(ui_DevicessContainer, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(ui_DevicessContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(ui_DevicessContainer, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(ui_DevicessContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(ui_DevicessContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(ui_DevicessContainer, 6, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(ui_DevicessContainer, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

        
        //panel_obj->buttons_for_sensor_widget_show_screen = MainPanelForSensorsWidget_buttonsContainerCreate(panel_obj->on_create_elements_array[0], panel_obj->parentScreenGlobalIndex);

        lvgl_port_unlock();
        return panel_obj;
    } else return NULL;
}
//esp_err_t ui_MainPanelForDevicesMainSensorsPanelDelete(ui_MainPanelForDevicesMainSensorsPanel_t* main_panel_obj);

/*************************************************************** ui_ScreenTopPanelObject_t; ***********************************************************/
ui_ScreenTopPanelObject_t* ScreenTopPanelObjectCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_ScreenTopPanelObject_t* panel_obj = NULL;
        panel_obj = calloc(1, sizeof(ui_ScreenTopPanelObject_t));
        if (panel_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }
        panel_obj->on_create_elements_count = 0;
        panel_obj->on_create_elements_array = NULL;
        panel_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;
        
       
        lv_obj_t* TopPanel = NULL;
        TopPanel = lv_obj_create(parent);
        panel_obj->on_create_elements_count = panel_obj->on_create_elements_count + 1;
        panel_obj->on_create_elements_array = (lv_obj_t**)calloc(panel_obj->on_create_elements_count, sizeof(lv_obj_t*));
        panel_obj->on_create_elements_array[panel_obj->on_create_elements_count-1] = TopPanel;
        lv_obj_set_width(TopPanel, 460);
        lv_obj_set_height(TopPanel, 106);
        lv_obj_set_x(TopPanel, 0);
        lv_obj_set_y(TopPanel, 36);
        lv_obj_set_align(TopPanel, LV_ALIGN_TOP_MID);
        lv_obj_set_flex_flow(TopPanel, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(TopPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_add_flag(TopPanel, LV_OBJ_FLAG_SCROLL_ONE);     /// Flags
        lv_obj_set_scroll_dir(TopPanel, LV_DIR_HOR);
        lv_obj_set_style_radius(TopPanel, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(TopPanel, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(TopPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(TopPanel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(TopPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(TopPanel, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(TopPanel, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(TopPanel, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(TopPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(TopPanel, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_bg_color(TopPanel, lv_color_hex(0x2F5B7C), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(TopPanel, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(TopPanel, lv_color_hex(0x61C6CE), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(TopPanel, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(TopPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(TopPanel, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(TopPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(TopPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(TopPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(TopPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

        panel_obj->top_panel_buttons = TopPanelWidget_buttonsCreate(panel_obj->on_create_elements_array[0], panel_obj->parentScreenGlobalIndex);
        //if(panel_obj->panel_type ==TOP_PANEL_FOR_SENSORS_WIDGET_SHOW)
        //{
            
            //ui_TopPanelForSensorsWidget_buttons_t* btns_obj = TopPanelForSensorsWidget_buttonsCreate(panel_obj->on_create_elements_array[0], panel_obj->parentScreenGlobalIndex);
            
        //}
        
        lvgl_port_unlock();
        return panel_obj;
    } else return NULL;
}

esp_err_t ui_ScreenTopPanelObjectDelete(ui_ScreenTopPanelObject_t* top_panel_obj)
{
    if (lvgl_port_lock(0) == true)
        {
            // удаляем элементы, которые могли добавиться
            //...
            // удаляем панель
            for (int i = 0; i < top_panel_obj->on_create_elements_count; i++) {
                lv_obj_del(top_panel_obj->on_create_elements_array[i]);
                top_panel_obj->on_create_elements_array[i] = NULL;
            }
            top_panel_obj->on_create_elements_array[0] = NULL;
            // удаляем структуру
            free(top_panel_obj);
            top_panel_obj = NULL;
            lvgl_port_unlock();
            return ESP_OK;
        } else return ESP_FAIL;
}

/*************************************************************** ui_ScreenMainPanelObject_t; ***********************************************************/
ui_ScreenMainPanelObject_t* ScreenMainPanelObjectCreate(lv_obj_t* parent, main_panel_type_t panel_type, uint8_t parentScreenGlobalIndex)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_ScreenMainPanelObject_t* panel_obj = calloc(1, sizeof(ui_ScreenMainPanelObject_t));
        if (panel_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }

        panel_obj->on_create_elements_count = 0;
        panel_obj->on_create_elements_array = NULL;
        panel_obj->parentScreenGlobalIndex = parentScreenGlobalIndex;
        panel_obj->panel_type = panel_type;
       
        /*lv_obj_t* TopPanel = NULL;
        TopPanel = lv_obj_create(parent);
        panel_obj->on_create_elements_count = panel_obj->on_create_elements_count + 1;
        panel_obj->on_create_elements_array = (lv_obj_t**)calloc(panel_obj->on_create_elements_count, sizeof(lv_obj_t*));
        panel_obj->on_create_elements_array[panel_obj->on_create_elements_count-1] = TopPanel;*/

        lv_obj_t* MainPanel = lv_obj_create(parent);
        panel_obj->on_create_elements_count = panel_obj->on_create_elements_count + 1;
        panel_obj->on_create_elements_array = (lv_obj_t**)calloc(panel_obj->on_create_elements_count, sizeof(lv_obj_t*));
        panel_obj->on_create_elements_array[panel_obj->on_create_elements_count-1] = MainPanel;
        lv_obj_set_width(MainPanel, 466);
        lv_obj_set_height(MainPanel, 712);
        lv_obj_set_x(MainPanel, 0);
        lv_obj_set_y(MainPanel, 46);
        lv_obj_set_align(MainPanel, LV_ALIGN_TOP_MID);
        lv_obj_set_flex_flow(MainPanel, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(MainPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_remove_flag(MainPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_scrollbar_mode(MainPanel, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_radius(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(MainPanel, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(MainPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(MainPanel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(MainPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(MainPanel, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(MainPanel, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(MainPanel, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(MainPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_border_color(MainPanel, lv_color_hex(0x61C6CE), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(MainPanel, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(MainPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(MainPanel, LV_BORDER_SIDE_NONE, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(MainPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(MainPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(MainPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(MainPanel, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
        
        panel_obj->top_panel_obj_pointer = ScreenTopPanelObjectCreate(panel_obj->on_create_elements_array[0], panel_obj->parentScreenGlobalIndex);

        if(panel_obj->panel_type ==MAIN_PANEL_FOR_SENSORS_WIDGET_SHOW)
        {
            panel_obj->buttons_for_sensor_widget_show_screen = MainPanelForSensorsWidget_buttonsContainerCreate(panel_obj->on_create_elements_array[0],panel_obj->parentScreenGlobalIndex);
            panel_obj->main_panel_for_sensor_widget_show_screen = MainPanelForSensorsMainSensorsPanelCreate(panel_obj->on_create_elements_array[0],panel_obj->parentScreenGlobalIndex);    
        }else
        if(panel_obj->panel_type ==MAIN_PANEL_FOR_DEVICES_WIDGET_SHOW)
        {
            panel_obj->buttons_for_devices_widget_show_screen = MainPanelForDevicesWidget_buttonsContainerCreate(panel_obj->on_create_elements_array[0],panel_obj->parentScreenGlobalIndex);
            panel_obj->main_panel_for_devices_widget_show_screen = MainPanelForDevicesMainSensorsPanelCreate(panel_obj->on_create_elements_array[0],panel_obj->parentScreenGlobalIndex);    
        }


       
        lvgl_port_unlock();
        return panel_obj;
    } else return NULL;
}

esp_err_t ui_ScreenMainPanelObjectDelete(ui_ScreenMainPanelObject_t* main_panel_obj)
{
    if (lvgl_port_lock(0) == true)
        {
            // удаляем элементы, которые могли добавиться
            //...
            // удаляем панель
            for (int i = 0; i < main_panel_obj->on_create_elements_count; i++) {
                lv_obj_del(main_panel_obj->on_create_elements_array[i]);
                main_panel_obj->on_create_elements_array[i] = NULL;
            }
            main_panel_obj->on_create_elements_array[0] = NULL;
            // удаляем структуру
            free(main_panel_obj);
            main_panel_obj = NULL;
            lvgl_port_unlock();
            return ESP_OK;
        } else return ESP_FAIL;
}