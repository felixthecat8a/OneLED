#ifndef ONE_LED_H
#define ONE_LED_H

#include <Arduino.h>
#include "RedGreenBlueLED.h"
#include "OneMoreTime.h"
//#include "util/OneLDR.h" // component helper when ready

class OneLED {
  public:
    OneLED(
      uint8_t pin,
      bool isPWM = false,
      bool isESP32 = false,
      uint8_t channel = -1
    );

    ~OneLED();

    void begin();
    void on();
    void off();
    void toggle();
    bool isOn() const;
    void setPin(uint8_t pin);
    void setReverse(bool reverse);
    bool isReverse() const;
    bool isPWM() const;
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

  private:
    void writeRaw(uint8_t value);
    uint8_t _pin;
    bool _state;
    bool _activeLow;
    bool _isPWM;
    uint8_t _brightness;
    bool _isESP32;
    uint8_t _channel;
};

#endif
