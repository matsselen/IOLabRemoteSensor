/***************************************************************************//**
    \file Adc.c
    \brief Analog-to-digital conversion module driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions to setup and operate the
       12-bit ADC hardware on the MSP430.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Adc.c$
    $Revision: 10$
    $Date: Friday, October 24, 2014 10:53:12 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "Adc.h"
#include <EventQueue.h>
#include <Assert.h>
#include <Debug.h>
#include <Board.h>
#include <Gpio.h>
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Maximum value of an ADC result */
#define ADC_MAX_RESULT                  (4095u)

/** Scale factor used to improve accuracy in temperature calculations */
#define ADC_TEMP_SCALE_FACTOR           (256)

/** Length (in bytes) of the ADC12 calibration section in the TLV structure */
#define ADC_CAL_TLV_LENGTH              (16u)
/** Length (in bytes) of the REF calibration section in the TLV structure */
#define ADC_REF_CAL_TLV_LENGTH          (6u)

/** Structure to hold the ADC calibration info */
typedef struct AdcCalibrationInfo
{
    uint16_t gainFactor;
    int16_t offset;
    uint16_t temp30Ref15V;
    uint16_t temp85Ref15V;
    uint16_t temp30Ref20V;
    uint16_t temp85Ref20V;
    uint16_t temp30Ref25V;
    uint16_t temp85Ref25V;
} AdcCalibrationInfo_t;

/** Structure to hold the internal voltage reference calibration info */
typedef struct AdcReferenceCalibrationInfo
{
    uint16_t ref15VFactor;
    uint16_t ref20VFactor;
    uint16_t ref25VFactor;
} AdcReferenceCalibrationInfo_t;

/** Possible results when getting the ADC calibration info */
typedef enum AdcCalInfoResult
{
    AdcCalInfoResult_FAILURE,
    AdcCalInfoResult_SUCCESS
} AdcCalInfoResult_t;

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Indicates if the ADC module was initialized */
static volatile bool _initialized = false;

/** Indicates if a conversion is currently in progress. */
static volatile bool _conversionInProgress = false;

/** Stores the user-defined destinations where the conversion results should
    be copied to.
*/
static uint16_t* _resultDestinations[ADC_SEQUENCE_MAX_LENGTH] =
{
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL
};

/** Tracks the number of channels that are configured to be used in a
    conversion sequence.
*/
static uint16_t _numChannelsConfigured = 0u;

/** Indicates if the current sequence accesses the internal temperature
    sensor.
*/
static bool _useTempSensor = false;
/** Indicates if the current sequence requires the internal voltage
    reference.
*/
static bool _useInternalReference = false;
/** Tracks the currently selected internal voltage reference level */
static AdcReferenceLevel_t _selectedReferenceLevel = AdcReferenceLevel_1p5_VOLTS;

/** Stores the number of conversions that have occurred in the system */
static uint16_t _numberOfConversions = 0u;

/** Stores the ADC calibration info */
static AdcCalibrationInfo_t _adcCalibrationInfo;

/** Stores the internal voltage reference calibration info */
static AdcReferenceCalibrationInfo_t _refCalibrationInfo;

/** Stores the temperature sensor gain factor computed from the calibration
    values for the internal 1.5V reference level.
*/
static uint16_t _temperatureGainFactorRef15V = 0u;
/** Stores the temperature sensor gain factor computed from the calibration
    values for the internal 2.0V reference level.
*/
static uint16_t _temperatureGainFactorRef20V = 0u;
/** Stores the temperature sensor gain factor computed from the calibration
    values for the internal 2.5V reference level.
*/
static uint16_t _temperatureGainFactorRef25V = 0u;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
void _MoveSamplesToUserLocation(void);
__interrupt void _AdcIsr(void);
static AdcCalInfoResult_t _GetAdcCalibrationInfo(AdcCalibrationInfo_t* calInfo);
static AdcCalInfoResult_t _GetReferenceCalibrationInfo(AdcReferenceCalibrationInfo_t* calInfo);

