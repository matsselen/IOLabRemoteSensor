/***************************************************************************//**
    \file AdcManager.c
    \brief Analog to digital conversion manager

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The analog to digital conversion manager will provide configuration
       functions and macros for accessing analog conversion samples.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/AdcManager.c$
    $Revision: 11$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include <Adc.h>
#include "AdcManager.h"
#include <Timer.h>
#include <Board.h>
#include <Gpio.h>
#include <Assert.h>

#include <Serial.h>
#include <Debug.h>
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines how many ADC samples to drop to achieve "slower" sample rates assuming
    a 400Hz sample rate
*/
typedef enum EveryXSamples
{
	EveryXSamples_INVLALID = -1,
	EveryXSamples_0001     = 400u,
	EveryXSamples_0010     = 40u,
	EveryXSamples_0050     = 8u,
	EveryXSamples_0100     = 4u,
	EveryXSamples_0200     = 2u,
	EveryXSamples_0400     = 1u,
	/* the rest will only be able to run at 400 Hz */
	EveryXSamples_0800     = 1u,
	EveryXSamples_2400     = 1u,
	EveryXSamples_4800     = 1u,
	EveryXSamples_6000     = 1u
} EveryXSamples_t;

/** Defines a structure that will hold all the necessary data for a channel to be sampled */
typedef struct AdcChannelData
{
	/** Store the callback that knows how to handle the data */
	AdcDataCallback_t callback;
	/** Store the number of samples to skip to achieve the data rate */
	EveryXSamples_t   skipSamples;
	/** Store the number of samples that have been skipped since the last one
	    was kept.
	*/
	uint16_t          skippedSamples;
	/** Stores the last ADC value that was kept */
	uint16_t          sampleData;
} AdcChannelData_t;

/** Defines the reference to use for all of the adc channels */
#define ADC_REFERENCE   ( AdcReference_eVREFp_VREFm )
/** Defines the reference to use for the thermometer */
#define ADC_THERMOMETER_REFERENCE  (AdcReference_VREFp_AVSS)
/** Defines the reference voltage for the thermometer */
#define ADC_THERMOMETER_REFERENCE_VOLTAGE (AdcReferenceLevel_1p5_VOLTS)

/** Defines the number of powered sensors. This must be a binary flag enum */
typedef enum AdcPoweredChannel
{
	AdcPoweredChannel_NONE          = 0u,
	AdcPoweredChannel_FORCE_GAUGE   = 0x01,
	AdcPoweredChannel_AMBIENT_LIGHT = 0x02,
	AdcPoweredChannel_MICROPHONE    = 0x04
} AdcPoweredChannel_t;

/** Enum for the ADC timer rate that triggers the conversions */
typedef enum AdcTimerRate
{
	AdcTimerRate_INVALID = -1,
	AdcTimerRate_FIRST   = 0u,
	AdcTimerRate_0400HZ  = AdcTimerRate_FIRST,
	AdcTimerRate_0800HZ,
	AdcTimerRate_2400HZ,
	AdcTimerRate_4800HZ,
	AdcTimerRate_6000HZ,
	AdcTimerRate_LAST
} AdcTimerRate_t;

/** Defines how many channels need to be used for different sample rates */
typedef enum AdcSamplesNeeded
{
	AdcSamplesNeeded_0400HZ = AdcInputChannel_LAST,
	AdcSamplesNeeded_0800HZ = 2u,
	AdcSamplesNeeded_2400HZ = 6u,
	AdcSamplesNeeded_4800HZ = 12u,
	AdcSamplesNeeded_6000HZ = 15u
} AdcSamplesNeeded_t;

/** Defines the sampling modes of the ADC Manager */
typedef enum AdcManSampleMode
{
    AdcManSampleMode_INVALID = -1,
    AdcManSampleMode_FIRST   = 0u,
    AdcManSampleMode_NORMAL  = AdcManSampleMode_FIRST,
    AdcManSampleMode_FAST,
    AdcManSampleMode_LAST
} AdcManSampleMode_t;

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** This configuration will set the ADC to sample for the high speed settings */
static const AdcConfig_t _fastAdcConfig = { AdcConSeq_SEQ_OF_CHAN, AdcSht_0008, false };
/** This configuration will set the ADC to sample for the normal modes */
static const AdcConfig_t _normAdcConfig = { AdcConSeq_SEQ_OF_CHAN, AdcSht_0384, true };
/** This configuration will set the ADC to sample for the multi-channel high speed settings */
static const AdcConfig_t _fastMultiAdcConfig = { AdcConSeq_SEQ_OF_CHAN, AdcSht_0008, true };

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** tracks if the ADC manager is initialized */
static bool _initialized = false;

