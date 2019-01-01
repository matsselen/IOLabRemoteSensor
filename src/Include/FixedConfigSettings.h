/***************************************************************************//**
    \file FixedConfigSettings.h
    \brief Stores all of the settings for the supported fixed configurations.

    \b Description Stores all of the settings for the fixed configurations.  The
                   settings are for the start data, stop data, as well as the
                   buffer requirements.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/FixedConfigSettings.h$
    $Revision: 2$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

    Mats: 1/1/2019. Added new configuration to read A7 at 4.8 kHz

*******************************************************************************/

#ifndef __FIXEDCONFIGSETTINGS_H__
#define __FIXEDCONFIGSETTINGS_H__

#include <KeyValue.h>
#include <SensorId.h>
/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC CONSTANTS--------------------------------------*/

/** \name Gyroscope Configuration
    \{
*/
/** Defines how many bytes are in _gyroConfig */
#define CONFIG_SIZE_GYRO        ( 7u )
/** Defines the configuration settings for the Gyro fixed config */
static const uint8_t _gyroConfig[CONFIG_SIZE_GYRO] =
{
    3u, /* number of Key-Value pairs */
    SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE | GyroscopeSampleRate_380),
    SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION  | GyroscopeResolution_2000),
    SensorId_GYROSCOPE, (GyroscopeKey_MODE        | GyroscopeMode_ENABLE)
};

/** Defines how many bytes are in _gyroDisable */
#define DISABLE_SIZE_GYRO      ( 3u )
/** Defines the configuration settings to disable the gyro fixed config */
static const uint8_t _gyroDisable[DISABLE_SIZE_GYRO] =
{
        1u,  /* number of Key-Value pairs */
        SensorId_GYROSCOPE, (GyroscopeKey_MODE | GyroscopeMode_DISABLE)
};

/** Defines how many bytes are in _gyroBuffer */
#define BUFFER_SIZE_GYRO         ( 4u )
/** Defines the data buffer configuration for the Gyro fixed config */
static const uint8_t _gyroBuffer[BUFFER_SIZE_GYRO] =
{
    BUFFER_SIZE_GYRO - 1u,   /* number of meaningful bytes */
    1u,                      /* number of sensors */
    SensorId_GYROSCOPE, 48u
};
/** \} */

/** \name Accelerometer Configuration
    \{
*/
/** Defines how many bytes are in _accelConfig */
#define CONFIG_SIZE_ACCEL        ( 9u )
/** Defines the configuration settings for the Accelerometer fixed config */
static const uint8_t _accelConfig[CONFIG_SIZE_ACCEL] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_400),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE)
};

/** Defines how many bytes in are _accelDisable */
#define DISABLE_SIZE_ACCEL       ( 3u )
/** Defines the configuration settings to disable the accel fixed config */
static const uint8_t _accelDisable[DISABLE_SIZE_ACCEL] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE)
};

/** Defines how many bytes are in _accelBuffer */
#define BUFFER_SIZE_ACCEL         ( 4u )
/** Defines the data buffer configuration for the Accelerometer fixed config */
static const uint8_t _accelBuffer[BUFFER_SIZE_ACCEL] =
{
        BUFFER_SIZE_ACCEL - 1u,      /* number of meaningful bytes */
        1u,                          /* number of Sensors */
        SensorId_ACCELEROMETER, 48u
};
/** \} */

/** \name Inertial Measurement Unit (IMU) Configuration
    \{
*/
/** Defines how many bytes are in _imuConfig */
#define CONFIG_SIZE_IMU          ( 23u )
/** Defines the configuration settings for the IMU fixed config */
static const uint8_t _imuConfig[CONFIG_SIZE_IMU] =
{
        11u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_100),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_MAGNETOMETER,  (MagnetometerKey_SAMPLE_RATE  | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_95),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE),

        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE       | HighGainSampleRate_100),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE              | HighGainMode_ENABLE)
};

/** Defines how many bytes are in _imuDisable */
#define DISABLE_SIZE_IMU          ( 9u )
/** Defines the configuration settings to disable the imu fixed config */
static const uint8_t _imuDisable[DISABLE_SIZE_IMU] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_DISABLE),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_DISABLE),
        SensorId_GYROSCOPE,     (GyroscopeKey_MODE            | GyroscopeMode_DISABLE),
        SensorId_HIGH_GAIN_IN,  (HighGainKey_MODE             | HighGainMode_DISABLE)
};

/** Defines how many bytes are in _imuBuffer */
#define BUFFER_SIZE_IMU           ( 10u )
/** Defines the data buffer configuration for the IMU fixed config */
static const uint8_t _imuBuffer[BUFFER_SIZE_IMU] =
{
        BUFFER_SIZE_IMU - 1u,        /* number of meaningful bytes */
        4u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 12u,
        SensorId_MAGNETOMETER,  12u,
        SensorId_GYROSCOPE,     12u,
        SensorId_HIGH_GAIN_IN,  4u
};
/** \} */

/** \name Mini-motion Configuration
    \{
*/
/** Defines how many bytes are in _miniMotionConfig */
#define CONFIG_SIZE_MINIMOTION       ( 15u )
/** Defines the configuration settings for the mini-motion fixed config */
static const uint8_t _miniMotionConfig[CONFIG_SIZE_MINIMOTION] =
{
        7u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_200),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_ENCODER,       (EncoderKey_MODE | EncoderMode_ENABLE),

        SensorId_FORCE_GAUGE,  (ForceGaugeKey_SAMPLE_RATE  | ForceGaugeSampleRate_200),
        SensorId_FORCE_GAUGE,  (ForceGaugeKey_MODE         | ForceGaugeMode_ENABLE)
};

/** Defines how many bytes are in _miniMotionDisable */
#define DISABLE_SIZE_MINIMOTION       ( 7u )
/** Defines the configuration settings to disable the mini motion fixed config */
static const uint8_t _miniMotionDisable[DISABLE_SIZE_MINIMOTION] =
{
        3u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE),
        SensorId_ENCODER,       (EncoderKey_MODE       | EncoderMode_DISABLE),
        SensorId_FORCE_GAUGE,   (ForceGaugeKey_MODE    | ForceGaugeMode_DISABLE)
};

/** Defines how many bytes are in _miniMotionBuffer */
#define BUFFER_SIZE_MINIMOTION        ( 8u )
/** Defines the data buffer configuration for the mini motion fixed config */
static const uint8_t _miniMotionBuffer[BUFFER_SIZE_MINIMOTION] =
{
        BUFFER_SIZE_MINIMOTION - 1u,        /* number of meaningful bytes */
        3u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 24u,
        SensorId_ENCODER,  8u,
        SensorId_FORCE_GAUGE,     8u
};
/** \} */

/** \name Pendulum Configuration
    \{
*/
/** Defines how many bytes are in _pendulumConfig */
#define CONFIG_SIZE_PENDULUM          ( 19u )
/** Defines the configuration settings for the pendulum fixed config */
static const uint8_t _pendulumConfig[CONFIG_SIZE_PENDULUM] =
{
        9u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_100),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_95),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE),

        SensorId_FORCE_GAUGE,  (ForceGaugeKey_SAMPLE_RATE  | ForceGaugeSampleRate_100),
        SensorId_FORCE_GAUGE,  (ForceGaugeKey_MODE         | ForceGaugeMode_ENABLE)
};

/** Defines how many bytes are in _pendulumDisable */
#define DISABLE_SIZE_PENDULUM          ( 7u )
/** Defines the configuration to disable the pendulum fixed config */
static const uint8_t _pendulumDisable[DISABLE_SIZE_PENDULUM] =
{
        3u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_DISABLE),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_DISABLE),
        SensorId_FORCE_GAUGE,  (ForceGaugeKey_MODE         | ForceGaugeMode_DISABLE)
};

