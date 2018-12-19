/***************************************************************************//**
    \file TimerTick.c
    \brief The system tick.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The timer tick will provide a system tick for triggering
       periodic events.This module provides an interface to register an ISR that
       will be called every "tick" or to request a timeout after a given number
       of ticks.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/TimerTick.c$
    $Revision: 7$
    $Date: Monday, August 06, 2012 7:33:16 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "TimerTick.h"
#include <Timer.h>
#include <EventQueue.h>
#include <Assert.h>
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** The maximum number of active timeouts */
#define MAX_NUM_TIMEOUTS		(10u)

/** The maximum number of timer tick ISRs */
#define MAX_NUM_TICK_ISR		(3u)

/** Frequency (in Hz) of the timer source.
    Depends on the timer configuration performed in TimerTick_Init()
*/
#define TIMER_SOURCE_FREQ_HZ	(6000000ul)
/** Number of timer source clock periods per timer tick (add 500 before /1000 to prevent fixed point rounding errors) */
#define TIMER_SOURCE_PERIODS_PER_TICK	(((TIMER_SOURCE_FREQ_HZ * TIMERTICK_MSEC_PER_TICK) + 500u) / 1000u)
/** The value of the source clock compare module for the compare to occur at */
#define TIMER_COMPARE_VALUE 	(TIMER_SOURCE_PERIODS_PER_TICK - 1u)

/** Structure used to store a timeout */
typedef struct Timeout
{
	bool isActive;
	uint16_t tickCount;
	TickTimeoutType_t type;
	uint16_t periodTicks;
	TickTimeoutCallback_t callback;
	struct Timeout* next;
} Timeout_t;

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Indicates a timer interrupt is running */
static volatile bool _timerActive = false;

/** Counts each tick that occurs.  Does not need to be volatile as the
    timer tick interrupt is always disabled when it is accessed.
*/
static uint16_t _tickCounter = 0u;

/** Storage for the timeout structure list */
static Timeout_t _timeoutList[MAX_NUM_TIMEOUTS];
/** Tracks the first item in the timeout list */
static Timeout_t* _startOfList = NULL;
/** Tracks if a callback is currently being run */
static bool _isCallbackRunning = false;

/** Stores the tick ISR function pointers */
static TickIsrPointer_t _tickIsrList[MAX_NUM_TICK_ISR] = {NULL, NULL, NULL};
/** Stores the active state of the ISR function pointers */
static bool _tickIsrActive[MAX_NUM_TICK_ISR] = {false, false, false};

/** Tracks the maximum number of timeouts that are active at one time */
static uint16_t _maxActiveTimeouts = 0u;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _InsertTimeout(Timeout_t* const newTimeout);
__interrupt void _TimerA1CC0Isr(void);

/***************************************************************************//**
    Initializes the timer tick module. Uses Timer A1 CC0 with timer in continuous
    mode.
*//****************************************************************************/
void TimerTick_Init(void)
{
    uint16_t i;

    for (i = 0u; i < MAX_NUM_TIMEOUTS; ++i)
    {
        _timeoutList[i].isActive = false;
    }

    Timer_A1_Configure(TimerSource_ACLK, TimerDivider_2, TimerRunMode_CONTINUOUS,
                       TIMER_COMPARE_VALUE);

    TIMER_A1_START(TimerRunMode_CONTINUOUS);

    /* Enable the timeout ISR so that once global interrupts are enabled,
       the timer is shown as active, which causes the watchdog timer to be reset.
    */
    TIMER_A1_CC0_INT_CLEAR();
    TIMER_A1_CC0_INT_ENABLE();
}

/***************************************************************************//**
    Processes the timeouts. This function must be called at a regular rate to
    process the timer tick timeouts.
*//****************************************************************************/
void TimerTick_ProcessTimeouts(void)
{
    Timeout_t* timeout;
    TickTimeoutType_t type;

    /* Only increment the counter and check for expired timeouts
       if timeouts are pending
    */
    if (_startOfList != NULL)
    {
        ++_tickCounter;

        /* Find all timeouts that have expired */
        timeout = _startOfList;
        while ((timeout != NULL) && (_tickCounter == timeout->tickCount))
        {
            /* Call the timeout's callback */
            if (timeout->callback != NULL)
            {
                _isCallbackRunning = true;

                (*(timeout->callback))();

                _isCallbackRunning = false;
            }

            /* Delete timeout from the list.
               Since the list is sorted, this timeout is always the first one
               in the list.
            */
            _startOfList = timeout->next;
            timeout->isActive = false;

            /* Re-insert repeat timeouts */
            type = timeout->type;
            if (type == TickTimeoutType_REPEAT)
            {
                _InsertTimeout(timeout);
            }

            timeout = _startOfList;
        }
    }
}

