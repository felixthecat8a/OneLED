#include "Bttn_Utils.h"

Bttn_Utils::Bttn_Utils(uint8_t pin, bool activeLow, uint16_t debounceTime)
  : _pin(pin), _activeLow(activeLow), _debounceTime(debounceTime),
    _holdTime(1000), _doubleClickTime(250), _state(BUTTON_IDLE),
    _pressedEvent(false), _releasedEvent(false), _heldEvent(false),
    _doubleClickEvent(false), _lastDebounceTime(0), _pressedTime(0),
    _lastReleaseTime(0), _lastReading(!activeLow) {}

bool Bttn_Utils::begin() {
  if (_pin < 0) return false;
  pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT);
  return true;
}

void Bttn_Utils::update() {
  bool reading = digitalRead(_pin);
  if (_activeLow) reading = !reading;

  uint32_t now = millis();

  switch (_state) {
    case BUTTON_IDLE:
      if (reading != _lastReading) {
        _lastDebounceTime = now;
        _state = BUTTON_DEBOUNCING;
      }
      break;

    case BUTTON_DEBOUNCING:
      if ((now - _lastDebounceTime) >= _debounceTime) {
        if (reading) {
          _pressedTime = now;
          _state = BUTTON_PRESSED;
        } else {
          _state = BUTTON_IDLE;
        }
      }
      break;

    case BUTTON_PRESSED:
      if (!reading) {
        _releasedEvent = true;
        _lastReleaseTime = now;
        _state = BUTTON_WAITING_DOUBLE;
      }
      else if ((now - _pressedTime) >= _holdTime) {
        _heldEvent = true;
        _state = BUTTON_HOLD;
      }
      break;

    case BUTTON_HOLD:
      if (!reading) {
        _releasedEvent = true;
        _state = BUTTON_IDLE;
      }
      break;

    case BUTTON_WAITING_DOUBLE:
      if (reading && (now - _lastReleaseTime <= _doubleClickTime)) {
        _doubleClickEvent = true;
        _state = BUTTON_DOUBLE_CLICK;
      }

      else if (now - _lastReleaseTime > _doubleClickTime) {
        _pressedEvent = true;
        _state = BUTTON_IDLE;
      }
      break;

    case BUTTON_DOUBLE_CLICK:
      if (!reading) {
        _state = BUTTON_IDLE;
      }
      break;

  }

  _lastReading = reading;
}

bool Bttn_Utils::isPressed() const {
  return _lastReading;
}

bool Bttn_Utils::wasPressed() {
  if (_pressedEvent) {
    _pressedEvent = false;
    return true;
  }
  return false;
}

bool Bttn_Utils::wasReleased() {
  if (_releasedEvent) {
    _releasedEvent = false;
    return true;
  }
  return false;
}

bool Bttn_Utils::wasHeld() {
  if (_heldEvent) {
    _heldEvent = false;
    return true;
  }
  return false;
}

bool Bttn_Utils::wasDoubleClicked() {
  if (_doubleClickEvent) {
    _doubleClickEvent = false;
    return true;
  }
  return false;
}

void Bttn_Utils::setHoldTime(uint16_t ms) {
  _holdTime = ms;
}

void Bttn_Utils::setDoubleClickTime(uint16_t ms) {
  _doubleClickTime = ms;
}

bool Bttn_Utils::isWaitingForDoubleClick() const {
  return _state == BUTTON_WAITING_DOUBLE;
}
