/***************************************************************************//**
    \file KeyValue.h
    \brief Contains the definitions of the Key-Value pairs

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Contains the definitions of the Key-Value pairs used for
                   configuring the different sensors in the IOLab system. The
                   keys and values defined in this module are based on the
                   document 1814F08 Data Protocol Key-Value table.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/KeyValue.h$
    $Revision: 18$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef KEYVALUE_H_
#define KEYVALUE_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the maximum number of Key's possible for a single sensor */
#define MAX_KEYS_POSSIBLE    ( 8u )
/** Defines the maximum number of values possible for a single sensor */
#define MAX_VALUES_POSSIBLE  ( 32u )

/** Defines the starting bit location of the key portion for the key-value */
#define KEY_STARTING_BIT ( 5u )

/** \name Accelerometer Key-Value pairs
    \{
*/
/** Defines the keys for the accelerometer */
typedef enum AccelerometerKey
{
	AccelerometerKey_INVALID     = -1,
	AccelerometerKey_FIRST       = 0u<<KEY_STARTING_BIT,
	AccelerometerKey_MODE        = 0u<<KEY_STARTING_BIT,
	AccelerometerKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	AccelerometerKey_RESOLUTION  = 2u<<KEY_STARTING_BIT,
	AccelerometerKey_OVER_SAMPLE = 3u<<KEY_STARTING_BIT,
	AccelerometerKey_LAST
} AccelerometerKey_t;

/** Defines the values for the accelerometer key AccelerometerKey_MODE */
typedef enum AccelerometerMode
{
	AccelerometerMode_INVALID = -1,
	AccelerometerMode_FIRST   = 0u,
	AccelerometerMode_DISABLE = 0u,
	AccelerometerMode_ENABLE  = 1u,
	AccelerometerMode_LAST
} AccelerometerMode_t;

/** Defines the values for the accelerometer key AccelerometerKey_SAMPLE_RATE */
typedef enum AccelerometerSampleRate
{
	AccelerometerSampleRate_INVALID = -1,
	AccelerometerSampleRate_FIRST   = 0u,
	AccelerometerSampleRate_1_56    = 0u,
	AccelerometerSampleRate_6_25    = 1u,
	AccelerometerSampleRate_12_5    = 2u,
	AccelerometerSampleRate_50      = 3u,
	AccelerometerSampleRate_100     = 4u,
	AccelerometerSampleRate_200     = 5u,
	AccelerometerSampleRate_400     = 6u,
	AccelerometerSampleRate_800     = 7u,
	AccelerometerSampleRate_LAST
} AccelerometerSampleRate_t;

/** Defines the default accelerometer sample rate of AccelerometerSampleRate_t */
#define DEFAULT_ACCELEROMETER_SAMPLE_RATE   ( AccelerometerSampleRate_100 )

/** Defines the values for the accelerometer key AccelerometerKey_RESOLUTION */
typedef enum AccelerometerResolution
{
	AccelerometerResolution_INVALID = -1,
	AccelerometerResolution_FIRST   = 0u,
	AccelerometerResolution_2g      = 0u,
	AccelerometerResolution_4g      = 1u,
	AccelerometerResolution_8g      = 2u,
	AccelerometerResolution_LAST
} AccelerometerResolution_t;

/** Defines the default accelerometer resolution of AccelerometerResolution_t */
#define DEFAULT_ACCELEROMETER_RESOLUTION    ( AccelerometerResolution_2g )

/** Defines the values for the accelerometer key AccelerometerKey_OVER_SAMPLE */
typedef enum AccelerometerOverSample
{
	AccelerometerOverSample_INVALID = -1,
	AccelerometerOverSample_FIRST   = 0u,
	AccelerometerOverSample_NORMAL  = 0u,
	AccelerometerOverSample_LNLP    = 1u, /** Low Noise Low Power */
	AccelerometerOverSample_HI_RES  = 2u,
	AccelerometerOverSample_LO_PWR  = 3u,
	AccelerometerOverSample_LAST
} AccelerometerOverSample_t;

/** Defines the default accelerometer oversample mode of AccelerometerOverSample_t */
#define DEFAULT_ACCELEROMETER_OVER_SAMPLE    ( AccelerometerOverSample_NORMAL )
/** \} */

/** \name Magnetometer Key-Value pairs
    \{
*/
/** Defines the keys for the magnetometer */
typedef enum MagnetometerKey
{
	MagnetometerKey_INVALID     = -1,
	MagnetometerKey_FIRST       = 0u<<KEY_STARTING_BIT,
	MagnetometerKey_MODE        = 0u<<KEY_STARTING_BIT,
	MagnetometerKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	MagnetometerKey_LAST
} MagnetometerKey_t;

/** Defines the values for the magnetometer key MagnetometerKey_MODE */
typedef enum MagnetometerMode
{
	MagnetometerMode_INVALID = -1,
	MagnetometerMode_FIRST   = 0u,
	MagnetometerMode_DISABLE = 0u,
	MagnetometerMode_ENABLE  = 1u,
	MagnetometerMode_LAST
} MagnetometerMode_t;

/** Defines the values for the magnetometer key MagnetometerKey_SAMPLE_RATE */
typedef enum MagnetometerSampleRate
{
	MagnetometerSampleRate_INVALID = -1,
	MagnetometerSampleRate_FIRST    = 0u,
	MagnetometerSampleRate_0_63     = 0u,
	MagnetometerSampleRate_1_25     = 1u,
	MagnetometerSampleRate_2_5      = 2u,
	MagnetometerSampleRate_5        = 3u,
	MagnetometerSampleRate_10       = 4u,
	MagnetometerSampleRate_20       = 5u,
	MagnetometerSampleRate_40       = 6u,
	MagnetometerSampleRate_80       = 7u,
	MagnetometerSampleRate_LAST
} MagnetometerSampleRate_t;

/** Defines the default magnetometer sample rate of MagnetometerSampleRate_t */
#define DEFAULT_MAGNETOMETER_SAMPLE_RATE   ( MagnetomterSampleRate_80 )
/** \} */

/** \name Gyroscope Key-Value pairs
    \{
*/
/** Defines the keys for the gyroscope */
typedef enum GyroscopeKey
{
	GyroscopeKey_INVALID     = -1,
	GyroscopeKey_FIRST       = 0u<<KEY_STARTING_BIT,
	GyroscopeKey_MODE        = 0u<<KEY_STARTING_BIT,
	GyroscopeKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	GyroscopeKey_RESOLUTION  = 2u<<KEY_STARTING_BIT,
	GyroscopeKey_LAST
} GyroscopeKey_t;

/** Defines the values for the Gyroscope key GyroscopeKey_MODE */
typedef enum GyroscopeMode
{
	GyroscopeMode_INVALID = -1,
	GyroscopeMode_FIRST   = 0u,
	GyroscopeMode_DISABLE = 0u,
	GyroscopeMode_ENABLE  = 1u,
	GyroscopeMode_LAST
} GyroscopeMode_t;

/** Defines the values for the Gyroscope key GyroscopeKey_SAMPLE_RATE */
typedef enum GyroscopeSampleRate
{
	GyroscopeSampleRate_INVALID = -1,
	GyroscopeSampleRate_FIRST   = 0u,
	GyroscopeSampleRate_95      = 0u,
	GyroscopeSampleRate_190     = 1u,
	GyroscopeSampleRate_380     = 2u,
	GyroscopeSampleRate_760     = 3u,
	GyroscopeSampleRate_LAST
} GyroscopeSampleRate_t;

/** Defines the default sample rate for the gyroscope key GyroscopeSampleRate_t */
#define DEFAULT_GYROSCOPE_SAMPLE_RATE   ( GyroscopeSampleRate_95 )

/** Defines the values for the gyroscope key GyroscopeKey_RESOLUTION */
typedef enum GyroscopeResolution
{
	GyroscopeResolution_INVALID = -1,
	GyroscopeResolution_FIRST   = 0u,
	GyroscopeResolution_250     = 0u,
	GyroscopeResolution_500     = 1u,
	GyroscopeResolution_2000    = 2u,
	GyroscopeResolution_LAST
} GyroscopeResolution_t;

/** Defines the default resolution for the gyroscope key GyroscopeResolution_t */
#define DEFAULT_GYROSCOPE_RESOLUTION    ( GyroscopeResolution_250 )
/** \} */

/** \name Barometer Key-Value pairs
    \{
*/
/** Defines the keys for the barometer */
typedef enum BarometerKey
{
	BarometerKey_INVALID     = -1,
	BarometerKey_FIRST       = 0u<<KEY_STARTING_BIT,
	BarometerKey_MODE        = 0u<<KEY_STARTING_BIT,
	BarometerKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	BarometerKey_LAST
} BarometerKey_t;

/** Defines the values for the Barometer key BarometerKey_MODE */
typedef enum BarometerMode
{
	BarometerMode_INVALID = -1,
	BarometerMode_FIRST   = 0u,
	BarometerMode_DISABLE = 0u,
	BarometerMode_ENABLE  = 1u,
	BarometerMode_LAST
} BarometerMode_t;

/** Defines the values for the Barometer key BarometerKey_SAMPLE_RATE */
typedef enum BarometerSampleRate
{
	BarometerSampleRate_INVALID = -1,
	BarometerSampleRate_FIRST   = 0u,
	BarometerSampleRate_1       = 0u,
	BarometerSampleRate_10      = 1u,
	BarometerSampleRate_50      = 2u,
	BarometerSampleRate_100     = 3u,
	BarometerSampleRate_LAST
} BarometerSampleRate_t;

/** Defines the default sample rate for the encoder key BarometerKey_SAMPLE_RATE */
#define DEFAULT_BAROMETER_SAMPLE_RATE   ( BarometerSampleRate_100 )
/** \} */

// TODO: spg - add ultrasonic

/** \name Microphone Key-Value pairs
    \{
*/

