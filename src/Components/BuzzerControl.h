/***************************************************************************//**
    \file BuzzerControl.h
    \brief Controls the settings of the buzzer

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the buzzer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BuzzerControl.h$
    $Revision: 1$
    $Date: Sunday, August 19, 2012 11:57:21 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BUZZERCONTROL_H_
#define BUZZERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void BuzzerControl_Init();
void BuzzerControl_SetConfig(uint8_t config);

#endif /* BUZZERCONTROL_H_ */
