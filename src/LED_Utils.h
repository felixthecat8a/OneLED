#ifndef LED_UTILS_H
#define LED_UTILS_H

#include <Arduino.h>

class LED_Utils {
  public:
    LED_Utils(
      uint8_t pin,
      bool isPWM = false,
      bool isESP32 = false,
      uint8_t channel = -1
    );

    ~LED_Utils();

    void begin();
    void on();
    void off();
    void toggle();
    bool isOn() const;
    void setPin(uint8_t pin);
    void setReverse(bool reverse);
    void setBrightness(uint8_t brightness);
    bool isReverse() const { return _activeLow; }
    bool isPWM() const { return _isPWM; }
    uint8_t getBrightness() const { return _brightness; }

  private:
    void _writeRaw(uint8_t value);
    uint8_t _pin;
    bool _state;
    bool _activeLow;
    bool _isPWM;
    uint8_t _brightness;
    bool _isESP32;
    uint8_t _channel;
};

#endif
