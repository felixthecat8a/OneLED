#include "OneLED.h"

#ifndef ESP32_PWM_FREQ
  #define ESP32_PWM_FREQ 5000 // 5 kHz frequency
#endif

#ifndef ESP32_PWM_RES
  #define ESP32_PWM_RES 8 // 8-bit resolution
#endif

OneLED::OneLED(uint8_t pin, bool isPWM, bool reverse)
  : _pin(pin), _state(false), _isPWM(isPWM), _reverse(reverse), _brightness(0) {}

bool OneLED::begin() {
  if (_pin < 0) { return false; }

  #ifdef ESP32
    if (_isPWM) {
      #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(_pin, ESP32_PWM_FREQ, ESP32_PWM_RES);
      #else
        ledcSetup(_pin, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcAttachPin(_pin, _pin);
      #endif
    } else {
      pinMode(_pin, OUTPUT);
    }
  #else
    pinMode(_pin, OUTPUT);
  #endif

  writeRaw(_reverse ? 255 : 0);
  return true;
}

void OneLED::on() {
  if (_isPWM) {
    setBrightness(255);
  } else {
    digitalWrite(_pin, _reverse ? LOW : HIGH);
  }
  _state = true;
}

void OneLED::off() {
  if (_isPWM) {
    setBrightness(0);
  } else {
    digitalWrite(_pin, _reverse ? HIGH : LOW);
  }
  _state = false;
}

void OneLED::toggle() {
  if (_isPWM) {
    _brightness = _state ? 0 : 255;
    setBrightness(_brightness);
  } else {
    _state = !_state;
    digitalWrite(_pin, (_state ^ _reverse) ? HIGH : LOW);
  }
}

bool OneLED::isOn() const {
  if (_isPWM) {
    return _reverse ? _brightness < 255 : _brightness > 0;
  }
  return _state;
}

void OneLED::setPin(uint8_t pin) {
  _pin = pin;
  begin();
}

void OneLED::setReverse(bool reverse) {
  _reverse = reverse;
  if (_isPWM) {
    setBrightness(_brightness);
  } else {
    digitalWrite(_pin, (_state ^ _reverse) ? HIGH : LOW);
  }
}

bool OneLED::isReverse() const { return _reverse; }
bool OneLED::isPWM() const { return _isPWM; }

void OneLED::setBrightness(uint8_t brightness) {
  if (!_isPWM) return;
  _brightness = brightness;
  writeRaw(_reverse ? (255 - _brightness) : _brightness);
  _state = (_brightness > 0);
}

uint8_t OneLED::getBrightness() const { return _brightness; }

void OneLED::writeRaw(uint8_t value) {
  #ifdef ESP32
    if (_isPWM)
      ledcWrite(_pin, value);
    else
      digitalWrite(_pin, value > 127 ? HIGH : LOW);
  #else
    if (_isPWM)
      analogWrite(_pin, value);
    else
      digitalWrite(_pin, value > 127 ? HIGH : LOW);
  #endif
}
