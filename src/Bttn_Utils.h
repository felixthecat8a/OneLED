#ifndef BTTN_UTILS_H
#define BTTN_UTILS_H

#include <Arduino.h>

class Bttn_Utils {
public:
  Bttn_Utils(uint8_t pin, bool activeLow = true, uint16_t debounceTime = 50)
  : _pin(pin),
    _activeLow(activeLow),
    _debounceTime(debounceTime),
    _holdTime(1000),
    _doubleClickTime(300),
    _state(State::Idle),
    _clickEvent(false),
    _releasedEvent(false),
    _heldEvent(false),
    _doubleClickEvent(false),
    _lastDebounceTime(0),
    _pressedTime(0),
    _lastReleaseTime(0),
    _lastReading(!activeLow) {}

  void begin() {
    pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT);
  }

  void update() {
    bool reading = digitalRead(_pin);
    if (_activeLow) reading = !reading;

    uint32_t now = millis();

    switch (_state) {
      case State::Idle:
        if (reading != _lastReading) {
          _lastDebounceTime = now;
          _state = State::Debouncing;
        }
        break;

      case State::Debouncing:
        if ((now - _lastDebounceTime) >= _debounceTime) {
          if (reading) {
            _pressedTime = now;
            _state = State::Pressed;
          } else {
            _state = State::Idle;
          }
        }
        break;

      case State::Pressed:
        if (!reading) {
          _releasedEvent = true;
          _lastReleaseTime = now;
          _state = State::WaitingDouble;
        }
        else if ((now - _pressedTime) >= _holdTime) {
          _heldEvent = true;
          _state = State::Held;
        }
        break;

      case State::Held:
        if (!reading) {
          _releasedEvent = true;
          _state = State::Idle;
        }
        break;

      case State::WaitingDouble:
        if (reading && (now - _lastReleaseTime <= _doubleClickTime)) {
          _doubleClickEvent = true;
          _state = State::DoubleClick;
        }

        else if (now - _lastReleaseTime > _doubleClickTime) {
          _clickEvent = true;
          _state = State::Idle;
        }
        break;

      case State::DoubleClick:
        if (!reading) {
          _state = State::Idle;
        }
        break;
    }

    _lastReading = reading;
  }

  bool isDown() const {
    return _lastReading;
  }

  bool wasClicked() {
    if (_clickEvent) {
      _clickEvent = false;
      return true;
    }
    return false;
  }

  bool wasReleased() {
    if (_releasedEvent) {
      _releasedEvent = false;
      return true;
    }
    return false;
  }

  bool wasHeld() {
    if (_heldEvent) {
      _heldEvent = false;
      return true;
    }
    return false;
  }

  bool wasDoubleClicked() {
    if (_doubleClickEvent) {
      _doubleClickEvent = false;
      return true;
    }
    return false;
  }

  void setHoldTime(uint16_t ms) {
    _holdTime = ms;
  }

  void setDoubleClickTime(uint16_t ms) {
    _doubleClickTime = ms;
  }

  bool isWaitingForDoubleClick() const {
    return _state == State::WaitingDouble;
  }

private:
  enum class State : uint8_t {
    Idle,
    Debouncing,
    Pressed,
    Held,
    WaitingDouble,
    DoubleClick
  };

  uint8_t _pin;
  bool _activeLow;
  uint16_t _debounceTime;
  uint16_t _holdTime;
  uint16_t _doubleClickTime;

  State _state;

  bool _clickEvent;
  bool _releasedEvent;
  bool _heldEvent;
  bool _doubleClickEvent;
  uint32_t _lastDebounceTime;
  uint32_t _pressedTime;
  uint32_t _lastReleaseTime;
  bool _lastReading;
};

#endif // BTTN_UTILS_H
