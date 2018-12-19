/***************************************************************************//**
    \file Pwm.h
    \brief Pulse width modulation module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Contains functions and macros for the pulse width modulation
       module.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Pwm.h$
    $Revision: 5$
    $Date: Friday, July 17, 2015 2:23:01 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef PWM_H_
#define PWM_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** PWM channels */
typedef enum PwmChannel
{
    PwmChannel_0 = 0,
    PwmChannel_1 = 1,
    PwmChannel_2 = 2,
    PwmChannel_3 = 3,
    PwmChannel_4 = 4,
    PwmChannel_5 = 5,
	PwmChannel_6 = 6
} PwmChannel_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void    Pwm_Init(void);
void    Pwm_SetFrequency(uint32_t freq);
bool    Pwm_SetDuty(PwmChannel_t channel, uint8_t duty);
uint8_t Pwm_GetDuty(PwmChannel_t channel);
bool    Pwm_Enable(PwmChannel_t channel, uint8_t duty);
void    Pwm_Disable(PwmChannel_t channel);

#endif /* PWM_H_ */