/** Stores the configuration information for all the ADC channels */
static AdcChannelConfig_t _adcAllChannelConfig[AdcManChannel_LAST];

/** Tracks if the ADC is active or not */
static bool _active = false;

static AdcPoweredChannel_t _poweredSensorsEnabled = AdcPoweredChannel_NONE;

/** tracks the timer rate which the ADC uses for sampling */
static AdcTimerRate_t _timerRate = AdcTimerRate_0400HZ;

/** tracks the current sample mode of the ADC Manager */
static AdcManSampleMode_t _sampleMode = AdcManSampleMode_NORMAL;

/** tracks how many unique high speed channels are currently active */
static uint16_t _highSpeedActiveChannels = 1u;

/** Stores the data from the ADC conversion */
static AdcChannelData_t _channels[AdcManChannel_LAST] =
{
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
		{
			NULL,
			EveryXSamples_INVLALID,
			0u,
			0u
		},
};

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static EveryXSamples_t _GetSamplesToSkip(AdcManSampleRate_t sampleRate);
static bool _AllChannelsDisabled(void);
static __inline void _InitialzeXAdcChannels(uint16_t x);
static void _SetupTimerConfig(void);
static __inline void _AdcStart(void);
static __inline void _AdcStop(void);
static __inline void _SetupAdcForNormalMode(
                                AdcManRef_t reference);
static __inline void _SetupAdcSingleChannel(
        AdcManChannel_t channel,
        AdcManRef_t reference);

/***************************************************************************//**
    Initializes the ADC manager
*//****************************************************************************/
void AdcManager_Init(void)
{
	AdcChannelConfig_t *config;
	uint16_t channelIndex;

	for (channelIndex = 0u; channelIndex < AdcManChannel_LAST; ++channelIndex)
	{
		config = &_adcAllChannelConfig[channelIndex];
		if (channelIndex != (uint16_t)AdcInputChannel_TEMP)
		{
		    config->reference = ADC_REFERENCE;
		}
		else
		{
		    config->reference = ADC_THERMOMETER_REFERENCE;
		}
		config->inputChannel = (AdcInputChannel_t)(channelIndex);
		config->destination = &(_channels[channelIndex].sampleData);
	}

	Adc_Init();
	_SetupAdcForNormalMode(AdcManRef_DEFAULT);
	_initialized = true;
}

