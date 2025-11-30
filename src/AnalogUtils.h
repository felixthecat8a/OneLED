#ifndef ONE_LDR_H
#define ONE_LDR_H

#include <Arduino.h>

#define SMA_SIZE 5

class AnalogIn {
public:
  explicit AnalogIn(uint8_t pin)
    : _pin(pin), _sampleIndex(0), _bufferFilled(false) {}

  virtual ~AnalogIn() = default;

  virtual void begin(int resolutionADC = 10) {

    #if defined(analogReadResolution)
      analogReadResolution(resolutionADC);
      _scaleADC = getScaleFactorADC(resolutionADC);
    #else
      _scaleADC = getScaleFactorADC(10);
    #endif

    pinMode(_pin, INPUT);

    int first = analogRead(_pin);
    _lastRaw = first;
    for (int i = 0; i < SMA_SIZE; i++) {
      _samples[i] = first;
    }
    _bufferFilled = true;
  }

  virtual void update() {
    _lastRaw = analogRead(_pin);
    _samples[_sampleIndex] = _lastRaw;
    _sampleIndex = (_sampleIndex + 1) % SMA_SIZE;
    if (_sampleIndex == 0) _bufferFilled = true;
  }

  virtual int readRaw() const {
    return _lastRaw;
  }

  virtual float readNormalized() const{
    return (float)readRaw() / _scaleADC;
  }

  virtual int readMapped(int minOut, int maxOut) const {
    int raw = constrain(readRaw(), 0, _scaleADC);
    return map(raw, 0, _scaleADC, minOut, maxOut);
  }

  virtual int readSmoothed() const {
    long sum = 0;
    uint8_t count = _bufferFilled ? SMA_SIZE : _sampleIndex;

    for (uint8_t i = 0; i < count; i++) {
      sum += _samples[i];
    }

    return count > 0 ? (sum / count) : 0;
  }

  virtual float readNormalizedSmoothed() const {
    return (float)readSmoothed() / _scaleADC;
  }

  virtual int readMappedSmoothed(int minOut, int maxOut) const {
    return map(readSmoothed(), 0, _scaleADC, minOut, maxOut);
  }

  unsigned int getScale() const {
    return _scaleADC;
  }

protected:
  uint8_t _pin;
  unsigned int _scaleADC = 1023;

  int _lastRaw;
  int _samples[SMA_SIZE];
  uint8_t _sampleIndex = 0;
  bool _bufferFilled = false;

  unsigned int getScaleFactorADC(int resolution) {
    if (resolution == 14) {
      return 16383;  // 14-bit max (2^14 - 1)
    } else if (resolution == 12) {
      return 4095;   // 12-bit max (2^12 - 1)
    } else {
      return 1023;   // 10-bit max (2^10 - 1)
    }
  }
};

class KnobUtils : public AnalogIn {
public:
  KnobUtils(uint8_t pin) : AnalogIn(pin) {}

  void begin(int resolutionADC = 10) override {
      AnalogIn::begin(resolutionADC);
  }

  void update() override {
      AnalogIn::update();
  }

  int getRawInput() const { return readRaw(); }
  float getVoltage() const { return readNormalized(); }
  float getSmoothedVoltage() const { return readNormalizedSmoothed(); }
  float getPercentage() const { return readNormalized() * 100.0f; }

  int mapTo(int minOut, int maxOut) const {
    return readMapped(minOut, maxOut);
  }

  int mapToSmoothed(int minOut, int maxOut) const {
    return readMappedSmoothed(minOut, maxOut);
  }

  // angle in degrees for 270° pots
  float getAngle(float maxDegrees = 270.0f) const {
    return readNormalizedSmoothed() * maxDegrees;
  }
};

/* GND >> 10kohm Resistor >> A1 >> Photoresistor >> 5V */

class LDR_Utils : public AnalogIn {
public:
  LDR_Utils(uint8_t pin, float rFixed = 10000.0, float vRef = 5.0, float luxCoeff = 500.0)
    : AnalogIn(pin), _rFixed(rFixed), _vRef(vRef), _luxCoeff(luxCoeff) {}

  void begin(int resolutionADC = 10) override {
      AnalogIn::begin(resolutionADC);
  }

  void update() override {
      AnalogIn::update();
  }

  int getRawInput() const { return readRaw(); }
  float getNormalized() const { return readNormalized(); }
  float getSmoothedNormalized() const { return readNormalizedSmoothed(); }

  float getVoltage() const { return getNormalized() * _vRef; }
  float getSmoothedVoltage() const { return getSmoothedNormalized() * _vRef; }
  float getPercentage() const { return getNormalized() * 100.0f; }

  float estimateLux() const {
    float voltage = getSmoothedVoltage();
    if (voltage <= 0.0f) return 0.0f;
    float resistance = _rFixed * ((_vRef / voltage) - 1.0f);
    float r_kOhm = resistance / 1000.0f;
    return _luxCoeff / pow(r_kOhm, 1.4f);
  }

private:
  float _rFixed;
  float _vRef;
  float _luxCoeff;
};

/* GND >> NTC Thermistor >> A2 >> 10kohm Resistor >> 5V */

class ThermistorUtils : public AnalogIn {
public:
  ThermistorUtils(uint8_t pin, float r25, float beta, float rPullup = 10000.0, float vRef = 5.0)
    : AnalogIn(pin), _r25(r25), _beta(beta), _rPullup(rPullup), _vRef(vRef) {}

  void begin(int resolutionADC = 10) override {
      AnalogIn::begin(resolutionADC);
  }

  void update() override {
      AnalogIn::update();
  }

  int getRawInput() const { return readRaw(); }
  float getNormalized() const { return readNormalized(); }
  float getSmoothedNormalized() const { return readNormalizedSmoothed(); }

  float getVoltage() const { return getNormalized() * _vRef; }
  float getSmoothedVoltage() const { return getSmoothedNormalized() * _vRef; }

  float getKelvin() {
    float resistance = _readResistance();
    if (resistance <= 0) return NAN;
    float invT = (1.0 / 298.15) + (1.0 / _beta) * log(resistance / _r25);
    return 1.0 / invT;
  }

  float getCelsius() {
    return getKelvin() - 273.15;
  }

  float getFahrenheit() {
    return (getCelsius() * 9.0 / 5.0) + 32.0;
  }

  float getTemperatureColorHueF(float tempF, float roomTempF = 72.0, float maxShiftF = 18.0) {
    //float tempF = getFahrenheit();
    float hue;

    if (tempF >= roomTempF) {
      float diff = min(tempF - roomTempF, maxShiftF);
      hue = GREEN_HUE - (diff * (HUE_SHIFT_RANGE / maxShiftF));
    } else {
      float diff = min(roomTempF - tempF, maxShiftF);
      hue = GREEN_HUE + (diff * (HUE_SHIFT_RANGE / maxShiftF));
    }

    return constrain(hue, 0.0, 240.0);
  }

private:
  float _r25;
  float _beta;
  float _rPullup;
  float _vRef;
  float _readResistance() {
    float vOut = readNormalizedSmoothed() * _vRef;
    if (vOut == 0 || vOut >= _vRef) return INFINITY;
    return (_rPullup * vOut) / (_vRef - vOut);
  }
  static constexpr float GREEN_HUE = 120.0;
  static constexpr float HUE_SHIFT_RANGE = 120.0;
};

#endif
