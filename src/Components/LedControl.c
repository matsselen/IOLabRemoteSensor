/***************************************************************************//**
    \file LedControl.c
    \brief LED Controller

    \b Project 1814 IOLab Remote Sensor Application

    \b Description LED controller that manages the LEDs and their state

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/LedControl.c$
    $Revision: 2$
    $Date: Thursday, July 12, 2012 10:49:01 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "LedControl.h"
#include <Gpio.h>
#include <Board.h>
#include <TimerTick.h>
#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Led counter starting value */
#define LED_COUNTER_START    ( 0x0001u )
/** Led counter ending value */
#define LED_COUNTER_END      ( 0x0200u )
/** Time between Led states */
#define LED_TIME_MSEC        ( 100u )

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** The counter for the led state. */
static uint16_t _ledCounter = LED_COUNTER_START;

/** Stores the setting for each led */
static uint16_t _ledState[Led_LAST] =
{
	LedState_OFF,
	LedState_OFF,
	LedState_OFF,
	LedState_OFF

};

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _Led_ProcessStates(void);

/***************************************************************************//**
    Initializes the Led Controller and sets up the timeout and callback for
    controlling the Led state.
*//****************************************************************************/
void LedControl_Init(void)
{
	/* set all the led states initially */
//	_Led_ProcessStates();

	/* register the timeout that will process the states */
	TimerTick_RequestTimeout(TickTimeoutType_REPEAT, LED_TIME_MSEC, _Led_ProcessStates);
}

/***************************************************************************//**
    Sets the specified Led to blink for the appropriate state.

    \param[in] led - which led to set
    \param[in] state - the state to set led to

*//****************************************************************************/
void LedControl_SetLed(Led_t led, LedState_t state)
{
	ASSERT((led >= Led_FIRST) && (led < Led_LAST), ErrorLedMode_SOFTWARE);

	_ledState[led] = state;
}

/***************************************************************************//**
    Increments the led state counter and sets the state of the leds.
*//****************************************************************************/
static void _Led_ProcessStates(void)
{
	static int16_t ledIndex;
	static const int16_t _ledPort[Led_LAST] =
	{
		LED0_RED_PORT,
		LED0_GREEN_PORT,
		LED1_RED_PORT,
		LED1_GREEN_PORT
	};

	static const uint8_t _ledPin[Led_LAST] =
	{
		LED0_RED_PIN,
		LED0_GREEN_PIN,
		LED1_RED_PIN,
		LED1_GREEN_PIN
	};

	for(ledIndex = Led_LAST - 1u; ledIndex >= Led_FIRST; --ledIndex)
	{
		if (_ledState[ledIndex] & _ledCounter)
		{
			Gpio_Set(_ledPort[ledIndex], _ledPin[ledIndex]);
		}
		else
		{
			Gpio_Clear(_ledPort[ledIndex], _ledPin[ledIndex]);
		}
	}

	_ledCounter = _ledCounter << 1u;
	if (_ledCounter > LED_COUNTER_END)
	{
		_ledCounter = LED_COUNTER_START;
	}
}
