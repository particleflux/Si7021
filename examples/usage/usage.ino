// Example usage for Si7021 library by Stefan Linke.

#include "Si7021.h"

// Initialize objects from the lib
Si7021 si7021;

void setup() {
    // initialize
    bool success = si7021.begin();

    Serial.printlnf("begin: %u", success);

    // read sensor info: serial number and firmware revision
    si7021.readDeviceInfo();
    Serial.printlnf(
            "%02x%02x%02x%02x-%02x%02x%02x%02x rev %02x",
            si7021.serial[0], si7021.serial[1], si7021.serial[2], si7021.serial[3],
            si7021.serial[4], si7021.serial[5], si7021.serial[6], si7021.serial[7],
            si7021.firmwareRevision
    );

    // some basic reading tests
    Serial.printlnf("hum: %.2f", si7021.readHumidity());
    Serial.printlnf("last temp: %.2f", si7021.readLastTemperature());
    Serial.printlnf("temp: %.2f", si7021.readTemperature());

    Serial.println("heater: on");
    si7021.heater(true);
    delay(2000);
    Serial.printlnf("temp: %.2f", si7021.readTemperature());
    Serial.println("heater: off");
    si7021.heater(false);
    delay(5000);
    Serial.printlnf("temp: %.2f", si7021.readTemperature());


    Serial.println("heater: on (high power)");
    si7021.setHeaterPower(15);
    si7021.heater(true);
    delay(2000);
    Serial.printlnf("temp: %.2f", si7021.readTemperature());
    Serial.println("heater: off");
    si7021.heater(false);
    delay(5000);
    Serial.printlnf("temp: %.2f", si7021.readTemperature());

}

void loop() {

}
