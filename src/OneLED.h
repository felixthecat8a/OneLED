#ifndef ONE_LED_H
#define ONE_LED_H

#include <Arduino.h>
#include "util/PulseWidthModulatedLED.h"
#include "util/RedGreenBlueLED.h"
#include "util/OneMoreTime.h"

class OneLED {
  public:
    OneLED(uint8_t pin, bool reverse = false);
    bool begin();
    void on();
    void off();
    void toggle();
    bool isOn();
    bool isReverse();
    void setPin(uint8_t pin);
    void setReverse(bool reverse);

  private:
    uint8_t _pin;
    bool _state;
    bool _reverse;
};

#endif
