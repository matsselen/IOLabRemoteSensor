/***************************************************************************//**
    \file AdcManager.h
    \brief Analog to digital conversion manager header file.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The analog to digital conversion manager will provide configuration
       functions and macros for accessing analog conversion samples.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/AdcManager.h$
    $Revision: 10$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

/* include Adc.h so that the channel enum will be valid */
#include <Adc.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the available ADC Channels by their purpose

    \note The enum value for the AdcManChannel must match the AdcInputChannel value
    for the particular input channel
*/
typedef enum AdcManChannel
{
	AdcManChannel_INVALID       = -1,
	AdcManChannel_FIRST         = AdcInputChannel_A0,
	AdcManChannel_HEADER_06     = AdcInputChannel_A0,
	AdcManChannel_HEADER_07     = AdcInputChannel_A1,
	AdcManChannel_HEADER_08     = AdcInputChannel_A2,
	/* ADC Input Channel A3 is unused */
	AdcManChannel_FORCE_GAUGE   = AdcInputChannel_A4,
	AdcManChannel_ECG_1         = AdcInputChannel_A5,
	AdcManChannel_ECG_2         = AdcInputChannel_A6,
	AdcManChannel_ECG_3         = AdcInputChannel_A7,
	/* ADC Input Channels A8-A9 are unused */
	AdcManChannel_TEMPERATURE   = AdcInputChannel_TEMP,
	/* ADC Input Channel A11 is unused */
	AdcManChannel_AMBIENT_LIGHT = AdcInputChannel_A12,
	AdcManChannel_HIGH_GAIN     = AdcInputChannel_A13,
	AdcManChannel_MICROPHONE    = AdcInputChannel_A14,
	AdcManChannel_BATTERY       = AdcInputChannel_A15,
	AdcManChannel_LAST
} AdcManChannel_t;

/** Defines the ADC sample rate options */
typedef enum AdcManSampleRate
{
	AdcManSampleRate_INVALID = -1,
	AdcManSampleRate_FIRST   = 0u,
	AdcManSampleRate_0001HZ  = 0u,
	AdcManSampleRate_0010HZ  = 1u,
	AdcManSampleRate_0050HZ  = 2u,
	AdcManSampleRate_0100HZ  = 3u,
	AdcManSampleRate_0200HZ  = 4u,
	AdcManSampleRate_0400HZ  = 5u,
	AdcManSampleRate_0800HZ  = 6u,
	AdcManSampleRate_2400HZ  = 7u,
	AdcManSampleRate_4800HZ  = 8u,
	AdcManSampleRate_6000HZ  = 9u,
	AdcManSampleRate_LAST
} AdcManSampleRate_t;

/** Defines the analog reference options */
typedef enum AdcManRefs
{
    AdcManRef_INVALID = -1,
    AdcManRef_FIRST   = 0u,
    AdcManRef_DEFAULT = AdcManRef_FIRST,
    AdcManRef_VREF    = AdcManRef_DEFAULT,
    AdcManRef_VCC,
    AdcManRef_LAST
} AdcManRef_t;

/** Defines a callback for function pointer for handling the conversion data.

	\note The callback must be in the form of:

	void callback(uint16_t sample);

	Where sample with be the ADC value that was sampled.
*/
typedef void (*AdcDataCallback_t)(uint16_t);

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void AdcManager_Init(void);
void AdcManager_EnableChannel(
		AdcManChannel_t channel,
		AdcManSampleRate_t rate,
		AdcManRef_t reference,
		AdcDataCallback_t callback);
void AdcManager_DisableChannel(
		AdcManChannel_t channel);
uint16_t AdcManager_SingleSampleBlocking(
		AdcManChannel_t channel);
void AdcManager_ProcessConversion(void);

uint16_t AdcManager_GetGainFactor(void);
uint16_t AdcManager_GetOffset(void);

void AdcManager_PoweredChannelControl(AdcManChannel_t channel, bool enable);

AdcReference_t AdcManager_AdcManRefToAdcRef(
        AdcManRef_t adcManRef);
#endif /* ADCMANAGER_H_ */