/** Defines how many bytes are in _pendulumBuffer */
#define BUFFER_SIZE_PENDULUM           ( 8u )
/** Defines the data buffer configuration for the pendulum fixed config */
static const uint8_t _pendulumBuffer[BUFFER_SIZE_PENDULUM] =
{
        BUFFER_SIZE_PENDULUM - 1u,        /* number of meaningful bytes */
        3u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 12u,
        SensorId_GYROSCOPE,     12u,
        SensorId_FORCE_GAUGE,   4u
};
/** \} */

/** \name Ambient Configuration
    \{
*/
/** Defines how many bytes are in _ambientConfig */
#define CONFIG_SIZE_AMBIENT          ( 13u )
/** Defines the configuration settings for the ambient fixed config */
static const uint8_t _ambientConfig[CONFIG_SIZE_AMBIENT] =
{
        6u, /* number of Key-Value pairs */
        SensorId_BAROMETER, (BarometerKey_MODE | BarometerMode_ENABLE),

        SensorId_BATTERY, (BatteryKey_SAMPLE_RATE | BatterySampleRate_50),
        SensorId_BATTERY, (BatteryKey_MODE        | BatteryMode_ENABLE),

        SensorId_AMBIENT_LIGHT,  (AmbientLightKey_SAMPLE_RATE | AmbientLightSampleRate_400),
        SensorId_AMBIENT_LIGHT,  (AmbientLightKey_MODE        | AmbientLightMode_ENABLE),

        SensorId_THERMOMETER, (ThermometerKey_MODE | ThermometerMode_ENABLE)
};

/** Defines how many bytes are in _ambientDisable */
#define DISABLE_SIZE_AMBIENT          ( 9u )
/** Defines the configuration to disable the ambient fixed config */
static const uint8_t _ambientDisable[DISABLE_SIZE_AMBIENT] =
{
        4u, /* number of Key-Value pairs */
        SensorId_BAROMETER,     (BarometerKey_MODE    | BarometerMode_DISABLE),
        SensorId_BATTERY,       (BatteryKey_MODE      | BatteryMode_DISABLE),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE | AmbientLightMode_DISABLE),
        SensorId_THERMOMETER,   (ThermometerKey_MODE  | ThermometerMode_DISABLE)
};

/** Defines how many bytes are in _ambientBuffer */
#define BUFFER_SIZE_AMBIENT           ( 10u )
/** Defines the data buffer configuration for the ambient fixed config */
static const uint8_t _ambientBuffer[BUFFER_SIZE_AMBIENT] =
{
        BUFFER_SIZE_AMBIENT - 1u,        /* number of meaningful bytes */
        4u,                          /* number of sensors */
        SensorId_BAROMETER,     8u,
        SensorId_BATTERY,       2u,
        SensorId_AMBIENT_LIGHT, 16u,
        SensorId_THERMOMETER,   4u
};
/** \} */

/** \name Electrocardiogram (ECG) (3-channel) Configuration
    \{
*/
/** Defines how many bytes are in _ecgConfig */
#define CONFIG_SIZE_ECG          ( 5u )
/** Defines the configuration settings for the ECG fixed config */
static const uint8_t _ecgConfig[CONFIG_SIZE_ECG] =
{
        2u, /* number of Key-Value pairs */
        SensorId_ECG,  (EcgKey_SAMPLE_RATE | EcgSampleRate_400),
        SensorId_ECG,  (EcgKey_MODE        | EcgMode_ENABLE)
};

/** Defines how many bytes are in _ecgDisable */
#define DISABLE_SIZE_ECG          ( 3u )
/** Defines the configuration to disable the ECG fixed config */
static const uint8_t _ecgDisable[DISABLE_SIZE_ECG] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ECG,     (EcgKey_MODE | EcgMode_DISABLE)
};

/** Defines how many bytes are in _ecgBuffer */
#define BUFFER_SIZE_ECG               ( 4u )
/** Defines the data buffer configuration for the ECG fixed config */
static const uint8_t _ecgBuffer[BUFFER_SIZE_ECG] =
{
        BUFFER_SIZE_ECG - 1u,        /* number of meaningful bytes */
        1u,                          /* number of sensors */
        SensorId_ECG, 48u
};

/* Mats: The following "hi1" static arrays contain the info needed to configure and run a new 
         fixed configuration in which A7 is read out at warp speed (4800 Hz) */

/** \} */

/** \name Analog_hi1 Input Configuration
    \{
*/
/** Defines how many bytes are in _analog_hi1Config */
#define CONFIG_SIZE_ANALOG_HI1         ( 7u )
/** Defines the configuration settings for the ANALOG_HI1 fixed config */
static const uint8_t _analog_hi1Config[CONFIG_SIZE_ANALOG_HI1] =
{
        3u, /* number of Key-Value pairs */
        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE| Analog7SampleRate_4800),
        SensorId_HEADER_7, (Analog7Key_REF_VOLT| Analog7RefVolt_3v3),
        SensorId_HEADER_7, (Analog7Key_MODE| Analog7Mode_ENABLE)

};

/** Defines how many bytes are in _analog_hi1Disable */
#define DISABLE_SIZE_ANALOG_HI1        ( 3u )
/** Defines the configuration to disable the ANALOG_HI1 fixed config */
static const uint8_t _analog_hi1Disable[DISABLE_SIZE_ANALOG_HI1] =
{
        1u, /* number of Key-Value pairs */
        SensorId_HEADER_7,      (Analog7Key_MODE| Analog7Mode_DISABLE)
};

/** Defines how many bytes are in _analog_hi1Buffer */
#define BUFFER_SIZE_ANALOG_HI1           ( 4u )
/** Defines the data buffer configuration for the ANALOG_HI1 fixed config */
static const uint8_t _analog_hi1Buffer[BUFFER_SIZE_ANALOG_HI1] =
{
        BUFFER_SIZE_ANALOG_HI1 - 1u,     /* number of meaningful bytes */
        1u,                              /* number of sensors */
        SensorId_HEADER_7,      96u      /* 96 is expected size, there is no extra room */

};


/** \} */

/** \name Header Input Configuration
    \{
*/
/** Defines how many bytes are in _headerConfig */
#define CONFIG_SIZE_HEADER          ( 33u )
/** Defines the configuration settings for the header fixed config */
static const uint8_t _headerConfig[CONFIG_SIZE_HEADER] =
{
        16u, /* number of Key-Value pairs */
        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE| Analog7SampleRate_100),
        SensorId_HEADER_7, (Analog7Key_MODE| Analog7Mode_ENABLE),

        SensorId_HEADER_8, (Analog8Key_SAMPLE_RATE| Analog8SampleRate_100),
        SensorId_HEADER_8, (Analog8Key_MODE| Analog8Mode_ENABLE),

        SensorId_HEADER_9, (Analog9Key_SAMPLE_RATE| Analog9SampleRate_100),
        SensorId_HEADER_9, (Analog9Key_MODE| Analog9Mode_ENABLE),

        SensorId_HEADER_1, (Header1Key_MODE | Header1Mode_INPUT),
        SensorId_HEADER_2, (Header2Key_MODE | Header2Mode_INPUT),
        SensorId_HEADER_3, (Header3Key_MODE | Header3Mode_INPUT),
        SensorId_HEADER_4, (Header4Key_MODE | Header4Mode_INPUT),
        SensorId_HEADER_5, (Header5Key_MODE | Header5Mode_INPUT),
        SensorId_HEADER_6, (Header6Key_MODE | Header6Mode_INPUT),
        SensorId_DIGITALINPUTS, (DigitalInputKey_SAMPLE_RATE | DigitalInputSampleRate_100),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE | DigitalInputMode_ENABLE),

        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE | HighGainSampleRate_200),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE | HighGainMode_ENABLE)
};

