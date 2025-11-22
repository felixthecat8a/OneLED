#ifndef ONE_MORE_TIME_H
#define ONE_MORE_TIME_H

#include <Arduino.h>

class OneMoreTime {
public:
  OneMoreTime();
  OneMoreTime(unsigned long interval);

  void begin(unsigned long interval);
  void update();

  bool tick();
  bool isExpired() const;
  bool hasElapsed() const;
  void reset();
  void restart();

  void setInterval(unsigned long interval);
  unsigned long getInterval() const;
  unsigned long elapsed() const;

  void pause();
  void resume();
  bool isPaused() const;

private:
  unsigned long _interval = 1000;  // default 1 second
  unsigned long _lastMillis = 0;
  unsigned long _now = 0;
  unsigned long _pauseStart = 0;
  bool _paused = false;
};

#endif
