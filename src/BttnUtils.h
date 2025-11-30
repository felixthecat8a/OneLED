#ifndef BTTN_UTILS_H
#define BTTN_UTILS_H

#include <Arduino.h>

class BttnUtils {
public:
  BttnUtils(uint8_t pin, bool activeLow = true, uint16_t debounceTime = 50);

  bool begin();
  void update();
  bool isPressed() const;
  bool wasPressed();
  bool wasReleased();
  bool wasHeld();
  bool wasDoubleClicked();
  void setHoldTime(uint16_t ms);
  void setDoubleClickTime(uint16_t ms);
  bool isWaitingForDoubleClick() const;

private:
  uint8_t _pin;
  bool _activeLow;
  uint16_t _debounceTime;
  uint16_t _holdTime;
  uint16_t _doubleClickTime;

  enum ButtonState {
    BUTTON_IDLE,
    BUTTON_DEBOUNCING,
    BUTTON_PRESSED,
    BUTTON_HOLD,
    BUTTON_WAITING_DOUBLE,
    BUTTON_DOUBLE_CLICK
  };


  ButtonState _state;

  bool _pressedEvent;
  bool _releasedEvent;
  bool _heldEvent;
  bool _doubleClickEvent;
  uint32_t _lastDebounceTime;
  uint32_t _pressedTime;
  uint32_t _lastReleaseTime;
  bool _lastReading;
};

#endif // BTTN_UTILS_H
