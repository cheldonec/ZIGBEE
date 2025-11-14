#include "../ui.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"
#include "ui_ScreenComponent.h"
#include "event_post_send.h"

// жест сверху вниз для смены видимости TopPanel
void ui_event_TopLableContainerClick_CB(lv_event_t * e)
{
  //ui_ScreenTopPanelObject_t* 
  lv_event_code_t event_code = lv_event_get_code(e);
  ui_ScreenObject_t* ui_Screen = (ui_ScreenObject_t*)lv_event_get_user_data(e);
  if(event_code == LV_EVENT_CLICKED) {
        lv_indev_wait_release(lv_indev_active());
        if (lvgl_port_lock(0) == true)
        {    //ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->on_create_elements_array[0]
            _ui_flag_modify(ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->on_create_elements_array[0], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
            lv_obj_t* temp_container =ui_Screen->main_panel_obj_pointer->main_panel_for_sensor_widget_show_screen->on_create_elements_array[0]; 
            int32_t temp_height = lv_obj_get_height(temp_container);
            if (temp_height < 520) {lv_obj_set_height(temp_container, 112 + temp_height);} else lv_obj_set_height(temp_container, 514);
            
            lvgl_port_unlock();
        }
    }
    /*if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_BOTTOM) {
        lv_indev_wait_release(lv_indev_active());
        if (lvgl_port_lock(0) == true)
        { 
            _ui_flag_modify(ui_ScreenTopPanel->on_create_elements_array[0], LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
            lvgl_port_unlock();
        }

    }*/
}

void ui_event_TopPanelBtnClick_CB(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    ui_ScreenObject_t* ui_Screen = (ui_ScreenObject_t*)lv_event_get_user_data(e);
    lv_obj_t* btn = lv_event_get_current_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        lv_indev_wait_release(lv_indev_active());
        if (btn == ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->on_create_elements_array[ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->button_home_index])
        {
            eventLoopPost(UI_SCREEN_EVENTS, UI_SCREEN_TP_HOME_CLICK, ui_Screen, sizeof(ui_ScreenObject_t), portMAX_DELAY);
            ESP_LOGI ("ui_Screen_component", "UI_SCREEN_TP_HOME_CLICK Button");
        } else
        if (btn == ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->on_create_elements_array[ui_Screen->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->button_devices_index])
        {
            eventLoopPost(UI_SCREEN_EVENTS, UI_SCREEN_TP_DEVICES_CLICK, ui_Screen, sizeof(ui_ScreenObject_t), portMAX_DELAY);
            ESP_LOGI ("ui_Screen_component", "UI_SCREEN_TP_DEVICES_CLICK Button");
        }   
    }
    //eventLoopPost(ZB_ACTION_HANDLER_EVENTS, ATTR_REPORT_EVENT, NULL, 0, portMAX_DELAY);
}
ui_ScreenObject_t* ui_ScreenObjectCreate(const char* name, uint8_t global_index, screen_type_t screen_type)
{
    if (lvgl_port_lock(0) == true)
    {
        ui_ScreenObject_t* screen_obj = NULL;
        screen_obj = calloc(1, sizeof(ui_ScreenObject_t));
        if (screen_obj == NULL) 
        {
            lvgl_port_unlock();
            return NULL;
        }
        screen_obj->on_create_elements_count = 0;
        screen_obj->MainLabelIndex = 0;
        screen_obj->BottomImageIndex = 0;
        screen_obj->top_panel_obj_pointer = NULL;
        screen_obj->main_panel_obj_pointer = NULL;
        screen_obj->on_create_elements_array = NULL;
        screen_obj->ScreenGlobalIndex = global_index;
        screen_obj->screen_type = screen_type;

        lv_obj_t* screen = lv_obj_create(NULL);
        screen_obj->on_create_elements_count = screen_obj->on_create_elements_count + 1;
        screen_obj->on_create_elements_array = (lv_obj_t**)calloc(screen_obj->on_create_elements_count, sizeof(lv_obj_t*));
        screen_obj->on_create_elements_array[screen_obj->on_create_elements_count-1] = screen;
        lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        lv_obj_set_style_bg_color(screen, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(screen, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(screen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        
        lv_obj_t* TopLable_Container = lv_obj_create(screen);
        screen_obj->on_create_elements_count = screen_obj->on_create_elements_count + 1;
        screen_obj->on_create_elements_array = (lv_obj_t**)realloc(screen_obj->on_create_elements_array, screen_obj->on_create_elements_count * sizeof(lv_obj_t*));
        screen_obj->on_create_elements_array[screen_obj->on_create_elements_count-1] = TopLable_Container;
        lv_obj_remove_style_all(TopLable_Container);
        lv_obj_set_width(TopLable_Container, 460);
        lv_obj_set_height(TopLable_Container, 42);
        lv_obj_set_align(TopLable_Container, LV_ALIGN_TOP_MID);
        lv_obj_remove_flag(TopLable_Container, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);   /// Flags
        lv_obj_set_style_radius(TopLable_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(TopLable_Container, lv_color_hex(0x2F5B7C), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(TopLable_Container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(TopLable_Container, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(TopLable_Container, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(TopLable_Container, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(TopLable_Container, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);
        

        lv_obj_t* TopLabel = lv_label_create(TopLable_Container);
        screen_obj->on_create_elements_count = screen_obj->on_create_elements_count + 1;
        screen_obj->on_create_elements_array = (lv_obj_t**)realloc(screen_obj->on_create_elements_array, screen_obj->on_create_elements_count * sizeof(lv_obj_t*));
        screen_obj->on_create_elements_array[screen_obj->on_create_elements_count-1] = TopLabel;
        screen_obj->MainLabelIndex = screen_obj->on_create_elements_count-1;
        lv_obj_set_width(TopLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(TopLabel, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(TopLabel, 0);
        lv_obj_set_y(TopLabel, 6);
        lv_obj_set_align(TopLabel, LV_ALIGN_TOP_MID);
        lv_label_set_text(TopLabel, name);
        lv_obj_set_style_text_color(TopLabel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(TopLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(TopLabel, &ui_font_CalibriBold22, LV_PART_MAIN | LV_STATE_DEFAULT);
        

        lv_obj_t* bottom_image = lv_image_create(screen);
        screen_obj->on_create_elements_count = screen_obj->on_create_elements_count + 1;
        screen_obj->on_create_elements_array = (lv_obj_t**)realloc(screen_obj->on_create_elements_array, screen_obj->on_create_elements_count * sizeof(lv_obj_t*));
        screen_obj->on_create_elements_array[screen_obj->on_create_elements_count-1] = bottom_image;
        lv_image_set_src(bottom_image, &ui_img_bg_line_bottom_png);
        lv_obj_set_width(bottom_image, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(bottom_image, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(bottom_image, 0);
        lv_obj_set_y(bottom_image, 380);
        lv_obj_set_align(bottom_image, LV_ALIGN_CENTER);
        lv_obj_add_flag(bottom_image, LV_OBJ_FLAG_CLICKABLE);     /// Flags
        lv_obj_remove_flag(bottom_image, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
        
        lv_obj_t* bottomLabel = lv_label_create(bottom_image);
        screen_obj->on_create_elements_count = screen_obj->on_create_elements_count + 1;
        screen_obj->on_create_elements_array = (lv_obj_t**)realloc(screen_obj->on_create_elements_array, screen_obj->on_create_elements_count * sizeof(lv_obj_t*));
        screen_obj->on_create_elements_array[screen_obj->on_create_elements_count-1] = bottomLabel;
        screen_obj->BottomImageIndex = screen_obj->on_create_elements_count-1;
        lv_obj_set_width(bottomLabel, LV_SIZE_CONTENT);   /// 1
        lv_obj_set_height(bottomLabel, LV_SIZE_CONTENT);    /// 1
        lv_obj_set_x(bottomLabel, 0);
        lv_obj_set_y(bottomLabel, 12);
        lv_obj_set_align(bottomLabel, LV_ALIGN_TOP_MID);
        lv_label_set_text(bottomLabel, "13.11.2025 09:07");
        lv_obj_set_style_text_color(bottomLabel, lv_color_hex(0x61C6CE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(bottomLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(bottomLabel, &ui_font_RobotoMedium14, LV_PART_MAIN | LV_STATE_DEFAULT);
       
        //screen_obj->on_create_elements_array = NULL;
        //screen_obj->on_create_elements_array = (lv_obj_t**)calloc(screen_obj->on_create_elements_count, sizeof(lv_obj_t*));
        /*if (screen_obj->on_create_elements_array == NULL) {
            free(screen_obj);
            screen_obj = NULL;
            lv_obj_del(screen);
            TopLabel = NULL;
            bottom_image = NULL;
            screen = NULL;
            return NULL;
        }*/
       /********** Panels Create *************************/
        screen_obj->main_panel_obj_pointer = NULL;
        screen_obj->top_panel_obj_pointer = NULL;
        if (screen_type == SCREEN_FOR_SENSORS_WIDGET_SHOW)
        {
            screen_obj->main_panel_obj_pointer = ScreenMainPanelObjectCreate(screen, MAIN_PANEL_FOR_SENSORS_WIDGET_SHOW, screen_obj->ScreenGlobalIndex);

            lv_obj_add_event_cb(screen_obj->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->on_create_elements_array[screen_obj->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->button_home_index],
            ui_event_TopPanelBtnClick_CB, LV_EVENT_ALL, screen_obj);

            lv_obj_add_event_cb(screen_obj->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->on_create_elements_array[screen_obj->main_panel_obj_pointer->top_panel_obj_pointer->top_panel_buttons->button_devices_index],
            ui_event_TopPanelBtnClick_CB, LV_EVENT_ALL, screen_obj);
            //screen_obj->top_panel_obj_pointer = ScreenTopPanelObjectCreate(screen_obj->main_panel_obj_pointer->on_create_elements_array[0], TOP_PANEL_FOR_SENSORS_WIDGET_SHOW);
        }
        //screen_obj->main_panel_obj_pointer->parentScreenGlobalIndex = screen_obj->ScreenGlobalIndex;
        //screen_obj->top_panel_obj_pointer->parentScreenGlobalIndex = screen_obj->ScreenGlobalIndex;
        
        // событие для показа/скрытия TopPanel (дом, устройства...)
        lv_obj_add_event_cb(TopLable_Container, ui_event_TopLableContainerClick_CB, LV_EVENT_ALL, screen_obj);
        lvgl_port_unlock();
        return screen_obj;
    } else return NULL;
}


esp_err_t ui_ScreenObjectLoadScreen(ui_ScreenObject_t* screen_obj)
{
        if (lvgl_port_lock(0) == true)
        {
            lv_scr_load(screen_obj->on_create_elements_array[0]);
            lvgl_port_unlock();
            return ESP_OK;
        }else return ESP_FAIL;
}

esp_err_t ui_ScreenObjectDelete(ui_ScreenObject_t* screen_obj)
{
        if (lvgl_port_lock(0) == true)
        {
            // сам экран пока не удаляем [0]
            // удаляем элементы созданные вначале
            for (int i = 1; i < screen_obj->on_create_elements_count; i++) {
                lv_obj_del(screen_obj->on_create_elements_array[i]);
                screen_obj->on_create_elements_array[i] = NULL;
            }
            // удаляем элементы, которые могли добавиться
            //...
            // удаляем сам экран [0]
            lv_obj_del(screen_obj->on_create_elements_array[0]);
            screen_obj->on_create_elements_array[0] = NULL;
            // удаляем структуру
            free(screen_obj);
            screen_obj = NULL;
            lvgl_port_unlock();
            return ESP_OK;
        }else return ESP_FAIL;
}

esp_err_t ui_ScreenObjectSetScreenName(ui_ScreenObject_t* screen_obj, const char* name)
{
    if (lvgl_port_lock(0) == true)
    {
        lv_label_set_text(screen_obj->on_create_elements_array[screen_obj->MainLabelIndex], name);
        lvgl_port_unlock();
        return ESP_OK;
    } else return ESP_FAIL;
}

esp_err_t ui_ScreenObjectSetTopPanel(ui_ScreenObject_t* screen_obj, ui_ScreenTopPanelObject_t* top_panel_obj)
{
        screen_obj->top_panel_obj_pointer = top_panel_obj;
        if (lvgl_port_lock(0) == true)
        {
            
            lv_obj_set_parent(screen_obj->top_panel_obj_pointer->on_create_elements_array[0], screen_obj->on_create_elements_array[0]);
            screen_obj->top_panel_obj_pointer->parentScreenGlobalIndex = screen_obj->ScreenGlobalIndex;
            lvgl_port_unlock();
            return ESP_OK;
        } else return ESP_FAIL;
}

esp_err_t ui_ScreenObjectSetMainPanel(ui_ScreenObject_t* screen_obj, ui_ScreenMainPanelObject_t* main_panel_obj)
{
        screen_obj->main_panel_obj_pointer = main_panel_obj;
        if (lvgl_port_lock(0) == true)
        {
            lv_obj_set_parent(screen_obj->main_panel_obj_pointer->on_create_elements_array[0], screen_obj->on_create_elements_array[0]);
            screen_obj->main_panel_obj_pointer->parentScreenGlobalIndex = screen_obj->ScreenGlobalIndex;
            lvgl_port_unlock();
                return ESP_OK;
        } else return ESP_FAIL;
}