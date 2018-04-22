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

    if (readRegister8(SI7021_READRHT_REG_CMD) != SI7021_DEFAULT_SETTINGS) {
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
    Wire.write(SI7021_MEASRH_NOHOLD_CMD);
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
    Wire.write(SI7021_MEASTEMP_NOHOLD_CMD);
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
    Wire.write(SI7021_READPREVTEMP_CMD);
    Wire.endTransmission(false);

    Wire.requestFrom(i2caddr, 3);
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
    Wire.write(SI7021_RESET_CMD);
    error = Wire.endTransmission();
    delay(50);

    return error == 0;
}

void Si7021::readDeviceInfo(void) {
    // serial number part 1
    Wire.beginTransmission(i2caddr);
    Wire.write((uint8_t) (SI7021_ID1_CMD >> 8));
    Wire.write((uint8_t) (SI7021_ID1_CMD & 0xFF));
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
    Wire.write((uint8_t) (SI7021_ID2_CMD >> 8));
    Wire.write((uint8_t) (SI7021_ID2_CMD & 0xFF));
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
    Wire.write((uint8_t) (SI7021_FIRMVERS_CMD >> 8));
    Wire.write((uint8_t) (SI7021_FIRMVERS_CMD & 0xFF));
    Wire.endTransmission();

    Wire.requestFrom(i2caddr, 1);
    firmwareRevision = Wire.read();
}

void Si7021::heater(bool state) {
    uint8_t config = readRegister8(SI7021_READRHT_REG_CMD);

    if (state) {
        config |= 1 << 2;
    } else {
        config &= ~(1 << 2);
    }

    writeRegister8(SI7021_WRITERHT_REG_CMD, config);
}


void Si7021::writeRegister8(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(i2caddr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();

    Serial.print("Wrote $"); Serial.print(reg, HEX); Serial.print(": 0x"); Serial.println(value, HEX);
}

uint8_t Si7021::readRegister8(uint8_t reg) {
    uint8_t value;
    Wire.beginTransmission(i2caddr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(i2caddr, 1);
    value = Wire.read();

    Serial.print("Read $"); Serial.print(reg, HEX); Serial.print(": 0x"); Serial.println(value, HEX);
    return value;
}
