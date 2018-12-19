/***************************************************************************//**
    \file EcgControl.h
    \brief Controls the ECG sensor readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the ECG sensor voltage.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/EcgControl.h$
    $Revision: 1$
    $Date: Thursday, September 06, 2012 2:53:22 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ECGCONTROL_H_
#define ECGCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void EcgControl_Init(void);
void EcgControl_SetConfig(uint8_t config);

#endif /* ECGCONTROL_H_ */
