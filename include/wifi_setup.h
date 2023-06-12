#ifndef WIFI_SETUP_H_
#define WIFI_SETUP_H_
#include <Arduino.h>
#include <HttpClient.h>
#include <WiFi.h>

#include "nvs.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void nvs_access();
void enable_wifi();
#endif