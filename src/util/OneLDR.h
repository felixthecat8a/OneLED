#ifndef ONE_LDR_H
#define ONE_LDR_H

#include <Arduino.h>

#define SMA_SIZE 5

/* AnalogInput */

class AnalogInput {
  public:
    explicit AnalogInput(uint8_t pin);
    void begin(int resolutionADC = 10);
    int readRaw() const;
    float readNormalized() const;
    int readMapped(int minOut, int maxOut) const;
    void update();
    int readSmoothed() const;
    float readNormalizedSmoothed() const;
    int readMappedSmoothed(int minOut, int maxOut) const;
    unsigned int getScale() const;

  private:
    uint8_t _pin;
    unsigned int _scaleADC = 1023;
    int _lastRaw;
    int _samples[SMA_SIZE];
    uint8_t _sampleIndex = 0;
    bool _bufferFilled = false;
    unsigned int getScaleFactorADC(int resolution);
};

/* GND >> 10kohm Resistor >> A1 >> Photoresistor >> 5V */

class OnePhotoresistor {
  public:
    OnePhotoresistor(uint8_t pin, float rFixed = 10000.0, float vRef = 5.0, float luxCoeff = 500.0);

    void begin(int resolutionADC = 10);
    void update();
    int getRawInput() const;
    float getNormalized() const;
    float getSmoothedNormalized() const;
    float getPercentage() const;
    float getVoltage() const;
    float getSmoothedVoltage() const;
    float estimateLux() const;

  private:
    AnalogInput _analog;
    float _rFixed;
    float _vRef;
    float _luxCoeff;
};

/* GND >> NTC Thermistor >> A2 >> 10kohm Resistor >> 5V */

class OneThermistor {
  public:
    OneThermistor(uint8_t pin, float r25, float beta, float rPullup = 10000.0, float vRef = 5.0);

    void begin(int resolutionADC = 10);
    void update();
    int getRawInput() const;
    float getVoltage() const;
    float getKelvin();
    float getCelsius();
    float getFahrenheit();

  private:
    AnalogInput _analog;
    float _r25;
    float _beta;
    float _rPullup;
    float _vRef;
    float _readResistance();
};

#endif
