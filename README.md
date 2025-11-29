**СТАТУС : В РАБОТЕ**

**DIY ZIGBEE панель для контроля и управлениями на базе**

**ESP32-P4 + ESP32-C6 Модуль с сенсорным экраном 4.3’’**

**И ESP32-H2 supermini**
![alt text](/readme_resource/esp32-P4-C6-H2.bmp)

**Ресурсы**

1.  Модуль с экраном (примеры, схемы)

http://pan.jczn1688.com/1/HMI%20display/JC4880P443C_I_W.zip

1.  LVGL PORT

https://components.espressif.com/components/espressif/esp_lvgl_port/versions/2.6.3/readme

1.  NCP (используется с изменениями)

https://github.com/espressif/esp-zigbee-sdk/tree/main/components/esp-zigbee-ncp

1.  Официальные примеры из esp_zigbee_sdk

https://github.com/espressif/esp-zigbee-sdk/tree/main/examples/esp_zigbee_host

https://github.com/espressif/esp-zigbee-sdk/tree/main/examples/esp_zigbee_ncp

1.  Графический дизайнер

https://squareline.io/

**Цель проекта**

Сенсорная панель ZIGBEE в режиме координатора с возможностью формирования сети, добавления устройств, настройки связей, мониторинга состояний и управления. На данном этапе подразумевается поддержка кластеров температуры, влажности и on/off

**Основные решения**

В составе модуля с дисплеем есть чип ESP32-C6, который тоже поддерживает ZIGBEE, однако оказалось предпочтительным использовать внешний модуль ESP32-H2, чтобы оставить возможность подключения к внешнему миру через WI-FI.

ESP32-H2 подключается в качестве NCP так как на начало реализации в esp-zigbee-sdk не было поддержки RCP для чипа ESP32-P4. (RCP был бы предпочтительней)

Базовые элементы графики рисовались в squareline studio, однако вся отрисовка в итоге происходит «вручную»

Для реализации обмена с NCP использованы стандартные компоненты и примеры esp-zigbee-sdk со значительной доработкой, поэтому исключена обратная совместимость.

**Состав репозитория**

Программа для модуля ESP32-P4

https://github.com/cheldonec/ZIGBEE/tree/main/esp32_p4_c6_wifi_h2_ncp

Программа для ESP32-H2 (NCP)

https://github.com/cheldonec/ZIGBEE/tree/main/zb_manager_ncp_h2

NB!!! **Проект находится в работе и на данный момент не реализует все поставленные цели.**

**Что сделано**

1.  Отрисовка главного экрана
2.  Отрисовка экрана устройств
3.  Открытие/закрытие сети для сопряжения устройств
4.  Добавлена отправка esp_event_post в esp_event_loop по событиям gui
5.  Отображение добавляемых устройств (в простейшем виде)
6.  NCP HOST добавлена обработка active_ep_req, simple_desc_req, read_attr_resp, report_attr. Добавлена отправка esp_event_post в esp_event_loop по реализованным событиям zigbee.
7.  Реализован основной алгоритм добавления устройства с поддержкой TUYA MAGIC и добавление в структуру устройств. (сама структура пока не сохраняется на flash!!! В работе)
