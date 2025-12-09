// Thêm các thư viện sleep cho ESP32
#include "task1_human_detection.h"
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
#ifdef PRINT_HUMAN_DETECTION
            Serial.print(sensor.statusString());
#endif
            if (sensor.presenceDetected()) {
                turnONLED(true);
#ifdef PRINT_HUMAN_DETECTION
                Serial.print(", distance: ");
                Serial.print(sensor.detectedDistance());
                Serial.print("cm");
#endif
            } else {
                turnONLED(false);
            }
            if (sensor.movingTargetDetected()) {
#ifdef PRINT_HUMAN_DETECTION
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
#endif
            }
            if (sensor.stationaryTargetDetected()) {
#ifdef PRINT_HUMAN_DETECTION
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
#endif
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