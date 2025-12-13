# Project JP Lights

Project JP Lights is an LED-control project built around an Arduino Nano Every and a WS2812 LED string.
The goal was to design a control box with a pushbutton that cycles through multiple lighting modes and colors.

The enclosure was designed in Tinkercad, exported as STL files, and 3D-printed.
This sketch uses the `ComponentUtils8A` library for handling input timing and animation.

## Setup

```cpp
#include <EEPROM.h>
#include <ComponentUtils8A.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>

/* Button & Timer Setup */
#define BUTTON_PIN 2
Bttn_Utils button(BUTTON_PIN, true, 50);

const unsigned long ANIMATION_INTERVAL = 250;
OneMoreTime animationTimer(ANIMATION_INTERVAL);

/* NeoPixel Setup */
#define DATA_PIN A0
#define NUM_PIXELS 91
// Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);
const unsigned long ANIMATION_INTERVAL = 250;
OneMoreTime animationTimer(ANIMATION_INTERVAL);

/* NeoPixel Setup */
#define DATA_PIN A0
#define NUM_PIXELS 91
// Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

/* Mode & Color Initial Variables */
int currentMode = 0;
const int maxModes = 5;
int colorIndex = 0;
// const int numColors = 12;
const int numColors = 4;

/* EEPROM  Setup */
#define EEPROM_MODE_ADDR 0
#define EEPROM_COLOR_ADDR 1

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
```

## Original Color Array and Functions

Originally, the project used a 12-color palette.

```cpp
#define COLOR_OPTIONS 12

int COLOR_RGB[COLOR_OPTIONS][3] = {
  { 250, 0, 0 }, { 250, 127, 0 }, { 250, 250, 0 }, { 127, 250, 0 }, 
  { 0, 250, 0 }, { 0, 250, 127 }, { 0, 250, 250 }, { 0, 127, 250 }, 
  { 0, 0, 250 }, { 127, 0, 250 }, { 250, 0, 250 }, { 250, 0, 127 },
};
const char* COLOR_NAME[COLOR_OPTIONS] = {
  "Red", "Orange", "Yellow", "LimeGreen",
  "Green", "SpringGreen", "Cyan", "SkyBlue",
  "Blue", "Purple", "Magenta", "Pink", 
};
```

It was later simplified to four classic Christmas-light colors.

```cpp
#define COLOR_OPTIONS 4

int COLOR_RGB[COLOR_OPTIONS][3] = {
  { 250, 0, 0 }, { 255, 75, 0 }, { 0, 250, 0 }, { 0, 0, 250 }
};

const char* COLOR_NAME[COLOR_OPTIONS] = {
  "Red", "Amber", "Green", "Blue"
};

void setArrayColor(int c) {
  Serial.println(COLOR_NAME[c]);
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,COLOR_RGB[c][0],COLOR_RGB[c][1],COLOR_RGB[c][2]);
  }
  strip.show();
}
```

## Updated Color Palette and Functions

```cpp
const uint32_t redLight = strip.Color(255, 0, 0);
const uint32_t amberLight = strip.Color(255, 75, 0);
const uint32_t greenLight = strip.Color(0, 255, 0);
const uint32_t blueLight = strip.Color(0, 0, 255);

const uint32_t LIGHT_COLORS[] = { redLight, amberLight, greenLight, blueLight };

void setOneChristmasTreeLightColor(int c) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, LIGHT_COLORS[c]);
  }
  strip.show();
}
```

### Selected Color and White Odd/Even Function

```cpp
void setChristmasTreeLightColorAndWhite(int c) {
  for(int i = 0; i < strip.numPixels(); i++) {
    if (i % 2 == 0) {
      strip.setPixelColor(i, LIGHT_COLORS[c]);
    } else {
      strip.setPixelColor(i,127,127,127);
    }
  }
  strip.show();
}
```

### Selected Color Chase Animation Function

```cpp
void updateChristmasTreeLightColorAndWhiteChase(int c) {
  animationTimer.update();
  static int animationStep = 0;
  if(animationTimer.tick()) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if ((i + animationStep) % 3 == 0) {
        strip.setPixelColor(i, LIGHT_COLORS[c]);
      } else {
        strip.setPixelColor(i,127,127,127);
      }
    }
    strip.show();
    animationStep = (animationStep + 1) % 3;
  }
}
```

### Show All Colors

```cpp
void setAllChristmasTreeLightColors() {
  for (int i = 0; i < strip.numPixels(); i++) {
    int c = i % numColors;
    strip.setPixelColor(i, LIGHT_COLORS[c]);
  }
  strip.show();
}
```

### Animate All Colors

```cpp
void updateChristmasTreeLightColorChase() {
  animationTimer.update();
  static int animationStep = 0;
  if(animationTimer.tick()) {
    for (int i = 0; i < strip.numPixels(); i++) {
      int c = (i + animationStep) % numColors;
      strip.setPixelColor(i, LIGHT_COLORS[c]);
    }
    strip.show();
    animationStep = (animationStep + 1) % numColors;
  }
}
```

### Default Lights Off Function

```cpp
void setOff() {
  //Serial.println("Lights Off");
  for (int i = 0 ; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}
```

## Main

```cpp
void setup() {
  //Serial.begin(9600);
  button.begin();
  button.setHoldTime(1000); 
  button.setDoubleClickTime(250);
  
  loadSettings();
  strip.begin();
  strip.setBrightness(42);
  strip.clear();

  setupChristmasTreeLightTwinkle();
}

void loop() {
  button.update();

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

  if (button.wasDoubleClicked()) {
    currentMode--;
    if (currentMode < 0) currentMode = maxModes;
    //Serial.println("Double Click");
  }

  saveSettings();

  switch (currentMode) {
    case 0: setOneChristmasTreeLightColor(colorIndex); break;
    case 1: setChristmasTreeLightColorAndWhite(colorIndex); break;
    case 2: updateChristmasTreeLightColorAndWhiteChase(colorIndex); break;
    case 3: setAllChristmasTreeLightColors(); break;
    case 4: updateChristmasTreeLightColorChase(); break;
    case 5: updateChristmasTreeLightTwinkle(); break;
    default: setOff(); break;
  }
}
```


## Christmas Tree Light Color Animations

Working on some Christmas Tree string light animations.

> Initial Variables

```cpp
struct ColorLight {
  int pixel;
  int brightness;
  int direction;
  int colorIndex;
};

#define NUM_LIGHTS 30
ColorLight lights[NUM_LIGHTS];

unsigned long lastLightsUpdate = 0;
unsigned long lightInterval = 20;
```

> Initiate Animation in `setup()`

```cpp
void setupChristmasTreeLightTwinkle() {
  for (int i = 0; i < NUM_LIGHTS; i++) {
    lights[i].pixel = random(strip.numPixels());
    lights[i].brightness = random(0, 255);
    lights[i].direction = random(0, 2) * 2 - 1; // -1 or 1
    lights[i].colorIndex = random(numColors);
  }
}
```

> Run Animation in `loop()`

```cpp
void updateChristmasTreeLightTwinkle() {
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
      light.colorIndex = random(numColors);
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
