idf_component_register(SRC_DIRS "src" "src/ha" "src/zcl" "src/zdo" "devices"
                       INCLUDE_DIRS "include" "include/ha" "include/zcl" "include/zdo" "devices/include"
                       PRIV_INCLUDE_DIRS "src/priv"
                       PRIV_REQUIRES nvs_flash driver event_post_send esp_timer)