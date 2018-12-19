/***************************************************************************//**
    \file HighGainControl.h
    \brief Controls the high gain input readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the high gain input.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/HighGainControl.h$
    $Revision: 1$
    $Date: Thursday, September 06, 2012 2:53:22 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef HIGHGAINCONTROL_H_
#define HIGHGAINCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void HighGainControl_Init(void);
void HighGainControl_SetConfig(uint8_t config);

#endif /* HIGHGAINCONTROL_H_ */
