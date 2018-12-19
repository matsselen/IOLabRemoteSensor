/***************************************************************************//**
    \file Header1Control.h
    \brief Controls the settings of header pin 1

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 1

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header1Control.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 9:44:14 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef HEADER1CONTROL_H_
#define HEADER1CONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Header1Control_Init();
void Header1Control_SetConfig(uint8_t config);

#endif /* HEADER1CONTROL_H_ */
