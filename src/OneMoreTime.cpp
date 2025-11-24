#include "OneMoreTime.h"

OneMoreTime::OneMoreTime(unsigned long interval)
  : _interval(interval) {
  reset();
}

void OneMoreTime::update() {
  _now = millis();
}

unsigned long OneMoreTime::elapsed() const {
  return _now - _lastMillis;
}

void OneMoreTime::reset() {
  _lastMillis = millis();
  _now = _lastMillis;
}

void OneMoreTime::restart() {
  _lastMillis = millis();
  _now = _lastMillis;
}

bool OneMoreTime::tick() {
  if (isExpired()) {
    restart();

    if (_callback != nullptr) {
      _callback();
    }

    return true;
  }
  return false;
}

bool OneMoreTime::isExpired() const {
  return elapsed() >= _interval;
}

void OneMoreTime::setInterval(unsigned long interval) {
  _interval = interval;
}

unsigned long OneMoreTime::getInterval() const {
  return _interval;
}

void OneMoreTime::pause() {
  if (!_paused) {
    _paused = true;
    _pauseStart = millis();
  }
}

void OneMoreTime::resume() {
  if (_paused) {
    unsigned long pausedDuration = millis() - _pauseStart;
    _lastMillis += pausedDuration;
    _now += pausedDuration;
    _paused = false;
  }
}

bool OneMoreTime::isPaused() const {
  return _paused;
}

void OneMoreTime::onExpire(TimerCallback cb) {
  _callback = cb;
}
