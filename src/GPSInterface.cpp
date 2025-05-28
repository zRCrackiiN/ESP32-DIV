// GPSInterface.cpp
#include "GPSInterface.h"
#include <HardwareSerial.h>

GPSInterface::GPSInterface(uint8_t rxPin, uart_port_t uartNum)
  : _rxPin(rxPin), _uartNum(uartNum) {
}

void GPSInterface::begin(uint32_t baud) {
    // Install UART driver (RX only)
    uart_driver_install(_uartNum, 2048, 0, 0, nullptr, 0);
    uart_config_t cfg = {
        .baud_rate = static_cast<int>(baud),
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk= UART_SCLK_APB
    };
    uart_param_config(_uartNum, &cfg);
    uart_set_pin(_uartNum,
                 UART_PIN_NO_CHANGE, // TX unused
                 _rxPin,             // RX pin
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);
    // Initialize last fix state
    _lastFix = _gps.location.isValid();
}

void GPSInterface::onFixChange(FixCallback cb) {
    _fixCb = cb;
}

void GPSInterface::update() {
    uint8_t buf;
    // Read all available bytes
    while (uart_read_bytes(_uartNum, &buf, 1, 0) > 0) {
        _gps.encode(buf);
    }
    // Check for fix change
    bool currFix = _gps.location.isValid();
    if (currFix != _lastFix) {
        _lastFix = currFix;
        if (_fixCb) _fixCb(currFix);
    }
}

bool GPSInterface::hasFix() const {
    // consider fix valid only if location is valid and age is within timeout
    return _gps.location.isValid() && (_gps.location.age() < LOST_TIMEOUT);
}

// Non-const definitions for these methods match the header declarations
double GPSInterface::latitude() {
    return _gps.location.lat();
}

double GPSInterface::longitude() {
    return _gps.location.lng();
}

unsigned long GPSInterface::fixAge() {
    return _gps.location.age();
}

const TinyGPSPlus& GPSInterface::parser() {
    return _gps;
}

bool GPSInterface::hdopValid() {
  return _gps.hdop.isValid();
}

float GPSInterface::hdop() {
  // if you want NaN when invalid, you can do:
  return _gps.hdop.isValid()
           ? _gps.hdop.value()
           : NAN;
}