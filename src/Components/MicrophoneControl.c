/***************************************************************************//**
    \file MicrophoneControl.c
    \brief Controls reading the microphone.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the microphone and manages the configurations
    				and settings associated with the microphone

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/MicrophoneControl.c$
    $Revision: 6$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "MicrophoneControl.h"
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

/** Stores the desired sample rate of the microphone */
static MicrophoneSampleRate_t _sampleRate = DEFAULT_MICROPHONE_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _MicrophoneDataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the ambient light sensor reading control.
*//****************************************************************************/
void MicrophoneControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the microphone.

    \param[in] config - the KeyValue configuration setting for the microphone.

*//****************************************************************************/
void MicrophoneControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case MicrophoneKey_MODE:
			switch(value)
			{
				case MicrophoneMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_MICROPHONE);
					break;

				case MicrophoneMode_ENABLE:
					switch(_sampleRate)
					{
						case MicrophoneSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

                        case MicrophoneSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_0800HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
                            break;

						case MicrophoneSampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_2400HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_4800HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
							break;

						case MicrophoneSampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_MICROPHONE, AdcManSampleRate_6000HZ, AdcManRef_DEFAULT, &_MicrophoneDataCallback);
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

		case MicrophoneKey_SAMPLE_RATE:
			if (((int16_t)(value) >= MicrophoneSampleRate_FIRST) && (value < MicrophoneSampleRate_LAST))
			{
				_sampleRate = ((MicrophoneSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid microphone key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}


/***************************************************************************//**
    Takes the microphone data and stores it in the proper location. This
    function is designed to be called every time there is data for the
    microphone.

    \param[in] data - The sample from the microphone that should be stored in
    				  the proper location.
*//****************************************************************************/
static void _MicrophoneDataCallback(uint16_t data)
{
#if 0
	#define MICROPHONE_USE_SERIAL
#endif

#ifndef MICROPHONE_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_MICROPHONE, 1u, &data);
#else
	Serial_SendStringNumber("microphone:", data);
#endif
}