/***************************************************************************//**
    Enables an ADC channel and starts sampling it at the given rate. The data
    from each sample will be passed into the callback function. Also enables the
    ADC module if this was the first/only active channel.

    \param[in]  channel - the desired sample channel
    \param[in]  rate - the rate at which to pass a sample into the callback function
    \param[in] callback - the function to call to pass the ADC sample data to

*//****************************************************************************/
void AdcManager_EnableChannel(
		AdcManChannel_t channel,
		AdcManSampleRate_t rate,
        AdcManRef_t reference,
		AdcDataCallback_t callback)
{
	bool firstEnabledChannel;
	AdcChannelData_t *storedChannel;
	uint16_t channelCounter;
	AdcSamplesNeeded_t samplesNeeded;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
	ASSERT((channel >= AdcManChannel_FIRST) && (channel < AdcManChannel_LAST), ErrorLedMode_SOFTWARE);
	ASSERT((rate >= AdcManSampleRate_FIRST) && (rate < AdcManSampleRate_LAST), ErrorLedMode_SOFTWARE);
	ASSERT((callback != NULL), ErrorLedMode_SOFTWARE);

	firstEnabledChannel = _AllChannelsDisabled();

	/* High speed sampling ( > 400 Hz) */
	if ((rate > AdcManSampleRate_0400HZ) && (rate < AdcManSampleRate_LAST))
	{
		switch (rate)
		{
		    case AdcManSampleRate_0800HZ:
		        /* fast mode can only handle a single sample rate, make sure they are the same */
		        if (_sampleMode == AdcManSampleMode_FAST)
		        {
		            ASSERT(_timerRate == AdcTimerRate_0800HZ, ErrorLedMode_SOFTWARE);
		        }
		        _timerRate = AdcTimerRate_0800HZ;
		        samplesNeeded = AdcSamplesNeeded_0800HZ;
		        break;

			case AdcManSampleRate_2400HZ:
                /* fast mode can only handle a single sample rate, make sure they are the same */
                if (_sampleMode == AdcManSampleMode_FAST)
                {
                    ASSERT(_timerRate == AdcTimerRate_2400HZ, ErrorLedMode_SOFTWARE);
                }
				_timerRate = AdcTimerRate_2400HZ;
				samplesNeeded = AdcSamplesNeeded_2400HZ;
				break;

			case AdcManSampleRate_4800HZ:
                /* fast mode can only handle a single sample rate, make sure they are the same */
                if (_sampleMode == AdcManSampleMode_FAST)
                {
                    ASSERT(_timerRate == AdcTimerRate_4800HZ, ErrorLedMode_SOFTWARE);
                }
				_timerRate = AdcTimerRate_4800HZ;
				samplesNeeded = AdcSamplesNeeded_4800HZ;
				break;

			case AdcManSampleRate_6000HZ:
                /* fast mode can only handle a single sample rate, make sure they are the same */
                if (_sampleMode == AdcManSampleMode_FAST)
                {
                    ASSERT(_timerRate == AdcTimerRate_6000HZ, ErrorLedMode_SOFTWARE);
                }
				_timerRate = AdcTimerRate_6000HZ;
				samplesNeeded = AdcSamplesNeeded_6000HZ;
				break;

			default:
				DEBUG("invalid rate");
				break;
		}
		/* must come after previous switch, to handle the transition into fast mode */
		if (_sampleMode != AdcManSampleMode_FAST)
		{
	        _sampleMode = AdcManSampleMode_FAST;
	        _highSpeedActiveChannels = 1;
	        Adc_Config(&_fastAdcConfig);
		}
		else /* alread in high speed mode */
		{
		    ++_highSpeedActiveChannels;
		    /* in order to add a channel in high speed mode, the ADC first needs to be stopped. Use the
		       first enabled variable to turn the ADC back on */
		    _AdcStop();
		    firstEnabledChannel = true;
		    /* if multiple channels are configured for high speed, use the fast multi mode */
		    Adc_Config(&_fastMultiAdcConfig);
		    samplesNeeded = (AdcSamplesNeeded_t)1u;
		}

		/* set all the channels to be used for single channel high speed */
		for (channelCounter = ((_highSpeedActiveChannels - 1u)*(uint16_t)samplesNeeded); channelCounter < ((_highSpeedActiveChannels)*(uint16_t)samplesNeeded); ++channelCounter)
		{
			_adcAllChannelConfig[channelCounter].inputChannel = (AdcInputChannel_t)(channel);
			if ((AdcInputChannel_t)(channel) == AdcInputChannel_TEMP)
			{
			    _adcAllChannelConfig[channelCounter].reference = ADC_THERMOMETER_REFERENCE;
			}
			else
			{
			    _adcAllChannelConfig[channelCounter].reference = AdcManager_AdcManRefToAdcRef(reference);
			}
			storedChannel = &_channels[channelCounter];

			storedChannel->callback = callback;
			storedChannel->skipSamples = _GetSamplesToSkip(rate);
			storedChannel->skippedSamples = 0u;
		}

		/* make sure all channels after this last added channel are not in use */
		for (channelCounter = channelCounter; channelCounter < (uint16_t)AdcManChannel_LAST; ++channelCounter)
		{
            _channels[channelCounter].callback = NULL;
		}

		_SetupTimerConfig();
	}
	/* Normal speed ADC sampling ( <= 400 Hz) */
	else if ((rate >= AdcManSampleRate_FIRST) && (rate <= AdcManSampleRate_0400HZ))
	{
		if (_sampleMode != AdcManSampleMode_NORMAL)
		{
		    _SetupAdcForNormalMode(reference);
		}
		else
		{
		    _SetupAdcSingleChannel(channel, reference);
		}

		storedChannel = &_channels[channel];

		storedChannel->callback = callback;
		storedChannel->skipSamples = _GetSamplesToSkip(rate);
		storedChannel->skippedSamples = 0u;
	}
	else
	{
		DEBUG("invalid sample rate");
	}

	AdcManager_PoweredChannelControl(channel, true);

	if (firstEnabledChannel)
	{
		_AdcStart();
	}
	else
	{
		DEBUG("adc running");
	}
}

