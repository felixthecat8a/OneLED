#ifndef BUZZER_UTILS_H
#define BUZZER_UTILS_H

#include <Arduino.h>

class PassiveBuzzer_Utils {
  public:
    PassiveBuzzer_Utils(uint8_t pin)
      : _pin(pin), _isBeeping(false), _beepDuration(0), _beepStart(0) {}

    void begin() {
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
    }

    void on() {
      digitalWrite(_pin, HIGH);
    }

    void off() {
      digitalWrite(_pin, LOW);
      _isBeeping = false;
    }

    void beep(unsigned long duration) {
      on();

      _isBeeping = true;
      _beepDuration = duration;
      _beepStart = millis();
    }

    bool isBeeping() const { return _isBeeping; }

    void update() {
      if (_isBeeping) {
        if (millis() - _beepStart >= _beepDuration) {
          off();
        }
      }
    }

  private:
    uint8_t _pin;

    bool _isBeeping;
    unsigned long _beepDuration;
    unsigned long _beepStart;

};

#endif
