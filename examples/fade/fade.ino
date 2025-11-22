#include <OneLED.h>

OneLED fadeLED(3, true);

const unsigned long FADE_INTERVAL = 5;
OneMoreTime fadeTimer(FADE_INTERVAL);

int brightness = 0;
int step = 1;

void setup() {
  fadeLED.begin();
}

void loop() {
  fadeInAndOut();
}

void fadeInAndOut() {
  fadeTimer.update();

  if (fadeTimer.tick()) {
    fadeLED.setBrightness(brightness);
    brightness += step;

    if (brightness >= 255) {
      brightness = 255;
      step = -1;
    } else if (brightness <= 0) {
      brightness = 0;
      step = 1;
    }
  }
}
