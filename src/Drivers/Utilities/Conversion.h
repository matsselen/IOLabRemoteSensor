/***************************************************************************//**
    \file Conversion.h
    \brief Header for the conversion module that will perform generic conversions.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The conversion module contains functions and macros that will
                   perform generic conversion such as string to integer type
                   conversions.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Conversion.h$
    $Revision: 2$
    $Date: Tuesday, June 05, 2012 6:06:20 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef CONVERSION_H_
#define CONVERSION_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Possible conversion status values.
    Fixed values are required as this is often converted into an integer or
    boolean.
*/
typedef enum ConversionStatus
{
	ConversionStatus_FAILURE = 0u,
	ConversionStatus_SUCCESS = 1u
} ConversionStatus_t;

/** Possible number sizes to convert to */
typedef enum NumberSize
{
	NumberSize_4_BITS,
    NumberSize_8_BITS,
    NumberSize_12_BITS,
    NumberSize_16_BITS
} NumberSize_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
ConversionStatus_t Conversion_HexStringToUint( const char_t string[],
                                NumberSize_t size,
                                uint16_t* result);
uint16_t Conversion_SwapBytes(uint16_t number);
#endif /* CONVERSION_H_ */
