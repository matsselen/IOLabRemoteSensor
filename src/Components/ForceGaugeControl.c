/***************************************************************************//**
    \file ForceGaugeControl.c
    \brief Controls the force gauge.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the force gauge and manages the configurations and
                   settings associated with the force gauge.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/ForceGaugeControl.c$
    $Revision: 8$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "ForceGaugeControl.h"
#include <SensorId.h>
#include <DataBuffer.h>
#include <KeyValue.h>
#include <AdcManager.h>
#include <Assert.h>
#include <Serial.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
bool _initialized = false;

/** Stores the desired sample rate of the force gauge */
static ForceGaugeSampleRate_t _sampleRate = DEFAULT_FORCE_GAUGE_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _ForceGaugeDataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the force gauge control.
*//****************************************************************************/
void FgControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and set its for the force gauge.

    \param[in] config - the KeyValue configuration setting for the force gauge.

*//****************************************************************************/
void FgControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case ForceGaugeKey_MODE:
			switch(value)
			{
				case ForceGaugeMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_FORCE_GAUGE);
					break;

				case ForceGaugeMode_ENABLE:
					switch(_sampleRate)
					{
						case ForceGaugeSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

                        case ForceGaugeSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_0800HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
                            break;

						case ForceGaugeSampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_2400HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_4800HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
							break;

						case ForceGaugeSampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_FORCE_GAUGE, AdcManSampleRate_6000HZ, AdcManRef_DEFAULT, &_ForceGaugeDataCallback);
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

		case ForceGaugeKey_SAMPLE_RATE:
			if (((int16_t)(value) >= ForceGaugeSampleRate_FIRST) && (value < ForceGaugeSampleRate_LAST))
			{
				_sampleRate = ((ForceGaugeSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid Force Gauge key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**
    Takes the force gauge data and stores it in the proper location. This function
    is designed to be called every time there is data for the force gauge.

    \param[in] data - The sample from the force gauge that should be stored in
                      the proper location.
*//****************************************************************************/
static void _ForceGaugeDataCallback(uint16_t data)
{
#if 0
	#define FORCE_GAUGE_USE_SERIAL
#endif

#ifndef FORCE_GAUGE_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_FORCE_GAUGE, 1u, &data);
#else
	Serial_SendStringNumber("fg:", data);
#endif
}
