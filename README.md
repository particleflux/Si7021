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

## LICENSE

Copyright 2018 Stefan Linke

Licensed under the MIT license
