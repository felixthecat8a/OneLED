#include "OneLED.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 13
#endif

OneLED::OneLED(uint8_t pin = LED_BUILTIN, bool reverse)
  : _pin(pin), _state(false), _reverse(reverse) {}

bool OneLED::begin() {
  if (_pin < 0 ) { return false; }
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _reverse ? HIGH : LOW);
  return true;
}

void OneLED::on() {
  digitalWrite(_pin, _reverse ? LOW : HIGH);
  _state = true;
}

void OneLED::off() {
  digitalWrite(_pin, _reverse ? HIGH : LOW);
  _state = false;
}

void OneLED::toggle() {
  _state = !_state;
  digitalWrite(_pin, (_state ^ _reverse) ? HIGH : LOW);
}

bool OneLED::isOn() {
  return _state;
}

bool OneLED::isReverse() {
  return _reverse;
}

void OneLED::setPin(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, (_state ^ _reverse) ? HIGH : LOW);
}

void OneLED::setReverse(bool reverse) {
  _reverse = reverse;
  digitalWrite(_pin, (_state ^ _reverse) ? HIGH : LOW);
}
