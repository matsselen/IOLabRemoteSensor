/***************************************************************************//**
    \file Header5Control.h
    \brief Controls the settings of header pin 5

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 5

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header5Control.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 3:05:20 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef HEADER5CONTROL_H_
#define HEADER5CONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Header5Control_Init();
void Header5Control_SetConfig(uint8_t config);

#endif /* HEADER5CONTROL_H_ */
