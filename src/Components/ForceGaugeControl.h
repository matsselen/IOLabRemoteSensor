/***************************************************************************//**
    \file ForceGaugeControl.h
    \brief Controls the force gauge.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the force gauge.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/ForceGaugeControl.h$
    $Revision: 1$
    $Date: Friday, August 03, 2012 3:21:27 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef FORCEGAUGECONTROL_H_
#define FORCEGAUGECONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void FgControl_Init(void);
void FgControl_SetConfig(uint8_t config);

#endif /* FORCEGAUGECONTROL_H_ */
