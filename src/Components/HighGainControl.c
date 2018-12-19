/***************************************************************************//**
    \file HighGainControl.c
    \brief Controls reading the high gain input.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the high gain input and manages the
    				configurations and settings associated with the high
    				gain input.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/HighGainControl.c$
    $Revision: 6$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "HighGainControl.h"
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

/** Stores the desired sample rate of the high gain input reading */
static HighGainSampleRate_t _sampleRate = DEFAULT_HIGH_GAIN_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _HighGainDataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the high gain input reading control.
*//****************************************************************************/
void HighGainControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the high gain input reading.

    \param[in] config - the KeyValue configuration setting for the high gain
    					input reading.

*//****************************************************************************/
void HighGainControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case HighGainKey_MODE:
			switch(value)
			{
				case HighGainMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_HIGH_GAIN);
					break;

				case HighGainMode_ENABLE:
					switch(_sampleRate)
					{
						case HighGainSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0001HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0010HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0050HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0100HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0200HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0400HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

                        case HighGainSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_0800HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
                            break;

						case HighGainSampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_2400HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_4800HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
							break;

						case HighGainSampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_HIGH_GAIN, AdcManSampleRate_6000HZ, AdcManRef_DEFAULT, &_HighGainDataCallback);
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
			if (((int16_t)(value) >= HighGainSampleRate_FIRST) && (value < HighGainSampleRate_LAST))
			{
				_sampleRate = ((HighGainSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid high gain read key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}


/***************************************************************************//**
    Takes the high gain input data and stores it in the proper location. This
    function is designed to be called every time there is data for the high
    gain input.

    \param[in] data - The sample from the high gain input that should be
    				  stored in the proper location.
*//****************************************************************************/
static void _HighGainDataCallback(uint16_t data)
{
#if 0
	#define HIGH_GAIN_USE_SERIAL
#endif

#ifndef HIGH_GAIN_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_HIGH_GAIN_IN, 1u, &data);
#else
	Serial_SendStringNumber("high gain:", data);
#endif
}
