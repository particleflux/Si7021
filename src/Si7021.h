#pragma once

/* Si7021 library by Stefan Linke
 */

// This will load the definition for common Particle variable types
#include "Particle.h"


#define SI7021_DEFAULT_ADDRESS           0x40
#define SI7021_DEFAULT_SETTINGS          0x3a

// I2C commands
#define SI7021_MEASRH_HOLD_CMD           0xE5
#define SI7021_MEASRH_NOHOLD_CMD         0xF5
#define SI7021_MEASTEMP_HOLD_CMD         0xE3
#define SI7021_MEASTEMP_NOHOLD_CMD       0xF3
#define SI7021_READPREVTEMP_CMD          0xE0
#define SI7021_RESET_CMD                 0xFE
#define SI7021_WRITERHT_REG_CMD          0xE6
#define SI7021_READRHT_REG_CMD           0xE7
#define SI7021_WRITEHEATER_REG_CMD       0x51
#define SI7021_READHEATER_REG_CMD        0x11
#define SI7021_ID1_CMD                   0xFA0F
#define SI7021_ID2_CMD                   0xFCC9
#define SI7021_FIRMVERS_CMD              0x84B8



class Si7021 {
public:
    Si7021();

    bool begin(void);

    float readTemperature(void);
    float readLastTemperature(void);

    /**
     * Reset the sensor to it's default configuration
     *
     * This is automatically called by begin()
     *
     * heater off
     * resolution: 12bit RH, 14bit temp
     *
     * @return bool true on success
     */
    bool reset(void);

    void readDeviceInfo(void);

    float readHumidity(void);

    /**
     * Turn the integrated heater on or off
     *
     * @param bool state
     */
    void heater(bool state);

    uint8_t serial[8];
    uint8_t firmwareRevision = 0;

private:
    uint8_t readRegister8(uint8_t reg);

    void writeRegister8(uint8_t reg, uint8_t value);

    int8_t i2caddr;
};
