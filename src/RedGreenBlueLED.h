#ifndef RED_GREEN_BLUE_LED_H
#define RED_GREEN_BLUE_LED_H

#include <Arduino.h>

struct RedGreenBlue {
  static const uint8_t BLACK[3];
  static const uint8_t WHITE[3];
  static const uint8_t HOTPINK[3];
  static const uint8_t RED[3];
  static const uint8_t ORANGE[3];
  static const uint8_t YELLOW[3];
  static const uint8_t LIMEGREEN[3];
  static const uint8_t GREEN[3];
  static const uint8_t SPRING[3];
  static const uint8_t CYAN[3];
  static const uint8_t SKYBLUE[3];
  static const uint8_t BLUE[3];
  static const uint8_t VIOLET[3];
  static const uint8_t MAGENTA[3];
};

class RedGreenBlueLED {
  public:
    RedGreenBlueLED(
      uint8_t redPin,
      uint8_t greenPin,
      uint8_t bluePin,
      bool isCommonAnode = true,
      bool isESP32 = false,
      uint8_t redChannel = -1,
      uint8_t greenChannel = -1,
      uint8_t blueChannel = -1
    );

    ~RedGreenBlueLED();

    void begin();
    void setRGB(const uint8_t rgb[3]);
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);
    uint8_t* getRGB() const { return _RGB; }
    uint8_t getRed() const { return _RGB[0]; }
    uint8_t getGreen() const { return _RGB[1]; }
    uint8_t getBlue() const { return _RGB[2]; }
    void setHex(uint32_t hex);
    uint32_t getHex() const;
    String getHexString() const;
    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const { return _brightness; }
    void setHSV(int hue, float sat = 1.0, float val = 1.0);
    void setGammaCorrection(bool enabled);
    void off() { setRGB(RedGreenBlue::BLACK); }
    void setWhite() { setRGB(RedGreenBlue::WHITE); }
    void setPink() { setRGB(RedGreenBlue::HOTPINK); }
    void setRed() { setRGB(RedGreenBlue::RED); }
    void setOrange() { setRGB(RedGreenBlue::ORANGE); }
    void setYellow() { setRGB(RedGreenBlue::YELLOW); }
    void setLime() { setRGB(RedGreenBlue::LIMEGREEN); }
    void setGreen() { setRGB(RedGreenBlue::GREEN); }
    void setSpring() { setRGB(RedGreenBlue::SPRING); }
    void setCyan() { setRGB(RedGreenBlue::CYAN); }
    void setSky() { setRGB(RedGreenBlue::SKYBLUE); }
    void setBlue() { setRGB(RedGreenBlue::BLUE); }
    void setViolet() { setRGB(RedGreenBlue::VIOLET); }
    void setMagenta() { setRGB(RedGreenBlue::MAGENTA); }
    void setCMYK(float cyan, float magenta, float yellow, float key);
    void mapColor(int value, int fromValue, int toValue);
    void animateColorwheel(uint32_t period);

  private:
    bool _isCommonAnode, _gammaEnabled;
    int _redPin, _greenPin, _bluePin;
    uint8_t _brightness = 255;
    bool _isESP32;
    uint8_t _redChannel;
    uint8_t _greenChannel;
    uint8_t _blueChannel;

    uint8_t _RGB[3];
    uint8_t _setColor(uint8_t color);
    inline void _showRGB(uint8_t red, uint8_t green, uint8_t blue);
    static const uint8_t _gammaTable[256];
};

#endif
