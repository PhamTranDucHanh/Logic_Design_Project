// Thêm các thư viện sleep cho ESP32
#include "task1_human_detection.h"
#include "esp_sleep.h"
#include "esp_system.h"
void human_detection_task1(void *pvParameters){
    MyLD2410 sensor(sensorSerial);
    sensor.enhancedMode();
    pinMode(10, OUTPUT);
    // Khởi tạo UART theo chân đã cấu hình trong task_human_detection.h
    sensorSerial.begin(LD2410_BAUD_RATE, SERIAL_8N1, LD2410_RX_PIN, LD2410_TX_PIN);
    vTaskDelay(500/portTICK_PERIOD_MS); // Chờ một chút để UART ổn định

    sensor.begin();
    sensor.setNoOneWindow(3); // Thiết lập cửa sổ không có người là 3 giây
    while (1)
    {
        if (sensor.check() == MyLD2410::Response::DATA) {
            Serial.print(sensor.statusString());
            if (sensor.presenceDetected()) {
                turnONLED(true);
                Serial.print(", distance: ");
                Serial.print(sensor.detectedDistance());
                Serial.print("cm");
            } else {
                turnONLED(false);
#ifdef POWER_OPTIMIZE
                Serial.println("No target detected. Entering light sleep mode...");
                // Cấu hình wakeup bằng GPIO7 (HIGH)
                Serial.print("GPIO7 state before sleep: ");
                esp_sleep_enable_ext0_wakeup(GPIO_NUM_7, 1); 
                esp_light_sleep_start();
                // Sau khi wakeup, restart lại hệ thống
                Serial.println("Woke up! Restarting system...");
                ESP.restart();
#endif
            }
            if (sensor.movingTargetDetected()) {
                Serial.print(" MOVING    = ");
                Serial.print(sensor.movingTargetSignal());
                Serial.print("@");
                Serial.print(sensor.movingTargetDistance());
                Serial.print("cm ");
                if (sensor.inEnhancedMode()) {
                    Serial.print("\n signals->[");
                    sensor.getMovingSignals().forEach(printValue);
                    Serial.print(" ] thresholds:");
                    sensor.getMovingThresholds().forEach(printValue);
                }
                Serial.println();
            }
            if (sensor.stationaryTargetDetected()) {
                Serial.print(" STATIONARY= ");
                Serial.print(sensor.stationaryTargetSignal());
                Serial.print("@");
                Serial.print(sensor.stationaryTargetDistance());
                Serial.print("cm ");
                if (sensor.inEnhancedMode()) {
                    Serial.print("\n signals->[");
                    sensor.getStationarySignals().forEach(printValue);
                    Serial.print(" ] thresholds:");
                    sensor.getStationaryThresholds().forEach(printValue);
                }
                Serial.println();
            }
            Serial.println();
        }

        vTaskDelay(300 / portTICK_PERIOD_MS); // Đợi 300ms trước khi đọc giá trị tiếp theo
    }
}

void printValue(const byte &val) {
  Serial.print(' ');
  Serial.print(val);
}

void turnONLED(bool detected) {
    int ledState = digitalRead(10);
    int wantState = detected ? HIGH : LOW;
    if (ledState != wantState) {
        digitalWrite(10, wantState);
        human_detected = wantState;
    }
    else return;
}