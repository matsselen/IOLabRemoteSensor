/***************************************************************************//**
    \file Analog9Control.c
    \brief Controls reading analog input in header pin 9.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the analog input on header pin 9 and manages the
    				configurations and settings associated with it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Analog9Control.c$
    $Revision: 5$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Analog9Control.h"
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

/** Stores the desired sample rate of the header pin */
static Analog9SampleRate_t _sampleRate = DEFAULT_ANALOG9_SAMPLE_RATE;

/** Stores the desired analog reference voltage of the header pin */
static Analog9RefVolt_t _refVolt = DEFAULT_ANALOG9_REF_VOLT;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static AdcManRef_t _Analog9_RefVoltToAdcManRef(Analog9RefVolt_t refVolt);
static void _Analog9DataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the header pin 9 reading control.
*//****************************************************************************/
void Analog9Control_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the header pin.

    \param[in] config - the KeyValue configuration setting for the header pin.

*//****************************************************************************/
void Analog9Control_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case Analog9Key_MODE:
			switch(value)
			{
				case Analog9Mode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_HEADER_08);
					break;

				case Analog9Mode_ENABLE:
					switch(_sampleRate)
					{
						case Analog9SampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0001HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0010HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0050HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0100HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0200HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0400HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

                        case Analog9SampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0800HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
                            break;

						case Analog9SampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_2400HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_4800HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
							break;

						case Analog9SampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_6000HZ, _Analog9_RefVoltToAdcManRef(_refVolt), &_Analog9DataCallback);
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

		case Analog9Key_SAMPLE_RATE:
			if (((int16_t)(value) >= Analog9SampleRate_FIRST) && (value < Analog9SampleRate_LAST))
			{
				_sampleRate = ((Analog9SampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

        case Analog9Key_REF_VOLT:
            if (((int16_t)(value) >= Analog9RefVolt_FIRST) && (value < Analog9RefVolt_LAST))
            {
                _refVolt = ((Analog9RefVolt_t)(value));
            }
            else
            {
                ASSERT(false, ErrorLedMode_SOFTWARE);
            }
            break;

		default:
			/* invalid analog input 9 key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**
    Converts from an Analog9RefVolt_t to an AdcManRef_t.

    \param[in] refVolt - the voltage reference selection to match
    \returns The AdcManRef_t value corresponding to the refVolt value

*//****************************************************************************/
static AdcManRef_t _Analog9_RefVoltToAdcManRef(Analog9RefVolt_t refVolt)
{
    AdcManRef_t adcRef;

    switch(refVolt)
    {
        case Analog9RefVolt_3v0:
            adcRef = AdcManRef_VREF;
            break;

        case Analog9RefVolt_3v3:
            adcRef = AdcManRef_VCC;
            break;

        default:
            adcRef = AdcManRef_DEFAULT;
            break;
    }

    return adcRef;
}

/***************************************************************************//**
    Takes the analog input data from header pin 9 and stores it in the proper
    location. This function is designed to be called every time there is data
    for the analog header pin 9.

    \param[in] data - The sample from header pin 9 that should be stored in
    				  the proper location.
*//****************************************************************************/
static void _Analog9DataCallback(uint16_t data)
{
#if 0
	#define ANALOG9_USE_SERIAL
#endif

#ifndef ANALOG9_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_HEADER_9, 1u, &data);
#else
	Serial_SendStringNumber("analog 9:", data);
#endif
}