/** Defines the keys for the Microphone */
typedef enum MicrophoneKey
{
	MicrophoneKey_INVALID     = -1,
	MicrophoneKey_FIRST       = 0u<<KEY_STARTING_BIT,
	MicrophoneKey_MODE        = 0u<<KEY_STARTING_BIT,
	MicrophoneKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	MicrophoneKey_LAST
} MicrophoneKey_t;

/** Defines the values for the Microphone key MicrophoneKey_MODE */
typedef enum MicrophoneMode
{
	MicrophoneMode_INVALID = -1,
	MicrophoneMode_FIRST   = 0u,
	MicrophoneMode_DISABLE = 0u,
	MicrophoneMode_ENABLE  = 1u,
	MicrophoneMode_LAST
} MicrophoneMode_t;

/** Defines the values for the Microphone key MicrophoneKey_SAMPLE_RATE */
typedef enum MicrophoneSampleRate
{
	MicrophoneSampleRate_INVALID   = -1,
	MicrophoneSampleRate_FIRST     = 0u,
	MicrophoneSampleRate_1         = 0u,
	MicrophoneSampleRate_10        = 1u,
	MicrophoneSampleRate_50        = 2u,
	MicrophoneSampleRate_100       = 3u,
	MicrophoneSampleRate_200       = 4u,
	MicrophoneSampleRate_400       = 5u,
	MicrophoneSampleRate_800       = 6u,
	MicrophoneSampleRate_2400      = 7u,
	MicrophoneSampleRate_4800      = 8u,
	MicrophoneSampleRate_6000      = 9u,
	MicrophoneSampleRate_LAST
} MicrophoneSampleRate_t;

/** Defines the default sample rate for the Microphone key MicrophoneKey_SAMPLE_RATE */
#define DEFAULT_MICROPHONE_SAMPLE_RATE   ( MicrophoneSampleRate_2400 )
/** \} */

/** \name Ambient Light Sensor Key-Value pairs
    \{
*/

/** Defines the keys for the AmbientLight */
typedef enum AmbientLightKey
{
	AmbientLightKey_INVALID     = -1,
	AmbientLightKey_FIRST       = 0u<<KEY_STARTING_BIT,
	AmbientLightKey_MODE        = 0u<<KEY_STARTING_BIT,
	AmbientLightKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	AmbientLightKey_LAST
} AmbientLightKey_t;

/** Defines the values for the AmbientLight key AmbientLightKey_MODE */
typedef enum AmbientLightMode
{
	AmbientLightMode_INVALID = -1,
	AmbientLightMode_FIRST   = 0u,
	AmbientLightMode_DISABLE = 0u,
	AmbientLightMode_ENABLE  = 1u,
	AmbientLightMode_LAST
} AmbientLightMode_t;

/** Defines the values for the AmbientLight key AmbientLightKey_SAMPLE_RATE */
typedef enum AmbientLightSampleRate
{
	AmbientLightSampleRate_INVALID   = -1,
	AmbientLightSampleRate_FIRST     = 0u,
	AmbientLightSampleRate_1         = 0u,
	AmbientLightSampleRate_10        = 1u,
	AmbientLightSampleRate_50        = 2u,
	AmbientLightSampleRate_100       = 3u,
	AmbientLightSampleRate_200       = 4u,
	AmbientLightSampleRate_400       = 5u,
    AmbientLightSampleRate_800       = 6u,
	AmbientLightSampleRate_2400      = 7u,
	AmbientLightSampleRate_4800      = 8u,
	AmbientLightSampleRate_6000      = 9u,
	AmbientLightSampleRate_LAST
} AmbientLightSampleRate_t;

/** Defines the default sample rate for the AmbientLight key AmbientLightKey_SAMPLE_RATE */
#define DEFAULT_AMBIENT_LIGHT_SAMPLE_RATE   ( AmbientLightSampleRate_50 )
/** \} */

/** \name ForceGauge Key-Value pairs
    \{
*/
/** Defines the keys for the ForceGauge */
typedef enum ForceGaugeKey
{
	ForceGaugeKey_INVALID     = -1,
	ForceGaugeKey_FIRST       = 0u<<KEY_STARTING_BIT,
	ForceGaugeKey_MODE        = 0u<<KEY_STARTING_BIT,
	ForceGaugeKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	ForceGaugeKey_LAST
} ForceGaugeKey_t;

/** Defines the values for the ForceGauge key ForceGaugeKey_MODE */
typedef enum ForceGaugeMode
{
	ForceGaugeMode_INVALID = -1,
	ForceGaugeMode_FIRST   = 0u,
	ForceGaugeMode_DISABLE = 0u,
	ForceGaugeMode_ENABLE  = 1u,
	ForceGaugeMode_LAST
} ForceGaugeMode_t;

/** Defines the values for the ForceGauge key ForceGaugeKey_SAMPLE_RATE */
typedef enum ForceGaugeSampleRate
{
	ForceGaugeSampleRate_INVALID   = -1,
	ForceGaugeSampleRate_FIRST     = 0u,
	ForceGaugeSampleRate_1         = 0u,
	ForceGaugeSampleRate_10        = 1u,
	ForceGaugeSampleRate_50        = 2u,
	ForceGaugeSampleRate_100       = 3u,
	ForceGaugeSampleRate_200       = 4u,
	ForceGaugeSampleRate_400       = 5u,
    ForceGaugeSampleRate_800       = 6u,
	ForceGaugeSampleRate_2400      = 7u,
	ForceGaugeSampleRate_4800      = 8u,
	ForceGaugeSampleRate_6000      = 9u,
	ForceGaugeSampleRate_LAST
} ForceGaugeSampleRate_t;

/** Defines the default sample rate for the ForceGauge key ForceGaugeKey_SAMPLE_RATE */
#define DEFAULT_FORCE_GAUGE_SAMPLE_RATE   ( ForceGaugeSampleRate_100 )
/** \} */

/** \name Encoder Key-Value pairs
    \{
*/
/** Defines the keys for the encoder */
typedef enum EncoderKey
{
	EncoderKey_INVALID     = -1,
	EncoderKey_FIRST       = 0u<<KEY_STARTING_BIT,
	EncoderKey_MODE        = 0u<<KEY_STARTING_BIT,
	EncoderKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	EncoderKey_LAST
} EncoderKey_t;

/** Defines the values for the Encoder key EncoderKey_MODE */
typedef enum EncoderMode
{
	EncoderMode_INVALID = -1,
	EncoderMode_FIRST   = 0u,
	EncoderMode_DISABLE = 0u,
	EncoderMode_ENABLE  = 1u,
	EncoderMode_LAST
} EncoderMode_t;

/** Defines the values for the Encoder key EncoderKey_SAMPLE_RATE */
typedef enum EncoderSampleRate
{
	EncoderSampleRate_INVALID = -1,
	EncoderSampleRate_FIRST   = 0u,
	EncoderSampleRate_50      = 0u,
	EncoderSampleRate_100     = 1u,
	EncoderSampleRate_LAST
} EncoderSampleRate_t;

/** Defines the default sample rate for the encoder key EncoderKey_SAMPLE_RATE */
#define DEFAULT_ENCODER_SAMPLE_RATE   ( EncoderSampleRate_100 )
/** \} */

/** \name ECG Key-Value pairs
    \{
*/

/** Defines the keys for the Ecg */
typedef enum EcgKey
{
	EcgKey_INVALID     = -1,
	EcgKey_FIRST       = 0u<<KEY_STARTING_BIT,
	EcgKey_MODE        = 0u<<KEY_STARTING_BIT,
	EcgKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	EcgKey_REF_VOLT    = 2u<<KEY_STARTING_BIT,
	EcgKey_LAST
} EcgKey_t;

/** Defines the values for the Ecg key EcgKey_MODE */
typedef enum EcgMode
{
	EcgMode_INVALID = -1,
	EcgMode_FIRST   = 0u,
	EcgMode_DISABLE = 0u,
	EcgMode_ENABLE  = 1u,
	EcgMode_LAST
} EcgMode_t;

/** Defines the values for the Ecg key EcgKey_SAMPLE_RATE */
typedef enum EcgSampleRate
{
	EcgSampleRate_INVALID   = -1,
	EcgSampleRate_FIRST     = 0u,
	EcgSampleRate_1         = 0u,
	EcgSampleRate_10        = 1u,
	EcgSampleRate_50        = 2u,
	EcgSampleRate_100       = 3u,
	EcgSampleRate_200       = 4u,
	EcgSampleRate_400       = 5u,
	EcgSampleRate_800       = 6u,
	EcgSampleRate_LAST
} EcgSampleRate_t;

/** Defines the default sample rate for the Ecg key EcgKey_SAMPLE_RATE */
#define DEFAULT_ECG_SAMPLE_RATE   ( EcgSampleRate_200 )

/** Defines the analog voltage reference options for the Ecg key EcgKey_REF_VOLT*/
typedef enum EcgRefVolt
{
    EcgRefVolt_INVALID = -1,
    EcgRefVolt_FIRST   = 0u,
    EcgRefVolt_3v0     = 0u,
    EcgRefVolt_3v3     = 1u,
    EcgRefVolt_LAST
} EcgRefVolt_t;

/** Defines the default reference voltage for the EcgKey_REF_VOLT */
#define DEFAULT_ECG_REF_VOLT    ( EcgRefVolt_3v0 )
/** \} */

/** \name Battery Key-Value pairs
    \{
*/
/** Defines the keys for the battery sensor */
typedef enum BatteryKey
{
	BatteryKey_INVALID     = -1,
	BatteryKey_FIRST       = 0u<<KEY_STARTING_BIT,
	BatteryKey_MODE        = 0u<<KEY_STARTING_BIT,
	BatteryKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	BatteryKey_LAST
} BatteryKey_t;

