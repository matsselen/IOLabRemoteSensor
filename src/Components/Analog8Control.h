/***************************************************************************//**
    \file Analog8Control.h
    \brief Controls the analog input readings on header pin 8.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the analog input on header pin 8.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Analog8Control.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 3:05:20 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ANALOG8CONTROL_H_
#define ANALOG8CONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Analog8Control_Init(void);
void Analog8Control_SetConfig(uint8_t config);

#endif /* ANALOG8CONTROL_H_ */
