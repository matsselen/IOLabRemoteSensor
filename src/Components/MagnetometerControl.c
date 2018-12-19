/***************************************************************************//**
    \file MagnetometerControl.c
    \brief Magnetometer control module.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Interfaces to the magnetometer driver to control and configure
                   the magnetometer with the proper settings and handle the data
                   coming from the magnetometer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/MagnetometerControl.c$
    $Revision: 7$
    $Date: Tuesday, August 21, 2012 7:12:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "MagnetometerControl.h"
#include <Magnetometer.h>
#include <EventQueue.h>
#include <SensorId.h>
#include <KeyValue.h>
#include <DataBuffer.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** tracks if the controller has been initialized */
static bool _initialized = false;

/** Stores the configuration settings for the magnetometer */
static MagConfig_t _magConfig =
{
		MagSampleRate_1280,
		MagOverSampleRatio_16,
		MagDataScaling_RAW,
		MagAutoReset_ENABLED,
		MagSampling_Auto,
		MagResolution_16BIT
};

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _MagDataReadyIsr(void);

/***************************************************************************//**
    Initializes the magnetometer controller.
*//****************************************************************************/
void MagControl_Init(void)
{
	Magnetometer_Init();
	Magnetometer_RegisterIsr(&_MagDataReadyIsr);
	_initialized = true;
}


/***************************************************************************//**
    Sets a configuration value to the accelerometer

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void MagControl_SetConfig(uint8_t keyValue)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(keyValue, &key, &value);

	switch(key)
	{
		case MagnetometerKey_MODE:
		{
			switch(value)
			{
				case MagnetometerMode_ENABLE:
					Magnetometer_Start();
					break;

				case MagnetometerMode_DISABLE:
					Magnetometer_Stop();
					break;

				default:
					DEBUG("unhandled mag mode");
					break;
			}
			break;
		}

		case MagnetometerKey_SAMPLE_RATE:
		{
			switch(value)
			{
				case MagnetometerSampleRate_0_63:
					_magConfig.sampleRate = MagSampleRate_80;
					_magConfig.overSampleRatio = MagOverSampleRatio_128;
					break;

				case MagnetometerSampleRate_1_25:
					_magConfig.sampleRate = MagSampleRate_160;
					_magConfig.overSampleRatio = MagOverSampleRatio_128;
					break;

				case MagnetometerSampleRate_2_5:
					_magConfig.sampleRate = MagSampleRate_320;
					_magConfig.overSampleRatio = MagOverSampleRatio_128;
					break;

				case MagnetometerSampleRate_5:
					_magConfig.sampleRate = MagSampleRate_640;
					_magConfig.overSampleRatio = MagOverSampleRatio_128;
					break;

				case MagnetometerSampleRate_10:
					_magConfig.sampleRate = MagSampleRate_1280;
					_magConfig.overSampleRatio = MagOverSampleRatio_128;
					break;

				case MagnetometerSampleRate_20:
					_magConfig.sampleRate = MagSampleRate_1280;
					_magConfig.overSampleRatio = MagOverSampleRatio_64;
					break;

				case MagnetometerSampleRate_40:
					_magConfig.sampleRate = MagSampleRate_1280;
					_magConfig.overSampleRatio = MagOverSampleRatio_32;
					break;

				case MagnetometerSampleRate_80:
					_magConfig.sampleRate = MagSampleRate_1280;
					_magConfig.overSampleRatio = MagOverSampleRatio_16;
					break;

				default:
					DEBUG("unhandled mag rate");
					break;
			}
			Magnetometer_Config(&_magConfig);
			break;
		}

		default:
			DEBUG("unhandled mag key");
			break;
	}
}

/***************************************************************************//**
    Gets a reading from the magnetometer.
*//****************************************************************************/
void MagControl_ReadMag(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

#if 1 /* write data to data buffer */
	static uint8_t magData[6u];
	uint16_t sampleSize = 0u;

	Magnetometer_ReadSampleArray(6u, &sampleSize, magData);

	if (sampleSize > 0)
	{
		DataBuffer_AddToSubarray8Bit(SensorId_MAGNETOMETER, sampleSize, magData);
	}

//	ASSERT(sampleSuccess, ErrorLedMode_SOFTWARE);
#else /* write data to serial debug */
	static MagSample_t magSample;
	bool sampleSuccess = false;

	sampleSuccess = Magnetometer_ReadSample(&magSample);
	if (sampleSuccess)
	{
		Serial_SendString3Numbers("mag:", magSample.xData, magSample.yData, magSample.zData);
	}
#endif
}

/***************************************************************************//**
    The data ready ISR handles the magnetometer data ready interrupt and will
    add an event to the event queue to notify the system to read the magnetometer.
*//****************************************************************************/
static void _MagDataReadyIsr(void)
{
	/** Define the event type to generate when the accelerometer has data ready. */
	static const Event_t _magDataEvent = Event_MAGNETOMETER_DATA_READY;
	/** Define the event priority of the accelerometer data ready event */
	static const EventPriority_t _magDataEventPriority = EventPriority_NORMAL;

	EventQueue_Add(_magDataEvent, _magDataEventPriority);
}
