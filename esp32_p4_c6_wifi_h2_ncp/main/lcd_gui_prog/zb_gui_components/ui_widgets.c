#include "../ui.h"
#include "esp_lvgl_port.h"
#include "ui_widgets.h"

ui_device_for_devices_screen_widget_t* ui_device_for_devices_screen_widget_create(lv_obj_t* parent, device_custom_t* data_source, uint8_t parentScreenGlobalIndex)
{
    if (data_source == NULL) return NULL;
    if (lvgl_port_lock(0) == true)
    {

        ui_device_for_devices_screen_widget_t* device_widget = NULL;
        device_widget = calloc(1, sizeof(ui_device_for_devices_screen_widget_t));
        if(device_widget == NULL) {
            lvgl_port_unlock();
            return NULL;}
        device_widget->device_main_widget = NULL;
        device_widget->friendlynamelabel = NULL;
        device_widget->WidgetSetupButton = NULL;
        device_widget->devfriendlynamecontainer = NULL;
        device_widget->WidgetSetupButtonImage = NULL;
        device_widget->data_source = data_source;
        device_widget->parentScreenGlobalIndex = parentScreenGlobalIndex;
        device_widget->device_main_widget = lv_obj_create(parent);
        lv_obj_remove_style_all(device_widget->device_main_widget);
        lv_obj_set_width(device_widget->device_main_widget, 408);
        lv_obj_set_height(device_widget->device_main_widget, LV_SIZE_CONTENT);    /// 50
        lv_obj_set_align(device_widget->device_main_widget, LV_ALIGN_CENTER);
        lv_obj_set_flex_flow(device_widget->device_main_widget, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(device_widget->device_main_widget, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
        lv_obj_remove_flag(device_widget->device_main_widget, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_radius(device_widget->device_main_widget, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(device_widget->device_main_widget, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(device_widget->device_main_widget, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(device_widget->device_main_widget, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(device_widget->device_main_widget, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(device_widget->device_main_widget, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        device_widget->devfriendlynamecontainer = lv_obj_create(device_widget->device_main_widget);
        lv_obj_remove_style_all(device_widget->devfriendlynamecontainer);
        lv_obj_set_width(device_widget->devfriendlynamecontainer, 406);
        lv_obj_set_height(device_widget->devfriendlynamecontainer, 50);
        lv_obj_set_align(device_widget->devfriendlynamecontainer, LV_ALIGN_CENTER);
        lv_obj_remove_flag(device_widget->devfriendlynamecontainer, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        device_widget->WidgetSetupButton = lv_button_create(device_widget->devfriendlynamecontainer);
        lv_obj_set_width(device_widget->WidgetSetupButton, 36);
        lv_obj_set_height(device_widget->WidgetSetupButton, 36);
        lv_obj_set_x(device_widget->WidgetSetupButton, -6);
        lv_obj_set_y(device_widget->WidgetSetupButton, 0);
        lv_obj_set_align(device_widget->WidgetSetupButton, LV_ALIGN_RIGHT_MID);
        lv_obj_add_flag(device_widget->WidgetSetupButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
        lv_obj_remove_flag(device_widget->WidgetSetupButton, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(device_widget->WidgetSetupButton, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(device_widget->WidgetSetupButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(device_widget->WidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_spread(device_widget->WidgetSetupButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        device_widget->WidgetSetupButtonImage = lv_image_create(device_widget->WidgetSetupButton);
        lv_image_set_src(device_widget->WidgetSetupButtonImage, &ui_img_icons_settings_24dp_61c6ce_fill0_wght200_grad0_opsz24_png);
        lv_obj_set_width(device_widget->WidgetSetupButtonImage, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(device_widget->WidgetSetupButtonImage, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(device_widget->WidgetSetupButtonImage, LV_ALIGN_CENTER);
        lv_obj_remove_flag(device_widget->WidgetSetupButtonImage, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE |
                        LV_OBJ_FLAG_SCROLLABLE);      /// Flags

        device_widget->friendlynamelabel = lv_label_create(device_widget->devfriendlynamecontainer);
        lv_obj_set_width(device_widget->friendlynamelabel, 406);
        lv_obj_set_height(device_widget->friendlynamelabel, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_align(device_widget->friendlynamelabel, LV_ALIGN_CENTER);
        lv_label_set_text(device_widget->friendlynamelabel, data_source->friendly_name);
        lv_obj_set_style_text_color(device_widget->friendlynamelabel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(device_widget->friendlynamelabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(device_widget->friendlynamelabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(device_widget->friendlynamelabel, &ui_font_RobotoBold18, LV_PART_MAIN | LV_STATE_DEFAULT);
        lvgl_port_unlock();
        return device_widget;
    }
    return NULL;
}