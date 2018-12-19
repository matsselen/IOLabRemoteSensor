/***************************************************************************//**
    \file Board.c
    \brief Board initialization module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This file contains the board initialization function.
       This function must be called immediately after start-up to configure
       the system clocks, the GPIO pins, and the analog input pins.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Board.c$
    $Revision: 4$
    $Date: Friday, July 06, 2012 2:33:06 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "Board.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

#include <Gpio.h>
#include <Watchdog.h>
#include <System.h>
#include <Pmm.h>
#include <Clock.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

/***************************************************************************//**
    Initializes the board to a good default state.  This includes configuring
    the GPIO pins, system clocks, power management module (PMM), system
    interrupts, and the watchdog timer.

    This should be one of the first Init functions called so that the board
    (in particular, the GPIO pins) are put in a good state ASAP.
*//****************************************************************************/
void Board_Init(void)
{
	/* Initialize the watchdog then pet it to keep the system alive */
	WATCHDOG_INIT();

	/* Initialize the system resources */
	System_Init();
	WATCHDOG_RESET();

	/* Initialize all GPIO pins */
	Gpio_Init();
	WATCHDOG_RESET();

	/* Initialize the power management module to allow for necessary speed and power settings */
	Pmm_Init();
	WATCHDOG_RESET();

	/* Setup system clocks */
	Clock_Init();
	WATCHDOG_RESET();
}
