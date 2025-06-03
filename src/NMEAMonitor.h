// NMEAMonitor.h
#pragma once
#include <Arduino.h>
#include "Touchscreen.h"
#include "shared.h"

// forward-declare the TFT instance (that you already have in your .ino/utils.cpp)
extern class TFT_eSPI tft;

namespace NMEAMonitor {
  void setup();
  void loop();
}
