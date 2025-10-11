#include "OneMoreTime.h"

OneMoreTime::OneMoreTime() {
  reset();
}

OneMoreTime::OneMoreTime(unsigned long interval)
  : _interval(interval) {
  reset();
}

void OneMoreTime::begin(unsigned long interval) {
  setInterval(interval);
  reset();
}

void OneMoreTime::update() {
  if (!_paused) {
    _now = millis();
  }
}

bool OneMoreTime::tick() {
  if (isExpired()) {
    restart();
    return true;
  }
  return false;
}

bool OneMoreTime::isExpired() const {
  return elapsed() >= _interval;
}

bool OneMoreTime::hasElapsed() const {
  return isExpired();
}

void OneMoreTime::reset() {
  _lastMillis = millis();
  _now = _lastMillis;
}

void OneMoreTime::restart() {
  _lastMillis = _now;
}

void OneMoreTime::setInterval(unsigned long interval) {
  _interval = interval;
}

unsigned long OneMoreTime::getInterval() const {
  return _interval;
}

unsigned long OneMoreTime::elapsed() const {
  return _now >= _lastMillis ? _now - _lastMillis : 0;
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