/***************************************************************************//**
    Disables an ADC channel and clears its callback pointer. Also disables the
    ADC module if this was the last/only active channel.

    \param[in]  channel - the desired sample channel to stop

*//****************************************************************************/
void AdcManager_DisableChannel(AdcManChannel_t channel)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
	ASSERT((channel >= AdcManChannel_FIRST) && (channel < AdcManChannel_LAST), ErrorLedMode_SOFTWARE);

	uint16_t channelCounter;
	bool channelWasDisabled = false;

	if (_sampleMode != AdcManSampleMode_NORMAL)
	{
		/* all channels could be set to a single input for high speed sampling */
		for (channelCounter = 0; channelCounter < AdcManChannel_LAST; ++channelCounter)
		{
		    /* disable channels whose input is the same as the disabled channel */
		    if (_adcAllChannelConfig[channelCounter].inputChannel == (AdcInputChannel_t)channel)
		    {
		        _channels[channelCounter].callback = NULL;
		        channelWasDisabled = true;
		    }
		}

		if (channelWasDisabled)
		{
		    --_highSpeedActiveChannels;
		}
	}
	else
	{
		_channels[channel].callback = NULL;
	}

	AdcManager_PoweredChannelControl(channel, false);

	if (_AllChannelsDisabled())
	{
		_AdcStop();

		/* always return to the NORMAL ADC mode when finished sampling. */
		_SetupAdcForNormalMode(AdcManRef_DEFAULT);
	}
	else
	{
		DEBUG("adc stopped");
	}
}

/***************************************************************************//**
    Takes a single sample of an ADC channel and returns the raw ADC counts

    \param[in] channel - the ADC channel which to sample

    \returns the value in raw ADC counts of the ADC channel.

*//****************************************************************************/
uint16_t AdcManager_SingleSampleBlocking(AdcManChannel_t channel)
{
	AdcChannelConfig_t config;
	uint16_t adcResult;

	config.destination = &adcResult;

	/* make sure the channel is valid before using it */
	if ((channel < AdcManChannel_LAST) && (channel >= AdcManChannel_FIRST))
	{
		config.inputChannel = (AdcInputChannel_t)channel;

		/* make sure the ADC isn't already busy */
		if (_active == true)
		{
			adcResult = 0u;
			DEBUG("sorry, ADC is busy");
		}
		else
		{
			/* set the channel you want to sample and sample it*/
		    if (channel != AdcManChannel_TEMPERATURE)
		    {
		        config.reference = ADC_REFERENCE;
		        Adc_SetChannelSequence(&config, 1u, AdcReferenceLevel_DONT_CARE);
		    }
		    else
		    {
		        config.reference = AdcReference_VREFp_AVSS;
		        Adc_SetChannelSequence(&config, 1u, ADC_THERMOMETER_REFERENCE_VOLTAGE);
		    }
			Adc_SingleConversionSequenceBlocking();
			/* reset the ADC so that it will work for all channels */
			_SetupTimerConfig();
		}
	}
	else
	{
		DEBUG("invalid ADC channel");
		adcResult = 0u;
	}

	return adcResult;
}

/***************************************************************************//**
    Should be called by the event handler for processing the ADC conversion
    complete event. This function will take the converted ADC data from the buffer
    and call the ADC sample callback functions at the desired rate.
*//****************************************************************************/
void AdcManager_ProcessConversion(void)
{
	uint16_t channelIndex;
	AdcChannelData_t *channelData;
	volatile uint16_t num;
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
	num = Adc_GetNumberOfConversions();
	if (num > 1u)
	{
		Serial_SendStringNumber("missed ADC:", num);
	}

	for (channelIndex = 0; channelIndex < AdcManChannel_LAST; ++channelIndex)
	{
		channelData = &_channels[channelIndex];
		if (channelData->callback != NULL)
		{
			++(channelData->skippedSamples);
			if (channelData->skippedSamples >= channelData->skipSamples)
			{
				channelData->skippedSamples = 0u;
				(*(channelData->callback))(channelData->sampleData);
			}
		}
	}

}

/***************************************************************************//**
    Gets the ADC gain factor that can be used to scale the ADC readings for
    higher accuracy.

    \returns The ADC gain factor

*//****************************************************************************/
uint16_t AdcManager_GetGainFactor(void)
{
	return Adc_GetGainFactor();
}

