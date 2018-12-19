/***************************************************************************//**
    \file DacControl.h
    \brief Controls the settings of the hardware DAC

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the hardware DAC.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/DacControl.h$
    $Revision: 1$
    $Date: Friday, August 10, 2012 5:44:25 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DACCONTROL_H_
#define DACCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void DacControl_Init();
void DacControl_SetConfig(uint8_t config);

#endif /* DACCONTROL_H_ */
