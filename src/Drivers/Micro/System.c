/***************************************************************************//**
    \file System.c
    \brief System module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This file provides functions to configure the system
       control module on MSP430x5xx microcontrollers.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/System.c$
    $Revision: 7$
    $Date: Tuesday, October 02, 2012 6:14:15 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "System.h"
#include <Board.h>
#include <Gpio.h>
#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** This will disable the boot strap loader. Some instances might want the bootstrap
 *  loader enabled so that the memory can be reprogrammed over the BSL
 */
#define DISABLE_BSL
/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

/***************************************************************************//**
    Initializes the system by enabling a few safety features.
*//****************************************************************************/
void System_Init(void)
{
#ifdef DISABLE_BSL
    /* Disable the bootstrap loader memory, acts like vacant memory */
    SYSBSLC |= SYSBSLOFF;

    /* Enable the Flash controller access vioaltion interrupt and
       the vacant memeory access interrupt.
    */
    /* Enable Flash controller access violation interrupt */
    SFRIE1 |= ACCVIE;

    /* TODO SPG: enable this interrupt in release version,
       the debugger causes this interrupt to occur
    */
#if 0
    /* Enable the vacant memory access interrupt */
    SFRIE1 |= VMAIE;
#endif
#endif
    /* Disable the internal LDO, note the register must be unlocked, then relocked */
    LDOKEYPID = LDOKEY;
    LDOPWRCTL &= ~(LDOOEN);
    LDOKEYPID = 0u;
}

/***************************************************************************//**
    Enables the power latch for the system.
*//****************************************************************************/
void System_PowerOn(void)
{
	Gpio_SetPortU(POWER_LATCH_PIN);
}

/***************************************************************************//**
    Disables the power latch for the system.
*//****************************************************************************/
void System_PowerOff(void)
{
	Gpio_ClearPortU(POWER_LATCH_PIN);
}

/***************************************************************************//**
    \return The reason for the most recent reset of the micro.
*//****************************************************************************/
SystemResetReason_t System_GetResetReason(void)
{
    uint16_t resetVector;
    SystemResetReason_t reason;

    resetVector = SYSRSTIV;
    reason = (SystemResetReason_t) resetVector;

    return reason;
}

/***************************************************************************//**
    Convert a reset reason to a human-readable string.

    \param[in] reason - Reset reason to convert.

    \return Pointer to the string describing the reset reason.
*//****************************************************************************/
const char_t* System_ConvertResetReasonToString(SystemResetReason_t reason)
{
	/** Defines the number of strings for reset reasons */
#define NUM_RESET_REASON_STRINGS    ( 17u )
    /** Strings describing the possible reset reasons */
    static const char_t* const _resetReasonStrings[NUM_RESET_REASON_STRINGS] =
    {
        "   Invalid",
        "   Brownout",
        "   Reset pin",
        "   PMM software BOR",
        "   Wakeup from LPMx.5",
        "   Security violation",
        "   Vcore dropped below SVSL level",
        "   Vcc dropped below SVSH level",
        "   Vcore reached SVML over-voltage level",
        "   Vcc reached SVMH over-voltage level",
        "   PMM software POR",
        "   Watchdog timeout",
        "   Watchdog password violation",
        "   Flash password violation",
        "   PLL unlock",
        "   peripheral/configuration area fetch",
        "   PMM password violation"
    };

    uint16_t index;
    const char_t* string;

    index = ((uint16_t) reason) / 2u;

    /* Make sure the index is a valid range */
    if (index < NUM_RESET_REASON_STRINGS)
    {
    	string = _resetReasonStrings[index];
    }
    else
    {
    	string = _resetReasonStrings[0u];
    }

    return string;
}
