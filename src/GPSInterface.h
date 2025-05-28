// GPSInterface.h
#ifndef GPSINTERFACE_H
#define GPSINTERFACE_H

#include <TinyGPSPlus.h>
#include <driver/uart.h>

// Callback type for GPS fix state changes
using FixCallback = void(*)(bool newFix);

class GPSInterface {
public:
    static constexpr uint32_t LOST_TIMEOUT = 5000; // ms after which fix is considered lost

public:
    GPSInterface(uint8_t rxPin = 39, uart_port_t uartNum = UART_NUM_2);
    /**
     * Initialize the GPS UART and parser.
     * Should be called in setup().
     */
    void begin(uint32_t baud = 115200);

    /**
     * Call regularly in loop() to feed incoming bytes into the parser.
     * Detects fix state changes and invokes callback if set.
     */
    void update();

    /**
     * Returns true if a valid GPS fix is currently available.
     */
    bool hasFix() const;

    /**
     * Attach a callback to be notified when fix state changes.
     */
    void onFixChange(FixCallback cb);

    /**
     * Retrieve the latest latitude in degrees. Undefined if no fix.
     */
    double latitude();

    /**
     * Retrieve the latest longitude in degrees. Undefined if no fix.
     */
    double longitude();

    /**
     * Retrieve the latest fix age (milliseconds since last valid update).
     */
    unsigned long fixAge();

    /**
     * Returns the underlying TinyGPS++ parser (read-only).
     */
    const TinyGPSPlus& parser();

    bool    hdopValid();
    float   hdop();

private:
    uint8_t       _rxPin;
    uart_port_t   _uartNum;
    TinyGPSPlus   _gps;
    bool          _lastFix = false;
    FixCallback   _fixCb   = nullptr;
};

#endif // GPSINTERFACE_H