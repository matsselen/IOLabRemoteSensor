/***************************************************************************//**
    \file ThermometerControl.c
    \brief Controls the thermometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the thermometer driver and controls the configuration
                   for it.  This uses the thermometer from the Gyroscope IC.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/ThermometerControl.c$
    $Revision: 3$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

//#define THERMOMETER_USE_GYRO
//#define THERMOMETER_USE_MAG
#define THERMOMETER_USE_ADC

//#define THERMOMETER_PRINT_TO_SERIAL

#include <ProjectTypes.h>
#include "ThermometerControl.h"
#if defined(THERMOMETER_USE_GYRO)
#include <Gyroscope.h>
#elif defined(THERMOMETER_USE_MAG)
#include <Magnetometer.h>
#elif defined(THERMOMETER_USE_ADC)
#include <AdcManager.h>
#endif
#include <TimerTick.h>
#include <DataBuffer.h>
#include <SensorId.h>
#include <KeyValue.h>
#include <Assert.h>

#ifdef THERMOMETER_PRINT_TO_SERIAL
#include <Serial.h>
#include <Debug.h>
#endif

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** defines how many milliseconds to wait between samples for a 50 Hz rate */
#define TIMEOUT_50HZ          ( 20u )

/** defines how many milliseconds to wait between samples for a 10 Hz rate */
#define TIMEOUT_10HZ          ( 100u )

/** defines how many milliseconds to wait between samples for a 1 Hz rate */
#define TIMEOUT_1HZ          ( 1000u )

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** tracks if the controller has been initialized */
static bool _initialized = false;

#if defined (THERMOMETER_USE_GYRO) || defined (THERMOMETER_USE_MAG)
/** Handle to the timer tick timeout for the barometer */
static TickTimeoutHandle_t _tickHandle;
/** tracks if the tick handle is in use to make sure not to use an invalid tick handle */
static bool _tickInUse = false;
#endif

/** Stores the thermometer sample rate setting */
static ThermometerSampleRate_t _sampleRate = DEFAULT_THERMOMETER_SAMPLE_RATE;
/** Stores the thermometer oversample setting */
static ThermometerOversample_t _oversample = DEFAULT_THERMOMETER_OVERSAMPLE;

/** Stores the oversampled thermometer data */
static uint32_t _data = 0u;
static uint16_t _numberAccumulated;
static uint16_t _numberToAccumulate;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
#if defined(THERMOMETER_USE_ADC)
static void _thermometerDataCallBack(uint16_t data);
#elif defined(THERMOMETER_USE_GYRO) || defined(THERMOMETER_USE_MAG)
static void _ReadTemp(void);
#endif
static __inline void _SetOverampling(void);

/***************************************************************************//**
    Initializes the thermometer controller.
*//****************************************************************************/
void ThermometerControl_Init(void)
{
    /* no device to init, since this uses the Gyroscope IC for the temperature, the
       gyroscope should be initialized in its controller */
	_initialized = true;
}

/***************************************************************************//**
    Sets a configuration value to the Thermometer

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void ThermometerControl_SetConfig(uint8_t keyValue)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(keyValue, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case ThermometerKey_MODE:
			switch(value)
			{
				case ThermometerMode_DISABLE:
#if defined(THERMOMETER_USE_GYRO)
				    Gyroscope_Stop();
#elif defined(THERMOMETER_USE_MAG)
				    Magnetometer_Stop();
#elif defined(THERMOMETER_USE_ADC)
				    AdcManager_DisableChannel(AdcManChannel_TEMPERATURE);
#endif
#ifndef THERMOMETER_USE_ADC
					TimerTick_CancelTimeout(_tickHandle);
					_tickInUse = false;
#endif
					break;

				case ThermometerMode_ENABLE:
#if defined(THERMOMETER_USE_GYRO)
				    Gyroscope_Start();
#elif defined (THERMOMETER_USE_MAG)
				    Magnetometer_Start();
#endif
					switch(_sampleRate)
					{
						case ThermometerSampleRate_1:
#ifndef THERMOMETER_USE_ADC
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_1HZ, &_ReadTemp);
							_tickInUse = true;
#else
							if (_oversample == ThermometerOversample_ENABLE)
							{
							    AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
							}
							else
							{
							    AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
							}
							_SetOverampling();
#endif
							break;
						case ThermometerSampleRate_10:
#ifndef THERMOMETER_USE_ADC
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_10HZ, &_ReadTemp);
							_tickInUse = true;
#else
                            if (_oversample == ThermometerOversample_ENABLE)
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            else
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            _SetOverampling();
#endif
							break;
						case ThermometerSampleRate_50:
#ifndef THERMOMETER_USE_ADC
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_50HZ, &_ReadTemp);
							_tickInUse = true;
#else
                            if (_oversample == ThermometerOversample_ENABLE)
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            else
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            _SetOverampling();
#endif
							break;

                        case ThermometerSampleRate_100:
#ifndef THERMOMETER_USE_ADC
                            ASSERT(true, ErrorLedMode_SOFTWARE);
#else
                            if (_oversample == ThermometerOversample_ENABLE)
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            else
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            _SetOverampling();
#endif
                            break;

                        case ThermometerSampleRate_200:
#ifndef THERMOMETER_USE_ADC
                            ASSERT(true, ErrorLedMode_SOFTWARE);
#else
                            if (_oversample == ThermometerOversample_ENABLE)
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            else
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            _SetOverampling();
#endif
                            break;

                        case ThermometerSampleRate_400:
#ifndef THERMOMETER_USE_ADC
                            ASSERT(true, ErrorLedMode_SOFTWARE);
#else
                            if (_oversample == ThermometerOversample_ENABLE)
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            else
                            {
                                AdcManager_EnableChannel(AdcManChannel_TEMPERATURE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_thermometerDataCallBack);
                            }
                            _SetOverampling();
#endif
                            break;

						default:
							ASSERT(false, ErrorLedMode_SOFTWARE);
					}
					break;

				default:
					ASSERT(false, ErrorLedMode_SOFTWARE);
					break;
			}
			break;

		case ThermometerKey_SAMPLE_RATE:
			if (((int16_t)(value) >= ThermometerSampleRate_FIRST) && (value < ThermometerSampleRate_LAST))
			{
				_sampleRate = ((ThermometerSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		case ThermometerKey_OVERSAMPLE:
		    if (((int16_t)(value) >= ThermometerOversample_FIRST) && (value < ThermometerOversample_LAST))
		    {
		        _oversample = (ThermometerOversample_t)value;
		    }
		    else
		    {
		        ASSERT(false, ErrorLedMode_SOFTWARE);
		    }
		    break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}


/***************************************************************************//**
    Reads the calibration values from the thermometer.

    \param[in] maxCalibrationLength - the maximum number of bytes allocated for calibration[]
    \param[out] actualCalibrationLength - the number of bytes of calibration data placed in calibration[]
    \param[out] calibration[] - the calibration data from the battery.

*//****************************************************************************/
void ThermometerControl_ReadCalibration(
        uint16_t  maxCalibrationLength,
        uint16_t *actualCalibrationLength,
        uint8_t   calibration[])
{
#ifdef THERMOMETER_USE_ADC
    /** Defines how many bytes are needed to store the calibration data */
#define NUM_TEMP_CALIBRATION_BYTES   ( 4u )
    uint16_t cal85C;
    uint16_t cal30C;


    ASSERT((calibration != NULL) && (actualCalibrationLength != NULL) &&
            (maxCalibrationLength >= NUM_TEMP_CALIBRATION_BYTES), ErrorLedMode_SOFTWARE);

    Adc_GetTempCalib(&cal85C, &cal30C);

    calibration[0u] = (uint8_t)(cal85C >> 8u);
    calibration[1u] = (uint8_t)(cal85C);
    calibration[2u] = (uint8_t)(cal30C >> 8u);
    calibration[3u] = (uint8_t)(cal30C);

    *actualCalibrationLength = NUM_TEMP_CALIBRATION_BYTES;
#else
    *actualactualCalibrationLength = 0u;
#endif
}