/** Defines how many bytes are in _headerDisable */
#define DISABLE_SIZE_HEADER          ( 11u )
/** Defines the configuration to disable the header fixed config */
static const uint8_t _headerDisable[DISABLE_SIZE_HEADER] =
{
        5u, /* number of Key-Value pairs */
        SensorId_HEADER_7,      (Analog7Key_MODE| Analog7Mode_DISABLE),
        SensorId_HEADER_8,      (Analog8Key_MODE| Analog8Mode_DISABLE),
        SensorId_HEADER_9,      (Analog9Key_MODE| Analog9Mode_DISABLE),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE | DigitalInputMode_DISABLE),
        SensorId_HIGH_GAIN_IN,  (HighGainKey_MODE | HighGainMode_DISABLE)
};

/** Defines how many bytes are in _headerBuffer */
#define BUFFER_SIZE_HEADER           ( 12u )
/** Defines the data buffer configuration for the header fixed config */
static const uint8_t _headerBuffer[BUFFER_SIZE_HEADER] =
{
        BUFFER_SIZE_HEADER - 1u,        /* number of meaningful bytes */
        5u,                          /* number of sensors */
        SensorId_HEADER_7,      4u,
        SensorId_HEADER_8,      4u,
        SensorId_HEADER_9,      4u,
        SensorId_DIGITALINPUTS, 2u,
        SensorId_HIGH_GAIN_IN,  8u
};
/** \} */

/** \name Slow Microphone Configuration
    \{
*/
/** Defines how many bytes are in slowMicrophoneConfig */
#define CONFIG_SIZE_SLOW_MICROPHONE   ( 5u )
/** Defines the configuration settings for the slowMicrophone fixed config */
static const uint8_t _slowMicrophoneConfig[CONFIG_SIZE_SLOW_MICROPHONE] =
{
        2u, /* number of key value pairs */
        SensorId_MICROPHONE, (MicrophoneKey_SAMPLE_RATE| MicrophoneSampleRate_2400),
        SensorId_MICROPHONE, (MicrophoneKey_MODE | MicrophoneMode_ENABLE)
};

/** Defines how many bytes are in slowMicrophoneDisable */
#define DISABLE_SIZE_SLOW_MICROPHONE   ( 3u )
/** Defines the configuration to disable the slowMicrophone fixed config */
static const uint8_t _slowMicrophoneDisable[DISABLE_SIZE_SLOW_MICROPHONE] =
{
        1u, /* number of key value pairs */
        SensorId_MICROPHONE, (MicrophoneKey_MODE | MicrophoneMode_DISABLE)
};

/** Defines how many bytes are in _slowMicrophoneBuffer */
#define BUFFER_SIZE_SLOW_MICROPHONE    ( 4u )
/** Defines the data buffer configuration for the slowMicrophone fixed config */
static const uint8_t _slowMicrophoneBuffer[BUFFER_SIZE_SLOW_MICROPHONE] =
{
        BUFFER_SIZE_SLOW_MICROPHONE- 1u, /* number of meaningful bytes */
        1u,                         /* number of sensors */
        SensorId_MICROPHONE,  48u
};
/** \} */

/** \name Magnetic Configuration
    \{
*/
/** Defines how many bytes are in _magneticConfig */
#define CONFIG_SIZE_MAGNETIC          ( 9u )
/** Defines the configuration settings for the magnetic fixed config */
static const uint8_t _magneticConfig[CONFIG_SIZE_MAGNETIC] =
{
        4u, /* number of Key-Value pairs */
        SensorId_MAGNETOMETER,  (MagnetometerKey_SAMPLE_RATE  | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_ENABLE),

        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE       | HighGainSampleRate_400),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE              | HighGainMode_ENABLE)
};

/** Defines how many bytes are in _magneticDisable */
#define DISABLE_SIZE_MAGNETIC          ( 5u )
/** Defines the configuration settings to disable the magnetic fixed config */
static const uint8_t _magneticDisable[DISABLE_SIZE_MAGNETIC] =
{
        2u, /* number of Key-Value pairs */
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_DISABLE),
        SensorId_HIGH_GAIN_IN,  (HighGainKey_MODE             | HighGainMode_DISABLE)
};

/** Defines how many bytes are in _magneticBuffer */
#define BUFFER_SIZE_MAGNETIC           ( 6u )
/** Defines the data buffer configuration for the magnetic fixed config */
static const uint8_t _magneticBuffer[BUFFER_SIZE_MAGNETIC] =
{
        BUFFER_SIZE_MAGNETIC - 1u,        /* number of meaningful bytes */
        2u,                          /* number of sensors */
        SensorId_MAGNETOMETER,  12u,
        SensorId_HIGH_GAIN_IN,  16u
};
/** \} */

/** \name Analog Header Configuration
    \{
*/
/** Defines how many bytes are in _anlgHeadConfig */
#define CONFIG_SIZE_ANLG_HEAD          ( 17u )
/** Defines the configuration settings for the analog header fixed config */
static const uint8_t _anlgHeadConfig[CONFIG_SIZE_ANLG_HEAD] =
{
        8u, /* number of Key-Value pairs */
        SensorId_ECG,      (EcgKey_SAMPLE_RATE | EcgSampleRate_100 ),
        SensorId_ECG,      (EcgKey_MODE        | EcgMode_ENABLE),

        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE| Analog7SampleRate_100),
        SensorId_HEADER_7, (Analog7Key_MODE| Analog7Mode_ENABLE),

        SensorId_HEADER_8, (Analog8Key_SAMPLE_RATE| Analog8SampleRate_100),
        SensorId_HEADER_8, (Analog8Key_MODE| Analog8Mode_ENABLE),

        SensorId_HEADER_9, (Analog9Key_SAMPLE_RATE| Analog9SampleRate_100),
        SensorId_HEADER_9, (Analog9Key_MODE| Analog9Mode_ENABLE)
};

/** Defines how many bytes are in _anlgHeadDisable */
#define DISABLE_SIZE_ANLG_HEAD          ( 9u )
/** Defines the configuration to disable the analog header fixed config */
static const uint8_t _anlgHeadDisable[DISABLE_SIZE_ANLG_HEAD] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ECG,           (EcgKey_MODE    | EcgMode_DISABLE),
        SensorId_HEADER_7,      (Analog7Key_MODE| Analog7Mode_DISABLE),
        SensorId_HEADER_8,      (Analog8Key_MODE| Analog8Mode_DISABLE),
        SensorId_HEADER_9,      (Analog9Key_MODE| Analog9Mode_DISABLE)
};

/** Defines how many bytes are in _anlgHeadBuffer */
#define BUFFER_SIZE_ANLG_HEAD           ( 10u )
/** Defines the data buffer configuration for the analog header fixed config */
static const uint8_t _anlgHeadBuffer[BUFFER_SIZE_ANLG_HEAD] =
{
        BUFFER_SIZE_ANLG_HEAD - 1u,  /* number of meaningful bytes */
        4u,                          /* number of sensors */
        SensorId_ECG,           12u,
        SensorId_HEADER_7,      4u,
        SensorId_HEADER_8,      4u,
        SensorId_HEADER_9,      4u
};
/** \} */

