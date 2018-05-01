#pragma once

/**
 * Si7021 library by Stefan Linke
 */

#include "Particle.h"

#ifndef SI7021_CUSTOM
    #ifndef SI7021_TINY
        #define SI7021_FEATURE_DEVICEINFO
        #define SI7021_FEATURE_HEATER
        #define SI7021_FEATURE_CRC
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
// those 2 byte commands are endianess-swapped to be sent bytewise
#define SI7021_CMD_ID1                   0x0FFA
#define SI7021_CMD_ID2                   0xC9FC
#define SI7021_CMD_FIRMVERS              0xB884

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

#ifdef SI7021_FEATURE_CRC
    /**
     * Indicates if the last Measurement's CRC was good
     *
     * This is the result of a CRC check on the data returned when a measurement
     * was done. It is only applicable for `readTemperature()` and
     * `readHumidity()`, and keeps its state until the next call of one of these.
     */
    bool wasLastMeasurementValid;
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

    /**
     * Set the desired heater power
     *
     * Configure the power level of the integrated heater. This can be set to
     * any value between 0 and 15 (0x0 - 0xf). Lower values mean less current
     * drawn and less heating.
     *
     * This does not automatically turn the heater on! Use Si7021::heater() for
     * changing the heaters state.
     *
     * Examples:
     *
     *      power       typical current draw (mA)
     *      0x0         3.09
     *      0x1         9.18
     *      0x2         15.24
     *      ...         ...
     *      0x4         27.39
     *      ...         ...
     *      0x8         51.69
     *      ...         ...
     *      0xf         94.20
     *
     * @param desired Desired power level
     */
    void setHeaterPower(uint8_t desired);
#endif


private:
    int8_t i2caddr;

    uint8_t sendBytes(uint8_t *data, int length, bool endTransmission = false);
    void readBytes(uint8_t *buffer, int length);
    bool cmd(uint8_t cmd, uint8_t *buffer, uint8_t expected, int duration);
    uint8_t crc8(uint8_t *data, uint8_t length);
};
