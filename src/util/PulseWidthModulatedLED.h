#ifndef PULSE_WIDTH_MODULATED_LED_H
#define PULSE_WIDTH_MODULATED_LED_H

#include <Arduino.h>

class PulseWidthModulatedLED {
  public:
    PulseWidthModulatedLED(uint8_t pin, bool reverse = false);
    void begin();
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;
    void setReverse(bool reverse);
    bool isReverse() const;
    void off();
    bool isOn() const;

  private:
    uint8_t _pin;
    bool _reverse;
    uint8_t _brightness;
    void _write(uint8_t value);
};

#endif
