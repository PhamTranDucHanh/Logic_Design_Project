#ifndef __TASK1_HUMAN_DETECTION__
#define __TASK1_HUMAN_DETECTION__
#include <Arduino.h>
#include "global.h"
#include <MyLD2410.h>

#define LD2410_RX_PIN 43
#define LD2410_TX_PIN 44
#define ENHANCED_MODE
// Chọn Serial cho sensor
#define sensorSerial Serial1
#define LD2410_BAUD_RATE 256000



void human_detection_task1(void *pvParameters);
void printValue(const byte &val);
void turnONLED(bool detected);
#endif