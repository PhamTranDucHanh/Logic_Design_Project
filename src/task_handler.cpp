#include <task_handler.h>

void handleWebSocketMessage(String message)
{
    Serial.println(message);
    // Tăng kích thước bộ nhớ cho JSON lớn hơn (mode light/temp/humi gửi xuống dài hơn 256 bytes)
    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, message);
    if (error)
    {
        Serial.print("Lỗi parse JSON! ");
        Serial.println(error.c_str());
        return;
    }
    JsonObject value = doc["value"];
    if (doc["page"] == "device")
    {
        if (!value.containsKey("gpio") || !value.containsKey("status"))
        {
            Serial.println("JSON thiếu thông tin gpio hoặc status");
            return;
        }

        int gpio = value["gpio"];
        String status = value["status"].as<String>();

        Serial.printf("Điều khiển GPIO %d → %s\n", gpio, status.c_str());
        pinMode(gpio, OUTPUT);
        if (status.equalsIgnoreCase("ON"))
        {
            digitalWrite(gpio, HIGH);
            Serial.printf("GPIO %d ON\n", gpio);
        }
        else if (status.equalsIgnoreCase("OFF"))
        {
            digitalWrite(gpio, LOW);
            Serial.printf("GPIO %d OFF\n", gpio);
        }
    }
    else if (doc["page"] == "setting")
    {
        String WIFI_SSID = doc["value"]["ssid"].as<String>();
        String WIFI_PASS = doc["value"]["password"].as<String>();
        String CORE_IOT_TOKEN = doc["value"]["token"].as<String>();
        String CORE_IOT_SERVER = doc["value"]["server"].as<String>();
        String CORE_IOT_PORT = doc["value"]["port"].as<String>();

        Serial.println("Nhận cấu hình từ WebSocket:");
        Serial.println("SSID: " + WIFI_SSID);
        Serial.println("PASS: " + WIFI_PASS);
        Serial.println("TOKEN: " + CORE_IOT_TOKEN);
        Serial.println("SERVER: " + CORE_IOT_SERVER);
        Serial.println("PORT: " + CORE_IOT_PORT);

        // Gọi hàm lưu cấu hình
        Save_info_File(WIFI_SSID, WIFI_PASS, CORE_IOT_TOKEN, CORE_IOT_SERVER, CORE_IOT_PORT);

        // Phản hồi lại client (tùy chọn)
        String msg = "{\"status\":\"ok\",\"page\":\"setting_saved\"}";
        ws.textAll(msg);
    }
    else if (doc["page"] == "neopixel")
    {
        String mode = doc["mode"].as<String>();
        glob_neopixel_mode = mode;
        if (mode == "custom") {
            JsonArray colorArr = doc["color"].as<JsonArray>();
            glob_neopixel_r = colorArr[0];
            glob_neopixel_g = colorArr[1];
            glob_neopixel_b = colorArr[2];
            glob_neopixel_effect = doc["effect"].as<String>();
            Serial.println("[NeoPixel] Mode: custom");
            Serial.printf("  Color: R=%d, G=%d, B=%d\n", glob_neopixel_r, glob_neopixel_g, glob_neopixel_b);
            Serial.println("  Effect: " + glob_neopixel_effect);
        } else if (mode == "temp") {
            JsonObject temp = doc["temp"];
            glob_temp_low_min = temp["low"]["min"].as<int>();
            glob_temp_low_max = temp["low"]["max"].as<int>();
            JsonArray lowColor = temp["low"]["color"].as<JsonArray>();
            glob_temp_low_r = lowColor[0];
            glob_temp_low_g = lowColor[1];
            glob_temp_low_b = lowColor[2];
            glob_temp_mid_min = temp["mid"]["min"].as<int>();
            glob_temp_mid_max = temp["mid"]["max"].as<int>();
            JsonArray midColor = temp["mid"]["color"].as<JsonArray>();
            glob_temp_mid_r = midColor[0];
            glob_temp_mid_g = midColor[1];
            glob_temp_mid_b = midColor[2];
            glob_temp_high_min = temp["high"]["min"].as<int>();
            glob_temp_high_max = temp["high"]["max"].as<int>();
            JsonArray highColor = temp["high"]["color"].as<JsonArray>();
            glob_temp_high_r = highColor[0];
            glob_temp_high_g = highColor[1];
            glob_temp_high_b = highColor[2];
            Serial.println("[NeoPixel] Mode: temp");
            Serial.printf("  Low:   %d-%d, R=%d, G=%d, B=%d\n", glob_temp_low_min, glob_temp_low_max, glob_temp_low_r, glob_temp_low_g, glob_temp_low_b);
            Serial.printf("  Mid:   %d-%d, R=%d, G=%d, B=%d\n", glob_temp_mid_min, glob_temp_mid_max, glob_temp_mid_r, glob_temp_mid_g, glob_temp_mid_b);
            Serial.printf("  High:  %d-%d, R=%d, G=%d, B=%d\n", glob_temp_high_min, glob_temp_high_max, glob_temp_high_r, glob_temp_high_g, glob_temp_high_b);
        } else if (mode == "humi") {
            JsonObject humi = doc["humi"];
            glob_humi_low_min = humi["low"]["min"].as<int>();
            glob_humi_low_max = humi["low"]["max"].as<int>();
            JsonArray lowColor = humi["low"]["color"].as<JsonArray>();
            glob_humi_low_r = lowColor[0];
            glob_humi_low_g = lowColor[1];
            glob_humi_low_b = lowColor[2];
            glob_humi_mid_min = humi["mid"]["min"].as<int>();
            glob_humi_mid_max = humi["mid"]["max"].as<int>();
            JsonArray midColor = humi["mid"]["color"].as<JsonArray>();
            glob_humi_mid_r = midColor[0];
            glob_humi_mid_g = midColor[1];
            glob_humi_mid_b = midColor[2];
            glob_humi_high_min = humi["high"]["min"].as<int>();
            glob_humi_high_max = humi["high"]["max"].as<int>();
            JsonArray highColor = humi["high"]["color"].as<JsonArray>();
            glob_humi_high_r = highColor[0];
            glob_humi_high_g = highColor[1];
            glob_humi_high_b = highColor[2];
            Serial.println("[NeoPixel] Mode: humi");
            Serial.printf("  Low:   %d-%d, R=%d, G=%d, B=%d\n", glob_humi_low_min, glob_humi_low_max, glob_humi_low_r, glob_humi_low_g, glob_humi_low_b);
            Serial.printf("  Mid:   %d-%d, R=%d, G=%d, B=%d\n", glob_humi_mid_min, glob_humi_mid_max, glob_humi_mid_r, glob_humi_mid_g, glob_humi_mid_b);
            Serial.printf("  High:  %d-%d, R=%d, G=%d, B=%d\n", glob_humi_high_min, glob_humi_high_max, glob_humi_high_r, glob_humi_high_g, glob_humi_high_b);
        } else if (mode == "light") {
            JsonObject light = doc["light"];
            glob_light_low_min = light["low"]["min"].as<int>();
            glob_light_low_max = light["low"]["max"].as<int>();
            JsonArray lowColor = light["low"]["color"].as<JsonArray>();
            glob_light_low_r = lowColor[0];
            glob_light_low_g = lowColor[1];
            glob_light_low_b = lowColor[2];
            glob_light_mid_min = light["mid"]["min"].as<int>();
            glob_light_mid_max = light["mid"]["max"].as<int>();
            JsonArray midColor = light["mid"]["color"].as<JsonArray>();
            glob_light_mid_r = midColor[0];
            glob_light_mid_g = midColor[1];
            glob_light_mid_b = midColor[2];
            glob_light_high_min = light["high"]["min"].as<int>();
            glob_light_high_max = light["high"]["max"].as<int>();
            JsonArray highColor = light["high"]["color"].as<JsonArray>();
            glob_light_high_r = highColor[0];
            glob_light_high_g = highColor[1];
            glob_light_high_b = highColor[2];
            Serial.println("[NeoPixel] Mode: light");
            Serial.printf("  Low:   %d-%d, R=%d, G=%d, B=%d\n", glob_light_low_min, glob_light_low_max, glob_light_low_r, glob_light_low_g, glob_light_low_b);
            Serial.printf("  Mid:   %d-%d, R=%d, G=%d, B=%d\n", glob_light_mid_min, glob_light_mid_max, glob_light_mid_r, glob_light_mid_g, glob_light_mid_b);
            Serial.printf("  High:  %d-%d, R=%d, G=%d, B=%d\n", glob_light_high_min, glob_light_high_max, glob_light_high_r, glob_light_high_g, glob_light_high_b);
        }
    }
}
