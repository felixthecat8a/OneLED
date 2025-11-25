#include "RedGreenBlueLED.h"

#ifdef ESP32
static bool _ledcChannelUsed[16] = { false };

static int8_t allocateLEDCChannel() {
  for (int i = 0; i < 16; i++) {
    if (!_ledcChannelUsed[i]) {
      _ledcChannelUsed[i] = true;
      return i;
    }
  }
  return -1; // No available channels
}
#endif

RedGreenBlueLED::RedGreenBlueLED(
  uint8_t redPin,
  uint8_t greenPin,
  uint8_t bluePin,
  bool isCommonAnode,
  bool isESP32,
  uint8_t redChannel,
  uint8_t greenChannel,
  uint8_t blueChannel
)
  : _redPin(redPin),
    _greenPin(greenPin),
    _bluePin(bluePin),
    _isCommonAnode(isCommonAnode),
    _isESP32(isESP32),
    _redChannel(redChannel),
    _greenChannel(greenChannel),
    _blueChannel(blueChannel),
    _brightness(255),
    _gammaEnabled(false)
{
  _RGB[0] = _RGB[1] = _RGB[2] = 0;
}

void RedGreenBlueLED::begin() {
  if (_isESP32) {
    #ifdef ESP32

      if (_redChannel < 0)   _redChannel   = allocateLEDCChannel();
      if (_greenChannel < 0) _greenChannel = allocateLEDCChannel();
      if (_blueChannel < 0)  _blueChannel  = allocateLEDCChannel();

      if (_redChannel < 0 || _greenChannel < 0 || _blueChannel < 0) {
        return;
      }

      #if ESP_ARDUINO_VERSION_MAJOR >= 3
        ledcAttach(_redPin, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcAttach(_greenPin, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcAttach(_bluePin, ESP32_PWM_FREQ, ESP32_PWM_RES);
      #else
        ledcSetup(_redChannel, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcSetup(_greenChannel, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcSetup(_blueChannel, ESP32_PWM_FREQ, ESP32_PWM_RES);
        ledcAttachPin(_redPin, _redChannel);
        ledcAttachPin(_greenPin, _greenChannel);
        ledcAttachPin(_bluePin, _blueChannel);
      #endif
    #endif
  } else {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
  }
}

uint8_t RedGreenBlueLED::_setColor(uint8_t color) {
  color = map(constrain(color, 0, 255), 0, 255, 0, _brightness);
  if (_gammaEnabled) { color = _gammaTable[color]; }
  if (_isCommonAnode) { color = 255 - color; }
  return color;
}

void RedGreenBlueLED::_showRGB(uint8_t red, uint8_t green, uint8_t blue) {
  _RGB[0] = red; _RGB[1] = green; _RGB[2] = blue;

  if (_isESP32) {
    #ifdef ESP32
      ledcWrite(_redPin, _setColor(red));
      ledcWrite(_greenPin, _setColor(green));
      ledcWrite(_bluePin, _setColor(blue));
    #endif
  } else {
    analogWrite(_redPin, _setColor(red));
    analogWrite(_greenPin, _setColor(green));
    analogWrite(_bluePin, _setColor(blue));
  }
}

void RedGreenBlueLED::setRGB(const uint8_t rgb[3]) {
  _showRGB(rgb[0], rgb[1], rgb[2]);
}

void RedGreenBlueLED::setRGB(uint8_t red, uint8_t green, uint8_t blue) {
  _showRGB(red, green, blue);
}

const uint8_t* RedGreenBlueLED::getRGB() const { return _RGB; }
const uint8_t RedGreenBlueLED::getRed() const { return _RGB[0]; }
const uint8_t RedGreenBlueLED::getGreen() const { return _RGB[1]; }
const uint8_t RedGreenBlueLED::getBlue() const { return _RGB[2]; }

void RedGreenBlueLED::setHex(uint32_t hex) {
  _showRGB((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF);
}

const uint32_t RedGreenBlueLED::getHex() const {
  return ((uint32_t)_RGB[0] << 16) | ((uint32_t)_RGB[1] << 8) | (uint32_t)_RGB[2];
}

String RedGreenBlueLED::getHexString() const {
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "#%06lX", getHex());
  return String(buffer);
}

void RedGreenBlueLED::setBrightness(uint8_t brightness) {
  _brightness = constrain(brightness, 0, 255);
  _showRGB(_RGB[0], _RGB[1], _RGB[2]);
}

const uint8_t RedGreenBlueLED::getBrightness() const {
  return _brightness;
}

void RedGreenBlueLED::setHSV(int hue, float sat, float val) {
  hue = constrain(hue, 0, 359);
  sat = constrain(sat, 0.0, 1.0);
  val = constrain(val, 0.0, 1.0);
  const int SECTOR_SIZE = 60;
  int hueSector = hue / SECTOR_SIZE;
  float hueFraction = (hue / (float)SECTOR_SIZE) - hueSector;
  float minVal = val * (1.0f - sat);
  float intVal1 = val * (1.0f - sat * hueFraction);
  float intVal2 = val * (1.0f - sat * (1.0f - hueFraction));
  float red, green, blue;
  switch (hueSector % 6) {
    case 0: red = val, green = intVal2, blue = minVal; break;
    case 1: red = intVal1, green = val, blue = minVal; break;
    case 2: red = minVal, green = val, blue = intVal2; break;
    case 3: red = minVal, green = intVal1, blue = val; break;
    case 4: red = intVal2, green = minVal, blue = val; break;
    case 5: red = val, green = minVal, blue = intVal1; break;
    default: red = green = blue = 0; break;
  }
  _showRGB(roundf(red * 255), roundf(green * 255), roundf(blue * 255));
}

void RedGreenBlueLED::setGammaCorrection(bool enabled) {
  _gammaEnabled = enabled;
}

const uint8_t RedGreenBlueLED::_gammaTable[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5,
  5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 12,
  13, 13, 14, 15, 15, 16, 16, 17, 18, 19, 19, 20, 21, 22, 23, 23,
  24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40,
  41, 42, 43, 44, 46, 47, 48, 50, 51, 52, 54, 55, 56, 58, 59, 61,
  62, 64, 65, 67, 68, 70, 71, 73, 74, 76, 78, 79, 81, 83, 84, 86,
  88, 90, 91, 93, 95, 97, 99,100,102,104,106,108,110,112, 114,116,
  118,120,122,124,126,128,130,132,134,136,138,141,143,145,147,149,
  152,154,156,159,161,163,166,168,170,173,175,178,180,183,185,188,
  190,193,195,198,201,203,206,209,211,214,217,220,222,225,228,231,
  234,237,239,242,245,248,251,254,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

void RedGreenBlueLED::off() { setRGB(RedGreenBlue::BLACK); }
void RedGreenBlueLED::setWhite() { setRGB(RedGreenBlue::WHITE); }
void RedGreenBlueLED::setPink() { setRGB(RedGreenBlue::HOTPINK); }
void RedGreenBlueLED::setRed() { setRGB(RedGreenBlue::RED); }
void RedGreenBlueLED::setOrange() { setRGB(RedGreenBlue::ORANGE); }
void RedGreenBlueLED::setYellow() { setRGB(RedGreenBlue::YELLOW); }
void RedGreenBlueLED::setLime() { setRGB(RedGreenBlue::LIMEGREEN); }
void RedGreenBlueLED::setGreen() { setRGB(RedGreenBlue::GREEN); }
void RedGreenBlueLED::setSpring() { setRGB(RedGreenBlue::SPRING); }
void RedGreenBlueLED::setCyan() { setRGB(RedGreenBlue::CYAN); }
void RedGreenBlueLED::setSky() { setRGB(RedGreenBlue::SKYBLUE); }
void RedGreenBlueLED::setBlue() { setRGB(RedGreenBlue::BLUE); }
void RedGreenBlueLED::setViolet() { setRGB(RedGreenBlue::VIOLET); }
void RedGreenBlueLED::setMagenta() { setRGB(RedGreenBlue::MAGENTA); }

void RedGreenBlueLED::setCMYK(float cyan, float magenta, float yellow, float key) {
  cyan = constrain(cyan, 0.0f, 1.0f);
  magenta = constrain(magenta, 0.0f, 1.0f);
  yellow = constrain(yellow, 0.0f, 1.0f);
  key = constrain(key, 0.0f, 1.0f);
  uint8_t red = roundf(255 * (1.0f - cyan) * (1.0f - key));
  uint8_t green = roundf(255 * (1.0f - magenta) * (1.0f - key));
  uint8_t blue = roundf(255 * (1.0f - yellow) * (1.0f - key));
  _showRGB(red, green, blue);
}

void RedGreenBlueLED::mapColor(int value, int fromValue, int toValue) {
  int minValue = min(fromValue, toValue);
  int maxValue = max(fromValue, toValue);
  value = constrain(value, minValue, maxValue);
  int hue = map(value, minValue, maxValue, 0, 359);
  setHSV(hue);
}

void RedGreenBlueLED::animateColorwheel(uint32_t period) {
  float t = (millis() % period) / (float)period;
  int hue = (int)(t * 360.0f);
  setHSV(hue, 1.0f, 1.0f);
}

const uint8_t RedGreenBlue::BLACK[3] = { 0, 0, 0 };
const uint8_t RedGreenBlue::WHITE[3] = { 255, 255, 255 };
const uint8_t RedGreenBlue::HOTPINK[3] = { 255, 0, 127 };
const uint8_t RedGreenBlue::RED[3] = { 255, 0, 0 };
const uint8_t RedGreenBlue::ORANGE[3] = { 255, 127, 0 };
const uint8_t RedGreenBlue::YELLOW[3] = { 255, 255, 0 };
const uint8_t RedGreenBlue::LIMEGREEN[3] = { 127, 255, 0 };
const uint8_t RedGreenBlue::GREEN[3] = { 0, 255, 0 };
const uint8_t RedGreenBlue::SPRING[3] = { 0, 255, 127 };
const uint8_t RedGreenBlue::CYAN[3] = { 0, 255, 255 };
const uint8_t RedGreenBlue::SKYBLUE[3] = { 0, 127, 255 };
const uint8_t RedGreenBlue::BLUE[3] = { 0, 0, 255 };
const uint8_t RedGreenBlue::VIOLET[3] = { 127, 0, 255 };
const uint8_t RedGreenBlue::MAGENTA[3] = { 255, 0, 255 };
