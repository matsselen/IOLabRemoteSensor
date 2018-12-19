/***************************************************************************//**
    \file BarometerControl.h
    \brief Controls the barometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the barometer driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BarometerControl.h$
    $Revision: 2$
    $Date: Tuesday, August 28, 2012 5:15:46 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BAROMETERCONTROL_H_
#define BAROMETERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void BarometerControl_Init(void);
void BarometerControl_SetConfig(uint8_t value);
void BarometerControl_ReadBar(void);
void BarometerControl_ReadCalibration(
		uint16_t  maxCalibrationLength,
		uint16_t *actualCalibrationLength,
		uint8_t   calibration[]);

#endif /* BAROMETERCONTROL_H_ */
