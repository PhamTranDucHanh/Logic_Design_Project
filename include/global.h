#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern volatile bool human_detected;
extern float glob_temperature;
extern float glob_humidity;
extern float glob_light_value;
extern String glob_light_level;

extern String WIFI_SSID;
extern String WIFI_PASS;
extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern SemaphoreHandle_t xBinarySemaphoreNeoPixel;

// NeoPixel config
extern String glob_neopixel_mode;
extern String glob_neopixel_effect;
// Temp ranges
extern int glob_temp_low_min, glob_temp_low_max;
extern int glob_temp_mid_min, glob_temp_mid_max;
extern int glob_temp_high_min, glob_temp_high_max;
// Humi ranges
extern int glob_humi_low_min, glob_humi_low_max;
extern int glob_humi_mid_min, glob_humi_mid_max;
extern int glob_humi_high_min, glob_humi_high_max;
// Light ranges
extern int glob_light_low_min, glob_light_low_max;
extern int glob_light_mid_min, glob_light_mid_max;
extern int glob_light_high_min, glob_light_high_max;

// NeoPixel color (RGB)
extern int glob_neopixel_r, glob_neopixel_g, glob_neopixel_b;
// Temp RGB
extern int glob_temp_low_r, glob_temp_low_g, glob_temp_low_b;
extern int glob_temp_mid_r, glob_temp_mid_g, glob_temp_mid_b;
extern int glob_temp_high_r, glob_temp_high_g, glob_temp_high_b;
// Humi RGB
extern int glob_humi_low_r, glob_humi_low_g, glob_humi_low_b;
extern int glob_humi_mid_r, glob_humi_mid_g, glob_humi_mid_b;
extern int glob_humi_high_r, glob_humi_high_g, glob_humi_high_b;
// Light RGB
extern int glob_light_low_r, glob_light_low_g, glob_light_low_b;
extern int glob_light_mid_r, glob_light_mid_g, glob_light_mid_b;
extern int glob_light_high_r, glob_light_high_g, glob_light_high_b;

#endif