/** Defines the values for the Battery key BatteryKey_MODE */
typedef enum BatteryMode
{
	BatteryMode_INVALID = -1,
	BatteryMode_FIRST   = 0u,
	BatteryMode_DISABLE = 0u,
	BatteryMode_ENABLE  = 1u,
	BatteryMode_LAST
} BatteryMode_t;

/** Defines the values for the Battery key BatteryKey_SAMPLE_RATE */
typedef enum BatterySampleRate
{
	BatterySampleRate_INVALID = -1,
	BatterySampleRate_FIRST   = 0u,
	BatterySampleRate_1       = 0u,
	BatterySampleRate_10      = 1u,
	BatterySampleRate_50      = 2u,
	BatterySampleRate_100     = 3u,
	BatterySampleRate_200     = 4u,
	BatterySampleRate_400     = 5u,
    BatterySampleRate_800     = 6u,
	BatterySampleRate_2400    = 7u,
	BatterySampleRate_4800    = 8u,
	BatterySampleRate_6000    = 9u,
	BatterySampleRate_LAST
} BatterySampleRate_t;

/** Defines the default sample rate for the battery key BatteryKey_SAMPLE_RATE */
#define DEFAULT_BATTERY_SAMPLE_RATE   ( BatterySampleRate_100 )
/** \} */

/** \name High Gain Input Key-Value pairs
    \{
*/

/** Defines the keys for the HighGain */
typedef enum HighGainKey
{
	HighGainKey_INVALID     = -1,
	HighGainKey_FIRST       = 0u<<KEY_STARTING_BIT,
	HighGainKey_MODE        = 0u<<KEY_STARTING_BIT,
	HighGainKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	HighGainKey_LAST
} HighGainKey_t;

/** Defines the values for the HighGain key HighGainKey_MODE */
typedef enum HighGainMode
{
	HighGainMode_INVALID = -1,
	HighGainMode_FIRST   = 0u,
	HighGainMode_DISABLE = 0u,
	HighGainMode_ENABLE  = 1u,
	HighGainMode_LAST
} HighGainMode_t;

/** Defines the values for the HighGain key HighGainKey_SAMPLE_RATE */
typedef enum HighGainSampleRate
{
	HighGainSampleRate_INVALID   = -1,
	HighGainSampleRate_FIRST     = 0u,
	HighGainSampleRate_1         = 0u,
	HighGainSampleRate_10        = 1u,
	HighGainSampleRate_50        = 2u,
	HighGainSampleRate_100       = 3u,
	HighGainSampleRate_200       = 4u,
	HighGainSampleRate_400       = 5u,
    HighGainSampleRate_800       = 6u,
	HighGainSampleRate_2400      = 7u,
	HighGainSampleRate_4800      = 8u,
	HighGainSampleRate_6000      = 9u,
	HighGainSampleRate_LAST
} HighGainSampleRate_t;

/** Defines the default sample rate for the HighGain key HighGainKey_SAMPLE_RATE */
#define DEFAULT_HIGH_GAIN_SAMPLE_RATE   ( HighGainSampleRate_100 )
/** \} */

/** \name Header Pin 1 Key-Value pairs
    \{
*/
/** Defines the keys for header pin 1 */
typedef enum Header1Key
{
	Header1Key_INVALID     	= -1,
	Header1Key_FIRST       	= 0u<<KEY_STARTING_BIT,
	Header1Key_MODE        	= 0u<<KEY_STARTING_BIT,
	Header1Key_OUTPUT_VALUE = 1u<<KEY_STARTING_BIT,
	Header1Key_LAST
} Header1Key_t;

/** Defines the values for header pin 1 key Header1Key_MODE */
typedef enum Header1Mode
{
	Header1Mode_INVALID = -1,
	Header1Mode_FIRST   = 0u,
	Header1Mode_DISABLE = 0u,
	Header1Mode_INPUT  	= 1u,
	Header1Mode_OUTPUT  = 2u,
	Header1Mode_LAST
} Header1Mode_t;

/** Defines the values for the Header1 key Header1Key_SAMPLE_RATE */
typedef enum Header1OutputValue
{
	Header1OutputValue_INVALID = -1,
	Header1OutputValue_FIRST   = 0u,
	Header1OutputValue_LOW     = 0u,
	Header1OutputValue_HIGH    = 1u,
	Header1OutputValue_LAST
} Header1OutputValue_t;

/** Defines the default sample rate for header pin 1 key Header1Key_SAMPLE_RATE */
#define DEFAULT_HEADER1_OUTPUT_VALUE   ( Header1OutputValue_LOW )
/** \} */


/** \name Header Pin 2 Key-Value pairs
    \{
*/
/** Defines the keys for header pin 2 */
typedef enum Header2Key
{
	Header2Key_INVALID     	= -1,
	Header2Key_FIRST       	= 0u<<KEY_STARTING_BIT,
	Header2Key_MODE        	= 0u<<KEY_STARTING_BIT,
	Header2Key_OUTPUT_VALUE = 1u<<KEY_STARTING_BIT,
	Header2Key_LAST
} Header2Key_t;

/** Defines the values for header pin 2 key Header2Key_MODE */
typedef enum Header2Mode
{
	Header2Mode_INVALID = -1,
	Header2Mode_FIRST   = 0u,
	Header2Mode_DISABLE = 0u,
	Header2Mode_INPUT  	= 1u,
	Header2Mode_OUTPUT  = 2u,
	Header2Mode_LAST
} Header2Mode_t;

/** Defines the values for the Header2 key Header2Key_SAMPLE_RATE */
typedef enum Header2OutputValue
{
	Header2OutputValue_INVALID = -1,
	Header2OutputValue_FIRST   = 0u,
	Header2OutputValue_LOW     = 0u,
	Header2OutputValue_HIGH    = 1u,
	Header2OutputValue_LAST
} Header2OutputValue_t;

/** Defines the default sample rate for header pin 2 key Header2Key_SAMPLE_RATE */
#define DEFAULT_HEADER2_OUTPUT_VALUE   ( Header2OutputValue_LOW )
/** \} */

/** \name Header Pin 3 Key-Value pairs
    \{
*/
/** Defines the keys for header pin 3 */
typedef enum Header3Key
{
	Header3Key_INVALID     	= -1,
	Header3Key_FIRST       	= 0u<<KEY_STARTING_BIT,
	Header3Key_MODE        	= 0u<<KEY_STARTING_BIT,
	Header3Key_OUTPUT_VALUE = 1u<<KEY_STARTING_BIT,
	Header3Key_LAST
} Header3Key_t;

/** Defines the values for header pin 3 key Header3Key_MODE */
typedef enum Header3Mode
{
	Header3Mode_INVALID = -1,
	Header3Mode_FIRST   = 0u,
	Header3Mode_DISABLE = 0u,
	Header3Mode_INPUT  	= 1u,
	Header3Mode_OUTPUT  = 2u,
	Header3Mode_LAST
} Header3Mode_t;

/** Defines the values for the Header3 key Header3Key_SAMPLE_RATE */
typedef enum Header3OutputValue
{
	Header3OutputValue_INVALID = -1,
	Header3OutputValue_FIRST   = 0u,
	Header3OutputValue_LOW     = 0u,
	Header3OutputValue_HIGH    = 1u,
	Header3OutputValue_LAST
} Header3OutputValue_t;

/** Defines the default sample rate for header pin 3 key Header3Key_SAMPLE_RATE */
#define DEFAULT_HEADER3_OUTPUT_VALUE   ( Header3OutputValue_LOW )
/** \} */

/** \name Header4 Key-Value pairs
    \{
*/
/** Defines the keys for Header4 */
typedef enum Header4Key
{
	Header4Key_INVALID     		= -1,
	Header4Key_FIRST       		= 0u<<KEY_STARTING_BIT,
	Header4Key_MODE        		= 0u<<KEY_STARTING_BIT,
	Header4Key_LOWFREQUENCY   	= 1u<<KEY_STARTING_BIT,
	Header4Key_MIDFREQUENCY   	= 2u<<KEY_STARTING_BIT,
	Header4Key_HIGHFREQUENCY	= 3u<<KEY_STARTING_BIT,
	Header4Key_DUTYCYCLE	  	= 4u<<KEY_STARTING_BIT,
	Header4Key_LAST
} Header4Key_t;

/** Defines the values for the Header4 key Header4Key_MODE */
typedef enum Header4Mode
{
	Header4Mode_INVALID = -1,
	Header4Mode_FIRST   = 0u,
	Header4Mode_DISABLE = 0u,
	Header4Mode_INPUT  	= 1u,
	Header4Mode_OUTPUT  = 2u,
	Header4Mode_LAST
} Header4Mode_t;

/** Defines the values for the Header pin 4 key Header4Key_FREQUENCY */
typedef enum Header4LowFrequency
{
	Header4LowFrequency_INVALID = -1,
	Header4LowFrequency_FIRST   = 0u,
	Header4LowFrequency_00      = 0u,
	Header4LowFrequency_01      = 1u,
	Header4LowFrequency_02      = 2u,
	Header4LowFrequency_03      = 3u,
	Header4LowFrequency_04      = 4u,
	Header4LowFrequency_05      = 5u,
	Header4LowFrequency_06      = 6u,
	Header4LowFrequency_07      = 7u,
	Header4LowFrequency_08      = 8u,
	Header4LowFrequency_09      = 9u,
	Header4LowFrequency_10      = 10u,
	Header4LowFrequency_11      = 11u,
	Header4LowFrequency_12      = 12u,
	Header4LowFrequency_13      = 13u,
	Header4LowFrequency_14      = 14u,
	Header4LowFrequency_15      = 15u,
	Header4LowFrequency_16      = 16u,
	Header4LowFrequency_17      = 17u,
	Header4LowFrequency_18      = 18u,
	Header4LowFrequency_19      = 19u,
	Header4LowFrequency_20      = 20u,
	Header4LowFrequency_21      = 21u,
	Header4LowFrequency_22      = 22u,
	Header4LowFrequency_23      = 23u,
	Header4LowFrequency_24      = 24u,
	Header4LowFrequency_25      = 25u,
	Header4LowFrequency_26      = 26u,
	Header4LowFrequency_27      = 27u,
	Header4LowFrequency_28      = 28u,
	Header4LowFrequency_29      = 29u,
	Header4LowFrequency_30      = 30u,
	Header4LowFrequency_31      = 31u,
	Header4LowFrequency_LAST
} Header4LowFrequency_t;

