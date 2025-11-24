#ifndef ONE_LED_H
#define ONE_LED_H

#include <Arduino.h>
#include "RedGreenBlueLED.h"
#include "OneMoreTime.h"
//#include "util/OneLDR.h" // component helper when ready

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

class OneLED {
public:
  OneLED(uint8_t pin = LED_BUILTIN, bool isPWM = false, bool reverse = false);

  bool begin();
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
  bool _reverse;
  bool _isPWM;
  uint8_t _brightness;
};

#endif
