/***************************************************************************//**
    \file EcgControl.c
    \brief Controls reading the ECG.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the ECG and manages the configurations and
    				settings associated with the ECG.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/EcgControl.c$
    $Revision: 6$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "EcgControl.h"
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
/** The most significant nibble should always be 0 since the A to D is 12 bits */
#define HIGH_NIBBLE 0xF000

/** Number of ADC channels for ECG */
#define NUM_ADC_CHANNELS	3u

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
static bool _initialized = false;

/** Stores the desired sample rate of the ECG reading */
static EcgSampleRate_t _sampleRate = DEFAULT_ECG_SAMPLE_RATE;

/** array containing latest samples from all 3 analog inputs for the ECG */
static uint16_t _dataSamples[NUM_ADC_CHANNELS] = {HIGH_NIBBLE, HIGH_NIBBLE, HIGH_NIBBLE};

/** Stores the desired analog reference voltage of the header pin */
static EcgRefVolt_t _refVolt = DEFAULT_ECG_REF_VOLT;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static AdcManRef_t _Ecg_RefVoltToAdcManRef(EcgRefVolt_t refVolt);
static void _Ecg1DataCallback(uint16_t data);
static void _Ecg2DataCallback(uint16_t data);
static void _Ecg3DataCallback(uint16_t data);
static void _EcgAddToDataBuffer(void);

/***************************************************************************//**
    Initializes the ECG reading control.
*//****************************************************************************/
void EcgControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the ECG reading.

    \param[in] config - the KeyValue configuration setting for the ECG reading.

*//****************************************************************************/
void EcgControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case EcgKey_MODE:
			switch(value)
			{
				case EcgMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_ECG_1);
					AdcManager_DisableChannel(AdcManChannel_ECG_2);
					AdcManager_DisableChannel(AdcManChannel_ECG_3);
					break;

				case AmbientLightMode_ENABLE:
					switch(_sampleRate)
					{
						case EcgSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0001HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0001HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0001HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

						case EcgSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0010HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0010HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0010HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

						case EcgSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0050HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0050HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0050HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

						case EcgSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0100HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0100HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0100HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

						case EcgSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0200HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0200HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0200HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

						case EcgSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0400HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0400HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0400HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
							break;

                        case EcgSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0800HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0800HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0800HZ, _Ecg_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
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

		case EcgKey_SAMPLE_RATE:
			if (((int16_t)(value) >= EcgSampleRate_FIRST) && (value < EcgSampleRate_LAST))
			{
				_sampleRate = ((EcgSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

        case EcgKey_REF_VOLT:
            if (((int16_t)(value) >= EcgRefVolt_FIRST) && (value < EcgRefVolt_LAST))
            {
                _refVolt = ((EcgRefVolt_t)(value));
            }
            else
            {
                ASSERT(false, ErrorLedMode_SOFTWARE);
            }
            break;

		default:
			/* invalid ECG read key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**
    Converts from an EcgRefVolt_t to an AdcManRef_t.

    \param[in] refVolt - the voltage reference selection to match
    \returns The AdcManRef_t value corresponding to the refVolt value

*//****************************************************************************/
static AdcManRef_t _Ecg_RefVoltToAdcManRef(EcgRefVolt_t refVolt)
{
    AdcManRef_t adcRef;

    switch(refVolt)
    {
        case EcgRefVolt_3v0:
            adcRef = AdcManRef_VREF;
            break;

        case EcgRefVolt_3v3:
            adcRef = AdcManRef_VCC;
            break;

        default:
            adcRef = AdcManRef_DEFAULT;
            break;
    }

    return adcRef;
}

/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the first ECG
    channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg1DataCallback(uint16_t data)
{
	_dataSamples[0] = data;

	if(((_dataSamples[0] & HIGH_NIBBLE) == 0) && ((_dataSamples[1] & HIGH_NIBBLE) == 0) && ((_dataSamples[2] & HIGH_NIBBLE) == 0))
	{
		_EcgAddToDataBuffer();

		_dataSamples[0] |= HIGH_NIBBLE;
		_dataSamples[1] |= HIGH_NIBBLE;
		_dataSamples[2] |= HIGH_NIBBLE;
	}
}

/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the second
    ECG channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg2DataCallback(uint16_t data)
{
	_dataSamples[1] = data;

	if(((_dataSamples[0] & HIGH_NIBBLE) == 0) && ((_dataSamples[1] & HIGH_NIBBLE) == 0) && ((_dataSamples[2] & HIGH_NIBBLE) == 0))
	{
		_EcgAddToDataBuffer();

		_dataSamples[0] |= HIGH_NIBBLE;
		_dataSamples[1] |= HIGH_NIBBLE;
		_dataSamples[2] |= HIGH_NIBBLE;
	}
}

/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the third ECG
    channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg3DataCallback(uint16_t data)
{
	_dataSamples[2] = data;

	if(((_dataSamples[0] & HIGH_NIBBLE) == 0) && ((_dataSamples[1] & HIGH_NIBBLE) == 0) && ((_dataSamples[2] & HIGH_NIBBLE) == 0))
	{
		_EcgAddToDataBuffer();

		_dataSamples[0] |= HIGH_NIBBLE;
		_dataSamples[1] |= HIGH_NIBBLE;
		_dataSamples[2] |= HIGH_NIBBLE;
	}
}

/***************************************************************************//**
    Takes the ECG data and stores it in the proper location. This
    function is designed to be called every time there is data for the ECG.

*//****************************************************************************/
static void _EcgAddToDataBuffer(void)
{
#if 0
	#define ECG_USE_SERIAL
#endif

#ifndef ECG_USE_SERIAL
	DataBuffer_AddToSubarray16Bit(SensorId_ECG, 3u, &_dataSamples[0]);
#else
	Serial_SendStringNumber("ECG1:", _dataSamples[0]);
	Serial_SendStringNumber("ECG2:", _dataSamples[1]);
	Serial_SendStringNumber("ECG3:", _dataSamples[2]);
#endif
}