/** Defines the values for the Header pin 4 key Header4Key_FREQUENCY */
typedef enum Header4MidFrequency
{
	Header4MidFrequency_INVALID = -1,
	Header4MidFrequency_FIRST   = 0u,
	Header4MidFrequency_00      = 0u,
	Header4MidFrequency_01      = 1u,
	Header4MidFrequency_02      = 2u,
	Header4MidFrequency_03      = 3u,
	Header4MidFrequency_04      = 4u,
	Header4MidFrequency_05      = 5u,
	Header4MidFrequency_06      = 6u,
	Header4MidFrequency_07      = 7u,
	Header4MidFrequency_08      = 8u,
	Header4MidFrequency_09      = 9u,
	Header4MidFrequency_10      = 10u,
	Header4MidFrequency_11      = 11u,
	Header4MidFrequency_12      = 12u,
	Header4MidFrequency_13      = 13u,
	Header4MidFrequency_14      = 14u,
	Header4MidFrequency_15      = 15u,
	Header4MidFrequency_16      = 16u,
	Header4MidFrequency_17      = 17u,
	Header4MidFrequency_18      = 18u,
	Header4MidFrequency_19      = 19u,
	Header4MidFrequency_20      = 20u,
	Header4MidFrequency_21      = 21u,
	Header4MidFrequency_22      = 22u,
	Header4MidFrequency_23      = 23u,
	Header4MidFrequency_24      = 24u,
	Header4MidFrequency_25      = 25u,
	Header4MidFrequency_26      = 26u,
	Header4MidFrequency_27      = 27u,
	Header4MidFrequency_28      = 28u,
	Header4MidFrequency_29      = 29u,
	Header4MidFrequency_30      = 30u,
	Header4MidFrequency_31      = 31u,
	Header4MidFrequency_LAST
} Header4MidFrequency_t;

/** Defines the values for the Header pin 4 key Header4Key_FREQUENCY */
typedef enum Header4HighFrequency
{
	Header4HighFrequency_INVALID = -1,
	Header4HighFrequency_FIRST   = 0u,
	Header4HighFrequency_00      = 0u,
	Header4HighFrequency_01      = 1u,
	Header4HighFrequency_02      = 2u,
	Header4HighFrequency_03      = 3u,
	Header4HighFrequency_04      = 4u,
	Header4HighFrequency_05      = 5u,
	Header4HighFrequency_06      = 6u,
	Header4HighFrequency_07      = 7u,
	Header4HighFrequency_08      = 8u,
	Header4HighFrequency_09      = 9u,
	Header4HighFrequency_10      = 10u,
	Header4HighFrequency_11      = 11u,
	Header4HighFrequency_12      = 12u,
	Header4HighFrequency_13      = 13u,
	Header4HighFrequency_14      = 14u,
	Header4HighFrequency_15      = 15u,
	Header4HighFrequency_16      = 16u,
	Header4HighFrequency_17      = 17u,
	Header4HighFrequency_18      = 18u,
	Header4HighFrequency_19      = 19u,
	Header4HighFrequency_20      = 20u,
	Header4HighFrequency_21      = 21u,
	Header4HighFrequency_22      = 22u,
	Header4HighFrequency_23      = 23u,
	Header4HighFrequency_24      = 24u,
	Header4HighFrequency_25      = 25u,
	Header4HighFrequency_26      = 26u,
	Header4HighFrequency_27      = 27u,
	Header4HighFrequency_28      = 28u,
	Header4HighFrequency_29      = 29u,
	Header4HighFrequency_30      = 30u,
	Header4HighFrequency_31      = 31u,
	Header4HighFrequency_LAST
} Header4HighFrequency_t;

/** Defines the values for the Header 4 pin key Header4Key_DUTYCYCLE */
typedef enum Header4DutyCycle
{
	Header4DutyCycle_INVALID = -1,
	Header4DutyCycle_FIRST   = 0u,
	Header4DutyCycle_00      = 0u,
	Header4DutyCycle_01      = 1u,
	Header4DutyCycle_02      = 2u,
	Header4DutyCycle_03      = 3u,
	Header4DutyCycle_04      = 4u,
	Header4DutyCycle_05      = 5u,
	Header4DutyCycle_06      = 6u,
	Header4DutyCycle_07      = 7u,
	Header4DutyCycle_08      = 8u,
	Header4DutyCycle_09      = 9u,
	Header4DutyCycle_10      = 10u,
	Header4DutyCycle_11      = 11u,
	Header4DutyCycle_12      = 12u,
	Header4DutyCycle_13      = 13u,
	Header4DutyCycle_14      = 14u,
	Header4DutyCycle_15      = 15u,
	Header4DutyCycle_16      = 16u,
	Header4DutyCycle_17      = 17u,
	Header4DutyCycle_18      = 18u,
	Header4DutyCycle_19      = 19u,
	Header4DutyCycle_20      = 20u,
	Header4DutyCycle_21      = 21u,
	Header4DutyCycle_22      = 22u,
	Header4DutyCycle_23      = 23u,
	Header4DutyCycle_24      = 24u,
	Header4DutyCycle_25      = 25u,
	Header4DutyCycle_26      = 26u,
	Header4DutyCycle_27      = 27u,
	Header4DutyCycle_28      = 28u,
	Header4DutyCycle_29      = 29u,
	Header4DutyCycle_30      = 30u,
	Header4DutyCycle_31      = 31u,
	Header4DutyCycle_LAST
} Header4DutyCycle_t;

/** Defines the default low frequency for header pin 4 key Header4Key_FREQUENCY */
#define DEFAULT_HEADER4_LOWFREQUENCY	( Header4LowFrequency_09 )

/** Defines the default mid frequency for header pin 4 key Header4Key_FREQUENCY */
#define DEFAULT_HEADER4_MIDFREQUENCY	( Header4MidFrequency_09 )

/** Defines the default high frequency for header pin 4 key Header4Key_FREQUENCY */
#define DEFAULT_HEADER4_HIGHFREQUENCY	( Header4HighFrequency_09 )

/** Defines the default duty cycle for header pin 4 key Header4Key_DUTYCYCLE */
#define DEFAULT_HEADER4_DUTYCYCLE	( Header4DutyCycle_16 )
/** \} */

/** \name Header5 Key-Value pairs
    \{
*/
/** Defines the keys for Header5 */
typedef enum Header5Key
{
	Header5Key_INVALID     		= -1,
	Header5Key_FIRST       		= 0u<<KEY_STARTING_BIT,
	Header5Key_MODE        		= 0u<<KEY_STARTING_BIT,
	Header5Key_LOWFREQUENCY   	= 1u<<KEY_STARTING_BIT,
	Header5Key_MIDFREQUENCY   	= 2u<<KEY_STARTING_BIT,
	Header5Key_HIGHFREQUENCY	= 3u<<KEY_STARTING_BIT,
	Header5Key_DUTYCYCLE	  	= 4u<<KEY_STARTING_BIT,
	Header5Key_LAST
} Header5Key_t;

/** Defines the values for the Header5 key Header5Key_MODE */
typedef enum Header5Mode
{
	Header5Mode_INVALID = -1,
	Header5Mode_FIRST   = 0u,
	Header5Mode_DISABLE = 0u,
	Header5Mode_INPUT  	= 1u,
	Header5Mode_OUTPUT  = 2u,
	Header5Mode_LAST
} Header5Mode_t;

/** Defines the values for the Header pin 5 key Header5Key_FREQUENCY */
typedef enum Header5LowFrequency
{
	Header5LowFrequency_INVALID = -1,
	Header5LowFrequency_FIRST   = 0u,
	Header5LowFrequency_00      = 0u,
	Header5LowFrequency_01      = 1u,
	Header5LowFrequency_02      = 2u,
	Header5LowFrequency_03      = 3u,
	Header5LowFrequency_04      = 4u,
	Header5LowFrequency_05      = 5u,
	Header5LowFrequency_06      = 6u,
	Header5LowFrequency_07      = 7u,
	Header5LowFrequency_08      = 8u,
	Header5LowFrequency_09      = 9u,
	Header5LowFrequency_10      = 10u,
	Header5LowFrequency_11      = 11u,
	Header5LowFrequency_12      = 12u,
	Header5LowFrequency_13      = 13u,
	Header5LowFrequency_14      = 14u,
	Header5LowFrequency_15      = 15u,
	Header5LowFrequency_16      = 16u,
	Header5LowFrequency_17      = 17u,
	Header5LowFrequency_18      = 18u,
	Header5LowFrequency_19      = 19u,
	Header5LowFrequency_20      = 20u,
	Header5LowFrequency_21      = 21u,
	Header5LowFrequency_22      = 22u,
	Header5LowFrequency_23      = 23u,
	Header5LowFrequency_24      = 24u,
	Header5LowFrequency_25      = 25u,
	Header5LowFrequency_26      = 26u,
	Header5LowFrequency_27      = 27u,
	Header5LowFrequency_28      = 28u,
	Header5LowFrequency_29      = 29u,
	Header5LowFrequency_30      = 30u,
	Header5LowFrequency_31      = 31u,
	Header5LowFrequency_LAST
} Header5LowFrequency_t;

