/***************************************************************************//**
    \file Assert.c
    \brief Manages asserts use to check expected logic conditions

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The assert module can be used to check for certain logic
       conditions and stop execution if the expected conditions were not present.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Assert.c$
    $Revision: 7$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Assert.h"

#include <Debug.h>
#include <Serial.h>
#include <Gpio.h>
#include <Board.h>
#include <Clock.h>
#include <Interrupt.h>
#include <Device.h>

#ifndef ASSERT_DISABLED
#include <stdio.h>
#endif

#ifdef SHORT_NAMES
#include <string.h>
#endif

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** The delay between loops of the assert handler. */
#define ERROR_HANDLER_DELAY_USECS (100000ul)


/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static const char_t* _ErrorModeToString( ErrorLedMode_t errorMode);
#ifdef SHORT_NAMES
static const char_t* _FindFileNameInPath(const char_t* fileNameWithPath);
#endif

/***************************************************************************//**
    Called when an assertion fails.  Writes the file name and line number
    via the serial debug port and then enters an infinite loop.  Within the
    loop, the LED Control error handler is called periodically so that the
    value of the error mode is displayed on the LEDs.

    \param[in] lineNumber - Line number where the assertion failed.
    \param[in] fileNameWithPath - Pointer to the name of the file with path
                                  where the assertion failed.
    \param[in] errorMode - Assigned error mode for this assertion.
*//****************************************************************************/
#pragma FUNC_NEVER_RETURNS(Assert)
void Assert(
    uint16_t lineNumber,
    const char_t* fileNameWithPath,
    ErrorLedMode_t errorMode)
{
	/* allocate space for the serial debug message */
	char_t replyString[SERIAL_PAYLOAD_MAX_LENGTH];
	const char_t* fileName;
	const char_t* errorName;

	INTERRUPT_DISABLE();

#ifdef SHORT_NAMES
	fileName = _FindFileNameInPath(fileNameWithPath);
#else
	fileName = fileNameWithPath;
#endif

	errorName = _ErrorModeToString(errorMode);

	if( fileNameWithPath != NULL )
	{
#ifndef ASSERT_DISABLED
		(void) snprintf(replyString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
			            "ASSERT:%s at %s:%d", errorName, fileName, lineNumber);
#endif
	}
	else
	{
#ifndef ASSERT_DISABLED
		(void) snprintf(replyString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
						"ASSERT:%s at Unknown:%d", errorName, lineNumber);
#endif
	}

	Serial_SendStringBlockingNoAssert(replyString);

    /* Prevent unused warning when asserts are disabled */
#ifdef ASSERT_DISABLED
    (void)errorName;
    (void)fileName;
#endif

	/* infinite loop */
	for(;;)
	{
		GPIO_TOGGLE(LED0_RED_PORT, LED0_RED_PIN);
		CLOCK_DELAY_US(ERROR_HANDLER_DELAY_USECS);
	}
}

/***************************************************************************//**
    Converts the ErrorLedMode_t enum into string names

    \param[in] errorMode - the error mode enum to be converted to a string

    \return Pointer to the string.
*//****************************************************************************/
static const char_t* _ErrorModeToString( ErrorLedMode_t errorMode)
{
	const char_t* errorString = NULL;

	switch(errorMode)
	{
		case ErrorLedMode_MEMORY:
			errorString = "memory";
			break;

		case ErrorLedMode_SOFTWARE:
			errorString = "software";
			break;

		case ErrorLedMode_NONE:
			errorString = "none";
			break;

		default:
			errorString = "unknown";
			break;
	}

	return errorString;
}


#ifdef SHORT_NAMES
/***************************************************************************//**
    Finds the file name string within the string containing the path
    and filename.

    \param[in] fileNameWithPath - Name of file with its path.

    \return Pointer to the file name string.
*//****************************************************************************/
static const char_t* _FindFileNameInPath(const char_t* fileNameWithPath)
{
    const char_t* lastSlash;
    const char_t* fileName = NULL;

    if (fileNameWithPath != NULL)
    {
/* Disable MISRA checks to allow pointer arithmetic and copying of
   fileNameWithPath pointer.
*/
/*lint -esym(960, 17.4) */
#pragma CHECK_MISRA("none")

        /* Find file name in path */
        lastSlash = strrchr(fileNameWithPath, '/');
        if (lastSlash != NULL)
        {
            /* Point to next character after slash */
            fileName = lastSlash + 1;
        }
        else
        {
            fileName = fileNameWithPath;
        }

#pragma RESET_MISRA("all")
/*lint +esym(960, 17.4) */
    }

    return fileName;
}
#endif
