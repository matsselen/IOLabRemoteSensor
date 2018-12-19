/***************************************************************************//**
    \file TimerTick.h
    \brief Timer Tick utility header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The timer tick will provide a system tick for triggering
       periodic events.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/TimerTick.h$
    $Revision: 4$
    $Date: Friday, July 06, 2012 2:32:38 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef TIMERTICK_H_
#define TIMERTICK_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the number of milliseconds per timer tick.
    Value based on timer setup.  Must be greater than 2 to convert from
    milliseconds to ticks without overflowing.
*/
#define TIMERTICK_MSEC_PER_TICK         (10u)
/** Defines the resolution (in milliseconds) of the timer tick timeout values. */
#define TIMERTICK_RESOLUTION_MSEC       (TIMERTICK_MSEC_PER_TICK)

/** Defines the possible types of timeouts */
typedef enum TickTimeoutType
{
	TickTimeoutType_SINGLE,
	TickTimeoutType_REPEAT
} TickTimeoutType_t;

/** Definition of a tick timeout handle */
typedef uint16_t TickTimeoutHandle_t;
/** Defines a tick timeout callback function pointer */
typedef void (*TickTimeoutCallback_t)(void);

/** Definition of a tick ISR handle */
typedef int16_t TickIsrHandle_t;
/** Defines a tick ISR function pointer */
typedef void (*TickIsrPointer_t)(void);

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void TimerTick_Init(void);
void TimerTick_ProcessTimeouts(void);

TickTimeoutHandle_t TimerTick_RequestTimeout(
    TickTimeoutType_t type,
    uint16_t milliseconds,
    TickTimeoutCallback_t callback);
void TimerTick_CancelTimeout(TickTimeoutHandle_t handle);
void TimerTick_CancelTimeoutFromCallback(void);

bool TimerTick_IsTimerActive(void);
uint16_t TimerTick_GetMaxActiveTimeouts(void);

TickIsrHandle_t TimerTick_RegisterTickIsr(TickIsrPointer_t tickIsr);
void TimerTick_UnregisterTickIsr(TickIsrHandle_t handle);
void TimerTick_EnableTickIsr(TickIsrHandle_t handle);
void TimerTick_DisableTickIsr(TickIsrHandle_t handle);

#endif /* TIMERTICK_H_ */