/***************************************************************************//**
    Request a timeout.  Adds a timeout structure to a sorted list.  List is
    sorted from smallest (earliest expiration time) to largest (latest
    expiration time).

    \note The resolution is equal to TIMERTICK_RESOLUTION_MSEC.

    \param[in] type - Type of timeout
    \param[in] milliseconds - Length of timeout in milliseconds
    \param[in] callback - Pointer to the function called when the timeout expires

    \return A handle to the requested timeout.
*//****************************************************************************/
TickTimeoutHandle_t TimerTick_RequestTimeout(
    TickTimeoutType_t type,
    uint16_t milliseconds,
    TickTimeoutCallback_t callback)
{
    bool isTimeoutActive;
    uint16_t index;
    uint16_t numTicks;
    Timeout_t* newTimeout;
    TickTimeoutHandle_t handle = MAX_NUM_TIMEOUTS;

    ASSERT(milliseconds >= TIMERTICK_RESOLUTION_MSEC, ErrorLedMode_SOFTWARE);
    ASSERT(callback != NULL, ErrorLedMode_SOFTWARE);

    /* Disable timeout ISR to prevent modifications to the timeout list. */
    TIMER_A1_CC0_INT_DISABLE();

    /* Get pointer to available storage */
    newTimeout = NULL;
    for (index = 0u; index < MAX_NUM_TIMEOUTS; ++index)
    {
        isTimeoutActive = _timeoutList[index].isActive;
        if (isTimeoutActive == false)
        {
            newTimeout = &_timeoutList[index];
            handle = (TickTimeoutHandle_t) index;

            /* Track the maximum number of timeout structures being used. */
            if ((index + 1u) > _maxActiveTimeouts)
            {
                _maxActiveTimeouts = index + 1u;
            }
            break;
        }
    }

    if (newTimeout != NULL)
    {
        /* Convert milliseconds to ticks */
        numTicks = (milliseconds + (TIMERTICK_MSEC_PER_TICK / 2u)) / TIMERTICK_MSEC_PER_TICK;

        /* Store timeout information */
        newTimeout->type = type;
        newTimeout->periodTicks = numTicks;
        newTimeout->callback = callback;

        /* Insert timeout into sorted list */
        _InsertTimeout(newTimeout);
    }
    else
    {
        /* No available storage for a new timeout */
        ASSERT(false, ErrorLedMode_SOFTWARE);
    }

    /* Always enable the timeout interrupt so that the timer is shown as
       active, which causes the watchdog timer to be reset.
    */
    TIMER_A1_CC0_INT_ENABLE();

    return handle;
}

/***************************************************************************//**
    Cancel the given timeout.  The timeout structure is deleted from the list.

    \note This function should not be called from within a timeout's callback
          function, as this could cause erros in the timeout list.
          Use TimerTick_CancelTimeoutFromCallback() instead.

    \param[in] handle - Handle of timeout to cancel
*//****************************************************************************/
void TimerTick_CancelTimeout(TickTimeoutHandle_t handle)
{
    Timeout_t* timeoutToDelete;
    Timeout_t* timeout;
    Timeout_t* previousTimeout;

    ASSERT(handle < MAX_NUM_TIMEOUTS, ErrorLedMode_SOFTWARE);
    ASSERT(_isCallbackRunning == false, ErrorLedMode_SOFTWARE);

    timeoutToDelete = &_timeoutList[handle];

    /* Timeout only needs to be canceled if it is active */
    if (timeoutToDelete->isActive)
    {
        /* Disable timeout ISR to prevent modifications to the timeout list. */
        TIMER_A1_CC0_INT_DISABLE();

        /* Find the location of the timeout before the timeout to delete */
        previousTimeout = NULL;
        timeout = _startOfList;
        while ((timeout != NULL) && (timeout != timeoutToDelete))
        {
            previousTimeout = timeout;
            timeout = timeout->next;
        }

        /* Assert that the timeout-to-delete was found in the list. */
        ASSERT(timeout != NULL, ErrorLedMode_SOFTWARE);

        /* Delete timeout from the list.
           timeout == timeoutToDelete at this point
        */
        if (previousTimeout == NULL)
        {
            /* Timeout is the first item in the list. */
            _startOfList = timeout->next;
        }
        else
        {
            /* Timeout is NOT the first item in the list */
            previousTimeout->next = timeout->next;
        }
        timeout->isActive = false;

        /* Always enable the timeout interrupt so that the timer is shown as
           active, which causes the watchdog timer to be reset.
        */
        TIMER_A1_CC0_INT_ENABLE();
    }
}

