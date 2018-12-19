/***************************************************************************//**
    \file Magnetometer.h
    \brief Magnetometer Interface Header

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header file for the magnetometer (3-axis Hall effect) sensor
       interface logic.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Magnetometer.h$
    $Revision: 4$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Defines the ADC sample rate in Hz that the magnetometer will run at. Lower
    sample rates consume less power. Output Data Rate = MagSampleRate / MagOverSampleRatio
 */
typedef enum MagSampleRate
{
	MagSampleRate_1280,
	MagSampleRate_640,
	MagSampleRate_320,
	MagSampleRate_160,
	MagSampleRate_80,
	MagSampleRate_40,
	MagSampleRate_20,
	MagSampleRate_10
} MagSampleRate_t;

/** Defines the oversampling ratio of the magnetometer. The higher the oversampling
    ratio the lower the noise in the measurement, but the slower the output data rate.
    Output Data Rate = MagSampleRate / MagOverSampleRatio
 */
typedef enum MagOverSampleRatio
{
	MagOverSampleRatio_16,
	MagOverSampleRatio_32,
	MagOverSampleRatio_64,
	MagOverSampleRatio_128
} MagOverSampleRatio_t;

/** Defines the scaling mode for the output data. */
typedef enum MagDataScaling
{
	MagDataScaling_RAW,   /// Outputs raw data from the ADC
	MagDataScaling_SCALED /// Scales the ADC data by the user set scaling values
} MagDataScaling_t;

/** Selects if the magnetometer should reset its sensor before each reading. Resetting is necessary
    after being exposed to a large magnetic field.

    \note Auto-reset is recommended.
 */
typedef enum MagAutoReset
{
	MagAutoReset_DISABLED,
	MagAutoReset_ENABLED
} MagAutoReset_t;

/** Defines how the sensor will gather samples Auto mode will collect samples at the configured
    rate determined by the sample rate and over sampling mode.
*/
typedef enum MagSamplingMode
{
	MagSampling_Auto,
	MagSampling_Triggered
} MagSamplingMode_t;

/** Defines what resolution the sensor readings will be. 8-bit resolution return the MSB only
    from a 16-bit value. This will allow for lower data rates when lower resolution data is
    acceptable.
 */
typedef enum MagResolution
{
	MagResolution_16BIT,
	MagResolution_8BIT
} MagResolution_t;

/** Defines the magnetometer configuration structure */
typedef struct MagConfig
{
	MagSampleRate_t sampleRate;
	MagOverSampleRatio_t overSampleRatio;
	MagDataScaling_t dataScaling;
	MagAutoReset_t autoReset;
	MagSamplingMode_t samplingMode;
	MagResolution_t resolution;
} MagConfig_t;

/** Defines the structure that will hold the sample data from
    a magnetometer reading.
 */
typedef struct MagSample
{
	int16_t xData;
	int16_t yData;
	int16_t zData;
} MagSample_t;

/** Defines an magnetometer ISR function pointer */
typedef void (*MagIsrPointer_t)(void);
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Magnetometer_Init(void);
bool Magnetometer_Config(const MagConfig_t* config);
void Magnetometer_Start(void);
void Magnetometer_Stop(void);
bool Magnetometer_TriggerSampleBlocking(MagSample_t* sample);
void Magnetometer_ResetSensorBlocking(void);
void Magnetometer_SetCalibration(const MagSample_t* calibration);
bool Magnetometer_ReadSample(MagSample_t* sample);
void Magnetometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[]);
int8_t Magnetometer_ReadTemp(void);
void Magnetometer_RegisterIsr(MagIsrPointer_t isr);
void Magnetometer_UnregisterIsr(void);
#endif /* MAGNETOMETER_H_ */