/** Defines the values for the Header pin 5 key Header5Key_FREQUENCY */
typedef enum Header5MidFrequency
{
	Header5MidFrequency_INVALID = -1,
	Header5MidFrequency_FIRST   = 0u,
	Header5MidFrequency_00      = 0u,
	Header5MidFrequency_01      = 1u,
	Header5MidFrequency_02      = 2u,
	Header5MidFrequency_03      = 3u,
	Header5MidFrequency_04      = 4u,
	Header5MidFrequency_05      = 5u,
	Header5MidFrequency_06      = 6u,
	Header5MidFrequency_07      = 7u,
	Header5MidFrequency_08      = 8u,
	Header5MidFrequency_09      = 9u,
	Header5MidFrequency_10      = 10u,
	Header5MidFrequency_11      = 11u,
	Header5MidFrequency_12      = 12u,
	Header5MidFrequency_13      = 13u,
	Header5MidFrequency_14      = 14u,
	Header5MidFrequency_15      = 15u,
	Header5MidFrequency_16      = 16u,
	Header5MidFrequency_17      = 17u,
	Header5MidFrequency_18      = 18u,
	Header5MidFrequency_19      = 19u,
	Header5MidFrequency_20      = 20u,
	Header5MidFrequency_21      = 21u,
	Header5MidFrequency_22      = 22u,
	Header5MidFrequency_23      = 23u,
	Header5MidFrequency_24      = 24u,
	Header5MidFrequency_25      = 25u,
	Header5MidFrequency_26      = 26u,
	Header5MidFrequency_27      = 27u,
	Header5MidFrequency_28      = 28u,
	Header5MidFrequency_29      = 29u,
	Header5MidFrequency_30      = 30u,
	Header5MidFrequency_31      = 31u,
	Header5MidFrequency_LAST
} Header5MidFrequency_t;

/** Defines the values for the Header pin 5 key Header5Key_FREQUENCY */
typedef enum Header5HighFrequency
{
	Header5HighFrequency_INVALID = -1,
	Header5HighFrequency_FIRST   = 0u,
	Header5HighFrequency_00      = 0u,
	Header5HighFrequency_01      = 1u,
	Header5HighFrequency_02      = 2u,
	Header5HighFrequency_03      = 3u,
	Header5HighFrequency_04      = 4u,
	Header5HighFrequency_05      = 5u,
	Header5HighFrequency_06      = 6u,
	Header5HighFrequency_07      = 7u,
	Header5HighFrequency_08      = 8u,
	Header5HighFrequency_09      = 9u,
	Header5HighFrequency_10      = 10u,
	Header5HighFrequency_11      = 11u,
	Header5HighFrequency_12      = 12u,
	Header5HighFrequency_13      = 13u,
	Header5HighFrequency_14      = 14u,
	Header5HighFrequency_15      = 15u,
	Header5HighFrequency_16      = 16u,
	Header5HighFrequency_17      = 17u,
	Header5HighFrequency_18      = 18u,
	Header5HighFrequency_19      = 19u,
	Header5HighFrequency_20      = 20u,
	Header5HighFrequency_21      = 21u,
	Header5HighFrequency_22      = 22u,
	Header5HighFrequency_23      = 23u,
	Header5HighFrequency_24      = 24u,
	Header5HighFrequency_25      = 25u,
	Header5HighFrequency_26      = 26u,
	Header5HighFrequency_27      = 27u,
	Header5HighFrequency_28      = 28u,
	Header5HighFrequency_29      = 29u,
	Header5HighFrequency_30      = 30u,
	Header5HighFrequency_31      = 31u,
	Header5HighFrequency_LAST
} Header5HighFrequency_t;

/** Defines the values for the Header 5 pin key Header5Key_DUTYCYCLE */
typedef enum Header5DutyCycle
{
	Header5DutyCycle_INVALID = -1,
	Header5DutyCycle_FIRST   = 0u,
	Header5DutyCycle_00      = 0u,
	Header5DutyCycle_01      = 1u,
	Header5DutyCycle_02      = 2u,
	Header5DutyCycle_03      = 3u,
	Header5DutyCycle_04      = 4u,
	Header5DutyCycle_05      = 5u,
	Header5DutyCycle_06      = 6u,
	Header5DutyCycle_07      = 7u,
	Header5DutyCycle_08      = 8u,
	Header5DutyCycle_09      = 9u,
	Header5DutyCycle_10      = 10u,
	Header5DutyCycle_11      = 11u,
	Header5DutyCycle_12      = 12u,
	Header5DutyCycle_13      = 13u,
	Header5DutyCycle_14      = 14u,
	Header5DutyCycle_15      = 15u,
	Header5DutyCycle_16      = 16u,
	Header5DutyCycle_17      = 17u,
	Header5DutyCycle_18      = 18u,
	Header5DutyCycle_19      = 19u,
	Header5DutyCycle_20      = 20u,
	Header5DutyCycle_21      = 21u,
	Header5DutyCycle_22      = 22u,
	Header5DutyCycle_23      = 23u,
	Header5DutyCycle_24      = 24u,
	Header5DutyCycle_25      = 25u,
	Header5DutyCycle_26      = 26u,
	Header5DutyCycle_27      = 27u,
	Header5DutyCycle_28      = 28u,
	Header5DutyCycle_29      = 29u,
	Header5DutyCycle_30      = 30u,
	Header5DutyCycle_31      = 31u,
	Header5DutyCycle_LAST
} Header5DutyCycle_t;

/** Defines the default low frequency for header pin 5 key Header5Key_FREQUENCY */
#define DEFAULT_HEADER5_LOWFREQUENCY	( Header5LowFrequency_09 )

/** Defines the default mid frequency for header pin 5 key Header5Key_FREQUENCY */
#define DEFAULT_HEADER5_MIDFREQUENCY	( Header5MidFrequency_09 )

/** Defines the default high frequency for header pin 5 key Header5Key_FREQUENCY */
#define DEFAULT_HEADER5_HIGHFREQUENCY	( Header5HighFrequency_09 )

/** Defines the default duty cycle for header pin 5 key Header5Key_DUTYCYCLE */
#define DEFAULT_HEADER5_DUTYCYCLE	( Header5DutyCycle_16 )
/** \} */

/** \name Header Pin 6 Key-Value pairs
    \{
*/
/** Defines the keys for header pin 6 */
typedef enum Header6Key
{
	Header6Key_INVALID     	= -1,
	Header6Key_FIRST       	= 0u<<KEY_STARTING_BIT,
	Header6Key_MODE        	= 0u<<KEY_STARTING_BIT,
	Header6Key_OUTPUT_VALUE = 1u<<KEY_STARTING_BIT,
	Header6Key_LAST
} Header6Key_t;

/** Defines the values for header pin 6 key Header6Key_MODE */
typedef enum Header6Mode
{
	Header6Mode_INVALID = -1,
	Header6Mode_FIRST   = 0u,
	Header6Mode_DISABLE = 0u,
	Header6Mode_INPUT  	= 1u,
	Header6Mode_OUTPUT  = 2u,
	Header6Mode_LAST
} Header6Mode_t;

/** Defines the values for the Header6 key Header6Key_SAMPLE_RATE */
typedef enum Header6OutputValue
{
	Header6OutputValue_INVALID = -1,
	Header6OutputValue_FIRST   = 0u,
	Header6OutputValue_LOW     = 0u,
	Header6OutputValue_HIGH    = 1u,
	Header6OutputValue_LAST
} Header6OutputValue_t;

/** Defines the default sample rate for header pin 6 key Header6Key_SAMPLE_RATE */
#define DEFAULT_HEADER6_OUTPUT_VALUE   ( Header6OutputValue_LOW )
/** \} */

/** \name Header 7 Input Key-Value pairs
    \{
*/

/** Defines the keys for the Analog7 header pin input */
typedef enum Analog7Key
{
	Analog7Key_INVALID     = -1,
	Analog7Key_FIRST       = 0u<<KEY_STARTING_BIT,
	Analog7Key_MODE        = 0u<<KEY_STARTING_BIT,
	Analog7Key_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	Analog7Key_REF_VOLT    = 2u<<KEY_STARTING_BIT,
	Analog7Key_LAST
} Analog7Key_t;

/** Defines the values for the Analog7 key Analog7Key_MODE */
typedef enum Analog7Mode
{
	Analog7Mode_INVALID = -1,
	Analog7Mode_FIRST   = 0u,
	Analog7Mode_DISABLE = 0u,
	Analog7Mode_ENABLE  = 1u,
	Analog7Mode_LAST
} Analog7Mode_t;

/** Defines the values for the Analog7 key Analog7Key_SAMPLE_RATE */
typedef enum Analog7SampleRate
{
	Analog7SampleRate_INVALID   = -1,
	Analog7SampleRate_FIRST     = 0u,
	Analog7SampleRate_1         = 0u,
	Analog7SampleRate_10        = 1u,
	Analog7SampleRate_50        = 2u,
	Analog7SampleRate_100       = 3u,
	Analog7SampleRate_200       = 4u,
	Analog7SampleRate_400       = 5u,
    Analog7SampleRate_800       = 6u,
	Analog7SampleRate_2400      = 7u,
	Analog7SampleRate_4800      = 8u,
	Analog7SampleRate_6000      = 9u,
	Analog7SampleRate_LAST
} Analog7SampleRate_t;

/** Defines the default sample rate for the Analog7 key Analog7Key_SAMPLE_RATE */
#define DEFAULT_ANALOG7_SAMPLE_RATE   ( Analog7SampleRate_100 )

/** Defines the analog voltage reference options for the Analog7 key Analog7Key_REF_VOLT*/
typedef enum Analog7RefVolt
{
    Analog7RefVolt_INVALID = -1,
    Analog7RefVolt_FIRST   = 0u,
    Analog7RefVolt_3v0     = 0u,
    Analog7RefVolt_3v3     = 1u,
    Analog7RefVolt_LAST
} Analog7RefVolt_t;

