
// only build basic functionality
#define SI7021_TINY
#include "Si7021.h"

Si7021 si7021;

void setup() {
    if (si7021.begin()) {
        Serial.println("Sensor initialized successfully");
    }
}

void loop() {
    Serial.printlnf("temperature: %.2fC\thumidity: %.2f%%",
                   si7021.readTemperature(), si7021.readHumidity());
    delay(5000);
}
