#include "PulseWidthModulatedLED.h"

PulseWidthModulatedLED::PulseWidthModulatedLED(uint8_t pin, bool reverse)
  : _pin(pin), _brightness(0), _reverse(reverse) {}

bool PulseWidthModulatedLED::begin() {
  if (_pin < 0 ) { return false; }
  pinMode(_pin, OUTPUT);
  analogWrite(_pin, _reverse ? 255 : 0);
  return true;
}

void PulseWidthModulatedLED::setBrightness(uint8_t brightness) {
  _brightness = brightness;
  analogWrite(_pin, _reverse ? (255 - _brightness) : _brightness);
}

uint8_t PulseWidthModulatedLED::getBrightness() const {
  return _brightness;
}

void PulseWidthModulatedLED::setReverse(bool reverse) {
  _reverse = reverse;
  analogWrite(_pin, _reverse ? (255 - _brightness) : _brightness);
}

bool PulseWidthModulatedLED::isReverse() const {
  return _reverse;
}

void PulseWidthModulatedLED::off() {
  _brightness = 0;
  analogWrite(_pin, _reverse ? 255 : 0);
}

bool PulseWidthModulatedLED::isOn() const {
  return _reverse ? _brightness < 255 : _brightness > 0;
}
