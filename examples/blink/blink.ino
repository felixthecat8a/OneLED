#include "OneLED.h"

OneLED boardLED(13);

const unsigned long BLINK_INTERVAL = 500;
OneMoreTime blinkTimer(BLINK_INTERVAL);

void setup() {
  boardLED.begin();
}

void loop() {
  blinkBoardLED();
}

void blinkBoardLED() {
  blinkTimer.update();

  if (blinkTimer.tick()) {
    boardLED.toggle();
  }
}
