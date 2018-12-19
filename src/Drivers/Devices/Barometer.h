/***************************************************************************//**
    \file Barometer.h
    \brief Header file for Barometer (and temperature)sensor driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The barometer sensor driver accesses the pressure and temperature
                   readings from the barometer sensor.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Barometer.h$
    $Revision: 3$
    $Date: Tuesday, August 28, 2012 5:15:46 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BAROMETER_H_
#define BAROMETER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the options for the barometer sample rates in Hz */
typedef enum BarSampleRate
{
	BarSampleRate_10
} BarSampleRate_t;

/** Defines a structure to hold the barometer configuration settings */
typedef struct BarConfig
{
	BarSampleRate_t sampleRate;
} BarometerConfig_t;

/** Defines a structure to hold the barometer sample data in */
typedef struct BarSample
{
	uint16_t pressure;
	uint16_t temperature;
} BarometerSample_t;

typedef struct BarometerCoef
{
	uint16_t a0;  /// Pressure offset coefficient
	uint16_t b1;  /// Pressure sensitivity coefficient
	uint16_t b2;  /// temperature coefficient of offset
	uint16_t c12; /// temperature coefficient of sensitivity
} BarometerCoef_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Barometer_Init(void);
void Barometer_Config(const BarometerConfig_t* config);
void Barometer_Start(void);
void Barometer_Stop(void);
void Barometer_TriggerSampleBlocking(BarometerSample_t* sample);
void Barometer_StartSampleConversion(void);
bool Barometer_ReadSample(BarometerSample_t* sample);
bool Barometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[]);
void Barometer_ReadCoefficients(BarometerCoef_t* coefficients);
void Barometer_ReadCoeffArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   coefficients[]);

#endif /* BAROMETER_H_ */
