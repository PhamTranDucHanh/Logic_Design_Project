#include "global.h"

QueueHandle_t xQueueSensorDataNeoPixel = xQueueCreate(5, sizeof(SensorData));
QueueHandle_t xQueueSensorDataCoreIOT = xQueueCreate(5, sizeof(SensorData));

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t xBinarySemaphoreNeoPixel = xSemaphoreCreateBinary();


QueueHandle_t xQueueNeoPixelConfig = xQueueCreate(5, sizeof(NeoPixelConfigStruct));