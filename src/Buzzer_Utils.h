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

class ActiveBuzzer_Utils {
  public:
    ActiveBuzzer_Utils(uint8_t pin)
      : _pin(pin), _isBeeping(false), _beepFreq(0), _beepDuration(0), _beepStart(0)
    {}

    void toneOn(unsigned int freq) {
      tone(_pin, freq);
    }

    void toneOff() {
      noTone(_pin);
    }

    void beep(unsigned int freq, unsigned long duration) {
      tone(_pin, freq);

      _isBeeping = true;
      _beepFreq = freq;
      _beepDuration = duration;
      _beepStart = millis();
    }

    bool isBeeping() const { return _isBeeping; }

    void update() {
      if (_isBeeping) {
        if (millis() - _beepStart >= _beepDuration) {
          noTone(_pin);
          _isBeeping = false;
        }
      }
    }

  private:
    uint8_t _pin;

    bool _isBeeping;
    unsigned int _beepFreq;
    unsigned long _beepDuration;
    unsigned long _beepStart;

};

#endif
