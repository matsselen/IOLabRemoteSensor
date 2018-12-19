/***************************************************************************//**
    \file Interrupt.c
    \brief Global interrupt control

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The macros defined in this file provide global control
       of the MCU's interrupts.  The global interrupt enable (GIE) bit must
       be set for any interrupt to fire.  If the GIE bit is clear, all
       interrupts (except the non-maskable interrupts) are suppressed.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Interrupt.c$
    $Revision: 3$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Interrupt.h"

#include <Device.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

/***************************************************************************//**
    Get the current global interrupt state and then disable global interrupts.

    Usage:
    \code
    IntState_t originalState = Interrupt_Lock();

        (Critical section, all maskable interrupts disabled)

    Interrupt_Unlock(originalState);
    \endcode

    \return Original interrupt state.
*//****************************************************************************/
IntState_t Interrupt_Lock(void)
{
    IntState_t originalState = IntState_DISABLED;

    if ((_get_SR_register() & GIE) > 0u)
    {
        originalState = IntState_ENABLED;
    }

    INTERRUPT_DISABLE();

    return originalState;
}

/***************************************************************************//**
    Restore previous global interrupt state.

    Usage:
    \code
    IntState_t originalState = Interrupt_Lock();

        (Critical section, all maskable interrupts disabled)

    Interrupt_Unlock(originalState);
    \endcode

    \param[in] prevState - Previous interrupt state
*//****************************************************************************/
void Interrupt_Unlock(IntState_t prevState)
{
    if (prevState == IntState_ENABLED)
    {
        INTERRUPT_ENABLE();
    }
}
