#include "OneLDR.h"

#include <cmath>

/* AnalogInput */

AnalogInput::AnalogInput(uint8_t pin) : _pin(pin), _sampleIndex(0), _bufferFilled(false) {}

void AnalogInput::begin(int resolutionADC) {

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

unsigned int AnalogInput::getScaleFactorADC(int resolution) {
  if (resolution == 14) {
    return 16383;  // 14-bit max (2^14 - 1)
  } else if (resolution == 12) {
    return 4095;   // 12-bit max (2^12 - 1)
  } else {
    return 1023;   // 10-bit max (2^10 - 1)
  }
}

void AnalogInput::update() {
  _lastRaw = analogRead(_pin);
  _samples[_sampleIndex] = _lastRaw;
  _sampleIndex = (_sampleIndex + 1) % SMA_SIZE;
  if (_sampleIndex == 0) _bufferFilled = true;
}

int AnalogInput::readRaw() const {
  return _lastRaw;
}

int AnalogInput::readSmoothed() const {
  long sum = 0;
  uint8_t count = _bufferFilled ? SMA_SIZE : _sampleIndex;

  for (uint8_t i = 0; i < count; i++) {
    sum += _samples[i];
  }

  return count > 0 ? (sum / count) : 0;
}

float AnalogInput::readNormalized() const {
  return (float)readRaw() / _scaleADC;
}

float AnalogInput::readNormalizedSmoothed() const {
  return (float)readSmoothed() / _scaleADC;
}

int AnalogInput::readMapped(int minOut, int maxOut) const {
  int raw = constrain(readRaw(), 0, _scaleADC);
  return map(raw, 0, _scaleADC, minOut, maxOut);
}

int AnalogInput::readMappedSmoothed(int minOut, int maxOut) const {
  return map(readSmoothed(), 0, _scaleADC, minOut, maxOut);
}

unsigned int AnalogInput::getScale() const {
  return _scaleADC;
}

/* OnePhotoresistor */

OnePhotoresistor::OnePhotoresistor(uint8_t pin, float rFixed, float vRef, float luxCoeff)
  : _analog(pin), _rFixed(rFixed), _vRef(vRef), _luxCoeff(luxCoeff) {}

void OnePhotoresistor::begin(int resolutionADC) {
  _analog.begin(resolutionADC);
}

void OnePhotoresistor::update() {
  _analog.update();
}

int OnePhotoresistor::getRawInput() const {
  return _analog.readRaw();
}

float OnePhotoresistor::getNormalized() const {
  return _analog.readNormalized();
}

float OnePhotoresistor::getSmoothedNormalized() const {
  return _analog.readNormalizedSmoothed();
}

float OnePhotoresistor::getPercentage() const {
  return getNormalized() * 100.0;
}

float OnePhotoresistor::getVoltage() const {
  return getNormalized() * _vRef;
}

float OnePhotoresistor::getSmoothedVoltage() const {
  return getSmoothedNormalized() * _vRef;
}

float OnePhotoresistor::estimateLux() const {
    float voltage = getVoltage();
    if (voltage <= 0.0) return 0.0;
    float resistance = (_rFixed * ((_vRef / voltage) - 1.0));
    float r_kOhm = resistance / 1000.0;
    return _luxCoeff / pow((double)r_kOhm, 1.4);
    //return _luxCoeff / ((resistance / 1000.0) + 1.0);
}

/* OneThermistor */

OneThermistor::OneThermistor(uint8_t pin, float r25, float beta, float rPullup, float vRef)
  : _analog(pin), _r25(r25), _beta(beta), _rPullup(rPullup), _vRef(vRef) {}

void OneThermistor::begin(int resolutionADC) {
  _analog.begin(resolutionADC);
}

void OneThermistor::update() {
  _analog.update();
}

int OneThermistor::getRawInput() const {
  return _analog.readRaw();
}

float OneThermistor::getVoltage() const {
  return _analog.readSmoothed() * _vRef / _analog.getScale();
}

float OneThermistor::_readResistance() {
  float vOut = _analog.readNormalizedSmoothed() * _vRef;
  if (vOut == 0 || vOut >= _vRef) return INFINITY;
  return (_rPullup * vOut) / (_vRef - vOut);
}

float OneThermistor::getKelvin() {
  float resistance = _readResistance();
  if (resistance <= 0) return NAN;
  float invT = (1.0 / 298.15) + (1.0 / _beta) * log(resistance / _r25);
  return 1.0 / invT;
}

float OneThermistor::getCelsius() {
  return getKelvin() - 273.15;
}

float OneThermistor::getFahrenheit() {
  return (getCelsius() * 9.0 / 5.0) + 32.0;
}
