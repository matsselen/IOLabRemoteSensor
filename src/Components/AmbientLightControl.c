/***************************************************************************//**
    \file AmbientLightControl.c
    \brief Controls reading the ambient light sensor.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the ambient light sensor and manages the
    				configurations and settings associated with the ambient
    				light sensor.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/AmbientLightControl.c$
    $Revision: 8$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "AmbientLightControl.h"
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
static bool _initialized = false;

/** Stores the desired sample rate of the ambient light sensor voltage reading */
static AmbientLightSampleRate_t _sampleRate = DEFAULT_AMBIENT_LIGHT_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _AmbientLightDataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the ambient light sensor reading control.
*//****************************************************************************/
void AmbientLightControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the ambient light sensor reading.

    \param[in] config - the KeyValue configuration setting for the ambient
    light sensor reading.

*//****************************************************************************/
void AmbientLightControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case AmbientLightKey_MODE:
			switch(value)
			{
				case AmbientLightMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_AMBIENT_LIGHT);
					break;

				case AmbientLightMode_ENABLE:
					switch(_sampleRate)
					{
						case AmbientLightSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

                        case AmbientLightSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_0800HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
                            break;

						case AmbientLightSampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_2400HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_4800HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
							break;

						case AmbientLightSampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_AMBIENT_LIGHT, AdcManSampleRate_6000HZ, AdcManRef_DEFAULT, &_AmbientLightDataCallback);
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

		case AmbientLightKey_SAMPLE_RATE:
			if (((int16_t)(value) >= AmbientLightSampleRate_FIRST) && (value < AmbientLightSampleRate_LAST))
			{
				_sampleRate = ((AmbientLightSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid Ambient light sensor read key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}


/***************************************************************************//**
    Takes the ambient light sensor data and stores it in the proper location. This
    function is designed to be called every time there is data for the ambient
    light sensor.

    \param[in] data - The sample from the ambient light sensor that should be
    				  stored in the proper location.
*//****************************************************************************/
static void _AmbientLightDataCallback(uint16_t data)
{
#if 0
	#define AMBIENT_LIGHT_USE_SERIAL
#endif

#ifndef AMBIENT_LIGHT_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_AMBIENT_LIGHT, 1u, &data);
#else
	Serial_SendStringNumber("ambient light:", data);
#endif
}