/** \name Header 3.3V Input Configuration
    \{
*/
/** Defines how many bytes are in _header3V3Config */
#define CONFIG_SIZE_HEADER_3V3          ( 39u )
/** Defines the configuration settings for the header 3.3V fixed config */
static const uint8_t _header3V3Config[CONFIG_SIZE_HEADER_3V3] =
{
        19u, /* number of Key-Value pairs */
        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE| Analog7SampleRate_100),
        SensorId_HEADER_7, (Analog7Key_REF_VOLT| Analog7RefVolt_3v3),
        SensorId_HEADER_7, (Analog7Key_MODE| Analog7Mode_ENABLE),

        SensorId_HEADER_8, (Analog8Key_SAMPLE_RATE| Analog8SampleRate_100),
        SensorId_HEADER_8, (Analog8Key_REF_VOLT| Analog8RefVolt_3v3),
        SensorId_HEADER_8, (Analog8Key_MODE| Analog8Mode_ENABLE),

        SensorId_HEADER_9, (Analog9Key_SAMPLE_RATE| Analog9SampleRate_100),
        SensorId_HEADER_9, (Analog9Key_REF_VOLT| Analog9RefVolt_3v3),
        SensorId_HEADER_9, (Analog9Key_MODE| Analog9Mode_ENABLE),

        SensorId_HEADER_1, (Header1Key_MODE | Header1Mode_INPUT),
        SensorId_HEADER_2, (Header2Key_MODE | Header2Mode_INPUT),
        SensorId_HEADER_3, (Header3Key_MODE | Header3Mode_INPUT),
        SensorId_HEADER_4, (Header4Key_MODE | Header4Mode_INPUT),
        SensorId_HEADER_5, (Header5Key_MODE | Header5Mode_INPUT),
        SensorId_HEADER_6, (Header6Key_MODE | Header6Mode_INPUT),
        SensorId_DIGITALINPUTS, (DigitalInputKey_SAMPLE_RATE | DigitalInputSampleRate_100),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE | DigitalInputMode_ENABLE),

        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE | HighGainSampleRate_200),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE | HighGainMode_ENABLE)
};

/** Defines how many bytes are in _header3V3Disable */
#define DISABLE_SIZE_HEADER_3V3          ( 11u )
/** Defines the configuration to disable the header 3.3V fixed config */
static const uint8_t _header3V3Disable[DISABLE_SIZE_HEADER_3V3] =
{
        5u, /* number of Key-Value pairs */
        SensorId_HEADER_7,      (Analog7Key_MODE| Analog7Mode_DISABLE),
        SensorId_HEADER_8,      (Analog8Key_MODE| Analog8Mode_DISABLE),
        SensorId_HEADER_9,      (Analog9Key_MODE| Analog9Mode_DISABLE),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE | DigitalInputMode_DISABLE),
        SensorId_HIGH_GAIN_IN,  (HighGainKey_MODE | HighGainMode_DISABLE)
};

/** Defines how many bytes are in _header3V3Buffer */
#define BUFFER_SIZE_HEADER_3V3           ( 12u )
/** Defines the data buffer configuration for the header 3.3V fixed config */
static const uint8_t _header3V3Buffer[BUFFER_SIZE_HEADER_3V3] =
{
        BUFFER_SIZE_HEADER_3V3 - 1u, /* number of meaningful bytes */
        5u,                          /* number of sensors */
        SensorId_HEADER_7,      4u,
        SensorId_HEADER_8,      4u,
        SensorId_HEADER_9,      4u,
        SensorId_DIGITALINPUTS, 2u,
        SensorId_HIGH_GAIN_IN,  8u
};
/** \} */

/** \name High-speed Gyroscope Configuration
    \{
*/
/** Defines how many bytes are in _gyroHiConfig */
#define CONFIG_SIZE_GYRO_HI          ( 7u )
/** Defines the configuration settings for the gyro hi fixed config */
static const uint8_t _gyroHiConfig[CONFIG_SIZE_GYRO_HI] =
{
        3u, /* number of Key-Value pairs */
        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE | GyroscopeSampleRate_760),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION  | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE        | GyroscopeMode_ENABLE)
};

/** Defines how many bytes are in _gyroHiDisable */
#define DISABLE_SIZE_GYRO_HI          ( 3u )
/** Defines the configuration to disable the gyro hi fixed config */
static const uint8_t _gyroHiDisable[DISABLE_SIZE_GYRO_HI] =
{
        1u, /* number of Key-Value pairs */
        SensorId_GYROSCOPE, (GyroscopeKey_MODE | GyroscopeMode_DISABLE)
};

/** Defines how many bytes are in _gyroHiBuffer */
#define BUFFER_SIZE_GYRO_HI           ( 4u )
/** Defines the data buffer configuration for the gyro hi fixed config */
static const uint8_t _gyroHiBuffer[BUFFER_SIZE_GYRO_HI] =
{
        BUFFER_SIZE_GYRO_HI - 1u, /* number of meaningful bytes */
        1u,                       /* number of sensors */
        SensorId_GYROSCOPE, 96u
};
/** \} */

/** \name High-speed Accelerometer Configuration
    \{
*/
/** Defines how many bytes are in _accelHiConfig */
#define CONFIG_SIZE_ACCEL_HI        ( 9u )
/** Defines the configuration settings for the High Speed Accelerometer fixed config */
static const uint8_t _accelHiConfig[CONFIG_SIZE_ACCEL_HI] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_800),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE)
};

/** Defines how many bytes are in _accelHiDisable */
#define DISABLE_SIZE_ACCEL_HI   ( 3u )
/** Defines the configuration settings to disable the accel hi fixed config */
static const uint8_t _accelHiDisable[DISABLE_SIZE_ACCEL_HI] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE)
};

/** Defines how many bytes are in _accelHiBuffer */
#define BUFFER_SIZE_ACCEL_HI         ( 4u )
/** Defines the data buffer configuration for the High Speed Accelerometer fixed config */
static const uint8_t _accelHiBuffer[BUFFER_SIZE_ACCEL_HI] =
{
        BUFFER_SIZE_ACCEL_HI - 1u,      /* number of meaningful bytes */
        1u,                          /* number of Sensors */
        SensorId_ACCELEROMETER, 96u
};
/** \} */

/** \name High-speed Inertial Measurement Unit (IMU) Configuration
    \{
*/
/** Defines how many bytes are in _imuHiConfig */
#define CONFIG_SIZE_IMU_HI          ( 19u )
/** Defines the configuration settings for the IMU HI fixed config */
static const uint8_t _imuHiConfig[CONFIG_SIZE_IMU_HI] =
{
        9u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_400),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_MAGNETOMETER,  (MagnetometerKey_SAMPLE_RATE  | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_190),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE)
};

/** Defines how many bytes are in _imuHiDisable */
#define DISABLE_SIZE_IMU_HI          ( 7u )
/** Defines the configuration settings to disable the imu hi fixed config */
static const uint8_t _imuHiDisable[DISABLE_SIZE_IMU_HI] =
{
        3u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_DISABLE),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_DISABLE),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_DISABLE)
};

/** Defines how many bytes are in _imuHiBuffer */
#define BUFFER_SIZE_IMU_HI           ( 8u )
/** Defines the data buffer configuration for the IMU HI fixed config */
static const uint8_t _imuHiBuffer[BUFFER_SIZE_IMU_HI] =
{
        BUFFER_SIZE_IMU_HI - 1u,        /* number of meaningful bytes */
        3u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 48u,
        SensorId_MAGNETOMETER,  12u,
        SensorId_GYROSCOPE,     24u
};
/** \} */

/** \name Motion Measurement Configuration
    \{
*/
/** Defines how many bytes are in _motionConfig */
#define CONFIG_SIZE_MOTION       ( 21u )
/** Defines the configuration settings for the motion fixed config */
static const uint8_t _motionConfig[CONFIG_SIZE_MOTION] =
{
        10u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_200),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_190),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE),

        SensorId_ENCODER, (EncoderKey_MODE | EncoderMode_ENABLE),

        SensorId_FORCE_GAUGE, (ForceGaugeKey_SAMPLE_RATE  | ForceGaugeSampleRate_200),
        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE         | ForceGaugeMode_ENABLE)
};

