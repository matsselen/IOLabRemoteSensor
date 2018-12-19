/***************************************************************************//**
    \file EcgControl.c
    \brief Controls reading the 6-channel ECG.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the 6-channel ECG and manages the configurations
    			   and
    			   settings associated with the ECG.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Ecg6ChanControl.c$
    $Revision: 2$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Ecg6ChanControl.h"
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
#define HIGH_NIBBLE         ( 0xF000 )

/** Number of ADC channels for ECG */
#define NUM_ADC_CHANNELS	( 6u )

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
static bool _initialized = false;

/** Stores the desired sample rate of the ECG reading */
static Ecg6ChanSampleRate_t _sampleRate = DEFAULT_ECG6CHAN_SAMPLE_RATE;

/** array containing latest samples from all 3 analog inputs for the ECG */
static uint16_t _dataSamples[NUM_ADC_CHANNELS] =
    {
        HIGH_NIBBLE,
        HIGH_NIBBLE,
        HIGH_NIBBLE,
        HIGH_NIBBLE,
        HIGH_NIBBLE,
        HIGH_NIBBLE
    };

/** Stores the desired analog reference voltage of the header pin */
static Ecg6RefVolt_t _refVolt = DEFAULT_ECG6_REF_VOLT;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static AdcManRef_t _Ecg6_RefVoltToAdcManRef(Ecg6RefVolt_t refVolt);
static void _Ecg1DataCallback(uint16_t data);
static void _Ecg2DataCallback(uint16_t data);
static void _Ecg3DataCallback(uint16_t data);
static void _Ecg4DataCallback(uint16_t data);
static void _Ecg5DataCallback(uint16_t data);
static void _Ecg6DataCallback(uint16_t data);
static void _EcgAddToDataBuffer(void);
static __inline bool _AllEcgChannelsFull(
    const uint16_t data[],
    uint16_t arraySize);
static __inline void _ClearDataArray(
    uint16_t data[],
    uint16_t arraySize);
static __inline void _HandleNewData(void);

/***************************************************************************//**
    Initializes the ECG reading control.
*//****************************************************************************/
void Ecg6Control_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the ECG reading.

    \param[in] config - the KeyValue configuration setting for the ECG reading.

*//****************************************************************************/
void Ecg6Control_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case Ecg6ChanKey_MODE:
			switch(value)
			{
				case Ecg6ChanMode_DISABLE:
					AdcManager_DisableChannel(AdcManChannel_ECG_1);
					AdcManager_DisableChannel(AdcManChannel_ECG_2);
					AdcManager_DisableChannel(AdcManChannel_ECG_3);
                    AdcManager_DisableChannel(AdcManChannel_HEADER_06);
                    AdcManager_DisableChannel(AdcManChannel_HEADER_07);
                    AdcManager_DisableChannel(AdcManChannel_HEADER_08);
					break;

				case Ecg6ChanMode_ENABLE:
					switch(_sampleRate)
					{
						case Ecg6ChanSampleRate_1:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0001HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

						case Ecg6ChanSampleRate_10:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0010HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

						case Ecg6ChanSampleRate_50:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0050HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

						case Ecg6ChanSampleRate_100:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0100HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

						case Ecg6ChanSampleRate_200:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0200HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

						case Ecg6ChanSampleRate_400:
							AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
							AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0400HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
							break;

                        case Ecg6ChanSampleRate_800:
                            AdcManager_EnableChannel(AdcManChannel_ECG_1, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg1DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_ECG_2, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg2DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_ECG_3, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg3DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_06, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg4DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_07, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg5DataCallback);
                            AdcManager_EnableChannel(AdcManChannel_HEADER_08, AdcManSampleRate_0800HZ, _Ecg6_RefVoltToAdcManRef(_refVolt), &_Ecg6DataCallback);
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

		case Ecg6ChanKey_SAMPLE_RATE:
			if (((int16_t)(value) >= Ecg6ChanSampleRate_FIRST) && (value < Ecg6ChanSampleRate_LAST))
			{
				_sampleRate = ((Ecg6ChanSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

        case Ecg6ChanKey_REF_VOLT:
            if (((int16_t)(value) >= Ecg6RefVolt_FIRST) && (value < Ecg6RefVolt_LAST))
            {
                _refVolt = ((Ecg6RefVolt_t)(value));
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
    Converts from an Ecg6RefVolt_t to an AdcManRef_t.

    \param[in] refVolt - the voltage reference selection to match
    \returns The AdcManRef_t value corresponding to the refVolt value

*//****************************************************************************/
static AdcManRef_t _Ecg6_RefVoltToAdcManRef(Ecg6RefVolt_t refVolt)
{
    AdcManRef_t adcRef;

    switch(refVolt)
    {
        case Ecg6RefVolt_3v0:
            adcRef = AdcManRef_VREF;
            break;

        case Ecg6RefVolt_3v3:
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
	_dataSamples[0u] = data;
    _HandleNewData();
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
	_dataSamples[1u] = data;
    _HandleNewData();
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
	_dataSamples[2u] = data;
    _HandleNewData();
}

/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the fourth ECG
    channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg4DataCallback(uint16_t data)
{
    _dataSamples[3u] = data;
    _HandleNewData();
}

/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the fifth ECG
    channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg5DataCallback(uint16_t data)
{
    _dataSamples[4u] = data;
    _HandleNewData();
}
/***************************************************************************//**
    Takes the ECG data and stores it in the data array. This
    function is designed to be called every time there is data for the sixth ECG
    channel.

    \param[in] data - The sample from the ECG that should be stored in
                      the proper location.
*//****************************************************************************/
static void _Ecg6DataCallback(uint16_t data)
{
    _dataSamples[5u] = data;
    _HandleNewData();
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
	DataBuffer_AddToSubarray16Bit(SensorId_ECG6CHAN, NUM_ADC_CHANNELS, &_dataSamples[0]);
#else
	Serial_SendStringNumber("ECG1:", _dataSamples[0]);
	Serial_SendStringNumber("ECG2:", _dataSamples[1]);
	Serial_SendStringNumber("ECG3:", _dataSamples[2]);
    Serial_SendStringNumber("ECG4:", _dataSamples[3]);
    Serial_SendStringNumber("ECG5:", _dataSamples[4]);
    Serial_SendStringNumber("ECG6:", _dataSamples[5]);
#endif
}

/***************************************************************************//**
    Checks if all of the channels in the ECG buffer have valid data in them.
*//****************************************************************************/
static __inline bool _AllEcgChannelsFull(
    const uint16_t data[],
    uint16_t arraySize)
{
    uint16_t index;
    bool isFull = true;

    for (index = 0u; index < arraySize; ++index)
    {
        if ((data[index] & HIGH_NIBBLE) > 0u)
        {
            isFull = false;
            break;
        }
    }

    return isFull;
}

/***************************************************************************//**
    Sets the high nibble for each element in the array.
*//****************************************************************************/
static __inline void _ClearDataArray(
        uint16_t data[],
        uint16_t arraySize)
{
    uint16_t index;

    ASSERT(data != NULL, ErrorLedMode_SOFTWARE);

    for (index = 0u; index < arraySize; ++index)
    {
        data[index] = HIGH_NIBBLE;
    }
}

/***************************************************************************//**
    Sets the high nibble for each element in the array.
*//****************************************************************************/
static __inline void _HandleNewData(void)
{
    if(_AllEcgChannelsFull(_dataSamples, ARRAY_LENGTH(_dataSamples)))
    {
        _EcgAddToDataBuffer();
        _ClearDataArray(_dataSamples, ARRAY_LENGTH(_dataSamples));
    }
}
