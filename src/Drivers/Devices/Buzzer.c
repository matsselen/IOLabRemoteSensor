/***************************************************************************//**
    \file BuzzerControl.c
    \brief Controls the buzzer

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the buzzer.

 *//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Buzzer.c$
    $Revision: 2$
    $Date: Friday, July 17, 2015 2:23:01 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

 *******************************************************************************/

#include <ProjectTypes.h>
#include <Board.h>
#include <Gpio.h>
#include "Buzzer.h"
#include <Pwm.h>
#include <Assert.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** Define a frequency to be the default pitch.  The default pitch is set to the
    resonant frequency of the buzzer. */
#define DEFAULT_BUZZER_FREQUENCY    ( 27300ul )
/** Define the default buzzer duty cycle.  The duty cycle is represented in 
    percent (%). */
#define DEFAULT_BUZZER_DUTY         ( BUZZER_DUTY_MAX_VOLUME )

/** Define the PWM channel of the buzzer */
#define BUZZER_PWM_CHANNEL  ( PwmChannel_1 )
/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/


/***************************************************************************//**
    Initialized the Buzzer and PWM driver.
 *//****************************************************************************/
void Buzzer_Init(void)
{
        Pwm_Init();
        Pwm_SetFrequency(DEFAULT_BUZZER_FREQUENCY);
        Pwm_SetDuty(BUZZER_PWM_CHANNEL, DEFAULT_BUZZER_DUTY);
}

/***************************************************************************//**
    Enables the Buzzer
 *//****************************************************************************/
void Buzzer_Enable(void)
{
    Pwm_Enable(BUZZER_PWM_CHANNEL, Pwm_GetDuty(BUZZER_PWM_CHANNEL));
}

/***************************************************************************//**
    Disables the Buzzer
 *//****************************************************************************/
void Buzzer_Disable(void)
{
    Pwm_Disable(BUZZER_PWM_CHANNEL);
}

/***************************************************************************//**
    Sets the frequency of the buzzer

    \param[in] freq - Frequency in HzX10 (37.6 Hz = 376)
 *//****************************************************************************/
void Buzzer_SetFrequency(PitchFrequency_t freq)
{
    Pwm_SetFrequency((uint32_t)freq);
}

/***************************************************************************//**
    Sets the duty cycle of the buzzer of the buzzer

    \param[in] duty - Duty cycle for the buzzer in percent (0 to 100)
 *//****************************************************************************/
void Buzzer_SetDuty(uint8_t duty)
{
    Pwm_SetDuty(BUZZER_PWM_CHANNEL, duty);
}
