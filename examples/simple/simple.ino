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
