/***************************************************************************//**
    \file AccelerometerControl.c
    \brief Controls the accelerometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the accelerometer driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/AccelerometerControl.c$
    $Revision: 7$
    $Date: Monday, August 20, 2012 9:20:19 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "AccelerometerControl.h"
#include <Accelerometer.h>
#include <EventQueue.h>
#include <DataBuffer.h>
#include <SensorId.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** tracks if the controller has been initialized */
static bool _initialized = false;

/** Stores the settings for the accelerometer */
static AccConfig_t _accConfig =
{
		AccOdr_100,
		AccResolution_14bit,
		AccFullScaleRange_4g,
		AccOverSample_HighRes
};
/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _AccDataReadyIsr(void);

/***************************************************************************//**
    Initializes the accelerometer controller.
*//****************************************************************************/
void AccControl_Init(void)
{
	Accelerometer_Init();
	Accelerometer_RegisterIsr(&_AccDataReadyIsr);
	_initialized = true;
}

/***************************************************************************//**
    Sets a configuration value to the accelerometer

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void AccControl_SetConfig(uint8_t keyValue)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(keyValue, &key, &value);

	switch(key)
	{
		case AccelerometerKey_MODE:
			switch(value)
			{
				case AccelerometerMode_DISABLE:
					Accelerometer_Stop();
					break;

				case AccelerometerMode_ENABLE:
					Accelerometer_Start();
					break;

				default:
					DEBUG("unhandled acc mode");
			}
			break;

		case AccelerometerKey_SAMPLE_RATE:
			switch(value)
			{
				case AccelerometerSampleRate_1_56:
					_accConfig.odr = AccOdr_1_56;
					break ;

				case AccelerometerSampleRate_6_25:
					_accConfig.odr = AccOdr_6_25;
					break ;

				case AccelerometerSampleRate_12_5:
					_accConfig.odr = AccOdr_12_5;
					break ;

				case AccelerometerSampleRate_50:
					_accConfig.odr = AccOdr_50;
					break ;

				case AccelerometerSampleRate_100:
					_accConfig.odr = AccOdr_100;
					break ;

				case AccelerometerSampleRate_200:
					_accConfig.odr = AccOdr_200;
					break ;

				case AccelerometerSampleRate_400:
					_accConfig.odr = AccOdr_400;
					break ;

				case AccelerometerSampleRate_800:
					_accConfig.odr = AccOdr_800;
					break ;

				default:
					DEBUG("unhandled acc sr");
					break;
			}
			Accelerometer_Config(&_accConfig);
			break;

		case AccelerometerKey_RESOLUTION:
			switch(value)
			{
			case AccelerometerResolution_2g:
				_accConfig.range = AccFullScaleRange_2g;
				break;

			case AccelerometerResolution_4g:
				_accConfig.range = AccFullScaleRange_4g;
				break;

			case AccelerometerResolution_8g:
				_accConfig.range = AccFullScaleRange_8g;
				break;

			default:
				DEBUG("unhandled acc res");
				break;
			}
			Accelerometer_Config(&_accConfig);
			break;

		case AccelerometerKey_OVER_SAMPLE:
			switch(value)
			{
			case AccelerometerOverSample_HI_RES:
				_accConfig.overSampleMode = AccOverSample_HighRes;
				break;

			case AccelerometerOverSample_LNLP:
				_accConfig.overSampleMode = AccOverSample_LNLP;
				break;

			case AccelerometerOverSample_LO_PWR:
				_accConfig.overSampleMode = AccOverSample_LowPower;
				break;

			case AccelerometerOverSample_NORMAL:
				_accConfig.overSampleMode = AccOverSample_Normal;
				break;

			default:
				DEBUG("unhandled acc os");
				break;
			}
			Accelerometer_Config(&_accConfig);
			break;

		default:
			DEBUG("accel unhandled key");
			break;
	}
}

/***************************************************************************//**
    Gets a reading from the accelerometer.
*//****************************************************************************/
void AccControl_ReadAcc(void)
{
#define MAX_DATA_BYTES	(6u)

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

#if 1 /* use data buffer for data */
	static uint8_t accelerometerSample[MAX_DATA_BYTES];
	uint16_t sampleSize = 0u;

	Accelerometer_ReadSampleArray(MAX_DATA_BYTES, &sampleSize, accelerometerSample);

	if (sampleSize > 0)
	{
		DataBuffer_AddToSubarray8Bit(SensorId_ACCELEROMETER, sampleSize, accelerometerSample);
	}

//	ASSERT(sampleSuccess, ErrorLedMode_SOFTWARE);
#else /* use serial print for data */
	static AccSample_t sample;
	bool sampleSuccess = false;

	sampleSuccess = Accelerometer_ReadSample(&sample);
	if (sampleSuccess)
	{
		Serial_SendString3Numbers("acc:", sample.xData, sample.yData, sample.zData);
	}
#endif
}

/***************************************************************************//**
    The data ready ISR handles the accelerometer data ready interrupt and will
    add an event to the event queue to notify the system to read the accelerometer.
*//****************************************************************************/
static void _AccDataReadyIsr(void)
{
	EventQueue_Add(Event_ACCELEROMETER_DATA_READY, EventPriority_NORMAL);
}
