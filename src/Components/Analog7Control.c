/***************************************************************************//**
    \file Analog7Control.c
    \brief Controls reading analog input in header pin 7.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the analog input on header pin 7 and manages the
    				configurations and settings associated with it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Analog7Control.c$
    $Revision: 5$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Analog7Control.h"
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
static Analog7SampleRate_t _sampleRate = DEFAULT_ANALOG7_SAMPLE_RATE;

/** Stores the desired analog reference voltage of the header pin */
static Analog7RefVolt_t _refVolt = DEFAULT_ANALOG7_REF_VOLT;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static AdcManRef_t _Analog7_RefVoltToAdcManRef(Analog7RefVolt_t refVolt);
static void _Analog7DataCallback(uint16_t data);


/***************************************************************************//**
    Initializes the header pin 7 reading control.
*//****************************************************************************/
void Analog7Control_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the header pin.

    \param[in] config - the KeyValue configuration setting for the header pin.

*//****************************************************************************/
void Analog7Control_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case Analog7Key_MODE:
			switch(value)
			{
				case Analog7Mode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_HEADER_06);
					break;

				case Analog7Mode_ENABLE:
					switch(_sampleRate)
					{
						case Analog7SampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0001HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0010HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0050HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0100HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0200HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0400HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

                        case Analog7SampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0800HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
                            break;

						case Analog7SampleRate_2400:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_2400HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_4800:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_4800HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
							break;

						case Analog7SampleRate_6000:
							AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_6000HZ, _Analog7_RefVoltToAdcManRef(_refVolt), &_Analog7DataCallback);
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

		case Analog7Key_SAMPLE_RATE:
			if (((int16_t)(value) >= Analog7SampleRate_FIRST) && (value < Analog7SampleRate_LAST))
			{
				_sampleRate = ((Analog7SampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		case Analog7Key_REF_VOLT:
		    if (((int16_t)(value) >= Analog7RefVolt_FIRST) && (value < Analog7RefVolt_LAST))
		    {
		        _refVolt = ((Analog7RefVolt_t)(value));
		    }
		    else
		    {
		        ASSERT(false, ErrorLedMode_SOFTWARE);
		    }
		    break;

		default:
			/* invalid analog input 7 key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**
    Converts from an Analog7RefVolt_t to an AdcManRef_t.

    \param[in] refVolt - the voltage reference selection to match
    \returns The AdcManRef_t value corresponding to the refVolt value

*//****************************************************************************/
static AdcManRef_t _Analog7_RefVoltToAdcManRef(Analog7RefVolt_t refVolt)
{
    AdcManRef_t adcRef;

    switch(refVolt)
    {
        case Analog7RefVolt_3v0:
            adcRef = AdcManRef_VREF;
            break;

        case Analog7RefVolt_3v3:
            adcRef = AdcManRef_VCC;
            break;

        default:
            adcRef = AdcManRef_DEFAULT;
            break;
    }

    return adcRef;
}

/***************************************************************************//**
    Takes the analog input data from header pin 7 and stores it in the proper
    location. This function is designed to be called every time there is data
    for the analog header pin 7.

    \param[in] data - The sample from header pin 7 that should be stored in
    				  the proper location.
*//****************************************************************************/
static void _Analog7DataCallback(uint16_t data)
{
#if 0
	#define ANALOG7_USE_SERIAL
#endif

#ifndef ANALOG7_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_HEADER_7, 1u, &data);
#else
	Serial_SendStringNumber("analog 7:", data);
#endif
}
