#pragma once

/**
 * Si7021 library by Stefan Linke
 */

#include "Particle.h"

#ifndef SI7021_CUSTOM
    #ifndef SI7021_TINY
        #define SI7021_FEATURE_DEVICEINFO
        #define SI7021_FEATURE_HEATER
    #endif
#endif


#define SI7021_DEFAULT_ADDRESS           0x40
#define SI7021_DEFAULT_SETTINGS          0x3a

// I2C commands
#define SI7021_CMD_MEASRH_HOLD           0xE5
#define SI7021_CMD_MEASRH_NOHOLD         0xF5
#define SI7021_CMD_MEASTEMP_HOLD         0xE3
#define SI7021_CMD_MEASTEMP_NOHOLD       0xF3
#define SI7021_CMD_READPREVTEMP          0xE0
#define SI7021_CMD_RESET                 0xFE
#define SI7021_CMD_WRITERHT_REG          0xE6
#define SI7021_CMD_READRHT_REG           0xE7
#define SI7021_CMD_WRITEHEATER_REG       0x51
#define SI7021_CMD_READHEATER_REG        0x11
#define SI7021_CMD_ID1                   0xFA0F
#define SI7021_CMD_ID2                   0xFCC9
#define SI7021_CMD_FIRMVERS              0x84B8

// configuration
#define SI7021_CFGBIT_RES0     0
#define SI7021_CFGBIT_HTRE     2
#define SI7021_CFGBIT_VDDS     6
#define SI7021_CFGBIT_RES1     7

#define SI7021_HEATER_MASK     0x0f



class Si7021 {
public:

#ifdef SI7021_FEATURE_DEVICEINFO
    /**
     * The device serial
     *
     * Contains the sensor serial number after a call to readDeviceInfo()
     * It consists of 8 bytes, for example:
     *  45db524915b5ffff
     */
    uint8_t serial[8];

    /**
     * The devices firmware revision
     *
     * This is one of the following:
     *
     *      0xff - firmware version 1.0
     *      0x20 - firmware version 2.0
     */
    uint8_t firmwareRevision = 0;

#endif

    Si7021();

    /**
     * Initialize the library and the sensor
     * @return success
     */
    bool begin(void);

    /**
     * Do a temperature measurement
     *
     * @return Temperature measured in degree Celsius
     */
    float readTemperature(void);

    /**
     * Get the temperature implicitly measured by the last humidity measurement
     *
     * Every humidity measurement implicitly measures the temperature too.
     * Instead of doing separate humidity and temperature measurements, one can
     * just measure the humidity once and get the internal temperature value
     * afterwards.
     *
     * @return Temperature measured in degree Celsius
     */
    float readLastTemperature(void);

    /**
     * Do a humidity measurement and read the humidity
     *
     * @return relative humidity in percent
     */
    float readHumidity(void);

    /**
     * Reset the sensor to it's default configuration
     *
     * This is automatically called by begin(), and resets
     * the sensors internal configuration registers.
     *
     *  - heater off
     *  - heater current: 3.09mA
     *  - resolution: 12bit RH, 14bit temp
     *
     * @return true on success
     */
    bool reset(void);

#ifdef SI7021_FEATURE_DEVICEINFO
    /**
     * Read device info
     *
     * Read out some sensor information: the serial number, and the firmware
     * revision. Those are accessible via the public properties firmwareRevision
     * and serial afterwards.
     */
    void readDeviceInfo(void);
#endif

#ifdef SI7021_FEATURE_HEATER
    /**
     * Turn the integrated heater on or off
     *
     * @param desired state (false == off, true == on)
     */
    void heater(bool state);
#endif


private:
    int8_t i2caddr;

    uint8_t readRegister8(uint8_t reg);
    void writeRegister8(uint8_t reg, uint8_t value);
};