/***************************************************************************//**
    Cancel the timeout running this callback.  The timeout structure is deleted
    from the list.
*//****************************************************************************/
void TimerTick_CancelTimeoutFromCallback(void)
{
    ASSERT(_isCallbackRunning == true, ErrorLedMode_SOFTWARE);
    ASSERT(_startOfList != NULL, ErrorLedMode_SOFTWARE);

    /* Change the type of the timeout to SINGLE so that the timeout does not
       get re-inserted into the list.  Since the callback of this timeout is
       running, the timeout must be the first timeout in the list.
    */
    _startOfList->type = TickTimeoutType_SINGLE;
}

/***************************************************************************//**
    Get timer active status and reset status.

    \return The timer interrupt is active or not.
*//****************************************************************************/
bool TimerTick_IsTimerActive(void)
{
    bool returnValue = _timerActive;

    _timerActive = false;

    return returnValue;
}

/***************************************************************************//**
    \return The maximum number of timeouts that are active at one time.
*//****************************************************************************/
uint16_t TimerTick_GetMaxActiveTimeouts(void)
{
    return _maxActiveTimeouts;
}

/***************************************************************************//**
    Register a tick ISR, that is an ISR that fires once every tick.

    \param[in] tickIsr - ISR to register.

    \return Handle of tick ISR.  If handle is negative, registration failed.
*//****************************************************************************/
TickIsrHandle_t TimerTick_RegisterTickIsr(TickIsrPointer_t tickIsr)
{
    int16_t index;
    TickIsrHandle_t handle = -1;

    ASSERT(tickIsr != NULL, ErrorLedMode_SOFTWARE);

    /* Find open spot in list */
    for (index = 0; index < MAX_NUM_TICK_ISR; ++index)
    {
        if (_tickIsrList[index] == NULL)
        {
            handle = (TickIsrHandle_t) index;
            break;
        }
    }

    /* If spot is found, ensure ISR is disabled first before saving
       function pointer.
    */
    if (handle >= 0)
    {
        TimerTick_DisableTickIsr(handle);
        _tickIsrList[handle] = tickIsr;
    }

    return handle;
}

/***************************************************************************//**
    Unregister the tick ISR with the given handle.

    \param[in] handle - Handle of tick ISR
*//****************************************************************************/
void TimerTick_UnregisterTickIsr(TickIsrHandle_t handle)
{
    ASSERT((handle >= 0) && (handle < MAX_NUM_TICK_ISR), ErrorLedMode_SOFTWARE);

    TimerTick_DisableTickIsr(handle);
    _tickIsrList[handle] = NULL;
}

/***************************************************************************//**
    Enables the tick ISR with the given handle.  The timer interrupt flag is
    cleared first, so the ISR will not be called for up to one tick period.

    \param[in] handle - Handle of tick ISR
*//****************************************************************************/
void TimerTick_EnableTickIsr(TickIsrHandle_t handle)
{
    ASSERT((handle >= 0) && (handle < MAX_NUM_TICK_ISR), ErrorLedMode_SOFTWARE);

    _tickIsrActive[handle] = true;
}

/***************************************************************************//**
    Disables the tick ISR with the given handle.

    \param[in] handle - Handle of tick ISR
*//****************************************************************************/
void TimerTick_DisableTickIsr(TickIsrHandle_t handle)
{
    ASSERT((handle >= 0) && (handle < MAX_NUM_TICK_ISR), ErrorLedMode_SOFTWARE);

    _tickIsrActive[handle] = false;
}

