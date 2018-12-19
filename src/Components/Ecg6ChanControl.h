/***************************************************************************//**
    \file EcgControl.h
    \brief Controls the 6-channel ECG sensor readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the 6-channel ECG sensor voltage.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Ecg6ChanControl.h$
    $Revision: 1$
    $Date: Friday, September 26, 2014 5:47:41 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ECG6CONTROL_H_
#define ECG6CONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Ecg6Control_Init(void);
void Ecg6Control_SetConfig(uint8_t config);

#endif /* ECG6CONTROL_H_ */
