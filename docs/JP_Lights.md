# Project JP Lights

I'm working on a project to control a WS2812 LED string using an Arduino Nano Every.


## My Code using the 'ComponentUtils8A' library:

```cpp
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <ComponentUtils8A.h>
/*********************************************************************/
/* NeoPixel Setup */
#define DATA_PIN A0
#define NUM_PIXELS 91
Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);
/*********************************************************************/
/* Button Setup */
#define BUTTON_PIN 2
Bttn_Utils button(BUTTON_PIN, true, 50);
/*********************************************************************/
/* EEPROM */
#define EEPROM_MODE_ADDR 0
#define EEPROM_COLOR_ADDR 1
/*********************************************************************/
/* Modes & Colors */
int currentMode = 0;
const int maxModes = 3;
int colorIndex = 0;
const int numColors = 12;
/*********************************************************************/
/* EEPROM Functions */
void loadSettings() {
  EEPROM.get(EEPROM_MODE_ADDR, currentMode);
  EEPROM.get(EEPROM_COLOR_ADDR, colorIndex);
  if (currentMode < 0 || currentMode > maxModes) currentMode = 0;
  if (colorIndex < 0 || colorIndex >= numColors) colorIndex = 0;
}

void saveSettings() {
  EEPROM.update(EEPROM_MODE_ADDR, currentMode);
  EEPROM.update(EEPROM_COLOR_ADDR, colorIndex);
}
/*********************************************************************/
#define NUM_COLOR_OPTIONS 12
int COLOR_RGB[NUM_COLOR_OPTIONS][3] = {
   {250, 10, 10}, {250, 127, 10}, {250, 250, 10}, {127, 250, 10}, 
  {10, 250, 0}, {10, 250, 127}, {10, 250, 250}, {10, 127, 250}, 
  {10, 10, 250}, {127, 10, 250},  {250, 10, 250},{250, 10, 127},
};
const char* COLOR_NAME[NUM_COLOR_OPTIONS] = {
  "Red", "Orange", "Yellow", "LimeGreen",
  "Green", "SpringGreen", "Cyan", "SkyBlue",
  "Blue", "Purple", "Magenta", "Pink", 
};
/********************************************************************************/
void setArrayColor(int c) {
  //Serial.println(COLOR_NAME[c]);
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
  }
  strip.show();
}
/********************************************************************************/
void setArrayColorAndWhite(int c) {
  //Serial.println(COLOR_NAME[c]); Serial.println(" and White");
  for(int i = 0; i < strip.numPixels(); i++) {
    if (i % 2 == 0) {
      strip.setPixelColor(i,COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
    } else {
      strip.setPixelColor(i,180,180,180);
    }
  }
  strip.show();
}
/********************************************************************************/
const unsigned long ANIMATION_INTERVAL = 150;
OneMoreTime animationTimer(ANIMATION_INTERVAL);

void updateArrayColorChase(int c) {
  animationTimer.update();
  static int animationStep = 0;
  if(animationTimer.tick()) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if ((i + animationStep) % 3 == 0) {
        strip.setPixelColor(i,COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
      } else {
        strip.setPixelColor(i,127,127,127);
      }
    }
    strip.show();
    animationStep = (animationStep + 1) % 3;
  }
}
/********************************************************************************/
void setOff() {
  //Serial.println("Lights Off");
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}
/*********************************************************************/
void setup() {
  //Serial.begin(9600);
  button.begin();
  button.setHoldTime(1000); 
  button.setDoubleClickTime(350);
  
  loadSettings();
  strip.begin();
  strip.setBrightness(42);
  strip.clear();

  setupChristmasTreeLights();
}

void loop() {
  button.update();

  if (button.wasDoubleClicked()) {
    currentMode = 0;
    //Serial.println("Double Click");
  }

  if (button.wasPressed()) {
    currentMode++;
    if (currentMode > maxModes) currentMode = 0;
    //Serial.println("Click");
  }

  if (button.wasHeld()) {
    colorIndex++;
    if (colorIndex >= numColors) colorIndex = 0;
    //Serial.println("Button Held");
  }

  saveSettings();

  switch (currentMode) {
    case 0: setArrayColor(colorIndex); break;
    case 1: setArrayColorAndWhite(colorIndex); break;
    case 2: updateArrayColorChase(colorIndex); break;
    case 3: updateChristmasTreeLights(); break;
    default: setOff(); break;
  }
}
```

Working on some string light animations.

```cpp

const uint32_t TREE_LIGHT_COLORS[] = {
  strip.Color(255, 0, 0),
  strip.Color(0, 255, 0),
  strip.Color(255, 180, 0),
  strip.Color(0, 0, 255)
};
const int NUM_TREE_LIGHT_COLORS = 4;

const uint32_t LIGHT_COLORS[] = {
  strip.Color(255, 0, 0),
  strip.Color(255, 255, 0),
  strip.Color(0, 255, 0),
  strip.Color(0, 255, 255),
  strip.Color(0, 0, 255),
  strip.Color(255, 0, 255)
};
const int NUM_LIGHT_COLORS = 6;

struct ColorLight {
  int pixel;
  int brightness;
  int direction;
  int colorIndex;
};

#define NUM_LIGHTS 20
ColorLight lights[NUM_LIGHTS];

unsigned long lastLightsUpdate = 0;
unsigned long lightInterval = 20;

void setupChristmasTreeLights() {
  for (int i = 0; i < NUM_LIGHTS; i++) {
    lights[i].pixel = random(strip.numPixels());
    lights[i].brightness = random(0, 255);
    lights[i].direction = random(0, 2) * 2 - 1; // -1 or 1
    lights[i].colorIndex = random(NUM_LIGHT_COLORS);
  }
}

void updateChristmasTreeLights() {
  unsigned long now = millis();
  if (now - lastLightsUpdate < lightInterval) return;
  lastLightsUpdate = now;

  strip.clear();

  for (int i = 0; i < NUM_LIGHTS; i++) {
    ColorLight &light = lights[i];

    light.brightness += light.direction * 5;

    if (light.brightness >= 255) {
      light.direction = -1;
    }
    if (light.brightness <= 0) {
      light.pixel = random(strip.numPixels());
      light.colorIndex = random(NUM_LIGHT_COLORS);
      light.direction = 1;
      light.brightness = 0;
    }

    uint32_t c = LIGHT_COLORS[light.colorIndex];

    uint8_t r = ((c >> 16) & 0xFF) * light.brightness / 255;
    uint8_t g = ((c >> 8) & 0xFF) * light.brightness / 255;
    uint8_t b = (c & 0xFF) * light.brightness / 255;

    strip.setPixelColor(light.pixel, strip.Color(r, g, b));
  }

  strip.show();
}
```
