#include "NMEAMonitor.h"
#include <TFT_eSPI.h>
#include <deque>

// extern from your app:
extern TFT_eSPI tft;

static constexpr int GPS_RX_PIN = 39;
static constexpr int TOP_MARGIN   = 20;
static constexpr int TEXT_FONT    = 1;
static constexpr int TEXT_SIZE    = 1;

void NMEAMonitor::setup() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(TEXT_FONT);
  tft.setTextSize(TEXT_SIZE);
  tft.setTextColor(TFT_WHITE);
  Serial1.begin(115200, SERIAL_8N1, GPS_RX_PIN, -1);
}

void NMEAMonitor::loop() {
  static String            line;
  static std::deque<String> buffer;

  // compute how many lines fit
  int lineHeight = tft.fontHeight(TEXT_FONT) * TEXT_SIZE;
  int maxLines   = (tft.height() - TOP_MARGIN) / lineHeight;

  // read bytes into 'line'
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\r') continue;
    if (c == '\n') {
      // push new line into buffer
      buffer.push_back(line);
      if (buffer.size() > maxLines) buffer.pop_front();
      line = "";

      // redraw whole block
      tft.fillRect(0, TOP_MARGIN, tft.width(), tft.height() - TOP_MARGIN, TFT_BLACK);
      int y = TOP_MARGIN;
      for (auto &ln : buffer) {
        tft.drawString(ln, 0, y, TEXT_FONT);
        y += lineHeight;
      }
    } else {
      line += c;
    }
  }
}
