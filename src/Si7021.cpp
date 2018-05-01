/* Si7021 library by Stefan Linke
 */

#include "Si7021.h"

Si7021::Si7021(void) {
    this->i2caddr = SI7021_DEFAULT_ADDRESS;
}

bool Si7021::begin(void) {
    uint8_t settings;

    Wire.begin();

    if (!reset()) {
        return false;
    }

    if (cmd(SI7021_CMD_READRHT_REG, &settings, 1, 0) && settings != SI7021_DEFAULT_SETTINGS) {
        // device is not running with default configuration
        // probably sth broke
        return false;
    }

    return true;
}

float Si7021::readHumidity(void) {
    uint8_t data[3];
    float humidity;

    cmd(SI7021_CMD_MEASRH_NOHOLD, data, 3, 25);
    humidity = ((float) (((uint16_t) data[0] << 8) | data[1]) * 125) / 65536 - 6;

    // clamp values to 0-100% range
    // it is in some cases possible that the values are outside of this range, according to spec
    if (humidity < 0) {
        humidity = 0.0;
    } else if (humidity > 100.0) {
        humidity = 100.0;
    }

    return humidity;
}

float Si7021::readTemperature(void) {
    uint8_t data[3];
    float temperature;

    cmd(SI7021_CMD_MEASTEMP_NOHOLD, data, 3, 25);
    temperature = ((float) (((uint16_t) data[0] << 8) | data[1]) * 175.72) / 65536 - 46.85;
    return temperature;
}

float Si7021::readLastTemperature(void) {
    uint8_t data[2];
    float temperature;

    cmd(SI7021_CMD_READPREVTEMP, data, 2, 0);
    temperature = ((float) (((uint16_t) data[0] << 8) | data[1]) * 175.72) / 65536 - 46.85;
    return temperature;
}

bool Si7021::reset(void) {
    uint8_t cmd = SI7021_CMD_RESET;
    uint8_t error = sendBytes(&cmd, 1, true);
    delay(50);

    return error == 0;
}

#ifdef SI7021_FEATURE_DEVICEINFO

void Si7021::readDeviceInfo(void) {
    uint16_t command = SI7021_CMD_ID1;

    // serial number part 1
    sendBytes((uint8_t*) &command, 2, true);

    Wire.requestFrom(i2caddr, 8);
    serial[0] = Wire.read();
    Wire.read();
    serial[1] = Wire.read();
    Wire.read();
    serial[2] = Wire.read();
    Wire.read();
    serial[3] = Wire.read();
    Wire.read();

    // serial number part 2
    command = SI7021_CMD_ID2;
    sendBytes((uint8_t*) &command, 2, true);

    Wire.requestFrom(i2caddr, 8);
    serial[4] = Wire.read();
    Wire.read();
    serial[5] = Wire.read();
    Wire.read();
    serial[6] = Wire.read();
    Wire.read();
    serial[7] = Wire.read();
    Wire.read();

    // firmware revision
    command = SI7021_CMD_FIRMVERS;
    sendBytes((uint8_t*) &command, 2, true);

    Wire.requestFrom(i2caddr, 1);
    firmwareRevision = Wire.read();
}

#endif

#ifdef SI7021_FEATURE_HEATER

void Si7021::heater(bool state) {
    uint8_t config;

    cmd(SI7021_CMD_READRHT_REG, &config, 1, 0);

    if (state) {
        config |= 1 << SI7021_CFGBIT_HTRE;
    } else {
        config &= ~(1 << SI7021_CFGBIT_HTRE);
    }

    uint8_t writeSequence[] = {SI7021_CMD_WRITERHT_REG, config};
    sendBytes(writeSequence, 2, true);
}

void Si7021::setHeaterPower(uint8_t desired) {
    uint8_t power;

    cmd(SI7021_CMD_READHEATER_REG, &power, 1, 0);

    // preserve reserved bits
    power &= ~((uint8_t) SI7021_HEATER_MASK);
    power |= desired & SI7021_HEATER_MASK;

    uint8_t writeSequence[] = {SI7021_CMD_WRITEHEATER_REG, power};
    sendBytes(writeSequence, 2, true);
}

#endif

uint8_t Si7021::sendBytes(uint8_t *data, int length, bool endTransmission) {
    Wire.beginTransmission(i2caddr);
    for(int i = 0; i < length; ++i) {
        Wire.write(data[i]);
    }
    return Wire.endTransmission(endTransmission);
}

void Si7021::readBytes(uint8_t *buffer, int length) {
    Wire.requestFrom(i2caddr, length);
    for(int i = 0; i < length; ++i) {
        buffer[i] = Wire.read();
    }
}

bool Si7021::cmd(uint8_t cmd, uint8_t *buffer, uint8_t expected, int duration) {
    uint8_t error = sendBytes(&cmd, 1);
    delay(duration);
    if (expected > 0 && buffer != NULL) {
        readBytes(buffer, expected);
    }
    return error == 0;
}
