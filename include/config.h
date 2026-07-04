#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ===== WiFi Configuration =====
#define WIFI_SSID "Ali TT"
#define WIFI_PASSWORD "2150068486"
#define WIFI_TIMEOUT 30

// ===== OTA Configuration =====
#define OTA_HOST "hatako77.github.io"
#define OTA_PATH "/firmware/"
#define OTA_VERSION_FILE "version.txt"
#define OTA_FIRMWARE_FILE "firmware.bin"
#define OTA_CHECK_INTERVAL 3600000

// ===== Web Server Configuration =====
#define WEB_SERVER_PORT 80
#define MAX_OTA_CLIENTS 1

// ===== Update Configuration =====
#define OTA_TIMEOUT 60000
#define REDIRECT_TIMEOUT 5
#define MAX_SKETCH_SIZE 0x200000

// ===== Debug Configuration =====
#define DEBUG_ENABLED true
#define SERIAL_BAUD 115200

#if DEBUG_ENABLED
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    // حذف DEBUG_PRINTF که مشکل داشت
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#endif
