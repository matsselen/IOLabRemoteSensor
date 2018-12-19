/***************************************************************************//**
    \file DigitalInputControl.h
    \brief Controls the digital input readings on the header and buttons.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the digital inputs on the header and buttons.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/DigitalInputControl.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 9:44:14 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DIGITALINPUTCONTROL_H_
#define DIGITALINPUTCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void DigitalInputControl_Init(void);
void DigitalInputControl_SetConfig(uint8_t config);

#endif /* DIGITALINPUTCONTROL_H_ */
