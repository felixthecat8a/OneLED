#include "PulseWidthModulatedLED.h"

PulseWidthModulatedLED::PulseWidthModulatedLED(uint8_t pin, bool reverse)
  : _pin(pin), _brightness(0), _reverse(reverse) {}

void PulseWidthModulatedLED::begin() {
  #ifdef ESP32
    #if ESP_ARDUINO_VERSION_MAJOR >= 3
      ledcAttach(_pin, 5000, 8);  // 5 kHz frequency, 8-bit resolution
    #else
      ledcSetup(_pin, 5000, 8);
      ledcAttachPin(_pin, _pin);
    #endif
  #else
    pinMode(_pin, OUTPUT);
  #endif
  _write(_reverse ? 255 : 0);
}

void PulseWidthModulatedLED::_write(uint8_t value) {
  #ifdef ESP32
    ledcWrite(_pin, value);
  #else
    analogWrite(_pin, value);
  #endif
}

void PulseWidthModulatedLED::setBrightness(uint8_t brightness) {
  _brightness = brightness;
  _write(_reverse ? (255 - _brightness) : _brightness);
}

uint8_t PulseWidthModulatedLED::getBrightness() const {
  return _brightness;
}

void PulseWidthModulatedLED::setReverse(bool reverse) {
  _reverse = reverse;
  _write(_reverse ? (255 - _brightness) : _brightness);
}

bool PulseWidthModulatedLED::isReverse() const {
  return _reverse;
}

void PulseWidthModulatedLED::off() {
  _brightness = 0;
  _write(_reverse ? 255 : 0);
}

bool PulseWidthModulatedLED::isOn() const {
  return _reverse ? _brightness < 255 : _brightness > 0;
}

