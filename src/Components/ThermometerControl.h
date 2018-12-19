/***************************************************************************//**
    \file ThermometerControl.h
    \brief Controls the Thermometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the Thermometer driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/ThermometerControl.h$
    $Revision: 1$
    $Date: Tuesday, April 09, 2013 9:35:08 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef THERMOMETERCONTROL_H_
#define THERMOMETERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void ThermometerControl_Init(void);
void ThermometerControl_SetConfig(uint8_t value);
void ThermometerControl_ReadCalibration(
        uint16_t  maxCalibrationLength,
        uint16_t *actualCalibrationLength,
        uint8_t   calibration[]);

#endif /* THERMOMETERCONTROL_H_ */