/** Defines the default reference voltage for the Analog7Key_REF_VOLT */
#define DEFAULT_ANALOG7_REF_VOLT    ( Analog7RefVolt_3v0 )

/** \} */

/** \name Header 8 Input Key-Value pairs
    \{
*/

/** Defines the keys for the Analog8 header pin input */
typedef enum Analog8Key
{
	Analog8Key_INVALID     = -1,
	Analog8Key_FIRST       = 0u<<KEY_STARTING_BIT,
	Analog8Key_MODE        = 0u<<KEY_STARTING_BIT,
	Analog8Key_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	Analog8Key_REF_VOLT    = 2u<<KEY_STARTING_BIT,
	Analog8Key_LAST
} Analog8Key_t;

/** Defines the values for the Analog8 key Analog8Key_MODE */
typedef enum Analog8Mode
{
	Analog8Mode_INVALID = -1,
	Analog8Mode_FIRST   = 0u,
	Analog8Mode_DISABLE = 0u,
	Analog8Mode_ENABLE  = 1u,
	Analog8Mode_LAST
} Analog8Mode_t;

/** Defines the values for the Analog8 key Analog8Key_SAMPLE_RATE */
typedef enum Analog8SampleRate
{
	Analog8SampleRate_INVALID   = -1,
	Analog8SampleRate_FIRST     = 0u,
	Analog8SampleRate_1         = 0u,
	Analog8SampleRate_10        = 1u,
	Analog8SampleRate_50        = 2u,
	Analog8SampleRate_100       = 3u,
	Analog8SampleRate_200       = 4u,
	Analog8SampleRate_400       = 5u,
    Analog8SampleRate_800       = 6u,
	Analog8SampleRate_2400      = 7u,
	Analog8SampleRate_4800      = 8u,
	Analog8SampleRate_6000      = 9u,
	Analog8SampleRate_LAST
} Analog8SampleRate_t;

/** Defines the default sample rate for the Analog8 key Analog8Key_SAMPLE_RATE */
#define DEFAULT_ANALOG8_SAMPLE_RATE   ( Analog8SampleRate_100 )


/** Defines the analog voltage reference options for the Analog8 key Analog8Key_REF_VOLT*/
typedef enum Analog8RefVolt
{
    Analog8RefVolt_INVALID = -1,
    Analog8RefVolt_FIRST   = 0u,
    Analog8RefVolt_3v0     = 0u,
    Analog8RefVolt_3v3     = 1u,
    Analog8RefVolt_LAST
} Analog8RefVolt_t;

/** Defines the default reference voltage for the Analog8Key_REF_VOLT */
#define DEFAULT_ANALOG8_REF_VOLT    ( Analog8RefVolt_3v0 )

/** \} */


/** \name Header 9 Input Key-Value pairs
    \{
*/

/** Defines the keys for the Analog9 header pin input */
typedef enum Analog9Key
{
	Analog9Key_INVALID     = -1,
	Analog9Key_FIRST       = 0u<<KEY_STARTING_BIT,
	Analog9Key_MODE        = 0u<<KEY_STARTING_BIT,
	Analog9Key_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	Analog9Key_REF_VOLT    = 2u<<KEY_STARTING_BIT,
	Analog9Key_LAST
} Analog9Key_t;

/** Defines the values for the Analog9 key Analog9Key_MODE */
typedef enum Analog9Mode
{
	Analog9Mode_INVALID = -1,
	Analog9Mode_FIRST   = 0u,
	Analog9Mode_DISABLE = 0u,
	Analog9Mode_ENABLE  = 1u,
	Analog9Mode_LAST
} Analog9Mode_t;

/** Defines the values for the Analog9 key Analog9Key_SAMPLE_RATE */
typedef enum Analog9SampleRate
{
	Analog9SampleRate_INVALID   = -1,
	Analog9SampleRate_FIRST     = 0u,
	Analog9SampleRate_1         = 0u,
	Analog9SampleRate_10        = 1u,
	Analog9SampleRate_50        = 2u,
	Analog9SampleRate_100       = 3u,
	Analog9SampleRate_200       = 4u,
	Analog9SampleRate_400       = 5u,
    Analog9SampleRate_800       = 6u,
	Analog9SampleRate_2400      = 7u,
	Analog9SampleRate_4800      = 8u,
	Analog9SampleRate_6000      = 9u,
	Analog9SampleRate_LAST
} Analog9SampleRate_t;

/** Defines the default sample rate for the Analog9 key Analog9Key_SAMPLE_RATE */
#define DEFAULT_ANALOG9_SAMPLE_RATE   ( Analog9SampleRate_100 )

/** Defines the analog voltage reference options for the Analog9 key Analog9Key_REF_VOLT*/
typedef enum Analog9RefVolt
{
    Analog9RefVolt_INVALID = -1,
    Analog9RefVolt_FIRST   = 0u,
    Analog9RefVolt_3v0     = 0u,
    Analog9RefVolt_3v3     = 1u,
    Analog9RefVolt_LAST
} Analog9RefVolt_t;

/** Defines the default reference voltage for the Analog9Key_REF_VOLT */
#define DEFAULT_ANALOG9_REF_VOLT    ( Analog9RefVolt_3v0 )

/** \} */

/** \name Header and Button Digital Input Key-Value pairs
    \{
*/

/** Defines the keys for the DigitalInput on the header and buttons */
typedef enum DigitalInputKey
{
	DigitalInputKey_INVALID     = -1,
	DigitalInputKey_FIRST       = 0u<<KEY_STARTING_BIT,
	DigitalInputKey_MODE        = 0u<<KEY_STARTING_BIT,
	DigitalInputKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
	DigitalInputKey_LAST
} DigitalInputKey_t;

/** Defines the values for the DigitalInput key DigitalInputKey_MODE */
typedef enum DigitalInputMode
{
	DigitalInputMode_INVALID = -1,
	DigitalInputMode_FIRST   = 0u,
	DigitalInputMode_DISABLE = 0u,
	DigitalInputMode_ENABLE  = 1u,
	DigitalInputMode_LAST
} DigitalInputMode_t;

/** Defines the values for the DigitalInput key DigitalInputKey_SAMPLE_RATE */
typedef enum DigitalInputSampleRate
{
	DigitalInputSampleRate_INVALID   = -1,
	DigitalInputSampleRate_FIRST     = 0u,
	DigitalInputSampleRate_1         = 0u,
	DigitalInputSampleRate_10        = 1u,
	DigitalInputSampleRate_20        = 2u,
	DigitalInputSampleRate_25       = 3u,
	DigitalInputSampleRate_50       = 4u,
	DigitalInputSampleRate_100       = 5u,
	DigitalInputSampleRate_LAST
} DigitalInputSampleRate_t;

/** Defines the default sample rate for the DigitalInput key DigitalInputKey_SAMPLE_RATE */
#define DEFAULT_DIGITALINPUT_SAMPLE_RATE   ( DigitalInputSampleRate_100 )
/** \} */


/** \name Dac Key-Value pairs
    \{
*/
/** Defines the keys for the encoder */
typedef enum DacKey
{
	DacKey_INVALID     = -1,
	DacKey_FIRST       = 0u<<KEY_STARTING_BIT,
	DacKey_MODE        = 0u<<KEY_STARTING_BIT,
	DacKey_AMPLITUDE   = 1u<<KEY_STARTING_BIT,
	DacKey_LAST
} DacKey_t;

/** Defines the values for the Dac key DacKey_MODE */
typedef enum DacMode
{
	DacMode_INVALID = -1,
	DacMode_FIRST    = 0u,
	DacMode_DISABLE  = 0u,
	DacMode_DC       = 1u,
	DacMode_SQUARE   = 2u,
	DacMode_TRIANGLE = 3u,
	DacMode_SAWTOOTH = 4u,
	DacMode_SINE     = 5u,
	DacMode_LAST
} DacMode_t;

/** Defines the values for the Dac key DacKey_AMPLITUDE */
typedef enum DacAmplitude
{
	DacAmplitude_INVALID = -1,
	DacAmplitude_FIRST   = 0u,
	DacAmplitude_00      = 0u,
	DacAmplitude_01      = 1u,
	DacAmplitude_02      = 2u,
	DacAmplitude_03      = 3u,
	DacAmplitude_04      = 4u,
	DacAmplitude_05      = 5u,
	DacAmplitude_06      = 6u,
	DacAmplitude_07      = 7u,
	DacAmplitude_08      = 8u,
	DacAmplitude_09      = 9u,
	DacAmplitude_10      = 10u,
	DacAmplitude_11      = 11u,
	DacAmplitude_12      = 12u,
	DacAmplitude_13      = 13u,
	DacAmplitude_14      = 14u,
	DacAmplitude_15      = 15u,
	DacAmplitude_16      = 16u,
	DacAmplitude_17      = 17u,
	DacAmplitude_18      = 18u,
	DacAmplitude_19      = 19u,
	DacAmplitude_20      = 20u,
	DacAmplitude_21      = 21u,
	DacAmplitude_22      = 22u,
	DacAmplitude_23      = 23u,
	DacAmplitude_24      = 24u,
	DacAmplitude_25      = 25u,
	DacAmplitude_26      = 26u,
	DacAmplitude_27      = 27u,
	DacAmplitude_28      = 28u,
	DacAmplitude_29      = 29u,
	DacAmplitude_30      = 30u,
	DacAmplitude_31      = 31u,
	DacAmplitude_LAST
} DacAmplitude_t;

/** Defines the default sample rate for the Dac key DacKey_SAMPLE_RATE */
#define DEFAULT_DAC_AMPLITUDE   ( DacAmplitude_17 )
/** \} */

