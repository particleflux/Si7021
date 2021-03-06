# Si7021

A Particle library for the Silicon Labs Si7021 Temperature and Humidity
sensor

## Hardware setup

Wire the sensor to your Particle Photon:

![schematic](doc/schematic.svg)

## Usage

Add the Si7021 library to your project and follow this simple example:

```cpp
#include "Si7021.h"

Si7021 si7021;
float temperature, humidity;

void setup() {
    if (si7021.begin()) {
        Serial.println("Sensor initialized successfully");
    }
}

void loop() {
    humidity = si7021.readHumidity();
    temperature = si7021.readLastTemperature();

    Serial.printlnf("temperature: %.2fC\thumidity: %.2f%%", temperature, humidity);
    delay(5000);
}
```

See the [examples](examples) folder for more details.

## Advanced conditional compilation

In case you do not require all features, some modules can be disabled
and thus be removed from compilation. The most radical approach would be
to `#define SI7021_TINY` as seen in [examples/tiny](examples/tiny) .
Defining this before the library is included, will remove support for
reading device info and for using the sensor internal heater.

If this is too much, you can also go with `SI7021_CUSTOM`, which, when
used standalone, equals the use of TINY. But with CUSTOM, you can
activate additional features with #defines.

```cpp
// this will enable the deviceInfo() functionality
#define SI7021_FEATURE_DEVICEINFO

// this will enable the heater() functionality
#define SI7021_FEATURE_HEATER

// this will enable CRC checks
#define SI7021_FEATURE_CRC
```

## LICENSE

Copyright 2018 Stefan Linke

Licensed under the MIT license


## References

[Si7021 datasheet](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf)
