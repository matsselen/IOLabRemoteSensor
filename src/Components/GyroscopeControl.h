/***************************************************************************//**
    \file GyroscopeControl.h
    \brief Controls the gyroscope driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the gyroscope driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/GyroscopeControl.h$
    $Revision: 1$
    $Date: Tuesday, August 07, 2012 10:10:04 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef GYROSCOPECONTROL_H_
#define GYROSCOPECONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void GyroControl_Init(void);
void GyroControl_SetConfig(uint8_t value);
void GyroControl_ReadGyro(void);

#endif /* GYROSCOPECONTROL_H_ */
