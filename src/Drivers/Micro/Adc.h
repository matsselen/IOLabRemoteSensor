/***************************************************************************//**
    \file Adc.h
    \brief Analog to digital conversion module header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header file for the 12-bit ADC module.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Adc.h$
    $Revision: 7$
    $Date: Friday, October 24, 2014 10:53:12 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ADC_H_
#define ADC_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Number of conversion memory registers in the ADC module. */
#define ADC_NUM_CONVERSION_MEMORY_REGISTERS     (16u)
/** Maximum number of channels in a conversion sequence. */
#define ADC_SEQUENCE_MAX_LENGTH                 (ADC_NUM_CONVERSION_MEMORY_REGISTERS)

/** Possible ADC conversion references.
    Enumeration values are defined to equal ADC register values.
*/
typedef enum AdcReference
{
    AdcReference_AVCC_AVSS = 0u,
    AdcReference_VREFp_AVSS = 1u,
    AdcReference_eVREFp_AVSS = 2u,
    AdcReference_AVCC_VREFm = 4u,
    AdcReference_VREFp_VREFm = 5u,
    AdcReference_eVREFp_VREFm = 6u
} AdcReference_t;

/** Possible ADC input channels.
    Enumeration values are defined to equal ADC register values.
*/
typedef enum AdcInputChannel
{
	AdcInputChannel_INVALID   = -1,
	AdcInputChannel_FIRST     = 0u,
    AdcInputChannel_A0        = 0u,
    AdcInputChannel_A1        = 1u,
    AdcInputChannel_A2        = 2u,
    AdcInputChannel_A3        = 3u,
    AdcInputChannel_A4        = 4u,
    AdcInputChannel_A5        = 5u,
    AdcInputChannel_A6        = 6u,
    AdcInputChannel_A7        = 7u,
    AdcInputChannel_eVREFp    = 8u,
    AdcInputChannel_VREFm     = 9u,
    AdcInputChannel_TEMP      = 10u,
    AdcInputChannel_AVCC_AVSS = 11u,
    AdcInputChannel_A12       = 12u,
    AdcInputChannel_A13       = 13u,
    AdcInputChannel_A14       = 14u,
    AdcInputChannel_A15       = 15u,
    AdcInputChannel_LAST
} AdcInputChannel_t;

/** Struct that contains the configuration for a channel in conversion
    sequence.
*/
typedef struct AdcChannelConfig
{
    AdcReference_t reference;
    AdcInputChannel_t inputChannel;
    uint16_t* destination;
} AdcChannelConfig_t;

/** Enum for the ADC Conversion Sequence options. */
typedef enum AdcConSeq
{
	AdcConSeq_SING_CHAN_SING_CONV = 0x00u,
	AdcConSeq_SEQ_OF_CHAN         = 0x01u,
	AdcConSeq_REPEAT_SING_CHAN    = 0x02u,
	AdcConSeq_REPEAT_SEQ_CHAN     = 0x03u
} AdcConSeq_t;

/** Enum for the ADC sample and hold time options in # clock cycles */
typedef enum AdcSht
{
	AdcSht_INVALID = -1,
	AdcSht_FIRST   = 0x0u,
	AdcSht_0004    = 0x0u,
	AdcSht_0008    = 0x01u,
	AdcSht_0016    = 0x02u,
	AdcSht_0032    = 0x03u,
	AdcSht_0064    = 0x04u,
	AdcSht_0096    = 0x05u,
	AdcSht_0128    = 0x06u,
	AdcSht_0192    = 0x07u,
	AdcSht_0256    = 0x08u,
	AdcSht_0384    = 0x09u,
	AdcSht_0512    = 0x0Au,
	AdcSht_0768    = 0x0Bu,
	AdcSht_1024    = 0x0Cu,
	AdcSht_LAST
} AdcSht_t;

/** Available ADC corrections that can be performed using the available
    calibration information.
*/
typedef enum AdcCorrection
{
    AdcCorrection_GAIN_OFFSET,
    AdcCorrection_REF,
    AdcCorrection_REF_GAIN_OFFSET
} AdcCorrection_t;

/** Possible ADC interval voltage reference levels. */
typedef enum AdcReferenceLevel
{
    AdcReferenceLevel_1p5_VOLTS = 0,
    AdcReferenceLevel_2p0_VOLTS,
    AdcReferenceLevel_2p5_VOLTS,
    AdcReferenceLevel_DONT_CARE
} AdcReferenceLevel_t;

/** Struct that contains the configurable settings of the ADC module */
typedef struct AdcConfig
{
	AdcConSeq_t conSeq;
	AdcSht_t    sampleHoldTime;
	bool        multipleSampleConversion;
} AdcConfig_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
extern void Adc_Init(void);
extern void Adc_Config( const AdcConfig_t *config);
extern bool Adc_IsConversionInProgress(void);
extern void Adc_SetChannelSequence(const AdcChannelConfig_t config[],
                                   uint16_t numChannels,
                                   AdcReferenceLevel_t refLevel);
extern void Adc_StartConversionSequenceOfChannels(void);
extern void Adc_SingleConversionSequenceBlocking(void);
extern void Adc_StopConversion(void);
extern uint16_t Adc_GetNumberOfConversions(void);

extern uint16_t Adc_GetGainFactor(void);
extern uint16_t Adc_GetOffset(void);
extern void Adc_GetTempCalib(uint16_t *cal85C, uint16_t *cal30C);
extern int16_t Adc_ConvertValueToTemp(uint16_t value);
extern void Adc_ApplyCorrection(
    AdcCorrection_t correction,
    uint16_t numValuesToCorrect,
    const uint16_t rawValues[],
    uint16_t correctedValues[]);

#endif /* ADC_H_ */
