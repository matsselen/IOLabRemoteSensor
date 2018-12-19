/***************************************************************************//**
    \file Watchdog.h
    \brief Watchdog module header file.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This file contains macros that control the watchdog timer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Watchdog.h$
    $Revision: 5$
    $Date: Thursday, September 06, 2012 7:27:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef WATCHDOG_H_
#define WATCHDOG_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** If defined, the watchdog timer is enabled. */
#if 1
#define ENABLE_WATCHDOG
#endif

/*-----------------------PUBLIC MACROS----------------------------------------*/

#ifdef ENABLE_WATCHDOG

/** Defines the following watchdog timer settings:
     -Timer will expire after 512K (512 * 1024) clock cycles
     -Timer is source from ACLK
     -Reset timer when written
*/
#define WATCHDOG_SETTINGS       (WDTCNTCL | WDTSSEL__ACLK | WDTIS__8192K)

/** Pre-initialize the watchdog timer. This is just to get the memory initialized before main starts */
#define WATCHDOG_PRE_INIT()     (WDTCTL = (WDTPW | WDTSSEL__VLO | WDTIS__512K))
/** Initialize the watchdog timer with the above settings. */
#define WATCHDOG_INIT()         WATCHDOG_START()
/** Starts the watchdog timer with the above settings. */
#define WATCHDOG_START()        (WDTCTL = (WDTPW | WATCHDOG_SETTINGS))
/** Reset the watchdog timer (keeps the above settings). */
#define WATCHDOG_RESET()        WATCHDOG_START()

#else /* DISABLE_WATCHDOG */

/** Defines the following watchdog timer settings:
     -Timer will expire after 2G (2 * 1024^3) clock cycles
     -Timer is source from the VLO clock
     -Reset timer when written
*/
#define WATCHDOG_SETTINGS       (WDTCNTCL | WDTSSEL__VLO | WDTIS__2G)

/** Pre-initialize the watchdog timer. This is just to get the memory initialized before main starts */
#define WATCHDOG_PRE_INIT()     WATCHDOG_STOP()
/** Initialize (stop) the watchdog timer.
    Note that this cannot be empty because the watchdog timer is running by default.
*/
#define WATCHDOG_INIT()         WATCHDOG_STOP()
/** Empty start watchdog macro. */
#define WATCHDOG_START()
/** Empty reset watchdog macro. */
#define WATCHDOG_RESET()

#endif /* ENABLE_WATCHDOG */

/** Stops the watchdog timer. */
#define WATCHDOG_STOP()         (WDTCTL = (WDTPW | WDTHOLD | WATCHDOG_SETTINGS))
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* WATCHDOG_H_ */