/** \name Buzzer Key-Value pairs
    \{
*/
/** Defines the keys for the Buzzer */
typedef enum BuzzerKey
{
	BuzzerKey_INVALID        = -1,
	BuzzerKey_FIRST          = 0u<<KEY_STARTING_BIT,
	BuzzerKey_MODE           = 0u<<KEY_STARTING_BIT,
	BuzzerKey_LOW_FREQUENCY  = 1u<<KEY_STARTING_BIT,
	BuzzerKey_HIGH_FREQUENCY = 2u<<KEY_STARTING_BIT,
	BuzzerKey_LOWPITCH       = 3u<<KEY_STARTING_BIT,
	BuzzerKey_MIDPITCH       = 4u<<KEY_STARTING_BIT,
	BuzzerKey_HIGHPITCH      = 5u<<KEY_STARTING_BIT,
	BuzzerKey_DUTYCYCLE      = 6u<<KEY_STARTING_BIT,
	BuzzerKey_LAST
} BuzzerKey_t;

/** Defines the values for the Buzzer key BuzzerKey_MODE */
typedef enum BuzzerMode
{
	BuzzerMode_INVALID 	= -1,
	BuzzerMode_FIRST    = 0u,
	BuzzerMode_DISABLE  = 0u,
	BuzzerMode_ENABLE	= 1u,
	BuzzerMode_LAST
} BuzzerMode_t;

/** Defines the values for the Buzzer key BuzzerKey_LOW_FREQUENCY */
typedef enum BuzzerLowFrequency
{
	BuzzerLowFrequency_INVALID = -1,
	BuzzerLowFrequency_FIRST   = 0u,
	BuzzerLowFrequency_00      = 0u,
	BuzzerLowFrequency_01      = 1u,
	BuzzerLowFrequency_02      = 2u,
	BuzzerLowFrequency_03      = 3u,
	BuzzerLowFrequency_04      = 4u,
	BuzzerLowFrequency_05      = 5u,
	BuzzerLowFrequency_06      = 6u,
	BuzzerLowFrequency_07      = 7u,
	BuzzerLowFrequency_08      = 8u,
	BuzzerLowFrequency_09      = 9u,
	BuzzerLowFrequency_10      = 10u,
	BuzzerLowFrequency_11      = 11u,
	BuzzerLowFrequency_12      = 12u,
	BuzzerLowFrequency_13      = 13u,
	BuzzerLowFrequency_14      = 14u,
	BuzzerLowFrequency_15      = 15u,
	BuzzerLowFrequency_16      = 16u,
	BuzzerLowFrequency_17      = 17u,
	BuzzerLowFrequency_18      = 18u,
	BuzzerLowFrequency_19      = 19u,
	BuzzerLowFrequency_20      = 20u,
	BuzzerLowFrequency_21      = 21u,
	BuzzerLowFrequency_22      = 22u,
	BuzzerLowFrequency_23      = 23u,
	BuzzerLowFrequency_24      = 24u,
	BuzzerLowFrequency_25      = 25u,
	BuzzerLowFrequency_26      = 26u,
	BuzzerLowFrequency_27      = 27u,
	BuzzerLowFrequency_28      = 28u,
	BuzzerLowFrequency_29      = 29u,
	BuzzerLowFrequency_30      = 30u,
	BuzzerLowFrequency_31      = 31u,
	BuzzerLowFrequency_LAST
} BuzzerLowFrequency_t;

/** Defines the values for the Buzzer key BuzzerKey_HIGH_FREQUENCY */
typedef enum BuzzerHighFrequency
{
    BuzzerHighFrequency_INVALID = -1,
    BuzzerHighFrequency_FIRST   = 0u,
    BuzzerHighFrequency_00      = 0u,
    BuzzerHighFrequency_01      = 1u,
    BuzzerHighFrequency_02      = 2u,
    BuzzerHighFrequency_03      = 3u,
    BuzzerHighFrequency_04      = 4u,
    BuzzerHighFrequency_05      = 5u,
    BuzzerHighFrequency_06      = 6u,
    BuzzerHighFrequency_07      = 7u,
    BuzzerHighFrequency_08      = 8u,
    BuzzerHighFrequency_09      = 9u,
    BuzzerHighFrequency_10      = 10u,
    BuzzerHighFrequency_11      = 11u,
    BuzzerHighFrequency_12      = 12u,
    BuzzerHighFrequency_13      = 13u,
    BuzzerHighFrequency_14      = 14u,
    BuzzerHighFrequency_15      = 15u,
    BuzzerHighFrequency_16      = 16u,
    BuzzerHighFrequency_17      = 17u,
    BuzzerHighFrequency_18      = 18u,
    BuzzerHighFrequency_19      = 19u,
    BuzzerHighFrequency_20      = 20u,
    BuzzerHighFrequency_21      = 21u,
    BuzzerHighFrequency_22      = 22u,
    BuzzerHighFrequency_23      = 23u,
    BuzzerHighFrequency_24      = 24u,
    BuzzerHighFrequency_25      = 25u,
    BuzzerHighFrequency_26      = 26u,
    BuzzerHighFrequency_27      = 27u,
    BuzzerHighFrequency_28      = 28u,
    BuzzerHighFrequency_29      = 29u,
    BuzzerHighFrequency_30      = 30u,
    BuzzerHighFrequency_31      = 31u,
    BuzzerHighFrequency_LAST
} BuzzerHighFrequency_t;

/** Defines the values for the Buzzer key BuzzerKey_LOWPITCHES */
typedef enum BuzzerLowPitch
{
	BuzzerLowPitch_INVALID = -1,
	BuzzerLowPitch_FIRST   = 0u,
	BuzzerLowPitch_00      = 0u,
	BuzzerLowPitch_01      = 1u,
	BuzzerLowPitch_02      = 2u,
	BuzzerLowPitch_03      = 3u,
	BuzzerLowPitch_04      = 4u,
	BuzzerLowPitch_05      = 5u,
	BuzzerLowPitch_06      = 6u,
	BuzzerLowPitch_07      = 7u,
	BuzzerLowPitch_08      = 8u,
	BuzzerLowPitch_09      = 9u,
	BuzzerLowPitch_10      = 10u,
	BuzzerLowPitch_11      = 11u,
	BuzzerLowPitch_12      = 12u,
	BuzzerLowPitch_13      = 13u,
	BuzzerLowPitch_14      = 14u,
	BuzzerLowPitch_15      = 15u,
	BuzzerLowPitch_16      = 16u,
	BuzzerLowPitch_17      = 17u,
	BuzzerLowPitch_18      = 18u,
	BuzzerLowPitch_19      = 19u,
	BuzzerLowPitch_20      = 20u,
	BuzzerLowPitch_21      = 21u,
	BuzzerLowPitch_22      = 22u,
	BuzzerLowPitch_23      = 23u,
	BuzzerLowPitch_24      = 24u,
	BuzzerLowPitch_25      = 25u,
	BuzzerLowPitch_26      = 26u,
	BuzzerLowPitch_27      = 27u,
	BuzzerLowPitch_28      = 28u,
	BuzzerLowPitch_29      = 29u,
	BuzzerLowPitch_30      = 30u,
	BuzzerLowPitch_31      = 31u,
	BuzzerLowPitch_LAST
} BuzzerLowPitch_t;

/** Defines the values for the Buzzer key BuzzerKey_MIDPITCHES */
typedef enum BuzzerMidPitch
{
    BuzzerMidPitch_INVALID = -1,
    BuzzerMidPitch_FIRST   = 0u,
    BuzzerMidPitch_00      = 0u,
    BuzzerMidPitch_01      = 1u,
    BuzzerMidPitch_02      = 2u,
    BuzzerMidPitch_03      = 3u,
    BuzzerMidPitch_04      = 4u,
    BuzzerMidPitch_05      = 5u,
    BuzzerMidPitch_06      = 6u,
    BuzzerMidPitch_07      = 7u,
    BuzzerMidPitch_08      = 8u,
    BuzzerMidPitch_09      = 9u,
    BuzzerMidPitch_10      = 10u,
    BuzzerMidPitch_11      = 11u,
    BuzzerMidPitch_12      = 12u,
    BuzzerMidPitch_13      = 13u,
    BuzzerMidPitch_14      = 14u,
    BuzzerMidPitch_15      = 15u,
    BuzzerMidPitch_16      = 16u,
    BuzzerMidPitch_17      = 17u,
    BuzzerMidPitch_18      = 18u,
    BuzzerMidPitch_19      = 19u,
    BuzzerMidPitch_20      = 20u,
    BuzzerMidPitch_21      = 21u,
    BuzzerMidPitch_22      = 22u,
    BuzzerMidPitch_23      = 23u,
    BuzzerMidPitch_24      = 24u,
    BuzzerMidPitch_25      = 25u,
    BuzzerMidPitch_26      = 26u,
    BuzzerMidPitch_27      = 27u,
    BuzzerMidPitch_28      = 28u,
    BuzzerMidPitch_29      = 29u,
    BuzzerMidPitch_30      = 30u,
    BuzzerMidPitch_31      = 31u,
    BuzzerMidPitch_LAST
} BuzzerMidPitch_t;

/** Defines the values for the Buzzer key BuzzerKey_HIGHPITCHES */
typedef enum BuzzerHighPitch
{
	BuzzerHighPitch_INVALID = -1,
	BuzzerHighPitch_FIRST   = 0u,
	BuzzerHighPitch_00      = 0u,
	BuzzerHighPitch_01      = 1u,
	BuzzerHighPitch_02      = 2u,
	BuzzerHighPitch_03      = 3u,
	BuzzerHighPitch_04      = 4u,
	BuzzerHighPitch_05      = 5u,
	BuzzerHighPitch_06      = 6u,
	BuzzerHighPitch_07      = 7u,
	BuzzerHighPitch_08      = 8u,
	BuzzerHighPitch_09      = 9u,
	BuzzerHighPitch_10      = 10u,
	BuzzerHighPitch_11      = 11u,
	BuzzerHighPitch_12      = 12u,
	BuzzerHighPitch_13      = 13u,
	BuzzerHighPitch_14      = 14u,
	BuzzerHighPitch_15      = 15u,
	BuzzerHighPitch_16      = 16u,
	BuzzerHighPitch_17      = 17u,
    BuzzerHighPitch_18      = 18u,
    BuzzerHighPitch_19      = 19u,
    BuzzerHighPitch_20      = 20u,
    BuzzerHighPitch_21      = 21u,
    BuzzerHighPitch_22      = 22u,
    BuzzerHighPitch_23      = 23u,
    BuzzerHighPitch_24      = 24u,
    BuzzerHighPitch_25      = 25u,
    BuzzerHighPitch_26      = 26u,
    BuzzerHighPitch_27      = 27u,
    BuzzerHighPitch_28      = 28u,
    BuzzerHighPitch_29      = 29u,
    BuzzerHighPitch_30      = 30u,
    BuzzerHighPitch_31      = 31u,
	BuzzerHighPitch_LAST
} BuzzerHighPitch_t;