/***************************************************************************//**
    Gets the ADC offset that can be used to shift the ADC readings for higher
    accuracy.

    \returns The ADC offset

*//****************************************************************************/
uint16_t AdcManager_GetOffset(void)
{
	return Adc_GetOffset();
}

/***************************************************************************//**
    Accepts an ADC sample rate and converts that rate into the number of ADC
    samples that must be skipped in able to achieve that sample rate.

    \param[in]  sampleRate - The desired ADC sample rate

    \returns The number of samples to skip to get the desired sample rate

*//****************************************************************************/
EveryXSamples_t _GetSamplesToSkip(AdcManSampleRate_t sampleRate)
{
	EveryXSamples_t samplesToSkip;
	switch (sampleRate)
	{
	case AdcManSampleRate_0001HZ:
		samplesToSkip = EveryXSamples_0001;
		break;

	case AdcManSampleRate_0010HZ:
		samplesToSkip = EveryXSamples_0010;
		break;

	case AdcManSampleRate_0050HZ:
		samplesToSkip = EveryXSamples_0050;
		break;

	case AdcManSampleRate_0100HZ:
		samplesToSkip = EveryXSamples_0100;
		break;

	case AdcManSampleRate_0200HZ:
		samplesToSkip = EveryXSamples_0200;
		break;

	case AdcManSampleRate_0400HZ:
        samplesToSkip = EveryXSamples_0400;
        break;

	case AdcManSampleRate_0800HZ:
	    samplesToSkip = EveryXSamples_0800;
	    break;

	case AdcManSampleRate_2400HZ:
		samplesToSkip = EveryXSamples_2400;
		break;

	case AdcManSampleRate_4800HZ:
		samplesToSkip = EveryXSamples_4800;
		break;

	case AdcManSampleRate_6000HZ:
		samplesToSkip = EveryXSamples_6000;
		break;

	default:
		samplesToSkip = EveryXSamples_INVLALID;
		break;
	}
	return samplesToSkip;
}

/***************************************************************************//**
	Checks to see if all of the ADC channels are disabled.

	\returns True if all channels are disabled, False otherwise

*//****************************************************************************/
bool _AllChannelsDisabled(void)
{
	bool channelIsEnabled = false;
	int16_t index;

	for (index = AdcManChannel_LAST - 1; index >= AdcManChannel_FIRST; --index)
	{
		if (_channels[index].callback != NULL)
		{
			channelIsEnabled = true;
			break;
		}
	}
	return (channelIsEnabled == false);
}

/***************************************************************************//**
	Initializes all of the ADC channels to be used in the ADC driver.

	\param[in] x - the number of channels to initialize the ADC to sample

*//****************************************************************************/
static __inline void _InitialzeXAdcChannels(uint16_t x)
{
	ASSERT(x <= AdcManChannel_LAST, ErrorLedMode_SOFTWARE);
	Adc_SetChannelSequence(_adcAllChannelConfig, x, ADC_THERMOMETER_REFERENCE_VOLTAGE);
}

