#ifndef RED_GREEN_BLUE_LED_H
#define RED_GREEN_BLUE_LED_H

#include <Arduino.h>

class RedGreenBlueLED {
  public:
    RedGreenBlueLED(int redPin, int greenPin, int bluePin, bool isCommonAnode = true);
    bool begin();
    void setRGB(const uint8_t rgb[3]);
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);
    const uint8_t* getRGB() const;
    void setHex(uint32_t hex);
    const uint32_t getHex() const;
    String getHexString() const;
    void setBrightness(uint8_t brightness);
    const uint8_t getBrightness() const;
    void setHSV(int hue, float sat = 1.0, float val = 1.0);
    void setGammaCorrection(bool enabled);
    void off();
    void setWhite();
    void setPink();
    void setRed();
    void setOrange();
    void setYellow();
    void setLime();
    void setGreen();
    void setSpring();
    void setCyan();
    void setSky();
    void setBlue();
    void setViolet();
    void setMagenta();
    void setCMYK(float cyan, float magenta, float yellow, float key);
    void mapColor(int value, int fromValue, int toValue);

  private:
    bool _isCommonAnode, _gammaEnabled;
    int _redPin, _greenPin, _bluePin;
    uint8_t _brightness = 255;
    uint8_t _RGB[3];
    uint8_t setColor(uint8_t color);
    inline void showRGB(uint8_t red, uint8_t green, uint8_t blue);
    static const uint8_t _gammaTable[256];
};

class RedGreenBlue {
  public:
    static const uint8_t BLACK[3];
    static const uint8_t WHITE[3];
    static const uint8_t HOT_PINK[3];
    static const uint8_t RED[3];
    static const uint8_t ORANGE[3];
    static const uint8_t YELLOW[3];
    static const uint8_t LIME_GREEN[3];
    static const uint8_t GREEN[3];
    static const uint8_t SPRING_GREEN[3];
    static const uint8_t CYAN[3];
    static const uint8_t SKY_BLUE[3];
    static const uint8_t BLUE[3];
    static const uint8_t VIOLET[3];
    static const uint8_t MAGENTA[3];
};

#endif
