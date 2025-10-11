# OneLED

A lightweight Arduino library for simple LED control.


## Installing the Library into Arduino IDE

### Install via ZIP file

1. Download the library ZIP file from the latest release.
2. Open the Arduino IDE.
3. Go to **Sketch > Include Library > Add .ZIP Library...**.
4. Select the downloaded `.zip` file and click **Open**.
5. The library is now installed and ready to use.

## Features

### OneLED

A minimal class to control a single digital LED with optional logic inversion.

- `on()`, `off()`, `toggle()` for easy control
- `isOn()` to check LED state

### PulseWidthModulatedLED

A simple PWM-based LED brightness controller using `analogWrite`.

- `setBrightness(value)` for 0–255 control
- Tracks brightness state
- Works with any PWM-capable pin

### RedGreenBlueLED

Control a common cathode or common anode RGB LED with ease. 
Supports gamma correction, HSV conversion, brightness control, and color presets.

- `setRGB()`, `setHex()`, and `setHSV()` color input
- `setBrightness()` for global dimming
- `setGammaCorrection(true)` for perceptual brightness
- 14 built-in color presets 
- Compatible with both common cathode and anode RGB LEDs

### OneMoreTime

A lightweight Arduino class for non-blocking timing based on `millis()`. 

- `tick()` returns true once per interval
- `isExpired()` for manual checking
- `restart()` and `reset()` for control
- Configurable interval with `setInterval()`
- Works on all boards using `millis()`
