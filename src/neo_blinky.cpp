#include "neo_blinky.h"

enum NeoPixelState {
    STATE_DEFAULT,
    STATE_CUSTOM,
    STATE_TEMP,
    STATE_HUMI,
    STATE_LIGHT
};

NeoPixelState getNeoPixelState(const String& mode) {
    if (mode == "custom") return STATE_CUSTOM;
    if (mode == "temp") return STATE_TEMP;
    if (mode == "humi") return STATE_HUMI;
    if (mode == "light") return STATE_LIGHT;
    return STATE_DEFAULT;
}

NeoPixelState state = STATE_DEFAULT;

void neo_blinky(void *pvParameters) {
    Adafruit_NeoPixel strip(LED_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.clear();
    strip.show();

    while (1) {
        // Ưu tiên kiểm tra semaphore
        if (xSemaphoreTake(xBinarySemaphoreNeoPixel, 0) == pdTRUE) {
            // Đèn trắng báo hiệu kết nối WiFi thành công
            strip.setPixelColor(0, strip.Color(255, 255, 255));
            strip.show();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            strip.setPixelColor(0, strip.Color(0, 0, 0));
            strip.show();
            continue;
        }

        switch (state) {
            case STATE_DEFAULT:
                // Nhấp nháy đỏ bình thường
                strip.setPixelColor(0, strip.Color(255, 0, 0));
                strip.show();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                strip.setPixelColor(0, strip.Color(0, 0, 0));
                strip.show();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                if (glob_neopixel_mode == "custom") state = STATE_CUSTOM;
                else if (glob_neopixel_mode == "temp") state = STATE_TEMP;
                else if (glob_neopixel_mode == "humi") state = STATE_HUMI;
                else if (glob_neopixel_mode == "light") state = STATE_LIGHT;
                break;

            case STATE_CUSTOM: {
                // Hiển thị màu custom với hiệu ứng
                if (glob_neopixel_effect == "static") {
                    // Hiển thị màu liên tục
                    strip.setPixelColor(0, strip.Color(glob_neopixel_r, glob_neopixel_g, glob_neopixel_b));
                    strip.show();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                } else if (glob_neopixel_effect == "blink") {
                    // Chớp tắt màu đã chọn
                    strip.setPixelColor(0, strip.Color(glob_neopixel_r, glob_neopixel_g, glob_neopixel_b));
                    strip.show();
                    vTaskDelay(300 / portTICK_PERIOD_MS);
                    strip.setPixelColor(0, strip.Color(0, 0, 0));
                    strip.show();
                    vTaskDelay(300 / portTICK_PERIOD_MS);
                } else if (glob_neopixel_effect == "fade") {
                    // Mờ dần màu đã chọn
                    static int fadeValue = 0;
                    static int fadeDir = 1;
                    fadeValue += fadeDir * 15;
                    if (fadeValue >= 255) { fadeValue = 255; fadeDir = -1; }
                    if (fadeValue <= 0) { fadeValue = 0; fadeDir = 1; }
                    int r = (glob_neopixel_r * fadeValue) / 255;
                    int g = (glob_neopixel_g * fadeValue) / 255;
                    int b = (glob_neopixel_b * fadeValue) / 255;
                    strip.setPixelColor(0, strip.Color(r, g, b));
                    strip.show();
                    vTaskDelay(80 / portTICK_PERIOD_MS);
                } else {
                    // Mặc định: static
                    strip.setPixelColor(0, strip.Color(glob_neopixel_r, glob_neopixel_g, glob_neopixel_b));
                    strip.show();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                }
                // Chuyển trạng thái
                if (glob_neopixel_mode == "temp") state = STATE_TEMP;
                else if (glob_neopixel_mode == "humi") state = STATE_HUMI;
                else if (glob_neopixel_mode == "light") state = STATE_LIGHT;
                break;
            }

            case STATE_TEMP: {
                // Hiển thị màu theo nhiệt độ (ví dụ: lấy giá trị glob_temperature)
                int r = 0, g = 0, b = 0;
                if (glob_temperature <= glob_temp_low_max) {
                    r = glob_temp_low_r; g = glob_temp_low_g; b = glob_temp_low_b;
                } else if (glob_temperature <= glob_temp_mid_max) {
                    r = glob_temp_mid_r; g = glob_temp_mid_g; b = glob_temp_mid_b;
                } else {
                    r = glob_temp_high_r; g = glob_temp_high_g; b = glob_temp_high_b;
                }
                strip.setPixelColor(0, strip.Color(r, g, b));
                strip.show();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                // Chuyển trạng thái
                if (glob_neopixel_mode == "humi") state = STATE_HUMI;
                else if (glob_neopixel_mode == "light") state = STATE_LIGHT;
                else if (glob_neopixel_mode == "custom") state = STATE_CUSTOM;
                break;
            }
            case STATE_HUMI: {
                // Hiển thị màu theo độ ẩm
                int r = 0, g = 0, b = 0;
                if (glob_humidity <= glob_humi_low_max) {
                    r = glob_humi_low_r; g = glob_humi_low_g; b = glob_humi_low_b;
                } else if (glob_humidity <= glob_humi_mid_max) {
                    r = glob_humi_mid_r; g = glob_humi_mid_g; b = glob_humi_mid_b;
                } else {
                    r = glob_humi_high_r; g = glob_humi_high_g; b = glob_humi_high_b;
                }
                strip.setPixelColor(0, strip.Color(r, g, b));
                strip.show();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                // Chuyển trạng thái
                if (glob_neopixel_mode == "light") state = STATE_LIGHT;
                else if (glob_neopixel_mode == "custom") state = STATE_CUSTOM;
                else if (glob_neopixel_mode == "temp") state = STATE_TEMP;
                break;
            }
            case STATE_LIGHT: {
                // Hiển thị màu theo ánh sáng
                int r = 0, g = 0, b = 0;
                if (glob_light_value <= glob_light_low_max) {
                    r = glob_light_low_r; g = glob_light_low_g; b = glob_light_low_b;
                } else if (glob_light_value <= glob_light_mid_max) {
                    r = glob_light_mid_r; g = glob_light_mid_g; b = glob_light_mid_b;
                } else {
                    r = glob_light_high_r; g = glob_light_high_g; b = glob_light_high_b;
                }
                strip.setPixelColor(0, strip.Color(r, g, b));
                strip.show();
                vTaskDelay(100 / portTICK_PERIOD_MS);
                // Chuyển trạng thái
                if (glob_neopixel_mode == "custom") state = STATE_CUSTOM;
                else if (glob_neopixel_mode == "temp") state = STATE_TEMP;
                else if (glob_neopixel_mode == "humi") state = STATE_HUMI;
                break;
            }
            default:
                break;
        }
    }
}