/** Defines how many bytes are in _motionDisable */
#define DISABLE_SIZE_MOTION       ( 9u )
/** Defines the configuration settings to disable the motion fixed config */
static const uint8_t _motionDisable[DISABLE_SIZE_MOTION] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE),
        SensorId_GYROSCOPE,     (GyroscopeKey_MODE     | GyroscopeMode_DISABLE),
        SensorId_ENCODER,       (EncoderKey_MODE       | EncoderMode_DISABLE),
        SensorId_FORCE_GAUGE,   (ForceGaugeKey_MODE    | ForceGaugeMode_DISABLE)
};

/** Defines how many bytes are in _motionBuffer */
#define BUFFER_SIZE_MOTION        ( 10u )
/** Defines the data buffer configuration for the motion fixed config */
static const uint8_t _motionBuffer[BUFFER_SIZE_MOTION] =
{
        BUFFER_SIZE_MOTION - 1u,        /* number of meaningful bytes */
        4u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 24u,
        SensorId_GYROSCOPE,     24u,
        SensorId_ENCODER,        8u,
        SensorId_FORCE_GAUGE,    8u
};
/** \} */

/** \name Sports Configuration
    \{
*/
/** Defines how many bytes are in _sportsConfig */
#define CONFIG_SIZE_SPORTS          ( 23u )
/** Defines the configuration settings for the sports fixed config */
static const uint8_t _sportsConfig[CONFIG_SIZE_SPORTS] =
{
        11u, /* number of Key-Value pairs */

        SensorId_ECG, (EcgKey_SAMPLE_RATE | EcgSampleRate_200),
        SensorId_ECG, (EcgKey_MODE        | EcgMode_ENABLE),

        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_200),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_MAGNETOMETER,  (MagnetometerKey_SAMPLE_RATE  | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_190),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE)
};

/** Defines how many bytes are in _sportsDisable */
#define DISABLE_SIZE_SPORTS          ( 9u )
/** Defines the configuration settings to disable the sports fixed config */
static const uint8_t _sportsDisable[DISABLE_SIZE_SPORTS] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ECG, (EcgKey_MODE        | EcgMode_DISABLE),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_DISABLE),
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE         | MagnetometerMode_DISABLE),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_DISABLE)
};

/** Defines how many bytes are in _sportsBuffer */
#define BUFFER_SIZE_SPORTS           ( 10u )
/** Defines the data buffer configuration for the sports fixed config */
static const uint8_t _sportsBuffer[BUFFER_SIZE_SPORTS] =
{
        BUFFER_SIZE_SPORTS - 1u,        /* number of meaningful bytes */
        4u,                          /* number of sensors */
        SensorId_ECG,           24u,
        SensorId_ACCELEROMETER, 24u,
        SensorId_MAGNETOMETER,  12u,
        SensorId_GYROSCOPE,     24u
};
/** \} */

/** \name High-speed Pendulum Configuration
    \{
*/
/** Defines how many bytes are in _pendulumHiConfig */
#define CONFIG_SIZE_PENDULUM_HI          ( 19u )
/** Defines the configuration settings for the pendulum hi fixed config */
static const uint8_t _pendulumHiConfig[CONFIG_SIZE_PENDULUM_HI] =
{
        9u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_200),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE         | GyroscopeSampleRate_190),
        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION          | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_ENABLE),

        SensorId_FORCE_GAUGE,  (ForceGaugeKey_SAMPLE_RATE  | ForceGaugeSampleRate_200),
        SensorId_FORCE_GAUGE,  (ForceGaugeKey_MODE         | ForceGaugeMode_ENABLE)
};

/** Defines how many bytes are in _pendulumHiDisable */
#define DISABLE_SIZE_PENDULUM_HI          ( 7u )
/** Defines the configuration to disable the pendulum hi fixed config */
static const uint8_t _pendulumHiDisable[DISABLE_SIZE_PENDULUM_HI] =
{
        3u, /* number of Key-Value pairs */
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_DISABLE),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE                | GyroscopeMode_DISABLE),
        SensorId_FORCE_GAUGE,  (ForceGaugeKey_MODE         | ForceGaugeMode_DISABLE)
};

/** Defines how many bytes are in _pendulumHiBuffer */
#define BUFFER_SIZE_PENDULUM_HI           ( 8u )
/** Defines the data buffer configuration for the pendulum hi fixed config */
static const uint8_t _pendulumHiBuffer[BUFFER_SIZE_PENDULUM_HI] =
{
        BUFFER_SIZE_PENDULUM_HI - 1u,        /* number of meaningful bytes */
        3u,                          /* number of sensors */
        SensorId_ACCELEROMETER, 24u,
        SensorId_GYROSCOPE,     24u,
        SensorId_FORCE_GAUGE,   8u
};
/** \} */

/** \name Kitchen Sink Configuration
    \{
*/
/** Defines how many bytes are in kitchenSinkConfig */
#define CONFIG_SIZE_KITCHENSINK   ( 45u )
/** Defines the configuration settings for the kitchenSink fixed config */
static const uint8_t _kitchenSinkConfig[CONFIG_SIZE_KITCHENSINK] =
{
        22u, /* number of key value pairs */
        SensorId_MAGNETOMETER, (MagnetometerKey_SAMPLE_RATE | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER, (MagnetometerKey_MODE | MagnetometerMode_ENABLE),

        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_100),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_ENCODER, (EncoderKey_MODE | EncoderMode_ENABLE),

        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE | ForceGaugeMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION  | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE | GyroscopeSampleRate_95),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE        | GyroscopeMode_ENABLE),

        SensorId_AMBIENT_LIGHT, (AmbientLightKey_SAMPLE_RATE | AmbientLightSampleRate_100),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE        | AmbientLightMode_ENABLE),

        SensorId_BATTERY, (BatteryKey_SAMPLE_RATE | BatterySampleRate_100),
        SensorId_BATTERY, (BatteryKey_MODE        | BatteryMode_ENABLE),

        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE | HighGainSampleRate_100),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE        | HighGainMode_ENABLE),

        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE | Analog7SampleRate_100),
        SensorId_HEADER_7, (Analog7Key_MODE        | Analog7Mode_ENABLE),

        SensorId_DIGITALINPUTS, (DigitalInputKey_SAMPLE_RATE | DigitalInputSampleRate_100),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE        | DigitalInputMode_ENABLE),

        SensorId_BAROMETER, (BarometerKey_MODE | BarometerMode_ENABLE)
};

/** Defines how many bytes are in kitchenSinkDisable */
#define DISABLE_SIZE_KITCHEN_SINK       ( 23u )
/** Defines the configuration to disable the kitchen sink fixed config */
static const uint8_t _kitchenSinkDisable[DISABLE_SIZE_KITCHEN_SINK] =
{
        11u, /* number of key value pairs */
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE  | MagnetometerMode_DISABLE),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE),
        SensorId_ENCODER,       (EncoderKey_MODE       | EncoderMode_DISABLE),
        SensorId_FORCE_GAUGE,   (ForceGaugeKey_MODE    | ForceGaugeMode_DISABLE),
        SensorId_GYROSCOPE,     (GyroscopeKey_MODE     | GyroscopeMode_DISABLE),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE  | AmbientLightMode_DISABLE),
        SensorId_BATTERY,       (BatteryKey_MODE       | BatteryMode_DISABLE),
        SensorId_HIGH_GAIN_IN,  (HighGainKey_MODE      | HighGainMode_DISABLE),
        SensorId_HEADER_7,      (Analog7Key_MODE       | Analog7Mode_DISABLE),
        SensorId_DIGITALINPUTS, (DigitalInputKey_MODE  | DigitalInputMode_DISABLE),
        SensorId_BAROMETER,     (BarometerKey_MODE     | BarometerMode_DISABLE)
};