/***************************************************************************//**
	Sets up the array of channel configs with the proper settings for the current
	mode.
*//****************************************************************************/
void _SetupTimerConfig(void)
{
    uint16_t numberChannelsNeeded;

	/* stop the timer before configuring it */
	TIMER_A0_STOP();

	/* Configure the timer to run at 4Hz. ACLK/64 = 187.5kHz, 46875 count = 250ms */
	/* Configure the timer to run at 100Hz.  ACLK/2 = 6MHz, 60000 count = 10ms    */
	/* Configure the timer to run at 400Hz.  ACLK/2 = 6MHz, 15000 count = 2.5ms   */
	/* Configure the timer to run at 800Hz.  ACLK/2 = 6MHz, 7500 count = 1.25ms   */
	/* Configure the timer to run at 2400Hz. ACLK/2 = 6MHz, 2500 count = 416.6us  */
	/* Configure the timer to run at 4800Hz. ACLK/2 = 6MHz, 1250 count = 208.3us  */
	/* Configure the timer to run at 6kHz.   ACLK/2 = 6MHz, 1000 count = 166.7us  */

	/* NOTE: Set the number of channels such that the interrupt to service all the conversions
	   doesn't occur faster than 400 Hz.
	*/

	switch (_timerRate)
	{
		case AdcTimerRate_0400HZ:
			Timer_A0_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_UP, 15000u - 1u);
			Timer_A0CCX_Configure(TimerA0CC_1, OUTMOD_3, 1000u - 1u);
			numberChannelsNeeded = (_highSpeedActiveChannels > 1) ? _highSpeedActiveChannels : AdcSamplesNeeded_0400HZ * _highSpeedActiveChannels;
			_InitialzeXAdcChannels(numberChannelsNeeded);
			break;

		case AdcTimerRate_0800HZ:
		    Timer_A0_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_UP, 7500 - 1u);
		    Timer_A0CCX_Configure(TimerA0CC_1, OUTMOD_3, 1000u - 1u);
            numberChannelsNeeded = (_highSpeedActiveChannels > 1) ? _highSpeedActiveChannels : AdcSamplesNeeded_0800HZ * _highSpeedActiveChannels;
		    /* 800Hz / 100Hz = 2 samples per frame, 2 sample fifo = 400Hz interrupt rate */
		    _InitialzeXAdcChannels(numberChannelsNeeded);
		    break;

		case AdcTimerRate_2400HZ:
			Timer_A0_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_UP, 2500u - 1u);
			Timer_A0CCX_Configure(TimerA0CC_1, OUTMOD_3, 1000u - 1u);
			numberChannelsNeeded = (_highSpeedActiveChannels > 1) ? _highSpeedActiveChannels : AdcSamplesNeeded_2400HZ * _highSpeedActiveChannels;
			/* 2400Hz / 100Hz = 24 samples per frame, 6 sample fifo = 400Hz interrupt rate */
			_InitialzeXAdcChannels(numberChannelsNeeded);
			break;

		case AdcTimerRate_4800HZ:
			Timer_A0_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_UP, 1250u - 1u);
			Timer_A0CCX_Configure(TimerA0CC_1, OUTMOD_3, 1000u - 1u);
            numberChannelsNeeded = (_highSpeedActiveChannels > 1) ? _highSpeedActiveChannels : AdcSamplesNeeded_4800HZ * _highSpeedActiveChannels;
			/* 4800Hz / 100Hz = 48 samples per frame, 12 sample fifo = 400Hz interrupt rate */
			_InitialzeXAdcChannels(numberChannelsNeeded);
			break;

		case AdcTimerRate_6000HZ:
			Timer_A0_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_UP, 1000u - 1u);
			Timer_A0CCX_Configure(TimerA0CC_1, OUTMOD_3, 1000u - 1u);
            numberChannelsNeeded = (_highSpeedActiveChannels > 1) ? _highSpeedActiveChannels : AdcSamplesNeeded_6000HZ * _highSpeedActiveChannels;
			/* 6000Hz / 100Hz = 60 samples per frame, 15 sample fifo = 400Hz interrupt rate */
			_InitialzeXAdcChannels(numberChannelsNeeded);
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;

	}
}

/***************************************************************************//**
    Takes an ADC channel and if it is a powered channel will enable or disable
    the power rail for the channel.

    \NOTE This function should only be used internally
    to the ADC manager, except for board level test and debugging purposes.

    \param[in] channel - the channel that should be enabled or disabled
    \param[in] enable  - true to enable, false to disable

*//****************************************************************************/
void AdcManager_PoweredChannelControl(AdcManChannel_t channel, bool enable)
{
	switch(channel)
	{
		case AdcManChannel_FORCE_GAUGE:
			_poweredSensorsEnabled = enable ?
					(AdcPoweredChannel_t)(_poweredSensorsEnabled |  AdcPoweredChannel_FORCE_GAUGE) :
					(AdcPoweredChannel_t)(_poweredSensorsEnabled & ~AdcPoweredChannel_FORCE_GAUGE);
			break;

		case AdcManChannel_AMBIENT_LIGHT:
			_poweredSensorsEnabled = enable ?
					(AdcPoweredChannel_t)(_poweredSensorsEnabled |  AdcPoweredChannel_AMBIENT_LIGHT) :
					(AdcPoweredChannel_t)(_poweredSensorsEnabled & ~AdcPoweredChannel_AMBIENT_LIGHT);
			break;

		case AdcManChannel_MICROPHONE:
			_poweredSensorsEnabled = enable ?
					(AdcPoweredChannel_t)(_poweredSensorsEnabled |  AdcPoweredChannel_MICROPHONE) :
					(AdcPoweredChannel_t)(_poweredSensorsEnabled & ~AdcPoweredChannel_MICROPHONE);
			break;

		default:
			/* do nothing */
			break;
	}

	/* if any powered channel is enabled, the rail must be enabled */
	if (_poweredSensorsEnabled != AdcPoweredChannel_NONE)
	{
		/* ENABLE */
		/* active low output for V2 */
		GPIO_CLEAR(POWER_ADC_V2_ENABLE_PORT, POWER_ADC_V2_ENABLE_PIN);
		/* active high output for V1 */
		GPIO_SET(POWER_ADC_V1_ENABLE_PORT, POWER_ADC_V1_ENABLE_PIN);
	}
	else
	{
		/* DISABLE */
		/* active low output for V2 */
		GPIO_SET(POWER_ADC_V2_ENABLE_PORT, POWER_ADC_V2_ENABLE_PIN);
		/* active high output for V1 */
		GPIO_CLEAR(POWER_ADC_V1_ENABLE_PORT, POWER_ADC_V1_ENABLE_PIN);
	}
}

