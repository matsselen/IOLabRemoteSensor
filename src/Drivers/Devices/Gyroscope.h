/***************************************************************************//**
    \file Gyroscope.h
    \brief Header file for Gyroscope sensor driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The gyroscope sensor driver accesses the 3-axis
                   readings from the gyroscope sensor.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Gyroscope.h$
    $Revision: 6$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

/** Defines an gyroscope ISR function pointer */
typedef void (*GyroIsrPointer_t)(void);
/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the options for the gyroscope sample rates in Hz */
/*typedef enum GyroSampleRate
{
	GyroSampleRate_10
} GyroSampleRate_t;
*/
/** Defines a structure to hold the gyroscope configuration settings */
/*typedef struct GyroConfig
{
	GyroSampleRate_t sampleRate;
} GyroscopeConfig_t;
*/
/** Defines the output data rates that the gyroscope can run at. */
typedef enum GyroOdr
{
	GyroOdr_760,
	GyroOdr_380,
	GyroOdr_190,
	GyroOdr_95
} GyroOdr_t;

/** Defines the resolution setting for the gyroscope in degrees per
	second.
*/
typedef enum GyroResolution
{
	GyroResolution_2000,
	GyroResolution_500,
	GyroResolution_250
} GyroResolution_t;

/** Defines the Bandwidth setting for the gyroscope. The cutoff value
	depends on the output data rate.
*/
typedef enum GyroBandwidth
{
	GyroBandwidth_0,
	GyroBandwidth_1,
	GyroBandwidth_2,
	GyroBandwidth_3
} GyroBandwidth_t;

/** Defines a structure to hold the gyroscope configuration data in. */
typedef struct GyroConfig
{
	GyroOdr_t odr;
	GyroResolution_t resolution;
//	GyroBandwidth_t bandwidth;
} GyroConfig_t;


/** Defines a structure to hold the gyroscope sample data in */
typedef struct GyroSample
{
	uint16_t xData;
	uint16_t yData;
	uint16_t zData;
} GyroSample_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Gyroscope_Init(void);
void Gyroscope_Config(const GyroConfig_t* config);
// void Gyroscope_Config(void);
void Gyroscope_Start(void);
void Gyroscope_Stop(void);
bool Gyroscope_ReadSample(GyroSample_t* sample);
void Gyroscope_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[]);
int8_t Gyroscope_ReadTemperature(void);
void Gyroscope_RegisterIsr(GyroIsrPointer_t isr);
void Gyroscope_UnregisterIsr(void);

#endif /* GYROSCOPE_H_ */