/***************************************************************************//**
    Insert a new timeout structure into the list.  List is sorted from smallest
    (earliest expiration time) to largest (latest expiration time).

    \param[in] newTimeout - New timeout to insert into the list.
*//****************************************************************************/
static void _InsertTimeout(Timeout_t* const newTimeout)
{
    uint16_t periodTicks;
    uint16_t tickCount;
    uint16_t currentTickCount;
    Timeout_t* timeout;
    Timeout_t* nextLatestTimeout;
    Timeout_t* previousTimeout;

    /* Determine next tick count when the timeout will occur.
       Adjust the tick counter if the timeout period causes the tick counter
       to rollover.
    */
    periodTicks = newTimeout->periodTicks;
    currentTickCount = _tickCounter;
    if (periodTicks > (0xFFFFu - currentTickCount))
    {
        /* Reduce the current tickCount of every active timeout by the
           current tick count and reset the _tickCounter to zero.
           Each tickCount will be greater than the current tick count,
           otherwise the timeout would have expired.
        */
        timeout = _startOfList;
        while (timeout != NULL)
        {
            tickCount = timeout->tickCount;
            timeout->tickCount = tickCount - currentTickCount;

            timeout = timeout->next;
        }
        currentTickCount = 0u;
        _tickCounter = 0u;
    }

    newTimeout->tickCount = periodTicks + currentTickCount;

/* The compiler incorrectly flags MISRA rule 17.6 in the following code.
   The rule states that the address of an automatic variable should not be
   stored.  The compiler flags the storage of the newTimeout pointer in
   static variables.  This is not a violation, as only the pointer is an
   automatic variable, not the object it points to.
*/
#pragma CHECK_MISRA("-17.6")
    /* If the timeout list is empty, start new list.
       Otherwise, search list for correct location.
    */
    if (_startOfList == NULL)
    {
        newTimeout->next = NULL;
        _startOfList = newTimeout;
    }
    else
    {
        /* Find first timeout that expires after new timeout */
        nextLatestTimeout = _startOfList;
        previousTimeout = NULL;
        while ((nextLatestTimeout->tickCount <= newTimeout->tickCount) &&
               (nextLatestTimeout->next != NULL))
        {
            previousTimeout = nextLatestTimeout;
            nextLatestTimeout = nextLatestTimeout->next;
        }

        if (nextLatestTimeout->tickCount <= newTimeout->tickCount)
        {
            /* Reached last item in the list.
               Insert new timeout after the last one.
            */
            nextLatestTimeout->next = newTimeout;
            newTimeout->next = NULL;
        }
        else
        {
            /* Found next latest timeout in the list.  Insert the new
               timeout before this one.
            */
            if (previousTimeout == NULL)
            {
                _startOfList = newTimeout;
            }
            else
            {
                previousTimeout->next = newTimeout;
            }
            newTimeout->next = nextLatestTimeout;
        }
    }
#pragma RESET_MISRA("all")

    /* Indicate that timeout is active */
    newTimeout->isActive = true;
}

/***************************************************************************//**
    Interrupt service routine for the timer A1 compare/capture 0 interrupt.
    The interrupt flag is cleared in hardware when the ISR is called.

    Increment tick counter and check timeout list for expired timeouts.

    \note Worst case ISR length depends on the time spent in the callbacks.
*//****************************************************************************/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void _TimerA1CC0Isr(void)
{
    /* set the compare module for the next timeout value */
    TIMER_A1_CC0_INCREMENT(TIMER_COMPARE_VALUE);

    /* Indicate that the timer interrupt fired */
    _timerActive = true;

    /* Call the Tick ISRs */
	if ((_tickIsrActive[0] == true) && (_tickIsrList[0] != NULL))
	{
		(*_tickIsrList[0])();
	}

	if ((_tickIsrActive[1] == true) && (_tickIsrList[1] != NULL))
	{
		(*_tickIsrList[1])();
	}

	if ((_tickIsrActive[2] == true) && (_tickIsrList[2] != NULL))
	{
		(*_tickIsrList[2])();
	}

	/* add a system tick event as low priority because tick is not as time critical */
	EventQueue_Add(Event_SYSTEM_TICK, EventPriority_LOWEST);

    _low_power_mode_off_on_exit();
}
