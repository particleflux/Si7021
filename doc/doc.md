# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`SI7021_FEATURE_DEVICEINFO`](#Si7021_8h_1a65f8305d091a8d326d30c03b3ca90706)            | Build device info functionality
`define `[`SI7021_FEATURE_HEATER`](#Si7021_8h_1a722e6219ecb1593012de8a6fde955654)            | Build Heater support
`define `[`SI7021_FEATURE_CRC`](#Si7021_8h_1a4c5f213a59f27b1390d89a09ff8e963b)            | Do CRC checks for each measurement
`class `[`Si7021`](#classSi7021) | 

## Members

#### `define `[`SI7021_FEATURE_DEVICEINFO`](#Si7021_8h_1a65f8305d091a8d326d30c03b3ca90706) 

Build device info functionality

This adds `readDeviceInfo()` functionality and the corresponding `serial` and `firmwareRevision` fields

#### `define `[`SI7021_FEATURE_HEATER`](#Si7021_8h_1a722e6219ecb1593012de8a6fde955654) 

Build Heater support

Add internal heater control commands

#### `define `[`SI7021_FEATURE_CRC`](#Si7021_8h_1a4c5f213a59f27b1390d89a09ff8e963b) 

Do CRC checks for each measurement

# class `Si7021` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`serial`](#classSi7021_1a8610518e3ee555e3e16e92e39ae55360) | The device serial
`public uint8_t `[`firmwareRevision`](#classSi7021_1a5d63026b7a2a5cc90b101303377ea363) | The devices firmware revision
`public bool `[`wasLastMeasurementValid`](#classSi7021_1a519491bad7d2816a3f36ca2dac241709) | Indicates if the last Measurement's CRC was good
`public  `[`Si7021`](#classSi7021_1ac47ac7a96302ed8fdf401a6fa6c4fccd)`()` | 
`public bool `[`begin`](#classSi7021_1a726dd631c7a2ae9af25ba1498a843563)`(void)` | Initialize the library and the sensor 
`public float `[`readTemperature`](#classSi7021_1af69384a0c8d36e977ec453c52b983527)`(void)` | Do a temperature measurement
`public float `[`readLastTemperature`](#classSi7021_1ab4e258214e1fab776c066b95ef2b4ee6)`(void)` | Get the temperature implicitly measured by the last humidity measurement
`public float `[`readHumidity`](#classSi7021_1abda41c6defc0dd333a924fc74f0249d0)`(void)` | Do a humidity measurement and read the humidity
`public bool `[`reset`](#classSi7021_1a1c2ee6a9d54409225509050e295e421c)`(void)` | Reset the sensor to it's default configuration
`public void `[`readDeviceInfo`](#classSi7021_1a16e6426310ba3128f68deb6421809886)`(void)` | Read device info
`public void `[`heater`](#classSi7021_1a14a1db961eff1790fd27ffa14e9db68d)`(bool state)` | Turn the integrated heater on or off
`public void `[`setHeaterPower`](#classSi7021_1ae0513b306fff4255e24e475823a159a6)`(uint8_t desired)` | Set the desired heater power

## Members

#### `public uint8_t `[`serial`](#classSi7021_1a8610518e3ee555e3e16e92e39ae55360) 

The device serial

Contains the sensor serial number after a call to `readDeviceInfo()` It consists of 8 bytes, for example: `45db524915b5ffff`

#### `public uint8_t `[`firmwareRevision`](#classSi7021_1a5d63026b7a2a5cc90b101303377ea363) 

The devices firmware revision

This is one of the following:

* 0xff - firmware version 1.0
* 0x20 - firmware version 2.0

#### `public bool `[`wasLastMeasurementValid`](#classSi7021_1a519491bad7d2816a3f36ca2dac241709) 

Indicates if the last Measurement's CRC was good

This is the result of a CRC check on the data returned when a measurement was done. It is only applicable for `readTemperature()` and `readHumidity()`, and keeps its state until the next call of one of these.

#### `public  `[`Si7021`](#classSi7021_1ac47ac7a96302ed8fdf401a6fa6c4fccd)`()` 

#### `public bool `[`begin`](#classSi7021_1a726dd631c7a2ae9af25ba1498a843563)`(void)` 

Initialize the library and the sensor 
#### Returns
success

#### `public float `[`readTemperature`](#classSi7021_1af69384a0c8d36e977ec453c52b983527)`(void)` 

Do a temperature measurement

#### Returns
Temperature measured in degree Celsius

#### `public float `[`readLastTemperature`](#classSi7021_1ab4e258214e1fab776c066b95ef2b4ee6)`(void)` 

Get the temperature implicitly measured by the last humidity measurement

Every humidity measurement implicitly measures the temperature too. Instead of doing separate humidity and temperature measurements, one can just measure the humidity once and get the internal temperature value afterwards.

#### Returns
Temperature measured in degree Celsius

#### `public float `[`readHumidity`](#classSi7021_1abda41c6defc0dd333a924fc74f0249d0)`(void)` 

Do a humidity measurement and read the humidity

#### Returns
relative humidity in percent

#### `public bool `[`reset`](#classSi7021_1a1c2ee6a9d54409225509050e295e421c)`(void)` 

Reset the sensor to it's default configuration

This is automatically called by `begin()`, and resets the sensors internal configuration registers.

 * heater off
 * heater current: 3.09mA
 * resolution: 12bit RH, 14bit temp

#### Returns
true on success

#### `public void `[`readDeviceInfo`](#classSi7021_1a16e6426310ba3128f68deb6421809886)`(void)` 

Read device info

Read out some sensor information: the serial number, and the firmware revision. Those are accessible via the public properties firmwareRevision and serial afterwards.

#### `public void `[`heater`](#classSi7021_1a14a1db961eff1790fd27ffa14e9db68d)`(bool state)` 

Turn the integrated heater on or off

#### Parameters
* `state` desired state (false == off, true == on)

#### `public void `[`setHeaterPower`](#classSi7021_1ae0513b306fff4255e24e475823a159a6)`(uint8_t desired)` 

Set the desired heater power

Configure the power level of the integrated heater. This can be set to any value between 0 and 15 (0x0 - 0xf). Lower values mean less current drawn and less heating.

This does not automatically turn the heater on! Use [Si7021::heater()](#classSi7021_1a14a1db961eff1790fd27ffa14e9db68d) for changing the heaters state.

Examples:

     power       typical current draw (mA)
     0x0         3.09
     0x1         9.18
     0x2         15.24
     ...         ...
     0x4         27.39
     ...         ...
     0x8         51.69
     ...         ...
     0xf         94.20

#### Parameters
* `desired` Desired power level

Generated by [Moxygen](https://sourcey.com/moxygen)