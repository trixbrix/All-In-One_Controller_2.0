# All-In-One Controller 2.0 - Trixbrix.eu

This is the code powering the functionality of our All-In-One Controller. The controller is capable of controlling servos, LEDs and getting feedback from IR detectors.
The code is organized as a simple Arduino / Plarform.io project. The heart of the controller is the ATmega328p. You can upload new firmware to the controller using a micro-USB cable.

## How to set up the project

Here's a [detailed documentation](docs/Setting%20up%20the%20project.pdf) showing the installation process of VSCode and Platform.io plugin and the confiruation of the COM port on a Windows machine.
A very similar procedure is required on Linux or MacOS.

## How to adjust the servo angles

The following [constants](lib/Trixbrix/Trixbrix/Defines.h) are responsible for the servo angles and are a good staring point for tweaking the code.

```
#define SERVO1_RIGHT_CLICK 56 //angle for switch servo in position 1
#define SERVO1_LEFT_CLICK 98 //angle for switch servo in position 2
#define SERVO2_RIGHT_CLICK 0 //angle for boom barrier  servo in posiotion down
#define SERVO2_LEFT_CLICK 88 //angle for boom barrier  servo in posiotion up
#define DIFF 0 //angle which switch servo goes back after reaching position defined by values above, leave 0 for function disable
```

## How to contribute

If you find a bug or have a suggestion for a new feature, feel free to open an issue on Github. Code contributions are most welcome :)

## License - MIT

Copyright 2022 Trixbrix.eu

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
