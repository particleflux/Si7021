/* Si7021 library by Stefan Linke
 */

#include "Si7021.h"

Si7021::Si7021(void) {
    this->i2caddr = SI7021_DEFAULT_ADDRESS;
}

bool Si7021::begin(void) {
    Wire.begin();

    if (!reset()) {
        return false;
    }

    if (readRegister8(SI7021_CMD_READRHT_REG) != SI7021_DEFAULT_SETTINGS) {
        // device is not running with default configuration
        // probably sth broke
        return false;
    }

    return true;
}

float Si7021::readHumidity(void) {
    float humidity = 0;
    uint16_t hum;

    Wire.beginTransmission(i2caddr);
    Wire.write(SI7021_CMD_MEASRH_NOHOLD);
    Wire.endTransmission(false);
    delay(25);

    Wire.requestFrom(i2caddr, 3);
    hum = Wire.read();
    hum <<= 8;
    hum |= Wire.read();
    Wire.read();

    humidity = hum;
    humidity *= 125;
    humidity /= 65536;
    humidity -= 6;

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
    float temperature;

    Wire.beginTransmission(i2caddr);
    Wire.write(SI7021_CMD_MEASTEMP_NOHOLD);
    Wire.endTransmission(false);
    delay(25);

    Wire.requestFrom(i2caddr, 3);
    uint16_t temp = Wire.read();
    temp <<= 8;
    temp |= Wire.read();
    Wire.read();

    temperature = temp;
    temperature *= 175.72;
    temperature /= 65536;
    temperature -= 46.85;

    return temperature;
}

float Si7021::readLastTemperature(void) {
    float temperature;

    Wire.beginTransmission(i2caddr);
    Wire.write(SI7021_CMD_READPREVTEMP);
    Wire.endTransmission(false);

    Wire.requestFrom(i2caddr, 2);
    uint16_t temp = Wire.read();
    temp <<= 8;
    temp |= Wire.read();

    temperature = temp;
    temperature *= 175.72;
    temperature /= 65536;
    temperature -= 46.85;

    return temperature;
}

bool Si7021::reset(void) {
    uint8_t error = 0;
    Wire.beginTransmission(i2caddr);
    Wire.write(SI7021_CMD_RESET);
    error = Wire.endTransmission();
    delay(50);

    return error == 0;
}

#ifdef SI7021_FEATURE_DEVICEINFO

void Si7021::readDeviceInfo(void) {
    // serial number part 1
    Wire.beginTransmission(i2caddr);
    Wire.write((uint8_t) (SI7021_CMD_ID1 >> 8));
    Wire.write((uint8_t) (SI7021_CMD_ID1 & 0xFF));
    Wire.endTransmission();

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
    Wire.beginTransmission(i2caddr);
    Wire.write((uint8_t) (SI7021_CMD_ID2 >> 8));
    Wire.write((uint8_t) (SI7021_CMD_ID2 & 0xFF));
    Wire.endTransmission();

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
    Wire.beginTransmission(i2caddr);
    Wire.write((uint8_t) (SI7021_CMD_FIRMVERS >> 8));
    Wire.write((uint8_t) (SI7021_CMD_FIRMVERS & 0xFF));
    Wire.endTransmission();

    Wire.requestFrom(i2caddr, 1);
    firmwareRevision = Wire.read();
}

#endif

#ifdef SI7021_FEATURE_HEATER

void Si7021::heater(bool state) {
    uint8_t config = readRegister8(SI7021_CMD_READRHT_REG);

    if (state) {
        config |= 1 << SI7021_CFGBIT_HTRE;
    } else {
        config &= ~(1 << SI7021_CFGBIT_HTRE);
    }

    writeRegister8(SI7021_CMD_WRITERHT_REG, config);
}

#endif

void Si7021::writeRegister8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(i2caddr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t Si7021::readRegister8(uint8_t reg) {
    uint8_t value;
    Wire.beginTransmission(i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(i2caddr, 1);
    value = Wire.read();
    return value;
}
