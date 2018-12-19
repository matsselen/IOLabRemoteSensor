/***************************************************************************//**
    \file GyroscopeControl.c
    \brief Controls the gyroscope driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the gyroscope driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/GyroscopeControl.c$
    $Revision: 5$
    $Date: Tuesday, September 11, 2012 5:22:11 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "GyroscopeControl.h"
#include <Gyroscope.h>
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

/** Stores the settings for the gyroscope */
static GyroConfig_t _gyroConfig =
{
	GyroOdr_95,
	GyroResolution_250,
};

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _GyroDataReadyIsr(void);

/***************************************************************************//**
    Initializes the accelerometer controller.
*//****************************************************************************/
void GyroControl_Init(void)
{
	Gyroscope_Init();
	Gyroscope_RegisterIsr(&_GyroDataReadyIsr);
	_initialized = true;
}

/***************************************************************************//**
    Sets a configuration value to the accelerometer

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void GyroControl_SetConfig(uint8_t keyValue)
{

	Key_t key;
	Value_t value;

	KeyValue_Separate(keyValue, &key, &value);

	switch(key)
	{
		case GyroscopeKey_MODE:
			switch(value)
			{
				case GyroscopeMode_DISABLE:
					Gyroscope_Stop();
					break;

				case GyroscopeMode_ENABLE:
					Gyroscope_Start();
					break;

				default:
					DEBUG("unhandled gyro mode");
			}
			break;

		case GyroscopeKey_SAMPLE_RATE:
			switch(value)
			{
				case GyroscopeSampleRate_95:
					_gyroConfig.odr = GyroOdr_95;
					break ;

				case GyroscopeSampleRate_190:
					_gyroConfig.odr = GyroOdr_190;
					break ;

				case GyroscopeSampleRate_380:
					_gyroConfig.odr = GyroOdr_380;
					break ;

				case GyroscopeSampleRate_760:
					_gyroConfig.odr = GyroOdr_760;
					break ;

				default:
					DEBUG("unhandled gyro sample rate");
			}

			Gyroscope_Config(&_gyroConfig);

			break;

		case GyroscopeKey_RESOLUTION:
			switch(value)
			{
				case GyroscopeResolution_250:
					_gyroConfig.resolution = GyroResolution_250;
					break ;

				case GyroscopeResolution_500:
					_gyroConfig.resolution = GyroResolution_500;
					break ;

				case GyroscopeResolution_2000:
					_gyroConfig.resolution = GyroResolution_2000;
					break ;

				default:
					DEBUG("unhandled gyro resolution");
			}

			Gyroscope_Config(&_gyroConfig);

			break;


		default:
			DEBUG("unhandled gyro key");
			break;
	}

}

/***************************************************************************//**
    Gets a reading from the accelerometer.
*//****************************************************************************/
void GyroControl_ReadGyro(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

#if 1 /* use data buffer for data */
	static uint8_t gyroscopeSample[6u];
	uint16_t sampleSize = 0u;

	Gyroscope_ReadSampleArray(6u, &sampleSize, gyroscopeSample);

	if (sampleSize > 0)
	{
		DataBuffer_AddToSubarray8Bit(SensorId_GYROSCOPE, sampleSize, gyroscopeSample);
	}

#else /* use serial print for data */
	bool sampleSuccess = false;
	static GyroSample_t sample;
	sampleSuccess = Gyroscope_ReadSample(&sample);
	if (sampleSuccess)
	{
		Serial_SendString3Numbers("gyro:", sample.xData, sample.yData, sample.zData);
	}
	else
	{
		Serial_SendString("gyro fail!");
	}
#endif
}

/***************************************************************************//**
    The data ready ISR handles the gyroscope data ready interrupt and will
    add an event to the event queue to notify the system to read the gyroscope.
*//****************************************************************************/
static void _GyroDataReadyIsr(void)
{
	EventQueue_Add(Event_GYROSCOPE_DATA_READY, EventPriority_NORMAL);
}
