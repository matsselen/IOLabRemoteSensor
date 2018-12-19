/***************************************************************************//**
    \file AccelerometerControl.h
    \brief Controls the accelerometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the accelerometer driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/AccelerometerControl.h$
    $Revision: 1$
    $Date: Thursday, June 28, 2012 8:55:31 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ACCELEROMETERCONTROL_H_
#define ACCELEROMETERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void AccControl_Init(void);
void AccControl_SetConfig(uint8_t value);
void AccControl_ReadAcc(void);

#endif /* ACCELEROMETERCONTROL_H_ */
