#ifndef UI_PANELCOMPONENT_H
#define UI_PANELCOMPONENT_H
#include "esp_err.h"
#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************** ui_TopPanelForSensorsWidget_buttons_t; ************************************************/
typedef struct {
    uint8_t                 on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**              on_create_elements_array;
    uint8_t                 parentScreenGlobalIndex;
    uint8_t                 button_home_index;
    uint8_t                 button_devices_index;
    uint8_t                 button_switch_all_off_index;
}ui_TopPanelWidget_buttons_t;
 
ui_TopPanelWidget_buttons_t* TopPanelWidget_buttonsCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);
esp_err_t ui_TopPanelWidget_buttonsDelete(ui_TopPanelWidget_buttons_t* buttons_obj);

/*************************************************************** ui_MainPanelForSensorsWidget_buttons_t; ************************************************/
typedef struct {
    uint8_t                 on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**              on_create_elements_array;
    uint8_t                 parentScreenGlobalIndex;
    uint8_t                 button_add_widget_index;
    uint8_t                 button_switch_all_on_index;
    uint8_t                 button_switch_all_off_index;
    uint8_t                 button_settings_index;
}ui_MainPanelForSensorsWidget_buttonsContainer_t;
 
ui_MainPanelForSensorsWidget_buttonsContainer_t* MainPanelForSensorsWidget_buttonsContainerCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);
esp_err_t ui_MainPanelForSensorsWidget_buttonsContainerDelete(ui_MainPanelForSensorsWidget_buttonsContainer_t* buttons_obj);

/*************************************************************** ui_MainPanelForSensorsMainSensorsPanel_t; ********************************************/
typedef struct {
    uint8_t                                             on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**                                          on_create_elements_array;
    uint8_t                                             parentScreenGlobalIndex;
    //ui_MainPanelForSensorsWidget_buttonsContainer_t*    buttons_for_sensor_widget_show_screen;
}ui_MainPanelForSensorsMainSensorsPanel_t;

ui_MainPanelForSensorsMainSensorsPanel_t* MainPanelForSensorsMainSensorsPanelCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);
esp_err_t ui_MainPanelForSensorsMainSensorsPanelDelete(ui_MainPanelForSensorsMainSensorsPanel_t* main_panel_obj);

/*************************************************************** ui_MainPanelForDevicesWidget_buttons_t; ************************************************/
typedef struct {
    uint8_t                 on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**              on_create_elements_array;
    uint8_t                 parentScreenGlobalIndex;
    uint8_t                 img_network_state_index;
    uint8_t                 button_open_network_index;
    uint8_t                 button_close_network_index;
    uint8_t                 button_settings_index;
}ui_MainPanelForDevicesWidget_buttonsContainer_t;
 
ui_MainPanelForDevicesWidget_buttonsContainer_t* MainPanelForDevicesWidget_buttonsContainerCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);
esp_err_t MainPanelForDevicesWidget_buttonsContainer_set_state(ui_MainPanelForDevicesWidget_buttonsContainer_t* container, uint8_t state); // меняеет иконку 0-выключена

/*************************************************************** ui_MainPanelForDevicesMainSensorsPanel_t; ********************************************/
typedef struct {
    uint8_t                                             on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**                                          on_create_elements_array;
    uint8_t                                             parentScreenGlobalIndex;
    //ui_MainPanelForSensorsWidget_buttonsContainer_t*    buttons_for_sensor_widget_show_screen;
}ui_MainPanelForDevicesMainSensorsPanel_t;

ui_MainPanelForDevicesMainSensorsPanel_t* MainPanelForDevicesMainSensorsPanelCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);
esp_err_t ui_MainPanelForDevicesMainSensorsPanelDelete(ui_MainPanelForDevicesMainSensorsPanel_t* main_panel_obj);
/*************************************************************** ui_ScreenTopPanelObject_t; ***********************************************************/

typedef struct {
    uint8_t                                 on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**                              on_create_elements_array;
    uint8_t                                 parentScreenGlobalIndex;
    ui_TopPanelWidget_buttons_t*            top_panel_buttons;
}ui_ScreenTopPanelObject_t;

ui_ScreenTopPanelObject_t* ScreenTopPanelObjectCreate(lv_obj_t* parent, uint8_t parentScreenGlobalIndex);

esp_err_t ui_ScreenTopPanelObjectDelete(ui_ScreenTopPanelObject_t* top_panel_obj);

/*************************************************************** ui_ScreenMainPanelObject_t; ***********************************************************/
typedef enum {
    MAIN_PANEL_EMPTY                   = 0,
    MAIN_PANEL_FOR_SENSORS_WIDGET_SHOW = 1,
    MAIN_PANEL_FOR_DEVICES_WIDGET_SHOW = 2,
}main_panel_type_t;
typedef struct {
    uint8_t                     on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**                  on_create_elements_array;
    uint8_t                     parentScreenGlobalIndex;
    main_panel_type_t           panel_type;
    ui_ScreenTopPanelObject_t*                          top_panel_obj_pointer;
    ui_MainPanelForSensorsMainSensorsPanel_t*           main_panel_for_sensor_widget_show_screen;
    ui_MainPanelForSensorsWidget_buttonsContainer_t*    buttons_for_sensor_widget_show_screen;

    ui_MainPanelForDevicesMainSensorsPanel_t*           main_panel_for_devices_widget_show_screen;
    ui_MainPanelForDevicesWidget_buttonsContainer_t*    buttons_for_devices_widget_show_screen;
    
}ui_ScreenMainPanelObject_t;

ui_ScreenMainPanelObject_t* ScreenMainPanelObjectCreate(lv_obj_t* parent, main_panel_type_t panel_type, uint8_t parentScreenGlobalIndex);

esp_err_t ui_ScreenMainPanelObjectDelete(ui_ScreenMainPanelObject_t* main_panel_obj);

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif // UI_PANELCOMPONENT_H