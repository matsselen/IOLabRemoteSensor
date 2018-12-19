/***************************************************************************//**
    \file PushButtonControl.h
    \brief Control for the push buttons

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The push button control will control the push buttons and
       manage the debouncing of them.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/PushButtonControl.h$
    $Revision: 4$
    $Date: Friday, July 17, 2015 2:22:23 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef PUSHBUTTONCONTROL_H_
#define PUSHBUTTONCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Defines the push buttons that exist in the system */
typedef enum PushButtons
{
    PushButton_INVALID = -1,
    PushButton_FIRST   = 0u,
    PushButton_ZERO    = PushButton_FIRST,
    PushButton_ONE,
    PushButton_LAST
} PushButtons_t;
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void PushButtonControl_Init(void);

bool PushButtonControl_isButtonPressed(PushButtons_t button);
uint16_t PushButtonControl_ButtonPressedDuration(PushButtons_t button);

#endif /* PUSHBUTTONCONTROL_H_ */
