

idf_component_register(
    INCLUDE_DIRS "." "lcd_gui_prog" #"esp32_p4_function_ev_board" "esp32_p4_function_ev_board/include" "esp32_p4_function_ev_board/include/bsp"
    #PRIV_INCLUDE_DIRS "esp32_p4_function_ev_board/priv_include"
    #REQUIRES "esp_psram" "spiffs" "nvs_flash"
    SRCS 
                        #"esp32_p4_function_ev_board/esp32_p4_function_ev_board.c"
                        "esp_zb_host.c"
                        "lcd_gui_prog/fonts/ui_font_Calibri12.c"
                        "lcd_gui_prog/fonts/ui_font_Calibri14.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBold14.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBold18.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBold16.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBold24.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoMedium12.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoMedium14.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoMedium48.c"
                        "lcd_gui_prog/fonts/ui_font_CalibriBold22.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBold12.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoBlack22.c"
                        #"lcd_gui_prog/fonts/ui_font_RobotoBold14.c"
                        #"lcd_gui_prog/fonts/ui_font_RobotoBold14.c"
                        #"lcd_gui_prog/fonts/ui_font_RobotoBold14.c"
                        
                        #"lcd_gui_prog/screens/ui_ScreenMainSettings.c"
                        #"lcd_gui_prog/screens/ui_TemplaitesScreen1.c"
                        #"lcd_gui_prog/screens/ui_Screen4.c"
                        "lcd_gui_prog/screens/ui_MainScreen.c"
                        "lcd_gui_prog/screens/ui_RoomsScreen.c"
                        "lcd_gui_prog/zb_gui_components/ui_ScreenComponent.c"
                        "lcd_gui_prog/zb_gui_components/ui_PanelComponent.c"
                        "lcd_gui_prog/zb_gui_components/ui_widgets.c"
                        "lcd_gui_prog/fonts/ui_font_RobotoCyrInfoLabels.c"

                        "lcd_gui_prog/ui.c"
                        "lcd_gui_prog/components/ui_comp_hook.c"
                        "lcd_gui_prog/ui_helpers.c"
                        "lcd_gui_prog/ui_events.c"
                        #"lcd_gui_prog/fonts/ui_font_RobotoCyrTitles.c"
                        #"lcd_gui_prog/fonts/ui_font_RobotoCyrInfoLabels.c"

                        #"lcd_gui_prog/lvgl_test_screen.c" 
                        "lcd_gui_prog/lvgl_port_zb_interface.c"
                        "lcd_gui_prog/lvgl_port_gui_init.c"
                        #"lcd_gui_prog/gui_top_menu.c" 
                        #"lcd_gui_prog/gui_main_screen.c" 
                        #"gpio_buttons.c" 
                        #"zb_manager_callbacks.c" 
                        
                       
                       #PRIV_REQUIRES "esp_psram"
                       #REQUIRES "nvs_flash"
                       )
                    lvgl_port_add_images(${COMPONENT_LIB} "lcd_gui_prog/images/")
