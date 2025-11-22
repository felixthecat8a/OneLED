# OneLED

![GitHub Release](https://img.shields.io/github/v/release/felixthecat8a/OneLED?logo=arduino&logoColor=00878F&logoSize=auto&color=00878F)
[![License: MIT](https://img.shields.io/github/license/felixthecat8a/OneLED)](https://opensource.org/licenses/MIT)
![GitHub repo size](https://img.shields.io/github/repo-size/felixthecat8a/OneLED)
![GitHub repo file or directory count](https://img.shields.io/github/directory-file-count/felixthecat8a/OneLED)
![GitHub top language](https://img.shields.io/github/languages/top/felixthecat8a/OneLED)

![Check Arduino](https://github.com/felixthecat8a/OneLED/actions/workflows/compile-examples.yml/badge.svg)
![Spell Check](https://github.com/felixthecat8a/OneLED/actions/workflows/spell-check.yml/badge.svg)

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

A minimal class to control a single digital LED or PWM-based LED with optional logic inversion.

- `on()`, `off()`, `toggle()` for easy control
- `isOn()` to check LED state
- `setBrightness(value)` for 0–255 control

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