#ifndef THERMOMETER_USE_ADC
/***************************************************************************//**
    Gets a reading from the Thermometer.
*//****************************************************************************/
static void _ReadTemp(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    uint8_t temperature;
#if defined(THERMOMETER_USE_GYRO)
    temperature = (uint8_t)Gyroscope_ReadTemperature();
#elif defined(THERMOMETER_USE_MAG)
    temperature = (uint8_t)Magnetometer_ReadTemp();
#endif

	DataBuffer_AddToSubarray8Bit(SensorId_THERMOMETER, 1u, &temperature);
#ifdef THERMOMETER_PRINT_TO_SERIAL /* use serial print for data */
    #ifndef ENABLE_PRINTF
    Serial_SendStringNumber("temp:", temperature);
    #else
    Serial_Printf("temp:%x", temperature);
    #endif
#endif
}
#else
/***************************************************************************//**
    Takes the analog input data from internal thermometer and stores it in the
    proper location. This function is designed to be called every time there is
    data for the thermometer.

    \param[in] data - The sample from header pin 7 that should be stored in
                      the proper location.
*//****************************************************************************/
static void _thermometerDataCallBack(uint16_t data)
{
    static uint16_t dataCopy[2u];

    _data += data;
    if (++_numberAccumulated >= _numberToAccumulate)
    {
        dataCopy[0u] = (uint16_t)(_data>>16u);
        dataCopy[1u] = (uint16_t)(_data);
        DataBuffer_AddToSubarray16Bit(SensorId_THERMOMETER, 2u, dataCopy);

#ifdef THERMOMETER_PRINT_TO_SERIAL /* use serial print for data */
        Serial_SendStringNumberU32("Otemp:", _data);
#endif

        /* reset counters and data */
        _numberAccumulated = 0u;
        _data = 0u;
    }

#ifdef THERMOMETER_PRINT_TO_SERIAL /* use serial print for data */
    #ifndef ENABLE_PRINTF
    Serial_SendStringNumber("temp:", data);
    #else
    Serial_Printf("temp:%x", data);
    #endif
#endif
}
#endif

/***************************************************************************//**
    Sets all the registers appropriately for the current oversampling or not-
    oversampling settings.
*//****************************************************************************/
static __inline void _SetOverampling(void)
{
    if (_oversample == ThermometerOversample_ENABLE)
    {
        switch(_sampleRate)
        {
            case ThermometerSampleRate_1:
                _numberAccumulated = 0u;
                _numberToAccumulate = 400u;
                break;

            case ThermometerSampleRate_10:
                _numberAccumulated = 0u;
                _numberToAccumulate = 40u;
                break;

            case ThermometerSampleRate_50:
                _numberAccumulated = 0u;
                _numberToAccumulate = 8u;
                break;

            case ThermometerSampleRate_100:
                _numberAccumulated = 0u;
                _numberToAccumulate = 4u;
                break;

            case ThermometerSampleRate_200:
                _numberAccumulated = 0u;
                _numberToAccumulate = 2u;
                break;

            case ThermometerSampleRate_400:
                _numberAccumulated = 0u;
                _numberToAccumulate = 1u;
                break;

            default:
                break;
        }
    }
    else
    {
        _numberAccumulated = 0u;
        _numberToAccumulate = 1u;
    }
}
