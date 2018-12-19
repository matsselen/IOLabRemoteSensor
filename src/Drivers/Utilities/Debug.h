/***************************************************************************//**
    \file Debug.h
    \brief Debug module that enables/disables debugging

    \b Project IOLabRemoteSensorApplication

    \b Description The debug module controls the if the debugging is enabled or
       disabled.

    \note <b>This file assumes that Serial.h is included for printing
       serial debug messages</b>.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Debug.h$
    $Revision: 3$
    $Date: Thursday, November 13, 2014 6:08:37 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DEBUG_H_
#define DEBUG_H_

#include <Serial.h>
/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** If defined, debug messages are enabled. */
//#define ENABLE_DEBUG_MESSAGES

/*-----------------------PUBLIC MACROS----------------------------------------*/

#ifdef ENABLE_DEBUG_MESSAGES

/** Macro used to print messages to the debug serial port.
    Does not block.
*/
#ifndef SERIAL_ENABLE_PRINTF
#define DEBUG(message)              Serial_SendString(message)
#else
#define DEBUG( message... )         Serial_Printf( message );
#endif
/** Macro used to print messages to the debug serial port.
    Blocks until the entire message is sent.
*/
#define DEBUG_BLOCKING(message)     Serial_SendStringBlocking(message)

#else

/** Debug messages are off.  Macro definition is empty */
#define DEBUG(message)
/** Debug messages are off.  Macro definition is empty */
#define DEBUG_BLOCKING(message)

#endif

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* DEBUG_H_ */
