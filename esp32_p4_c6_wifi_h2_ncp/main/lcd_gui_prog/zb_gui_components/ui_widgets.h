#ifndef UI_WIDGETS_H

#define UI_WIDGETS_H

#include "esp_err.h"
#include "zb_manager_devices.h"


typedef struct ui_device_for_devices_screen_widget_s {
        lv_obj_t*               device_main_widget;                         // lv_obj_t* device_widget
        lv_obj_t*               devfriendlynamecontainer;                   // in device_main_widget
        lv_obj_t*               friendlynamelabel;                          // in devfriendlynamecontainer
        lv_obj_t*               WidgetSetupButton;                          // in devfriendlynamecontainer
        lv_obj_t*               WidgetSetupButtonImage;                     // in WidgetSetupButtonImage
        device_custom_t*        data_source;
        uint8_t                 parentScreenGlobalIndex;
} ui_device_for_devices_screen_widget_t;

ui_device_for_devices_screen_widget_t* ui_device_for_devices_screen_widget_create(lv_obj_t* parent, device_custom_t* data_source, uint8_t parentScreenGlobalIndex);//// parent widget is 

#endif