/** Defines the values for the Buzzer key BuzzerKey_DUTYCYCLE */
typedef enum BuzzerDutyCycle
{
	BuzzerDutyCycle_INVALID = -1,
	BuzzerDutyCycle_FIRST   = 0u,
	BuzzerDutyCycle_00      = 0u,
	BuzzerDutyCycle_01      = 1u,
	BuzzerDutyCycle_02      = 2u,
	BuzzerDutyCycle_03      = 3u,
	BuzzerDutyCycle_04      = 4u,
	BuzzerDutyCycle_05      = 5u,
	BuzzerDutyCycle_06      = 6u,
	BuzzerDutyCycle_07      = 7u,
	BuzzerDutyCycle_08      = 8u,
	BuzzerDutyCycle_09      = 9u,
	BuzzerDutyCycle_10      = 10u,
	BuzzerDutyCycle_11      = 11u,
	BuzzerDutyCycle_12      = 12u,
	BuzzerDutyCycle_13      = 13u,
	BuzzerDutyCycle_14      = 14u,
	BuzzerDutyCycle_15      = 15u,
	BuzzerDutyCycle_16      = 16u,
	BuzzerDutyCycle_17      = 17u,
	BuzzerDutyCycle_18      = 18u,
	BuzzerDutyCycle_19      = 19u,
	BuzzerDutyCycle_20      = 20u,
	BuzzerDutyCycle_21      = 21u,
	BuzzerDutyCycle_22      = 22u,
	BuzzerDutyCycle_23      = 23u,
	BuzzerDutyCycle_24      = 24u,
	BuzzerDutyCycle_25      = 25u,
	BuzzerDutyCycle_26      = 26u,
	BuzzerDutyCycle_27      = 27u,
	BuzzerDutyCycle_28      = 28u,
	BuzzerDutyCycle_29      = 29u,
	BuzzerDutyCycle_30      = 30u,
	BuzzerDutyCycle_31      = 31u,
	BuzzerDutyCycle_LAST
} BuzzerDutyCycle_t;

/** Defines the default frequency for the buzzer key BuzzerKey_LOW_FREQUENCY */
#define DEFAULT_BUZZER_LOW_FREQUENCY	( BuzzerLowFrequency_09 )

/** Defines the default frequency for the buzzer key BuzzerKey_HIGH_FREQUENCY */
#define DEFAULT_BUZZER_HIGH_FREQUENCY    ( BuzzerHighFrequency_09 )

/** Defines the default low pitch for the buzzer key BuzzerKey_LOWPITCH */
#define DEFAULT_BUZZER_LOWPITCH		( BuzzerLowPitch_20)

/** Defines the default low pitch for the buzzer key BuzzerKey_MEDPITCH */
#define DEFAULT_BUZZER_MIDPITCH     ( BuzzerMidPitch_12)

/** Defines the default high pitch for the buzzer key BuzzerKey_HIGHPITCH */
#define DEFAULT_BUZZER_HIGHPITCH	( BuzzerHighPitch_04 )

/** Defines the default duty cycle for the buzzer key BuzzerKey_DUTYCYCLE */
#define DEFAULT_BUZZER_DUTYCYCLE	( BuzzerDutyCycle_16 )
/** \} */


/** \name Thermometer Key-Value pairs
    \{
*/
/** Defines the keys for the thermometer */
typedef enum ThermometerKey
{
    ThermometerKey_INVALID     = -1,
    ThermometerKey_FIRST       = 0u<<KEY_STARTING_BIT,
    ThermometerKey_MODE        = 0u<<KEY_STARTING_BIT,
    ThermometerKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
    ThermometerKey_OVERSAMPLE  = 2u<<KEY_STARTING_BIT,
    ThermometerKey_LAST
} ThermometerKey_t;

/** Defines the values for the Thermometer key ThermometerKey_MODE */
typedef enum ThermometerMode
{
    ThermometerMode_INVALID = -1,
    ThermometerMode_FIRST   = 0u,
    ThermometerMode_DISABLE = 0u,
    ThermometerMode_ENABLE  = 1u,
    ThermometerMode_LAST
} ThermometerMode_t;

/** Defines the values for the Thermometer key ThermometerKey_SAMPLE_RATE */
typedef enum ThermometerSampleRate
{
    ThermometerSampleRate_INVALID  = -1,
    ThermometerSampleRate_FIRST    = 0u,
    ThermometerSampleRate_1        = 0u,
    ThermometerSampleRate_10       = 1u,
    ThermometerSampleRate_50       = 2u,
    ThermometerSampleRate_100      = 3u,
    ThermometerSampleRate_200      = 4u,
    ThermometerSampleRate_400      = 5u,
    ThermometerSampleRate_LAST
} ThermometerSampleRate_t;

/** Defines the default sample rate for the thermometer key ThermometerKey_SAMPLE_RATE */
#define DEFAULT_THERMOMETER_SAMPLE_RATE   ( ThermometerSampleRate_50 )

/** Defines the values for the Thermometer key ThermometerKey_OVERSAMPLE */
typedef enum ThermometerOversample
{
    ThermometerOversample_INVALID = -1,
    ThermometerOversample_FIRST   = 0u,
    ThermometerOversample_ENABLE  = 0u,
    ThermometerOversample_DISABLE = 1u,
    ThermometerOversample_LAST
} ThermometerOversample_t;

/** Defines the default oversampling mode for the thermometer key ThermometerKey_OVERSAMPLE */
#define DEFAULT_THERMOMETER_OVERSAMPLE   ( ThermometerOversample_ENABLE )
/** \} */

/** \name ECG6CHAN Key-Value pairs
    \{
*/

/** Defines the keys for the Ecg6Chan */
typedef enum Ecg6ChanKey
{
    Ecg6ChanKey_INVALID     = -1,
    Ecg6ChanKey_FIRST       = 0u<<KEY_STARTING_BIT,
    Ecg6ChanKey_MODE        = 0u<<KEY_STARTING_BIT,
    Ecg6ChanKey_SAMPLE_RATE = 1u<<KEY_STARTING_BIT,
    Ecg6ChanKey_REF_VOLT    = 2u<<KEY_STARTING_BIT,
    Ecg6ChanKey_LAST
} Ecg6ChanKey_t;

/** Defines the values for the Ecg6Chan key Ecg6ChanKey_MODE */
typedef enum Ecg6ChanMode
{
    Ecg6ChanMode_INVALID = -1,
    Ecg6ChanMode_FIRST   = 0u,
    Ecg6ChanMode_DISABLE = 0u,
    Ecg6ChanMode_ENABLE  = 1u,
    Ecg6ChanMode_LAST
} Ecg6ChanMode_t;

/** Defines the values for the Ecg6Chan key Ecg6ChanKey_SAMPLE_RATE */
typedef enum Ecg6ChanSampleRate
{
    Ecg6ChanSampleRate_INVALID   = -1,
    Ecg6ChanSampleRate_FIRST     = 0u,
    Ecg6ChanSampleRate_1         = 0u,
    Ecg6ChanSampleRate_10        = 1u,
    Ecg6ChanSampleRate_50        = 2u,
    Ecg6ChanSampleRate_100       = 3u,
    Ecg6ChanSampleRate_200       = 4u,
    Ecg6ChanSampleRate_400       = 5u,
    Ecg6ChanSampleRate_800       = 6u,
    Ecg6ChanSampleRate_LAST
} Ecg6ChanSampleRate_t;

/** Defines the default sample rate for the Ecg6Chan key Ecg6ChanKey_SAMPLE_RATE */
#define DEFAULT_ECG6CHAN_SAMPLE_RATE   ( Ecg6ChanSampleRate_400 )

/** Defines the analog voltage reference options for the Ecg6 key Ecg6Key_REF_VOLT*/
typedef enum Ecg6RefVolt
{
    Ecg6RefVolt_INVALID = -1,
    Ecg6RefVolt_FIRST   = 0u,
    Ecg6RefVolt_3v0     = 0u,
    Ecg6RefVolt_3v3     = 1u,
    Ecg6RefVolt_LAST
} Ecg6RefVolt_t;

/** Defines the default reference voltage for the Ecg6Key_REF_VOLT */
#define DEFAULT_ECG6_REF_VOLT    ( Ecg6RefVolt_3v0 )

/** \} */


/** Defines a type for the key value pair */
typedef uint8_t KeyValue_t;

/** Defines a type for the key of a key value pair */
typedef uint8_t Key_t;

/** Defines a type for the value of a key value pair */
typedef uint8_t Value_t;

/** Defines the maximum number of Key's possible for a single sensor */
#define MAX_KEYS_POSSIBLE    ( 8u )
/** Defines the maximum number of values possible for a single sensor */
#define MAX_VALUES_POSSIBLE  ( 32u )

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void KeyValue_Separate(KeyValue_t keyValue, Key_t *key, Value_t *value);
KeyValue_t KeyValue_Combine(Key_t key, Value_t value);

#endif /* KEYVALUE_H_ */
