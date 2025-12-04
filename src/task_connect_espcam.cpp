#include "task_connect_espcam.h"
#include <ArduinoHttpClient.h>
#include <WiFi.h>


// THAY THẾ BẰNG IP CỦA ESP32-CAM (bạn sẽ thấy IP này trong Serial Monitor của CAM ~ hardcode)
// const char* serverUrl = "http://192.168.91.73/capture";  // Ví dụ: IP của CAM

// =======================================================
// Dữ liệu ảnh là một buffer (mảng byte) chứa ảnh JPEG
void processImage(uint8_t *imageBuffer, size_t imageSize) {
  Serial.printf("Đã nhận ảnh! Kích thước: %u bytes\n", imageSize);

  // TỪ ĐÂY: Bạn có thể làm bất cứ điều gì bạn muốn
  // Ví dụ 1: In 100 byte đầu tiên ra Serial
  // for(int i=0; i<100; i++) {
  //   Serial.printf("%02X ", imageBuffer[i]);
  // }
  // Serial.println();

  // Ví dụ 2: Dùng thư viện TJpg_Decoder để giải nén JPEG
  // (Bạn cần thêm thư viện này vào platformio.ini)

  // Ví dụ 3: Chạy mô hình TinyML (ESP-NN) để nhận diện
  // (Đây là một bước nâng cao)

  Serial.println("Xử lý ảnh xong.");
}

// Sửa lại host để chỉ chứa địa chỉ IP
const char* host = "192.168.91.73"; // <-- Địa chỉ IP của ESP32-CAM
const int port = 80;
const char* path = "/capture";

// Khai báo client ở ngoài để dùng lại
WiFiClient wifi_client;
HttpClient http_client = HttpClient(wifi_client, host, port);

// Hàm gọi server để lấy ảnh
void getImageFromServer() {
    // Kiểm tra WiFi trước khi gọi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi chưa kết nối, bỏ qua request.");
        return;
    }

    Serial.print("Đang kết nối tới server ảnh: ");
    Serial.println(host);

    // Bắt đầu yêu cầu
    int err = http_client.get(path);
    if (err != 0) {
        Serial.printf("Lỗi kết nối: %d\n", err);
        http_client.stop();
        return;
    }

    // Lấy mã phản hồi
    int httpCode = http_client.responseStatusCode();

    if (httpCode == 200) { // 200 là mã OK
        Serial.println("Kết nối thành công, đang tải ảnh...");

        int imageSize = http_client.contentLength(); // Lấy kích thước ảnh
        if (imageSize <= 0) {
            Serial.println("Không nhận được nội dung ảnh (Content-Length: 0 hoặc -1).");
            http_client.stop();
            return;
        }

        // QUAN TRỌNG: Giới hạn kích thước ảnh để tránh crash
        const int MAX_IMAGE_SIZE = 100 * 1024; // 100KB
        if (imageSize > MAX_IMAGE_SIZE) {
            Serial.printf("Ảnh quá lớn: %d bytes. Giới hạn: %d bytes\n", imageSize, MAX_IMAGE_SIZE);
            http_client.stop();
            return;
        }

        // Tạo buffer để chứa ảnh - CHỈ dùng heap thông thường
        uint8_t *imageBuffer = (uint8_t *)malloc(imageSize);

        if (!imageBuffer) {
            Serial.println("Không đủ bộ nhớ để tạo buffer ảnh!");
            Serial.printf("Bộ nhớ heap còn trống: %d bytes\n", ESP.getFreeHeap());
            http_client.stop();
            return;
        }

        Serial.printf("Đã cấp phát buffer %d bytes\n", imageSize);

        // Bỏ qua các header (phần này bắt buộc với thư viện này)
        http_client.skipResponseHeaders();

        // Đọc dữ liệu ảnh vào buffer
        size_t bytesRead = 0;
        unsigned long timeout = millis();
        while (bytesRead < imageSize && (millis() - timeout < 10000)) { // Timeout 10s
            if (wifi_client.available()) {
                int c = wifi_client.read(imageBuffer + bytesRead, imageSize - bytesRead);
                if (c > 0) {
                    bytesRead += c;
                    timeout = millis(); // Reset timeout
                }
            }
            vTaskDelay(10 / portTICK_PERIOD_MS); // Yield cho FreeRTOS
        }

        // Gọi hàm xử lý ảnh
        if (bytesRead == imageSize) {
            processImage(imageBuffer, imageSize);
        } else {
            Serial.printf("Lỗi: Tải ảnh không hoàn tất. Đọc được %u / %u bytes\n", bytesRead, imageSize);
        }

        // Rất quan trọng: Giải phóng bộ nhớ sau khi dùng
        free(imageBuffer);

    } else {
        Serial.printf("Kết nối thất bại, mã lỗi HTTP: %d\n", httpCode);
    }

    // Đóng kết nối
    http_client.stop();
}

void connect_to_esp32cam(void *pvParameters) {
  Serial.println("[ESP32CAM Task] Đang khởi động...");
  
  // Chờ WiFi kết nối (chờ semaphore vô thời hạn)
  Serial.println("[ESP32CAM Task] Đang chờ WiFi kết nối...");
  xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY);
  
  Serial.println("[ESP32CAM Task] WiFi đã kết nối!");
  
  // In thông tin bộ nhớ
  Serial.printf("[ESP32CAM Task] Heap còn trống: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("[ESP32CAM Task] Heap lớn nhất: %d bytes\n", ESP.getMaxAllocHeap());
  
  // Delay ban đầu trước khi bắt đầu
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  
  // Vòng lặp chính
  while (1) {
      // In thông tin bộ nhớ
    Serial.printf("[ESP32CAM Task] Heap còn trống: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("[ESP32CAM Task] Heap lớn nhất: %d bytes\n", ESP.getMaxAllocHeap());
    getImageFromServer();
    vTaskDelay(5000 / portTICK_PERIOD_MS); // Chờ 5 giây trước khi lấy ảnh tiếp
  }
}