/***************************************************************************//**
    This function initializes the ADC variables.

    Each channel is sampled for about 3 microseconds.  Each conversion
    (sample time + conversion time) takes about 6 microseconds.

    \note The reference voltage takes 75 microseconds to settle (typical)
    and the temperature sensor, which is sourced by the reference voltage,
    requires a minimum sampling time of 30 microseconds.
*//****************************************************************************/
void Adc_Init(void)
{
    AdcCalInfoResult_t result;
	/** Stores the default ADC configuration for the ADC module */
	const AdcConfig_t config = { AdcConSeq_SEQ_OF_CHAN, AdcSht_1024, true };
    uint16_t diff;

	Adc_Config(&config);

    result = _GetAdcCalibrationInfo(&_adcCalibrationInfo);
    ASSERT(result == AdcCalInfoResult_SUCCESS, ErrorLedMode_SOFTWARE);
    result = _GetReferenceCalibrationInfo(&_refCalibrationInfo);
    ASSERT(result == AdcCalInfoResult_SUCCESS, ErrorLedMode_SOFTWARE);

    /* Calculate temperature gain factor (scaled for accuracy) for each
       internal voltage reference level using calibration values taken at
       30C and 85C.

                     (85 - 30)(Scale Factor)
       gainFactor = ------------------------
                        Cal@85 - Cal@30

       Note that adding half of the denominator to the numerator before
       dividing rounds the value to the nearest.
    */
    diff = _adcCalibrationInfo.temp85Ref15V - _adcCalibrationInfo.temp30Ref15V;
    _temperatureGainFactorRef15V = ((((uint16_t) 55u) * ((uint16_t) ADC_TEMP_SCALE_FACTOR)) + (diff / 2u)) / diff;

    diff = _adcCalibrationInfo.temp85Ref20V - _adcCalibrationInfo.temp30Ref20V;
    _temperatureGainFactorRef20V = ((((uint16_t) 55u) * ((uint16_t) ADC_TEMP_SCALE_FACTOR)) + (diff / 2u)) / diff;

    diff = _adcCalibrationInfo.temp85Ref25V - _adcCalibrationInfo.temp30Ref25V;
    _temperatureGainFactorRef25V = ((((uint16_t) 55u) * ((uint16_t) ADC_TEMP_SCALE_FACTOR)) + (diff / 2u)) / diff;
#if 0 && defined(SERIAL_ENABLE_PRINTF)
    DEBUG("1.5,30:%d", _adcCalibrationInfo.temp30Ref15V);
    DEBUG("1.5,85:%d", _adcCalibrationInfo.temp85Ref15V);
    DEBUG("2.0,30:%d", _adcCalibrationInfo.temp30Ref20V);
    DEBUG("2.0,85:%d", _adcCalibrationInfo.temp85Ref20V);
    DEBUG("2.5,30:%d", _adcCalibrationInfo.temp30Ref25V);
    DEBUG("2.5,85:%d", _adcCalibrationInfo.temp85Ref25V);
#endif
    _initialized = true;
}

