/***************************************************************************//**
    \file Clock.c
    \brief Clock configuration module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This file provides functions to configure the Unified Clock
       System (UCS) on MSP430x5xx microcontrollers.  It also handles any crystal
       failure interrupts.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Clock.c$
    $Revision: 7$
    $Date: Thursday, June 28, 2012 6:01:23 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "Clock.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

#include <Gpio.h>
#include <Board.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** Maximum amount of time (in microseconds) that the oscillator input takes
    to stabilize.  Usually happens within 25 microseconds.  This timeout is
    just used to exit a while loop.
*/
#define OSC_STABILIZE_TIMEOUT_USECS         (250u)
/** Approximate delay (exact MCLK freq not known, probably about 1 MHz) used
    in the loop waiting for the oscillator input to stabilize.
*/
#define OSC_STABILIZE_LOOP_INTERVAL_USECS   (25u)

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

/***************************************************************************//**
    Setup the system clocks.  An external 24 MHz oscillator is used to
    source ACLK and SMCLK as the reference clock source.

    ACLK is sourced by XT2 divided by 32 at 750 KHz.
    SMCLK is sourced by XT2 divided by 1 at 24 MHz.
    MCLK is sourced by the XT2 divided by 1 at 24 MHz.
*//****************************************************************************/
void Clock_Init(void)
{
    uint16_t timeout;

    /* Turn off FLL.  Locks DCO at current frequency. */
    _bis_SR_register(SCG0);

    /* Crystal oscillator settings:
       Turn off XT1.
       Drive strength level 2 for XT2 (operating range of 16 to 24 MHz).
       High frequency mode for XT2.
       No bypass for XT2.
       SMCLK on.
       XT2 on.
    */
    UCSCTL6 = XT2DRIVE_2 | XT1OFF;

    /* Enable XT2 oscillator pins. */
    Gpio_Configure(OSCILLATOR_IN_PORT, OSCILLATOR_IN_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(OSCILLATOR_OUT_PORT, OSCILLATOR_OUT_PIN,
                   GpioType_PERIPHERAL_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);

    /* Wait for oscillator input to stabilize or a timeout expires. */
    timeout = OSC_STABILIZE_TIMEOUT_USECS / OSC_STABILIZE_LOOP_INTERVAL_USECS;
    while (((UCSCTL7 & XT2OFFG) > 0u) && (timeout > 0u))
    {
        /* Clear oscillator fault flag and wait. */
        UCSCTL7 &= ~(XT2OFFG);

        _delay_cycles(OSC_STABILIZE_LOOP_INTERVAL_USECS);
        --timeout;
    }

    ASSERT(timeout > 0u, ErrorLedMode_SOFTWARE);

    /* Set DCO to lowest range  and disable modulation so save power since it isn't used */
    UCSCTL1 = DCORSEL_0;

    /* We are not using the FLL so leave it default and off */
    // no code for this

    /* Clock source settings:
       Ext ACLK = XT2CLK / 32
       ACLK = XT2CLK / 2
       SMCLK = XT2CLK / 1
       MCLK = XT2CLK / 1
    */
    UCSCTL4 = SELA__XT2CLK | SELS__XT2CLK | SELM__XT2CLK;
    UCSCTL5 = DIVPA__32 | DIVA__2 | DIVS__1 | DIVM__1;

    /* Enable conditional requests for SMCLK, MCLK, and ACLK. */
    UCSCTL8 = SMCLKREQEN | MCLKREQEN | ACLKREQEN;

    /* Clear DCO and all oscillator fault flags. */
    UCSCTL7 = 0u;

    /* Clear, then enable oscillator fault interrupt. */
    SFRIFG1 &= ~(OFIFG);
    SFRIE1 |= OFIE;
}