/** Defines how many bytes are in _kitchenSinkBuffer */
#define BUFFER_SIZE_KITCHENSINK   ( 24u )
/** Defines the data buffer configuration for the kitchen sink fixed config */
static const uint8_t _kitchenSinkBuffer[BUFFER_SIZE_KITCHENSINK] =
{
        BUFFER_SIZE_KITCHENSINK - 1u, /* number of meaningful bytes */
        11u,                           /* number of sensors */
        SensorId_MAGNETOMETER,  12u,
        SensorId_ACCELEROMETER, 12u,
        SensorId_ENCODER,       8u,
        SensorId_FORCE_GAUGE,   4u,
        SensorId_GYROSCOPE,     12u,
        SensorId_AMBIENT_LIGHT, 4u,
        SensorId_BATTERY,       4u,
        SensorId_HIGH_GAIN_IN,  4u,
        SensorId_HEADER_7,      4u,
        SensorId_DIGITALINPUTS, 2u,
        SensorId_BAROMETER,     16u
};
/** \} */

/** \name Microphone Configuration
    \{
*/
/** Defines how many bytes are in microphoneConfig */
#define CONFIG_SIZE_MICROPHONE   ( 5u )
/** Defines the configuration settings for the microphone fixed config */
static const uint8_t _microphoneConfig[CONFIG_SIZE_MICROPHONE] =
{
        2u, /* number of key value pairs */
        SensorId_MICROPHONE, (MicrophoneKey_SAMPLE_RATE| MicrophoneSampleRate_4800),
        SensorId_MICROPHONE, (MicrophoneKey_MODE | MicrophoneMode_ENABLE)
};

/** Defines how many bytes are in microphoneDisable */
#define DISABLE_SIZE_MICROPHONE   ( 3u )
/** Defines the configuration to disable the microphone fixed config */
static const uint8_t _microphoneDisable[DISABLE_SIZE_MICROPHONE] =
{
        1u, /* number of key value pairs */
        SensorId_MICROPHONE, (MicrophoneKey_MODE | MicrophoneMode_DISABLE)
};

/** Defines how many bytes are in _microphoneBuffer */
#define BUFFER_SIZE_MICROPHONE    ( 4u )
/** Defines the data buffer configuration for the microphone fixed config */
static const uint8_t _microphoneBuffer[BUFFER_SIZE_MICROPHONE] =
{
        BUFFER_SIZE_MICROPHONE- 1u, /* number of meaningful bytes */
        1u,                         /* number of sensors */
        SensorId_MICROPHONE,  96u
};
/** \} */

/** \name High-speed Ambient Light Sensor (ALS) Configuration
    \{
*/
/** Defines how many bytes are in alsHiConfig */
#define CONFIG_SIZE_ALS_HI    ( 5u )
/** Defines the configuration settings for the als high speed fixed config */
static const uint8_t _alsHiConfig[CONFIG_SIZE_ALS_HI] =
{
        2u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_SAMPLE_RATE| AmbientLightSampleRate_4800),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE       | AmbientLightMode_ENABLE)
};

/** Defines how many bytes are in alsHiDisable */
#define DISABLE_SIZE_ALS_HI   ( 3u )
/** Defines the configuration to disable the ambient light fixed config */
static const uint8_t _alsHiDisable[DISABLE_SIZE_ALS_HI] =
{
        1u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE | AmbientLightMode_DISABLE)
};

/** Defines how many bytes are in _alsHiBuffer */
#define BUFFER_SIZE_ALS_HI    ( 4u )
/** Defines the data buffer configuration for the ambient light fixed config */
static const uint8_t _alsHiBuffer[BUFFER_SIZE_ALS_HI] =
{
        BUFFER_SIZE_ALS_HI - 1u, /* number of meaningful bytes */
        1u,                      /* number of sensors */
        SensorId_AMBIENT_LIGHT,  96u
};
/** \} */

/** \name Ambeint Light + Accelerometer Configuration
    \{
*/
/** Defines how many bytes are in alsAccConfig */
#define CONFIG_SIZE_ALS_ACC    ( 13u )
/** Defines the configuration settings for the als acc fixed config */
static const uint8_t _alsAccConfig[CONFIG_SIZE_ALS_ACC] =
{
        (CONFIG_SIZE_ALS_ACC - 1u)/2u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_SAMPLE_RATE| AmbientLightSampleRate_800),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE       | AmbientLightMode_ENABLE),

        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_800),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),
};

/** Defines how many bytes are in alsAccDisable */
#define DISABLE_SIZE_ALS_ACC   ( 5u )
/** Defines the configuration to disable the als acc fixed config */
static const uint8_t _alsAccDisable[DISABLE_SIZE_ALS_ACC] =
{
        (DISABLE_SIZE_ALS_ACC - 1u)/2u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE | AmbientLightMode_DISABLE),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE)
};

/** Defines how many bytes are in _alsAccBuffer */
#define BUFFER_SIZE_ALS_ACC    ( 6u )
/** Defines the data buffer configuration for the als acc fixed config */
static const uint8_t _alsAccBuffer[BUFFER_SIZE_ALS_ACC] =
{
        BUFFER_SIZE_ALS_ACC - 1u,       /* number of meaningful bytes */
        2u,                             /* number of sensors */
        SensorId_AMBIENT_LIGHT,  24u,   /* 16 is nominal size, this will allow room for a few extra */
        SensorId_ACCELEROMETER,  72u    /* 96 is double expected size, but this will allow room in the buffer */
};
/** \} */

/** \name Force Gauge + Accelerometer Configuration
    \{
*/
/** Defines how many bytes are in forceAccConfig */
#define CONFIG_SIZE_FORCE_ACC    ( 13u )
/** Defines the configuration settings for the force acc fixed config */
static const uint8_t _forceAccConfig[CONFIG_SIZE_FORCE_ACC] =
{
        (CONFIG_SIZE_FORCE_ACC - 1u)/2u, /* number of key value pairs */
        SensorId_FORCE_GAUGE, (ForceGaugeKey_SAMPLE_RATE| ForceGaugeSampleRate_800),
        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE       | ForceGaugeMode_ENABLE),

        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_800),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),
};

/** Defines how many bytes are in forceAccDisable */
#define DISABLE_SIZE_FORCE_ACC   ( 5u )
/** Defines the configuration to disable the force acc fixed config */
static const uint8_t _forceAccDisable[DISABLE_SIZE_FORCE_ACC] =
{
        (DISABLE_SIZE_FORCE_ACC - 1u)/2u, /* number of key value pairs */
        SensorId_FORCE_GAUGE,   (ForceGaugeKey_MODE    | ForceGaugeMode_DISABLE),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE)
};

/** Defines how many bytes are in _forceAccBuffer */
#define BUFFER_SIZE_FORCE_ACC    ( 6u )
/** Defines the data buffer configuration for the force acc fixed config */
static const uint8_t _forceAccBuffer[BUFFER_SIZE_FORCE_ACC] =
{
        BUFFER_SIZE_FORCE_ACC - 1u,         /* number of meaningful bytes */
        2u,                             /* number of sensors */
        SensorId_FORCE_GAUGE,    24u,   /* 16 is nominal size, this will allow room for a few extra */
        SensorId_ACCELEROMETER,  72u    /* 96 is double expected size, but this will allow room in the buffer */
};
/** \} */

/** \name Ambient Light + Microphone (Thunder & Lightning) Configuration
    \{
*/
/** Defines how many bytes are in alsMicConfig */
#define CONFIG_SIZE_ALS_MIC    ( 9u )
/** Defines the configuration settings for the ambient light sensor microphone fixed config */
static const uint8_t _alsMicConfig[CONFIG_SIZE_ALS_MIC] =
{
        (CONFIG_SIZE_ALS_MIC - 1u)/2u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_SAMPLE_RATE| ForceGaugeSampleRate_2400),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE       | ForceGaugeMode_ENABLE),

        SensorId_MICROPHONE, (MicrophoneKey_SAMPLE_RATE | MicrophoneSampleRate_2400),
        SensorId_MICROPHONE, (MicrophoneKey_MODE        | MicrophoneMode_ENABLE)
};

