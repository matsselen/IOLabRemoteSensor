/***************************************************************************//**
    \file MicrophoneControl.h
    \brief Controls the microphone readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the microphone.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/MicrophoneControl.h$
    $Revision: 1$
    $Date: Friday, September 07, 2012 11:27:30 AM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef MICROPHONECONTROL_H_
#define MICROPHONECONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void MicrophoneControl_Init(void);
void MicrophoneControl_SetConfig(uint8_t config);

#endif /* MICROPHONECONTROL_H_ */
