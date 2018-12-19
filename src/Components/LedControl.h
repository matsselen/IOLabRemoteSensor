/***************************************************************************//**
    \file LedControl.h
    \brief LED Controller header.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description LED controller that manages the LEDs and their state

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/LedControl.h$
    $Revision: 3$
    $Date: Monday, August 06, 2012 7:29:36 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef LEDCONTROL_H_
#define LEDCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the LED states that are possible */
typedef enum LedState
{
	LedState_INVALID         = -1,
	LedState_OFF             = 0x0000u,
	LedState_BLINK_1         = 0x0200u, /** Blink is mostly off then on for a short period of time. Opposite of wink */
	LedState_BLINK_2         = 0x0280u,
	LedState_BLINK_3         = 0x02A0u,
	LedState_BLINK_4         = 0x02A8u,
	LedState_BLINK_FAST_EVEN = 0x0155u,
	LedState_BLINK_FAST_ODD  = 0x02AAu,
	LedState_BLINK_SLOW_EVEN = 0x001Fu,
	LedState_BLINK_SLOW_ODD  = 0x03E0u,
	LedState_WINK_1          = 0x03FEu, /** Wink is mostly on then off for short period of time. Opposite of blink */
	LedState_WINK_2          = 0x03FAu,
	LedState_WINK_3          = 0x03EAu,
	LedState_WINK_4          = 0x03AAu,
	LedState_ON              = 0x03FFu
} LedState_t;

/** Defines the LEDs available in the control. */
typedef enum Led
{
	Led_INVALID = -1,
	Led_FIRST = 0,
	Led_0_RED = Led_FIRST,
	Led_0_GREEN,
	Led_1_RED,
	Led_1_GREEN,
	Led_LAST
} Led_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void LedControl_Init(void);
void LedControl_SetLed(Led_t led, LedState_t state);

#endif /* LEDCONTROL_H_ */
