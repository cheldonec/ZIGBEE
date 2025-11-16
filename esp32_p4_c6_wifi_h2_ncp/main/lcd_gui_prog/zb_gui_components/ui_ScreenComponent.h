#ifndef UI_SCREENCOMPONENT_H
#define UI_SCREENCOMPONENT_H
#include "esp_err.h"
#include "ui_PanelComponent.h"
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************** EVENTS ***********************************************************/
#define UI_SCREEN_EVENTS   (esp_event_base_t)("UI_SCREEN_EVENTS")
//const esp_event_base_t zb_action_handler_event_group = "ZB_ACTION_HANDLER_EVENTS";

// Определение конкретных событий
enum {
    UI_SCREEN_TP_HOME_CLICK,
    UI_SCREEN_TP_DEVICES_CLICK,
    UI_SCREEN_MP_NETWORK_OPEN_CLICK,
    UI_SCREEN_MP_NETWORK_CLOSE_CLICK,
};

typedef enum {
    SCREEN_EMPTY                   = 0,
    SCREEN_FOR_SENSORS_WIDGET_SHOW = 1,
    SCREEN_FOR_DEVICES_WIDGET_SHOW = 2,
}screen_type_t;

typedef struct {    
    uint8_t                         on_create_elements_count; // количество элементов в объекте, только при создании (для дальнейшего освобождения памяти). Динамически добавленные не учитываются
    lv_obj_t**                      on_create_elements_array;
    uint8_t                         MainLabelIndex;
    uint8_t                         BottomImageIndex;
    ui_ScreenTopPanelObject_t*      top_panel_obj_pointer;
    ui_ScreenMainPanelObject_t*     main_panel_obj_pointer;
    uint8_t                         ScreenGlobalIndex;
    screen_type_t                   screen_type;

}ui_ScreenObject_t;

ui_ScreenObject_t* ui_ScreenObjectCreate(const char* name, uint8_t global_index, screen_type_t screen_type);
esp_err_t ui_ScreenObjectLoadScreen(ui_ScreenObject_t* screen_obj); // lv_disp_load_scr

esp_err_t ui_ScreenObjectDelete(ui_ScreenObject_t* screen_obj);
esp_err_t ui_ScreenObjectSetScreenName(ui_ScreenObject_t* screen_obj, const char* name);
esp_err_t ui_ScreenObjectSetTopPanel(ui_ScreenObject_t* screen_obj, ui_ScreenTopPanelObject_t* top_panel_obj);
esp_err_t ui_ScreenObjectSetMainPanel(ui_ScreenObject_t* screen_obj, ui_ScreenMainPanelObject_t* main_panel_obj);


#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif // UI_SCREENCOMPONENT_H