#ifndef ONE_MORE_TIME_H
#define ONE_MORE_TIME_H

#include <Arduino.h>

typedef void (*TimerCallback)();

class OneMoreTime {
  public:
    OneMoreTime(unsigned long interval);

    void update();
    unsigned long elapsed() const;
    void reset();
    void restart();
    bool tick();

    bool isExpired() const;
    void setInterval(unsigned long interval);
    unsigned long getInterval() const;

    void pause();
    void resume();
    bool isPaused() const;

    void onExpire(TimerCallback cb);

  private:
    unsigned long _interval = 1000;
    unsigned long _lastMillis = 0;
    unsigned long _now = 0;
    unsigned long _pauseStart = 0;
    bool _paused = false;
    TimerCallback _callback = nullptr;
};

#endif