/***************************************************************************//**
    Configures the ADC module to use the specific ADC global configuration
    settings

    \param[in] config - Configuration information for the ADC module

*//****************************************************************************/
void Adc_Config( const AdcConfig_t *config)
{
    /* Disable ADC conversion interrupts */
    ADC12IE = 0u;

    /* ADC12 control 0:
       Don't care about reference voltage
       Reference generator off
       ADC on
       Stop conversions and disable overflow interrupts
    */
    ADC12CTL0 = ADC12ON;

    /* mask off and set the sample and hold time */
    ADC12CTL0 |= (config->sampleHoldTime * ADC12SHT10) & (ADC12SHT13|ADC12SHT12|ADC12SHT11|ADC12SHT10);
	ADC12CTL0 |= (config->sampleHoldTime * ADC12SHT00) & (ADC12SHT03|ADC12SHT02|ADC12SHT01|ADC12SHT00);

    /* use the config to set the multiple sample and conversion enable */
    if (config->multipleSampleConversion)
    {
    	ADC12CTL0 |= ADC12MSC;
    }
    else
    {
    	ADC12CTL0 &= ~ADC12MSC;
    }

    /* ADC12 control 1:
       Start with first conversion in sequence
       Use TA0.1 compare bit to start conversions
       Sample and hold signal from the sample timer
       Do not invert sample-and-hold signal
       Divide clock source by 1
       Use ACLK as source (12 MHz)
    */
    ADC12CTL1 = ADC12CSTARTADD_0 | ADC12SHS_1 | ADC12SHP |
                ADC12DIV_0 | ADC12SSEL_1;

    /* clear the conversion sequence mode select */
    ADC12CTL1 &= ~(ADC12CONSEQ1 | ADC12CONSEQ0);
    /* mask off the config use it to set the conversion sequence mode select */
    ADC12CTL1 |= (config->conSeq * ADC12CONSEQ0) & (ADC12CONSEQ1 | ADC12CONSEQ0);

    /* ADC12 control 2:
       Pre-divide the clock source by 4 (12MHz / 4 = 3MHz)
       Temperature sensor controlled by REFCTL0
       12-bit ADC resolution
       Binary data format
       Reference buffer supports slow sampling rate
       Reference output controlled by REFCTL0
       Reference buffer in burst mode
    */
    ADC12CTL2 = ADC12PDIV | ADC12RES_2 | ADC12SR | ADC12REFBURST;
}

/***************************************************************************//**
    \return True if a conversion is in progress, false otherwise.
*//****************************************************************************/
bool Adc_IsConversionInProgress(void)
{
    return _conversionInProgress;
}

/***************************************************************************//**
    Configures the ADC module to sample the given sequence of channels.

    \param[in] config - Configuration information for each channel in
                        the sequence.
    \param[in] numChannels - Number of channels in the sequence.
*//****************************************************************************/
void Adc_SetChannelSequence(
    const AdcChannelConfig_t config[],
    uint16_t numChannels,
    AdcReferenceLevel_t refLevel)
{
    /** Look-up table for the conversion memory control registers */
    static volatile uint8_t* const _conversionMemoryControlRegisters[ADC_NUM_CONVERSION_MEMORY_REGISTERS] =
    {
        &ADC12MCTL0,
        &ADC12MCTL1,
        &ADC12MCTL2,
        &ADC12MCTL3,
        &ADC12MCTL4,
        &ADC12MCTL5,
        &ADC12MCTL6,
        &ADC12MCTL7,
        &ADC12MCTL8,
        &ADC12MCTL9,
        &ADC12MCTL10,
        &ADC12MCTL11,
        &ADC12MCTL12,
        &ADC12MCTL13,
        &ADC12MCTL14,
        &ADC12MCTL15,
    };

    volatile uint8_t* conversionControlReg;
    AdcInputChannel_t channel;
    AdcReference_t reference;
    uint16_t value;
    uint16_t i;

    ASSERT(_conversionInProgress == false, ErrorLedMode_SOFTWARE);
    ASSERT(_initialized, ErrorLedMode_SOFTWARE);
    ASSERT(config != NULL, ErrorLedMode_SOFTWARE);
    ASSERT((numChannels > 0u) && (numChannels <= ADC_SEQUENCE_MAX_LENGTH), ErrorLedMode_SOFTWARE);

    _useTempSensor = false;
    _useInternalReference = false;

    /* Store config info */
    for (i = 0u; i < numChannels; ++i)
    {
        conversionControlReg = _conversionMemoryControlRegisters[i];

        *conversionControlReg = (((uint8_t) config[i].reference) * ADC12SREF0) +
                                (uint8_t) config[i].inputChannel;

        _resultDestinations[i] = config[i].destination;

        /* Check if internal voltage reference is requested */
        reference = config[i].reference;
        if ((reference == AdcReference_VREFp_AVSS) || (reference == AdcReference_VREFp_VREFm))
        {
            _useInternalReference = true;

            /* verify that the specified reference level is valid */
            ASSERT(refLevel < AdcReferenceLevel_DONT_CARE, ErrorLedMode_SOFTWARE);
            _selectedReferenceLevel = refLevel;
        }

        channel = config[i].inputChannel;
        if (channel == AdcInputChannel_TEMP)
        {
            _useTempSensor = true;

            /* When using the internal temperature sensor, the internal voltage reference must be used for
               the conversion function to work correctly. */
            ASSERT(_useInternalReference, ErrorLedMode_SOFTWARE);
        }
    }

    /* REF control 0:
       Reference module controlled by this register.
       Use selected reference.
       Temperature sensor on or off depending on use of temp sensor in sequence.
       Reference output is disabled.
       Reference enabled or disabled depending on use of temp sensor in sequence.
    */
    value = REFMSTR | (((uint16_t) _selectedReferenceLevel) * REFVSEL0) | REFTCOFF;
    if (_useInternalReference)
    {
        value |= REFON;
    }
    if (_useTempSensor)
    {
        value &= ((uint16_t) ~(REFTCOFF));
    }
    REFCTL0 = value;

    /* Mark last channel in sequence */
    conversionControlReg = _conversionMemoryControlRegisters[numChannels - 1u];
    *conversionControlReg = *conversionControlReg | ADC12EOS;

    _numChannelsConfigured = numChannels;

    /* Clear old interrupts. */
    ADC12IFG = 0u;

    /* Enable interrupt for last channel in sequence */
    ADC12IE = (uint16_t) (((uint16_t) 1u) << (_numChannelsConfigured - 1u));
}

