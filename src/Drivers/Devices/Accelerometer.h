/***************************************************************************//**
    \file Accelerometer.h
    \brief Accelerometer interface header.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header file for the 3-axis accelerometer sensor interface
                   logic.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Accelerometer.h$
    $Revision: 4$
    $Date: Sunday, July 01, 2012 7:22:34 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the output data rates that the accelerometer can run at. Lower sample
    rates consume less power.
 */
typedef enum AccOdr
{
	AccOdr_800,
	AccOdr_400,
	AccOdr_200,
	AccOdr_100,
	AccOdr_50,
	AccOdr_12_5,
	AccOdr_6_25,
	AccOdr_1_56
} AccOdr_t;

/** Defines the resolution setting for the accelerometer Lower resolution allows for
    lower bus traffic
*/
typedef enum AccResolution
{
	AccResolution_14bit,
	AccResolution_8bit
} AccResolution_t;

/** Defines the full scale range output from the accelerometer in g */
typedef enum AccFullScaleRange
{
	AccFullScaleRange_8g,
	AccFullScaleRange_4g,
	AccFullScaleRange_2g
} AccFullScaleRange_t;

/** Defines the over sampling mode for the accelerometer. The higher resolution uses
    more power.
*/
typedef enum AccOverSample
{
	AccOverSample_Normal,  /// High resolution
	AccOverSample_LNLP,    /// Low noise, but also low power
	AccOverSample_HighRes, /// Highest resolution, especially at lower ODR
	AccOverSample_LowPower /// lowest power, but highest noise
} AccOverSample_t;

/** Defines a structure to hold the accelerometer configuration data in. */
typedef struct AccConfig
{
	AccOdr_t odr;
	AccResolution_t resolution;
	AccFullScaleRange_t range;
	AccOverSample_t overSampleMode;
} AccConfig_t;

/** Defines a structure to hold the accelerometer data in. */
typedef struct AccSample
{
	int16_t xData;
	int16_t yData;
	int16_t zData;
} AccSample_t;


/** Defines an accelerometer ISR function pointer */
typedef void (*AccIsrPointer_t)(void);
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Accelerometer_Init(void);
bool Accelerometer_Config(const AccConfig_t* config);
void Accelerometer_Start(void);
void Accelerometer_Stop(void);
void Accelerometer_TriggerSampleBlocking(AccSample_t* sample);
void Accelerometer_SetCalibration(const AccSample_t* calibration);
bool Accelerometer_ReadSample(AccSample_t* sample);
void Accelerometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[]);
void Accelerometer_RegisterIsr(AccIsrPointer_t isr);
void Accelerometer_UnregisterIsr(void);
#endif /* ACCELEROMETER_H_ */
