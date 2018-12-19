/***************************************************************************//**
    \file BatteryControl.c
    \brief Controls reading the battery voltage.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the force gauge and manages the configurations and
                   settings associated with the force gauge.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BatteryControl.c$
    $Revision: 7$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "BatteryControl.h"
#include <SensorId.h>
#include <DataBuffer.h>
#include <KeyValue.h>
#include <AdcManager.h>
#include <Board.h>
#include <Gpio.h>
#include <Device.h>
#include <Assert.h>
#include <Serial.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
static bool _initialized = false;

/** Stores the desired sample rate of the battery voltage reading */
static BatterySampleRate_t _sampleRate = DEFAULT_BATTERY_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _BatteryVoltageDataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the battery voltage reading control.
*//****************************************************************************/
void BatteryControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the battery voltage reading.

    \param[in] config - the KeyValue configuration setting for the battery reading.

*//****************************************************************************/
void BatteryControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case BatteryKey_MODE:
			switch(value)
			{
				case BatteryMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_BATTERY);
					break;

				case BatteryMode_ENABLE:
					switch(_sampleRate)
					{
						case BatterySampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

                        case BatterySampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_0800HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
                            break;

						case BatterySampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_2400HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_4800HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
							break;

						case BatterySampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_BATTERY, AdcManSampleRate_6000HZ, AdcManRef_DEFAULT, &_BatteryVoltageDataCallback);
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

		case BatteryKey_SAMPLE_RATE:
			if (((int16_t)(value) >= BatterySampleRate_FIRST) && (value < BatterySampleRate_LAST))
			{
				_sampleRate = ((BatterySampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid Battery Voltage Read key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
BatteryCounts_t BatteryControl_GetCounts(void)
{
	BatteryCounts_t data = 0;
	uint16_t datau = 0;

	datau = AdcManager_SingleSampleBlocking(AdcManChannel_BATTERY);

	data = (BatteryCounts_t)datau;

	return data;

}

/***************************************************************************//**
    Reads the calibration values from the battery.

    \param[in] maxCalibrationLength - the maximum number of bytes allocated for calibration[]
    \param[out] actualCalibrationLength - the number of bytes of calibration data placed in calibration[]
    \param[out] calibration[] - the calibration data from the battery.

*//****************************************************************************/
void BatteryControl_ReadCalibration(
		uint16_t  maxCalibrationLength,
		uint16_t *actualCalibrationLength,
		uint8_t   calibration[])
{

	/** Defines how many bytes are needed to store the calibration data */
#define NUM_BATTERY_CALIBRATION_BYTES   ( 4u )
	uint16_t gain;
	uint16_t offset;


	ASSERT((calibration != NULL) && (actualCalibrationLength != NULL) &&
	        (maxCalibrationLength >= NUM_BATTERY_CALIBRATION_BYTES), ErrorLedMode_SOFTWARE);

	gain = AdcManager_GetGainFactor();
	offset = AdcManager_GetOffset();

	calibration[0u] = (uint8_t)(gain >> 8u);
	calibration[1u] = (uint8_t)(gain);
	calibration[2u] = (uint8_t)(offset >> 8u);
	calibration[3u] = (uint8_t)(offset);

	*actualCalibrationLength = NUM_BATTERY_CALIBRATION_BYTES;
}

/***************************************************************************//**
    Takes the battery voltage data and stores it in the proper location. This
    function is designed to be called every time there is data for the battery
    voltage.

    \param[in] data - The sample from the battery that should be stored in
                      the proper location.
*//****************************************************************************/
static void _BatteryVoltageDataCallback(uint16_t data)
{
#if 0
	#define BATTERY_USE_SERIAL
#endif

#ifndef BATTERY_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_BATTERY, 1u, &data);
#else
	Serial_SendStringNumber("batt:", data);
#endif
}
