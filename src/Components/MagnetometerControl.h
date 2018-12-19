/***************************************************************************//**
    \file MagnetometerControl.h
    \brief Header for the magnetometer control module.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header for the magnetometer control module. This will interface
                   to the magnetometer driver to configure and and retrieve data.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/MagnetometerControl.h$
    $Revision: 2$
    $Date: Monday, August 20, 2012 9:20:06 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef MAGNETOMETERCONTROL_H_
#define MAGNETOMETERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void MagControl_Init(void);
void MagControl_SetConfig(uint8_t keyValue);
void MagControl_ReadMag(void);

#endif /* MAGNETOMETERCONTROL_H_ */
