/***************************************************************************//**
    \file PushButtonControl.c
    \brief Control for the push buttons

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The push button control will control the push buttons and
       manage the debouncing of them.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/PushButtonControl.c$
    $Revision: 8$
    $Date: Friday, July 17, 2015 2:22:11 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "PushButtonControl.h"
#include <Gpio.h>
#include <Board.h>
#include <EventQueue.h>
#include <TimerTick.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** The number of milliseconds that must have elapsed for the button to be
    debounced.  */
#define BUTTON_DEBOUNCE_MS  ( 80u )

typedef struct PushButtonInfos
{
    uint16_t buttonPressedDuration;
    uint16_t previousButtonPressedDuration;
} PushButtonInfo_t;

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Define a variable to store the handle to the ISR to process the push buttons. */
static TickIsrHandle_t _processButtonsIsrHandle;

/** Stores how long the push buttons have been pressed */
static PushButtonInfo_t _buttonInfo[PushButton_LAST];

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _ProcessPushButtons(void);

/***************************************************************************//**
    Initializes the push button controller and regisisters the push button
    callback.
*//****************************************************************************/
void PushButtonControl_Init(void)
{
    /* Handle the case where the button is pressed before the power is turned on.
       Set the initial state such that the button is "already pressed" and the
       debounce has occurred. */
    if (GPIO_IS_LOW(BUTTON_0_PORT, BUTTON_0_PIN))
    {
        _buttonInfo[PushButton_ZERO].buttonPressedDuration = BUTTON_DEBOUNCE_MS;
        _buttonInfo[PushButton_ZERO].previousButtonPressedDuration = BUTTON_DEBOUNCE_MS;
    }

    if (GPIO_IS_LOW(BUTTON_1_PORT, BUTTON_1_PIN))
    {
        _buttonInfo[PushButton_ONE].buttonPressedDuration = BUTTON_DEBOUNCE_MS;
        _buttonInfo[PushButton_ONE].previousButtonPressedDuration = BUTTON_DEBOUNCE_MS;
    }

	_processButtonsIsrHandle = TimerTick_RegisterTickIsr(&_ProcessPushButtons);
	TimerTick_EnableTickIsr(_processButtonsIsrHandle);
}

/***************************************************************************//**
    Returns true if the button is pressed, false otherwise.  This takes into
    account the button debounce time.

    \param[in] button - The button enum value for the button of interest

    \returns True if the button is pressed, false otherwise

*//****************************************************************************/
bool PushButtonControl_isButtonPressed(PushButtons_t button)
{
    bool isPressed = false;

    /* Uses short circuit logic to ensure that button is a valid index before indexing
       into array of button info.  Order of comparison is critical */
    if ((button >= PushButton_FIRST) &&
        (button < PushButton_LAST)   &&
        (_buttonInfo[button].buttonPressedDuration > BUTTON_DEBOUNCE_MS))
    {
        isPressed = true;
    }

    return isPressed;
}

/***************************************************************************//**
    Gets the duration (in ms) that the button has been pressed.

    \param[in] button - The button enum value for the button of interest

    \returns The number of milliseconds which the button has been depressed.

    The resolution of the duration depends on the period of the system tick.The
    pressed duration does not account for the debounce time.  Example, if the
    button debounce time is 50 ms and the button has been pressed for 40 ms,
    this function will still return that the button has been "pressed" for
    40 ms even though the debounce time hasn't occured.

*//****************************************************************************/
uint16_t PushButtonControl_ButtonPressedDuration(PushButtons_t button)
{
    uint16_t buttonPressedDuration;

    if ((button >= PushButton_FIRST) &&
        (button <  PushButton_LAST))
    {
        buttonPressedDuration = _buttonInfo[button].buttonPressedDuration;
    }
    else
    {
        buttonPressedDuration = 0u;
    }

    return buttonPressedDuration;
}
/***************************************************************************//**
    Processes the push buttons by looking at the state of the buttons and
    generating the appropriate events based on the button state and how long the
    button has been in that state.

    Debounce the press only.  This sort of debounces the release as well (i.e. press
    won't happen from a bounce on release).

    \note function assumes it is called every system timer tick.
*//****************************************************************************/
static void _ProcessPushButtons(void)
{
	if (GPIO_IS_LOW(BUTTON_0_PORT, BUTTON_0_PIN))
	{
		_buttonInfo[PushButton_ZERO].buttonPressedDuration += TIMERTICK_MSEC_PER_TICK;
	}
	else
	{
	    _buttonInfo[PushButton_ZERO].buttonPressedDuration = 0u;
	}

	if (GPIO_IS_LOW(BUTTON_1_PORT, BUTTON_1_PIN))
	{
	    _buttonInfo[PushButton_ONE].buttonPressedDuration  += TIMERTICK_MSEC_PER_TICK;
	}
	else
	{
	    _buttonInfo[PushButton_ONE].buttonPressedDuration  = 0u;
	}

	/* If button 0 or 1 was pressed (debounced) and is now not pressed,
	   send event for the button release. 	 */
    if ((_buttonInfo[PushButton_ZERO].buttonPressedDuration >= BUTTON_DEBOUNCE_MS) &&
        (_buttonInfo[PushButton_ZERO].previousButtonPressedDuration < BUTTON_DEBOUNCE_MS))
    {
        EventQueue_Add(Event_BUTTON_0_PRESSED, EventPriority_LOWEST);
    }
    else if ((_buttonInfo[PushButton_ZERO].previousButtonPressedDuration > BUTTON_DEBOUNCE_MS) &&
        (_buttonInfo[PushButton_ZERO].buttonPressedDuration == 0u))
    {
        EventQueue_Add(Event_BUTTON_0_RELEASED, EventPriority_LOWEST);
    }

    if ((_buttonInfo[PushButton_ONE].buttonPressedDuration >= BUTTON_DEBOUNCE_MS) &&
        (_buttonInfo[PushButton_ONE].previousButtonPressedDuration < BUTTON_DEBOUNCE_MS))
    {
        EventQueue_Add(Event_BUTTON_1_PRESSED, EventPriority_LOWEST);
    }
    else if ((_buttonInfo[PushButton_ONE].previousButtonPressedDuration > BUTTON_DEBOUNCE_MS) &&
	    (_buttonInfo[PushButton_ONE].buttonPressedDuration == 0u))
	{
	    EventQueue_Add(Event_BUTTON_1_RELEASED, EventPriority_LOWEST);
	}

	_buttonInfo[PushButton_ZERO].previousButtonPressedDuration = _buttonInfo[PushButton_ZERO].buttonPressedDuration;
	_buttonInfo[PushButton_ONE].previousButtonPressedDuration = _buttonInfo[PushButton_ONE].buttonPressedDuration ;
}