/***************************************************************************//**
    Starts the conversion of a sequence of channels.
*//****************************************************************************/
void Adc_StartConversionSequenceOfChannels(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
    ASSERT(_conversionInProgress == false, ErrorLedMode_SOFTWARE);
    ASSERT((_numChannelsConfigured > 0u) && (_numChannelsConfigured <= ADC_SEQUENCE_MAX_LENGTH), ErrorLedMode_SOFTWARE);

    /* Start conversion */
    ADC12CTL0 |= ADC12ENC;

    _conversionInProgress = true;
}

/***************************************************************************//**
    Starts the conversion of a sequence of channels and blocks until they are
    finished.
*//****************************************************************************/
void Adc_SingleConversionSequenceBlocking(void)
{
	uint16_t interruptSettings = 0;
	uint16_t i = 0;
	uint16_t ctrl0Settings;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	/* make sure the ADC isn't currently busy, but don't wait forever */
	while(ADC12CTL1 & ADC12BUSY)
	{
		++i;
		ASSERT(i < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
	}

    ASSERT(_conversionInProgress == false, ErrorLedMode_SOFTWARE);
    ASSERT((_numChannelsConfigured > 0u) && (_numChannelsConfigured <= ADC_SEQUENCE_MAX_LENGTH), ErrorLedMode_SOFTWARE);

	/* store interrupt and control settings and disable ATD interrupts */
	interruptSettings = ADC12IE;
	ADC12IE = 0u;
	ctrl0Settings = ADC12CTL1;
	/* set the ADC to use a software trigger (clear the bits)*/
	ADC12CTL1 &= ~(ADC12SHS0 | ADC12SHS1);

	/* start the conversion */
	ADC12CTL0 |= ADC12ENC | ADC12SC;

	/* wait until the conversion is done */
	while(ADC12CTL1 & ADC12BUSY)
	{
		/* do nothing */
	}

	_MoveSamplesToUserLocation();
	/* clear the enable conversion bit because the conversion is complete */
	ADC12CTL0 &= ((uint8_t)~ADC12ENC);

	/* restore the interrupt and control settings */
	ADC12IE = interruptSettings;
	ADC12CTL1 = ctrl0Settings;
}

/***************************************************************************//**
    Stops the ADC from sampling.
*//****************************************************************************/
void Adc_StopConversion(void)
{
	ADC12CTL0 &= ((uint16_t)~(ADC12ENC | ADC12SC));
    _conversionInProgress = false;
}

/***************************************************************************//**
    Returns the number of conversions that have occurred since this function
    was last called.
*//****************************************************************************/
extern uint16_t Adc_GetNumberOfConversions(void)
{
	uint16_t number = _numberOfConversions;
	_numberOfConversions = 0u;
	return number;
}
/***************************************************************************//**
    Moves the samples from the sample buffer to the user memory locations.
*//****************************************************************************/
void _MoveSamplesToUserLocation(void)
{
    /** Look-up table for the conversion memory registers */
    static volatile uint16_t* const _conversionMemoryRegisters[ADC_NUM_CONVERSION_MEMORY_REGISTERS] =
    {
        &ADC12MEM0,
        &ADC12MEM1,
        &ADC12MEM2,
        &ADC12MEM3,
        &ADC12MEM4,
        &ADC12MEM5,
        &ADC12MEM6,
        &ADC12MEM7,
        &ADC12MEM8,
        &ADC12MEM9,
        &ADC12MEM10,
        &ADC12MEM11,
        &ADC12MEM12,
        &ADC12MEM13,
        &ADC12MEM14,
        &ADC12MEM15
    };

    uint16_t i;

	/* Move conversion results to user location */
	for (i = 0u; i < _numChannelsConfigured; ++i)
	{
		*_resultDestinations[i] = *_conversionMemoryRegisters[i];
	}
	_conversionInProgress = false;
}

/***************************************************************************//**
    Gets the ADC gain factor that can be used to scale the ADC readings for
    higher accuracy.

    \returns The ADC gain factor

*//****************************************************************************/
uint16_t Adc_GetGainFactor(void)
{
	static const uint32_t* const gainFactor = (uint32_t*)DEVICE_ADC12_GAIN_FACTOR_ADDRESS;
	return *gainFactor;
}

/***************************************************************************//**
    Gets the ADC offset that can be used to shift the ADC readings for higher
    accuracy.

    \returns The ADC offset

*//****************************************************************************/
uint16_t Adc_GetOffset(void)
{
	static const uint32_t* const offset= (uint32_t*)DEVICE_ADC12_OFFSET_ADDRESS;
	return *offset;
}

/***************************************************************************//**
    Gets the ADC calibration that is used for temperature conversion for more
    accurate readings.

    \param[out] cal85C - the calibration value at 85°C
    \param[out] cal30C - the calibration value at 30°C

*//****************************************************************************/
void Adc_GetTempCalib(uint16_t *cal85C, uint16_t *cal30C)
{
    ASSERT((cal85C != NULL) && (cal30C != NULL), ErrorLedMode_SOFTWARE);
    switch(_selectedReferenceLevel)
    {
        case AdcReferenceLevel_1p5_VOLTS:
            (*cal85C) = _adcCalibrationInfo.temp85Ref15V;
            (*cal30C) = _adcCalibrationInfo.temp30Ref15V;
            break;

        case AdcReferenceLevel_2p0_VOLTS:
            (*cal85C) = _adcCalibrationInfo.temp85Ref20V;
            (*cal30C) = _adcCalibrationInfo.temp30Ref20V;
            break;

        case AdcReferenceLevel_2p5_VOLTS:
            (*cal85C) = _adcCalibrationInfo.temp85Ref25V;
            (*cal30C) = _adcCalibrationInfo.temp30Ref25V;
            break;

        default:
            (*cal85C) = 0u;
            (*cal30C) = 0u;
            break;
    }
}

/***************************************************************************//**
    Converts an ADC raw value to a temperature in degrees Celsius.

    The raw ADC value should not be corrected in any way before being passed to
    this function, as the calibration values used to perform the conversion take
    the other corrections into account.

    \param[in] value - Raw ADC value to convert.

    \return The temperature in degrees Celsius.
*//****************************************************************************/
int16_t Adc_ConvertValueToTemp(uint16_t value)
{
    int16_t calValueAt30C = 0;
    int16_t tempGainFactor = 0;
    int16_t valueSigned;
    int16_t temperature;

    ASSERT(value <= ADC_MAX_RESULT, ErrorLedMode_SOFTWARE);
    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    /* Get the required values for the selected reference level */
    switch (_selectedReferenceLevel)
    {
        case AdcReferenceLevel_1p5_VOLTS:
            calValueAt30C = (int16_t) _adcCalibrationInfo.temp30Ref15V;
            tempGainFactor = (int16_t) _temperatureGainFactorRef15V;
            break;

        case AdcReferenceLevel_2p0_VOLTS:
            calValueAt30C = (int16_t) _adcCalibrationInfo.temp30Ref20V;
            tempGainFactor = (int16_t) _temperatureGainFactorRef20V;
            break;

        case AdcReferenceLevel_2p5_VOLTS:
            calValueAt30C = (int16_t) _adcCalibrationInfo.temp30Ref25V;
            tempGainFactor = (int16_t) _temperatureGainFactorRef25V;
            break;

        case AdcReferenceLevel_DONT_CARE:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    /* The equation used to relate the ADC value (sensor voltage) to temperature
       (in Celsius) is as follows:
           Vsense = TCsensor x Temp + Vsensor@0C
               where TCsensor is the sensor's gain (mV/degC)
                     Vsensor@0C is the sensor voltage at 0C
                     Vsense is the sensor voltage at Temp

       In order to use the calibration values taken at 30C and 85C, the equation
       can be modified as follows:
                     Cal@85 - Cal@30
           Vsense = ----------------- x (Temp - 30) + Cal@30
                         85 - 30

       Solving for the temperature gives:
                       85 - 30
           Temp = ----------------- x (Vsense - Cal@30) + 30
                   Cal@85 - Cal@30

       A scale factor is applied to the gain to make the integer math more
       accurate and then removed at the end of the calculation.  Before dividing
       by the scale factor, half of the scale factor is added to the dividend
       (or subtracted from the dividend if it is negative) to round to the nearest.
    */

    valueSigned = (int16_t) value;
    temperature = valueSigned - calValueAt30C;
    temperature = temperature * tempGainFactor;

    if (temperature >= 0)
    {
        temperature = (temperature + (ADC_TEMP_SCALE_FACTOR / 2)) / ADC_TEMP_SCALE_FACTOR;
    }
    else
    {
        temperature = (temperature - (ADC_TEMP_SCALE_FACTOR / 2)) / ADC_TEMP_SCALE_FACTOR;
    }

    temperature = temperature + 30;

    return temperature;
}

/***************************************************************************//**
    Applies the given correction to ADC raw values.  These corrections are
    based on the ADC and internal voltage reference (REF) calibration values.

    \param[in] correction - Correction to perform.
    \param[in] numValuesToCorrect - Number of values to correct.
    \param[in] rawValues - Raw ADC values to correct.
    \param[out] correctedValues - Corrected ADC values.
*//****************************************************************************/
void Adc_ApplyCorrection(
    AdcCorrection_t correction,
    uint16_t numValuesToCorrect,
    const uint16_t rawValues[],
    uint16_t correctedValues[])
{
    uint16_t refCalFactor = 0u;
    uint16_t value;
    uint16_t i;
    int16_t resultSigned;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    /* Get the adjustment factor for the selected reference level */
    switch (_selectedReferenceLevel)
    {
        case AdcReferenceLevel_1p5_VOLTS:
            refCalFactor = _refCalibrationInfo.ref15VFactor;
            break;

        case AdcReferenceLevel_2p0_VOLTS:
            refCalFactor = _refCalibrationInfo.ref20VFactor;
            break;

        case AdcReferenceLevel_2p5_VOLTS:
            refCalFactor = _refCalibrationInfo.ref25VFactor;
            break;

        case AdcReferenceLevel_DONT_CARE:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    /* Loop through all values */
    for (i = 0u; i < numValuesToCorrect; ++i)
    {
        /* Get the raw value */
        value = rawValues[i];
        ASSERT(value <= ADC_MAX_RESULT, ErrorLedMode_SOFTWARE);

        if ((correction == AdcCorrection_REF) ||
            (correction == AdcCorrection_REF_GAIN_OFFSET))
        {
            /* Apply internal voltage reference factor */
            value = value * 2u;
            value = (uint16_t) ((((uint32_t) value) * ((uint32_t) refCalFactor)) >> 16);
        }

        if ((correction == AdcCorrection_GAIN_OFFSET) ||
            (correction == AdcCorrection_REF_GAIN_OFFSET))
        {
            /* Apply ADC gain factor */
            value = value * 2u;
            value = (uint16_t) ((((uint32_t) value) * ((uint32_t) _adcCalibrationInfo.gainFactor)) >> 16);

            /* Apply ADC offset */
            resultSigned = (int16_t) value;
            resultSigned = resultSigned + _adcCalibrationInfo.offset;
            if (resultSigned > 0)
            {
                value = (uint16_t) resultSigned;
            }
            else
            {
                value = 0u;
            }
        }

        /* Store the corrected value */
        correctedValues[i] = value;
    }
}

/***************************************************************************//**
    Interrupt handler for the ADC12 interrupt.
*//****************************************************************************/
#pragma vector=ADC12_VECTOR
__interrupt void _AdcIsr(void)
{
	/** Look-up table for the conversion memory registers */
    static volatile uint16_t* const _conversionMemoryRegisters[ADC_NUM_CONVERSION_MEMORY_REGISTERS] =
    {
        &ADC12MEM0,
        &ADC12MEM1,
        &ADC12MEM2,
        &ADC12MEM3,
        &ADC12MEM4,
        &ADC12MEM5,
        &ADC12MEM6,
        &ADC12MEM7,
        &ADC12MEM8,
        &ADC12MEM9,
        &ADC12MEM10,
        &ADC12MEM11,
        &ADC12MEM12,
        &ADC12MEM13,
        &ADC12MEM14,
        &ADC12MEM15
    };

    uint16_t interruptVector;
    uint16_t i;

    /* Read clears both overflow interrupts. */
    interruptVector = ADC12IV;

    /* clear the old interrupts */
    ADC12IFG = 0u;

    ASSERT((interruptVector != ADC12IV_ADC12OVIFG) && (interruptVector != ADC12IV_ADC12TOVIFG), ErrorLedMode_SOFTWARE);

    /* Move conversion results to user location */
    for (i = 0u; i < _numChannelsConfigured; ++i)
    {
        *_resultDestinations[i] = *_conversionMemoryRegisters[i];
    }

    ++_numberOfConversions;
    EventQueue_Add(Event_ADC_CONVERSION_SEQUENCE_COMPLETE, EventPriority_NORMAL);

    /* now that the ADC is finished, clear then enable ADC enable conversion bit */
    ADC12CTL0 &= ((uint8_t)~ADC12ENC);
    ADC12CTL0 |= ADC12ENC;
    _conversionInProgress = false;
    _low_power_mode_off_on_exit();
}

/* Disable the following MISRA rules to access the TLV descriptors.
   - Rule 11.3:
     Violated by casting integers to pointers.
   - Rule 17.4:
     Violated because of pointer arithmetic.
*/
#pragma CHECK_MISRA("-11.3,-17.4")
/*lint -e923 -e946 -esym(960, 11.3) -esym(960, 17.4) */
/***************************************************************************//**
    Get the ADC calibration info out of the TLV (tag-length-value) device
    descriptors structure.

    \param[out] calInfo - Pointer to structure where the calibration info
                          should be stored.

    \return AdcCalInfoResult_SUCCESS if calibration info is found.
            Otherwise, AdcCalInfoResult_FAILURE.
*//****************************************************************************/
static AdcCalInfoResult_t _GetAdcCalibrationInfo(AdcCalibrationInfo_t* calInfo)
{
    AdcCalInfoResult_t result = AdcCalInfoResult_FAILURE;
    bool calInfoWasFound = false;
    bool reachedEndOfTable = false;
    uint8_t* tlvDescriptor;
    uint8_t tag;
    uint8_t length;
    uint16_t index;
    uint16_t value;

    /* Find ADC12 TLV descriptor */
    tlvDescriptor = (uint8_t*) TLV_START;
    while (tlvDescriptor < ((uint8_t*) TLV_END))
    {
        tag = tlvDescriptor[0];
        length = tlvDescriptor[1];

        if ((tag == TLV_ADC12CAL) && (length == ADC_CAL_TLV_LENGTH))
        {
            calInfoWasFound = true;
        }
        else if (tag == TLV_TAGEND)
        {
            reachedEndOfTable = true;
        }
        else
        {
            /* Do nothing */
        }

        if (calInfoWasFound || reachedEndOfTable)
        {
            break;
        }

        /* Go to next descriptor */
        tlvDescriptor += (length + 2u);
    }

    /* Extract ADC12 calibration info.
       Each 16-bit word is in little-endian format.
    */
    if (calInfoWasFound)
    {
        result = AdcCalInfoResult_SUCCESS;

        /* Start after tag and length bytes */
        index = 2u;

        /* Gain factor */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->gainFactor = value;
        index += 2u;

        /* Offset */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->offset = (int16_t) value;
        index += 2u;

        /* ADC reading of temp sensor at 30C using 1.5V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp30Ref15V = value;
        index += 2u;

        /* ADC reading of temp sensor at 85C using 1.5V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp85Ref15V = value;
        index += 2u;

        /* ADC reading of temp sensor at 30C using 2.0V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp30Ref20V = value;
        index += 2u;

        /* ADC reading of temp sensor at 85C using 2.0V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp85Ref20V = value;
        index += 2u;

        /* ADC reading of temp sensor at 30C using 2.5V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp30Ref25V = value;
        index += 2u;

        /* ADC reading of temp sensor at 85C using 2.5V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->temp85Ref25V = value;
    }

    return result;
}

/***************************************************************************//**
    Get the internal voltage referenced calibration info out of the TLV
    (tag-length-value) device descriptors structure.

    \param[out] calInfo - Pointer to structure where the calibration info
                          should be stored.

    \return AdcCalInfoResult_SUCCESS if calibration info is found.
            Otherwise, AdcCalInfoResult_FAILURE.
*//****************************************************************************/
static AdcCalInfoResult_t _GetReferenceCalibrationInfo(AdcReferenceCalibrationInfo_t* calInfo)
{
    AdcCalInfoResult_t result = AdcCalInfoResult_FAILURE;
    bool calInfoWasFound = false;
    bool reachedEndOfTable = false;
    uint8_t* tlvDescriptor;
    uint8_t tag;
    uint8_t length;
    uint16_t index;
    uint16_t value;

    /* Find REF TLV descriptor */
    tlvDescriptor = (uint8_t*) TLV_START;
    while (tlvDescriptor < ((uint8_t*) TLV_END))
    {
        tag = tlvDescriptor[0];
        length = tlvDescriptor[1];

        if ((tag == TLV_REFCAL) && (length == ADC_REF_CAL_TLV_LENGTH))
        {
            calInfoWasFound = true;
        }
        else if (tag == TLV_TAGEND)
        {
            reachedEndOfTable = true;
        }
        else
        {
            /* Do nothing */
        }

        if (calInfoWasFound || reachedEndOfTable)
        {
            break;
        }

        /* Go to next descriptor */
        tlvDescriptor += (length + 2u);
    }

    /* Extract ADC12 calibration info.
       Each 16-bit word is in little-endian format.
    */
    if (calInfoWasFound)
    {
        result = AdcCalInfoResult_SUCCESS;

        /* Start after tag and length bytes */
        index = 2u;

        /* Factor for 1.5 V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->ref15VFactor = value;
        index += 2u;

        /* Factor for 2.0 V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->ref20VFactor = value;
        index += 2u;

        /* Factor for 2.5 V reference */
        value = ((uint16_t) tlvDescriptor[index]);
        value |= (((uint16_t) tlvDescriptor[index + 1u]) << 8);
        calInfo->ref25VFactor = value;
    }

    return result;
}
/*lint +e923 +e946 +esym(960, 11.3) +esym(960, 17.4) */
#pragma RESET_MISRA("all")