/** Defines how many bytes are in alsMicDisable */
#define DISABLE_SIZE_ALS_MIC   ( 5u )
/** Defines the configuration to disable the ambient light microphone fixed config */
static const uint8_t _alsMicDisable[DISABLE_SIZE_ALS_MIC] =
{
        (DISABLE_SIZE_ALS_MIC - 1u)/2u, /* number of key value pairs */
        SensorId_AMBIENT_LIGHT,   (AmbientLightKey_MODE| AmbientLightMode_DISABLE),
        SensorId_MICROPHONE,      (MicrophoneKey_MODE  | MicrophoneMode_DISABLE)
};

/** Defines how many bytes are in _alsMicBuffer */
#define BUFFER_SIZE_ALS_MIC    ( 6u )
/** Defines the data buffer configuration for the ambient light microphone fixed config */
static const uint8_t _alsMicBuffer[BUFFER_SIZE_ALS_MIC] =
{
        BUFFER_SIZE_ALS_MIC - 1u,      /* number of meaningful bytes */
        2u,                            /* number of sensors */
        SensorId_AMBIENT_LIGHT, 48u,   /* 48 is expected size, there is no extra room */
        SensorId_MICROPHONE,    48u    /* 48 is expected size, there is no extra room */
};
/** \} */

/** \name High-speed ECG Configuration
    \{
*/
/** Defines how many bytes are in _hiEcgConfig */
#define CONFIG_SIZE_HI_ECG          ( 5u )
/** Defines the configuration settings for the HI_ECG fixed config */
static const uint8_t _hiEcgConfig[CONFIG_SIZE_HI_ECG] =
{
        2u, /* number of Key-Value pairs */
        SensorId_ECG,  (EcgKey_SAMPLE_RATE | EcgSampleRate_800),
        SensorId_ECG,  (EcgKey_MODE        | EcgMode_ENABLE)
};

/** Defines how many bytes are in _hiEcgDisable */
#define DISABLE_SIZE_HI_ECG          ( 3u )
/** Defines the configuration to disable the HI_ECG fixed config */
static const uint8_t _hiEcgDisable[DISABLE_SIZE_HI_ECG] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ECG,     (EcgKey_MODE | EcgMode_DISABLE)
};

/** Defines how many bytes are in _hiEcgBuffer */
#define BUFFER_SIZE_HI_ECG               ( 4u )
/** Defines the data buffer configuration for the HI_ECG fixed config */
static const uint8_t _hiEcgBuffer[BUFFER_SIZE_HI_ECG] =
{
        BUFFER_SIZE_HI_ECG - 1u,     /* number of meaningful bytes */
        1u,                          /* number of sensors */
        SensorId_ECG, 96u
};
/** \} */

/** \name High-speed High-gain Configuration
    \{
*/
/** Defines how many bytes are in hiGainConfig */
#define CONFIG_SIZE_HI_GAIN    ( 5u )
/** Defines the configuration settings for the high speed high gain fixed config */
static const uint8_t _hiGainConfig[CONFIG_SIZE_HI_GAIN] =
{
        2u, /* number of key value pairs */
        SensorId_HIGH_GAIN_IN, (HighGainKey_SAMPLE_RATE| HighGainSampleRate_4800),
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE       | HighGainMode_ENABLE)
};

/** Defines how many bytes are in hiGainDisable */
#define DISABLE_SIZE_HI_GAIN   ( 3u )
/** Defines the configuration to disable the high speed high gain fixed config */
static const uint8_t _hiGainDisable[DISABLE_SIZE_HI_GAIN] =
{
        1u, /* number of key value pairs */
        SensorId_HIGH_GAIN_IN, (HighGainKey_MODE | HighGainMode_DISABLE)
};

/** Defines how many bytes are in _hiGainBuffer */
#define BUFFER_SIZE_HI_GAIN    ( 4u )
/** Defines the data buffer configuration for the high speed high gain fixed config */
static const uint8_t _hiGainBuffer[BUFFER_SIZE_HI_GAIN] =
{
        BUFFER_SIZE_HI_GAIN - 1u,   /* number of meaningful bytes */
        1u,                         /* number of sensors */
        SensorId_HIGH_GAIN_IN,  96u
};
/** \} */

/** \name High-speed Force Gauge Configuration
    \{
*/
/** Defines how many bytes are in hiForceConfig */
#define CONFIG_SIZE_HI_FORCE    ( 5u )
/** Defines the configuration settings for the high speed force gauge fixed config */
static const uint8_t _hiForceConfig[CONFIG_SIZE_HI_FORCE] =
{
        2u, /* number of key value pairs */
        SensorId_FORCE_GAUGE, (ForceGaugeKey_SAMPLE_RATE| ForceGaugeSampleRate_4800),
        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE       | ForceGaugeMode_ENABLE)
};

/** Defines how many bytes are in hiForceDisable */
#define DISABLE_SIZE_HI_FORCE   ( 3u )
/** Defines the configuration to disable the high speed force gauge fixed config */
static const uint8_t _hiForceDisable[DISABLE_SIZE_HI_FORCE] =
{
        1u, /* number of key value pairs */
        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE | ForceGaugeMode_DISABLE)
};

/** Defines how many bytes are in _hiForceBuffer */
#define BUFFER_SIZE_HI_FORCE    ( 4u )
/** Defines the data buffer configuration for the high speed force gauge fixed config */
static const uint8_t _hiForceBuffer[BUFFER_SIZE_HI_FORCE] =
{
        BUFFER_SIZE_HI_FORCE - 1u,   /* number of meaningful bytes */
        1u,                          /* number of sensors */
        SensorId_FORCE_GAUGE,  96u
};
/** \} */

/** \name High-speed Analog Header Configuration
    \{
*/
/** Defines how many bytes are in _hiAnlgHeadConfig */
#define CONFIG_SIZE_HI_ANLG_HEAD          ( 17u )
/** Defines the configuration settings for the high speed analog header fixed config */
static const uint8_t _hiAnlgHeadConfig[CONFIG_SIZE_HI_ANLG_HEAD] =
{
        8u, /* number of Key-Value pairs */
        SensorId_ECG,      (EcgKey_SAMPLE_RATE | EcgSampleRate_400 ),
        SensorId_ECG,      (EcgKey_MODE        | EcgMode_ENABLE),

        SensorId_HEADER_7, (Analog7Key_SAMPLE_RATE| Analog7SampleRate_400),
        SensorId_HEADER_7, (Analog7Key_MODE| Analog7Mode_ENABLE),

        SensorId_HEADER_8, (Analog8Key_SAMPLE_RATE| Analog8SampleRate_400),
        SensorId_HEADER_8, (Analog8Key_MODE| Analog8Mode_ENABLE),

        SensorId_HEADER_9, (Analog9Key_SAMPLE_RATE| Analog9SampleRate_400),
        SensorId_HEADER_9, (Analog9Key_MODE| Analog9Mode_ENABLE)
};

/** Defines how many bytes are in _hiAnlgHeadDisable */
#define DISABLE_SIZE_HI_ANLG_HEAD          ( 9u )
/** Defines the configuration to disable the high speed analog header fixed config */
static const uint8_t _hiAnlgHeadDisable[DISABLE_SIZE_HI_ANLG_HEAD] =
{
        4u, /* number of Key-Value pairs */
        SensorId_ECG,           (EcgKey_MODE    | EcgMode_DISABLE),
        SensorId_HEADER_7,      (Analog7Key_MODE| Analog7Mode_DISABLE),
        SensorId_HEADER_8,      (Analog8Key_MODE| Analog8Mode_DISABLE),
        SensorId_HEADER_9,      (Analog9Key_MODE| Analog9Mode_DISABLE)
};

