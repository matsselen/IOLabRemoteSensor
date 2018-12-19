/***************************************************************************//**
    \file Buzzer.h
    \brief Driver for the buzzer

    \b Description Driver for the buzzer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Buzzer.h$
    $Revision: 2$
    $Date: Friday, July 17, 2015 2:23:01 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_

#include <PitchToFrequency.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Define the duty setting that gives maximum buzzer volume.  50% gives the maximum 
    volume because the sounder is moving the maximum amount in both directions.  The
    duty cycle is represented in percent (%).*/
#define BUZZER_DUTY_MAX_VOLUME  ( 50u )

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Buzzer_Init(void);
void Buzzer_Enable(void);
void Buzzer_Disable(void);
void Buzzer_SetFrequency(PitchFrequency_t freq);
void Buzzer_SetDuty(uint8_t duty);

#endif /* BUZZER_H_ */
