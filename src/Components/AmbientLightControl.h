/***************************************************************************//**
    \file AmbientLightControl.h
    \brief Controls the ambient light sensor readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the ambient light sensor voltage.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/AmbientLightControl.h$
    $Revision: 2$
    $Date: Thursday, September 06, 2012 2:54:37 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef AMBIENTLIGHTCONTROL_H_
#define AMBIENTLIGHTCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void AmbientLightControl_Init(void);
void AmbientLightControl_SetConfig(uint8_t config);

#endif /* AMBIENTLIGHTCONTROL_H_ */
