#include "global.h"

volatile bool human_detected = false;
float glob_temperature = 0;
float glob_humidity = 0;
float glob_light_value = 0;
String glob_light_level = "";

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "abcde";
String wifi_password = "123456789";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t xBinarySemaphoreNeoPixel = xSemaphoreCreateBinary();

// NeoPixel config
String glob_neopixel_mode = "";
String glob_neopixel_effect = "";
int glob_temp_low_min = -10, glob_temp_low_max = 20;
int glob_temp_mid_min = 21, glob_temp_mid_max = 35;
int glob_temp_high_min = 36, glob_temp_high_max = 50;
int glob_humi_low_min = 0, glob_humi_low_max = 40;
int glob_humi_mid_min = 41, glob_humi_mid_max = 70;
int glob_humi_high_min = 71, glob_humi_high_max = 100;
int glob_light_low_min = 0, glob_light_low_max = 30;
int glob_light_mid_min = 31, glob_light_mid_max = 70;
int glob_light_high_min = 71, glob_light_high_max = 100;

// NeoPixel color (RGB)
int glob_neopixel_r = 255, glob_neopixel_g = 0, glob_neopixel_b = 0;
// Temp RGB
int glob_temp_low_r = 0, glob_temp_low_g = 188, glob_temp_low_b = 212;
int glob_temp_mid_r = 255, glob_temp_mid_g = 193, glob_temp_mid_b = 7;
int glob_temp_high_r = 244, glob_temp_high_g = 67, glob_temp_high_b = 54;
// Humi RGB
int glob_humi_low_r = 66, glob_humi_low_g = 165, glob_humi_low_b = 245;
int glob_humi_mid_r = 0, glob_humi_mid_g = 188, glob_humi_mid_b = 212;
int glob_humi_high_r = 2, glob_humi_high_g = 136, glob_humi_high_b = 209;
// Light RGB
int glob_light_low_r = 41, glob_light_low_g = 39, glob_light_low_b = 39;
int glob_light_mid_r = 255, glob_light_mid_g = 193, glob_light_mid_b = 7;
int glob_light_high_r = 244, glob_light_high_g = 215, glob_light_high_b = 215;