/** Defines how many bytes are in _hiAnlgHeadBuffer */
#define BUFFER_SIZE_HI_ANLG_HEAD           ( 10u )
/** Defines the data buffer configuration for the high speed analog header fixed config */
static const uint8_t _hiAnlgHeadBuffer[BUFFER_SIZE_HI_ANLG_HEAD] =
{
        BUFFER_SIZE_HI_ANLG_HEAD - 1u,  /* number of meaningful bytes */
        4u,                             /* number of sensors */
        SensorId_ECG,           48u,
        SensorId_HEADER_7,      16u,
        SensorId_HEADER_8,      16u,
        SensorId_HEADER_9,      16u
};
/** \} */

/** \name 6-Channel ECG Configuration
    \{
*/
/** Defines how many bytes are in _ecg6Config */
#define CONFIG_SIZE_ECG6          ( 5u )
/** Defines the configuration settings for the 6-channel ECG fixed config */
static const uint8_t _ecg6Config[CONFIG_SIZE_ECG6] =
{
        2u, /* number of Key-Value pairs */
        SensorId_ECG6CHAN,      (Ecg6ChanKey_SAMPLE_RATE | Ecg6ChanSampleRate_400 ),
        SensorId_ECG6CHAN,      (Ecg6ChanKey_MODE        | Ecg6ChanMode_ENABLE),};

/** Defines how many bytes are in _ecg6Disable */
#define DISABLE_SIZE_ECG6          ( 3u )
/** Defines the configuration to disable the 6-channel ECG fixed config */
static const uint8_t _ecg6Disable[DISABLE_SIZE_ECG6] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ECG6CHAN,           (Ecg6ChanKey_MODE    | Ecg6ChanMode_DISABLE),
};

/** Defines how many bytes are in _ecg6Buffer */
#define BUFFER_SIZE_ECG6           ( 4u )
/** Defines the data buffer configuration for the 6-channel ECG fixed config */
static const uint8_t _ecg6Buffer[BUFFER_SIZE_ECG6] =
{
        BUFFER_SIZE_ECG6 - 1u,  /* number of meaningful bytes */
        1u,                          /* number of sensors */
        SensorId_ECG6CHAN,           96u,
};
/** \} */

/** \name High-speed 6-Channel ECG Configuration
    \{
*/
/** Defines how many bytes are in _hiEcg6Config */
#define CONFIG_SIZE_HI_ECG6          ( 5u )
/** Defines the configuration settings for the high speed 6-channel ECG fixed config */
static const uint8_t _hiEcg6Config[CONFIG_SIZE_HI_ECG6] =
{
        2u, /* number of Key-Value pairs */
        SensorId_ECG6CHAN,      (Ecg6ChanKey_SAMPLE_RATE | Ecg6ChanSampleRate_800 ),
        SensorId_ECG6CHAN,      (Ecg6ChanKey_MODE        | Ecg6ChanMode_ENABLE),};

/** Defines how many bytes are in _hiEcg6Disable */
#define DISABLE_SIZE_HI_ECG6          ( 3u )
/** Defines the configuration to disable the high speed 6-channel ECG fixed config */
static const uint8_t _hiEcg6Disable[DISABLE_SIZE_HI_ECG6] =
{
        1u, /* number of Key-Value pairs */
        SensorId_ECG6CHAN,           (Ecg6ChanKey_MODE    | Ecg6ChanMode_DISABLE),
};

/** Defines how many bytes are in _hiEcg6Buffer */
#define BUFFER_SIZE_HI_ECG6           ( 4u )
/** Defines the data buffer configuration for the high speed 6-channel ECG fixed config */
static const uint8_t _hiEcg6Buffer[BUFFER_SIZE_HI_ECG6] =
{
        BUFFER_SIZE_HI_ECG6 - 1u,  /* number of meaningful bytes */
        1u,                             /* number of sensors */
        SensorId_ECG6CHAN,           96u,
};
/** \} */

/** \name Self-Test Configuration
    \{
*/
/** Defines how many bytes are in selfTestConfig */
#define CONFIG_SIZE_SELFTEST   ( 31u )
/** Defines the configuration settings for the selfTest fixed config */
static const uint8_t _selfTestConfig[CONFIG_SIZE_SELFTEST] =
{
        15u, /* number of key value pairs */
        SensorId_MAGNETOMETER, (MagnetometerKey_SAMPLE_RATE | MagnetometerSampleRate_80),
        SensorId_MAGNETOMETER, (MagnetometerKey_MODE | MagnetometerMode_ENABLE),

        SensorId_ACCELEROMETER, (AccelerometerKey_SAMPLE_RATE | AccelerometerSampleRate_100),
        SensorId_ACCELEROMETER, (AccelerometerKey_RESOLUTION  | AccelerometerResolution_4g),
        SensorId_ACCELEROMETER, (AccelerometerKey_OVER_SAMPLE | AccelerometerOverSample_HI_RES),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE        | AccelerometerMode_ENABLE),

        SensorId_ENCODER, (EncoderKey_MODE | EncoderMode_ENABLE),

        SensorId_FORCE_GAUGE, (ForceGaugeKey_MODE | ForceGaugeMode_ENABLE),

        SensorId_GYROSCOPE, (GyroscopeKey_RESOLUTION  | GyroscopeResolution_2000),
        SensorId_GYROSCOPE, (GyroscopeKey_SAMPLE_RATE | GyroscopeSampleRate_95),
        SensorId_GYROSCOPE, (GyroscopeKey_MODE        | GyroscopeMode_ENABLE),

        SensorId_AMBIENT_LIGHT, (AmbientLightKey_SAMPLE_RATE | AmbientLightSampleRate_100),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE        | AmbientLightMode_ENABLE),

        SensorId_MICROPHONE, (MicrophoneKey_SAMPLE_RATE | MicrophoneSampleRate_100),
        SensorId_MICROPHONE, (MicrophoneKey_MODE        | MicrophoneMode_ENABLE)
};

/** Defines how many bytes are in selfTestDisable */
#define DISABLE_SIZE_SELFTEST       ( 15u )
/** Defines the configuration to disable the self-test fixed config */
static const uint8_t _selfTestDisable[DISABLE_SIZE_SELFTEST] =
{
        7u, /* number of key value pairs */
        SensorId_MAGNETOMETER,  (MagnetometerKey_MODE  | MagnetometerMode_DISABLE),
        SensorId_ACCELEROMETER, (AccelerometerKey_MODE | AccelerometerMode_DISABLE),
        SensorId_ENCODER,       (EncoderKey_MODE       | EncoderMode_DISABLE),
        SensorId_FORCE_GAUGE,   (ForceGaugeKey_MODE    | ForceGaugeMode_DISABLE),
        SensorId_GYROSCOPE,     (GyroscopeKey_MODE     | GyroscopeMode_DISABLE),
        SensorId_AMBIENT_LIGHT, (AmbientLightKey_MODE  | AmbientLightMode_DISABLE),
        SensorId_MICROPHONE,    (MicrophoneKey_MODE    | MicrophoneMode_DISABLE)
};

/** Defines how many bytes are in _selfTestBuffer */
#define BUFFER_SIZE_SELFTEST   ( 16u )
/** Defines the data buffer configuration for the self-test fixed config */
static const uint8_t _selfTestBuffer[BUFFER_SIZE_SELFTEST] =
{
        BUFFER_SIZE_SELFTEST - 1u, /* number of meaningful bytes */
        7u,                           /* number of sensors */
        SensorId_MAGNETOMETER,  12u,
        SensorId_ACCELEROMETER, 12u,
        SensorId_ENCODER,       8u,
        SensorId_FORCE_GAUGE,   4u,
        SensorId_GYROSCOPE,     12u,
        SensorId_AMBIENT_LIGHT, 4u,
        SensorId_MICROPHONE,    4u,
};
/** \} */

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __FIXEDCONFIGSETTINGS_H__ */

