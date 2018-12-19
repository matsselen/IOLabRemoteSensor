/***************************************************************************//**
    \file Analog7Control.h
    \brief Controls the analog input readings on header pin 7.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the analog input on header pin 7.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Analog7Control.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 3:05:20 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ANALOG7CONTROL_H_
#define ANALOG7CONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Analog7Control_Init(void);
void Analog7Control_SetConfig(uint8_t config);

#endif /* ANALOG7CONTROL_H_ */
