/***************************************************************************//**
    \file Device.h
    \brief Device specific include file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This include file includes the device specific header file so
                   that if the part number changes, it only will affect the source
                   code of this file.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/Device.h$
    $Revision: 4$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DEVICE_H_
#define DEVICE_H_

#if defined(__MSP430F5329__)
#include <MSP430F5329.h>
#elif defined(__MSP430F5325__)
#include <MSP430F5325.h>
#endif

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** \name ROM addresses for special values on the MSP430F5329
    \{
*/
/** Defines the address the Wafer Id of the microcontroller's die. */
#define DEVICE_WAFER_ID_ADDRESS 0x1A0A

/** Defines the address the Wafer x position of the microcontroller's die. */
#define DEVICE_WAFER_X_POS 0x1A0E

/** Defines the address the Wafer y position of the microcontroller's die. */
#define DEVICE_WAFER_Y_POS 0x1A10

/** Defines the address of the device Id for the microcontroller type. */
#define DEVICE_ID_ADDRESS 0x1A04

/** Defines the address of the hardware revision ID */
#define DEVICE_HW_REV_ADDRESS 0x1A06

/** Defines the address of the firmware revision ID */
#define DEVICE_FW_REV_ADDRESS 0x1A07

/** Defines the address of the ADC12 calibration tag */
#define DEVICE_ADC12_CALIBRATION_TAG 0x1A14

/** Defines the address of the ADC12 calibration length */
#define DEVICE_ADC12_CALIBRATION_LENGTH 0x1A15

/** Defines the address of the ADC12 gain factor */
#define DEVICE_ADC12_GAIN_FACTOR_ADDRESS 0x1A16

/** Defines the address of the ADC12 offset */
#define DEVICE_ADC12_OFFSET_ADDRESS 0x1A18
/** \} */

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* DEVICE_H_ */
