#ifndef UTILS_H
#define UTILS_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPSPlus.h>     // ← GPS parser

extern TFT_eSPI tft;

// SD manager
#ifndef SD_MANAGER_H
#define SD_MANAGER_H
extern bool sdMounted;
void mountSD();
void unmountSD();
#endif

// Notification
void showNotification(const char* title, const char* message);
void hideNotification();
void printWrappedText(int x, int y, int maxWidth, const char* text);

// Status‐bar & misc
void updateStatusBar();
float readBatteryVoltage();
float readInternalTemperature();
void drawStatusBar(float batteryVoltage, bool forceUpdate = false);
void initDisplay();
void showNotification(const char* title, const char* message);
void hideNotification();
void printWrappedText(int x, int y, int maxWidth, const char* text);
void loading(int frameDelay, uint16_t color, int16_t x, int16_t y, int repeats, bool center);
void displayLogo(uint16_t color, int displayTime);

namespace Terminal {
  void terminalSetup();
  void terminalLoop();
}

#endif // UTILS_H
