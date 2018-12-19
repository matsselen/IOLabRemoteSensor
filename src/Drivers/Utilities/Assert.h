/***************************************************************************//**
    \file Assert.h
    \brief Manages asserts use to check expected logic conditions

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The assert module can be used to check for certain logic
       conditions and stop execution if the expected conditions were not present.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Assert.h$
    $Revision: 6$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ASSERT_H_
#define ASSERT_H_

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Possible error modes.
    The value of each mode is blinked on the Safe and Pair LEDs simultaneously.
*/
typedef enum ErrorLedMode
{
    ErrorLedMode_NONE = 0u,         /* No error condition */
    ErrorLedMode_MEMORY,            /* Memory corruption */
    ErrorLedMode_SOFTWARE           /* Various SW errors */
} ErrorLedMode_t;

/*-----------------------PUBLIC MACROS----------------------------------------*/
#define ASSERT_DISABLED

#ifndef ASSERT_DISABLED

#define SHORT_NAMES
#ifdef __TI_COMPILER_VERSION__
/** Assertion macro used to check for invalid conditions.
    Version used for TI compiler, which does not like the do-while construct.
*/
#define ASSERT(expr, errorMode)     {                                                         \
                                        if(!(expr))                                           \
                                        {                                                     \
                                            Assert((uint16_t) __LINE__, __FILE__, errorMode); \
                                        }                                                     \
                                    }
#else
/** Assertion macro used to check for invalid conditions.
    Version used for Lint, as Lint complains about a NULL statement not being
    on a line by itself (MISRA rule 14.3) with the above definition.
*/
#define ASSERT(expr, errorMode)     /*lint -save -e506 -e774 */                               \
                                    do                                                        \
                                    {                                                         \
                                        if (!(expr))                                          \
                                        {                                                     \
                                            Assert((uint16_t) __LINE__, __FILE__, errorMode); \
                                        }                                                     \
                                    } while(__LINE__ == 0)                                    \
                                    /*lint -restore */
#endif

#else
/** Assertion macro disabled.  Declare a void call of expr so that the variable isn't unused */
#define ASSERT(expr, errorMode) ((void)(expr))
#endif

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
#ifndef ASSERT_DISABLED
extern void Assert(
    uint16_t lineNumber,
    const char_t* fileNameWithPath,
    ErrorLedMode_t errorMode);
#endif


#endif /* ASSERT_H_ */