/***************************************************************************//**
    Takes an ADC manager reference and converts it to an ADC reference

    \NOTE This function should only be used internally
    to the ADC manager, except for board level test and debugging purposes.

    \param[in] adcManRef - the AdcManRef_t ADC Manager analog reference selection
    \returns The AdcReference_t reference for the ADC driver

*//****************************************************************************/
AdcReference_t AdcManager_AdcManRefToAdcRef(
        AdcManRef_t adcManRef)
{
    AdcReference_t adcReference;

    switch(adcManRef)
    {
        case AdcManRef_VREF:
            adcReference = AdcReference_eVREFp_AVSS;
            break;

        case AdcManRef_VCC:
            adcReference = AdcReference_AVCC_AVSS;
            break;

        default:
            adcReference = ADC_REFERENCE;
            break;
    }

    return adcReference;
}

/***************************************************************************//**
    Starts the ADC and its timer.
*//****************************************************************************/
static __inline void _AdcStart(void)
{
    Adc_StartConversionSequenceOfChannels();
    TIMER_A0_START(TimerRunMode_UP);
    _active = true;
    DEBUG("adc run");
}

/***************************************************************************//**
    Stops the ADC and its timer.
*//****************************************************************************/
static __inline void _AdcStop(void)
{
    TIMER_A0_STOP();
    Adc_StopConversion();
    _active = false;
    DEBUG("adc stop");
}

/***************************************************************************//**
    Sets the proper variables, timers, and ADC settings for the normal sampling
    mode.
*//****************************************************************************/
static __inline void _SetupAdcForNormalMode(
        AdcManRef_t reference)
{
    uint16_t samplesNeeded;
    uint16_t channelCounter;

    _sampleMode = AdcManSampleMode_NORMAL;
    _highSpeedActiveChannels = 1u;
    _timerRate = AdcTimerRate_0400HZ;
    samplesNeeded = AdcSamplesNeeded_0400HZ;
    Adc_Config(&_normAdcConfig);

    /* set all channels to different ADC inputs */
    for (channelCounter = 0u; channelCounter < (uint16_t)samplesNeeded; ++channelCounter)
    {
        _adcAllChannelConfig[channelCounter].inputChannel = (AdcInputChannel_t)(channelCounter);
        if ((AdcInputChannel_t)(channelCounter) == AdcInputChannel_TEMP)
        {
            _adcAllChannelConfig[channelCounter].reference = ADC_THERMOMETER_REFERENCE;
        }
        else
        {
            _adcAllChannelConfig[channelCounter].reference = AdcManager_AdcManRefToAdcRef(reference);
        }
    }

    _SetupTimerConfig();
}

static __inline void _SetupAdcSingleChannel(
        AdcManChannel_t channel,
        AdcManRef_t reference)
{
    bool adcWasActive = _active;
    if (adcWasActive)
    {
        _AdcStop();
    }

    Serial_SendString3Numbers("AdcMan Single:", channel, reference, 0);
    _adcAllChannelConfig[channel].inputChannel = (AdcInputChannel_t)(channel);
    if (channel != AdcManChannel_TEMPERATURE)
    {
        _adcAllChannelConfig[channel].reference = AdcManager_AdcManRefToAdcRef(reference);
    }
    else
    {
        _adcAllChannelConfig[channel].reference = ADC_THERMOMETER_REFERENCE;
    }

    _SetupTimerConfig();

    if (adcWasActive)
    {
        _AdcStart();
    }
}
