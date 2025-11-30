#include <ComponentUtils8A.h>

RedGreenBlueLED rgbLED(9, 10, 11);

const unsigned long COLORWHEEL_INTERVAL = 50;
OneMoreTime colorwheelTimer(COLORWHEEL_INTERVAL);
const int RGB_LED_BRIGHTNESS = 90;
int hue = 0;

void setup() {
  rgbLED.begin();
  rgbLED.setBrightness(RGB_LED_BRIGHTNESS);
}

void loop() {
  spinColorwheel();
}

void spinColorwheel() {
  colorwheelTimer.update();
  if (colorwheelTimer.tick()) {
    rgbLED.setHSV(hue, 1.0, 1.0);
    hue = (hue + 1) % 360;
  }
}
