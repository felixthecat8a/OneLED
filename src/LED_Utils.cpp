#include "LED_Utils.h"

#ifdef ESP32
  static bool _ledcChannelUsed[16] = { false };

  static int8_t allocateLEDCChannel() {
    for (int i = 0; i < 16; i++) {
      if (!_ledcChannelUsed[i]) {
        _ledcChannelUsed[i] = true;
        return i;
      }
    }
    return -1; // none available
  }

  static void freeLEDCChannel(int8_t ch) {
    if (ch >= 0 && ch < 16) {
      _ledcChannelUsed[ch] = false;
    }
  }
#endif

#ifndef ESP32_PWM_FREQ
  #define ESP32_PWM_FREQ 5000 // 5 kHz frequency
#endif

#ifndef ESP32_PWM_RES
  #define ESP32_PWM_RES 8 // 8-bit resolution
#endif

LED_Utils::LED_Utils(uint8_t pin, bool isPWM, bool isESP32, uint8_t channel)
  : _pin(pin), _state(false), _isPWM(isPWM), _activeLow(false), _brightness(0),
  _isESP32(isESP32), _channel(channel) {}

LED_Utils::~LED_Utils() {
  #ifdef ESP32
    if (_isESP32 && _channel >= 0) {
      freeLEDCChannel(_channel);
      _channel = -1;
    }
  #endif
}

void LED_Utils::begin() {
  if (_isESP32 && _isPWM) {
    #ifdef ESP32
      if (_channel < 0) {
        _channel = allocateLEDCChannel();
        if (_channel < 0) return;
      }

      #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(_pin, ESP32_PWM_FREQ, ESP32_PWM_RES);
      #else
        ledcSetup(_channel, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcAttachPin(_pin, _channel);
      #endif
    #endif
  } else {
    pinMode(_pin, OUTPUT);
  }

  _writeRaw(_activeLow ? 255 : 0);
}

void LED_Utils::on() {
  if (_isPWM) {
    setBrightness(255);
  } else {
    digitalWrite(_pin, _activeLow ? LOW : HIGH);
  }
  _state = true;
}

void LED_Utils::off() {
  if (_isPWM) {
    setBrightness(0);
  } else {
    digitalWrite(_pin, _activeLow ? HIGH : LOW);
  }
  _state = false;
}

void LED_Utils::toggle() {
  if (_isPWM) {
    _brightness = _state ? 0 : 255;
    setBrightness(_brightness);
  } else {
    _state = !_state;
    digitalWrite(_pin, (_state ^ _activeLow) ? HIGH : LOW);
  }
}

bool LED_Utils::isOn() const {
  if (_isPWM) {
    return _activeLow ? _brightness < 255 : _brightness > 0;
  }
  return _state;
}

void LED_Utils::setPin(uint8_t pin) {
  _pin = pin;
  begin();
}

void LED_Utils::setReverse(bool reverse) {
  _activeLow = reverse;
  if (_isPWM) {
    setBrightness(_brightness);
  } else {
    digitalWrite(_pin, (_state ^ _activeLow) ? HIGH : LOW);
  }
}

void LED_Utils::setBrightness(uint8_t brightness) {
  if (!_isPWM) return;
  _brightness = brightness;
  _writeRaw(_activeLow ? (255 - _brightness) : _brightness);
  _state = (_brightness > 0);
}

void LED_Utils::_writeRaw(uint8_t value) {
  if (_isESP32 && _isPWM) {
    #ifdef ESP32
      #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcWrite(_pin, value);
      #else
        ledcWrite(_channel, value);
      #endif
    #endif
  } else if (_isPWM) {
    analogWrite(_pin, value);
  } else {
    digitalWrite(_pin, value > 127 ? HIGH : LOW);
  }
}
