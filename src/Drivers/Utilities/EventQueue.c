/***************************************************************************//**
    \file EventQueue.c
    \brief Event queue module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The event queue module provides a FIFO buffer for enqueueing
       events in the system.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/EventQueue.c$
    $Revision: 3$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "EventQueue.h"
#include <Interrupt.h>
#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** The maximum number of events minus one that the queue can store */
#define QUEUE_SIZE			(32u)

/** The number of priority levels */
#define NUM_PRIORITIES		(((int8_t) EventPriority_HIGHEST) + 1)

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Array to store the events, grouped by priority */
static Event_t _eventQueue[NUM_PRIORITIES][QUEUE_SIZE];

/** Stores the index of the next available spot in the queue for each priority */
static volatile uint16_t _queueIndexHead[NUM_PRIORITIES];

/** Stores the index of the next event to process from the queue for each priority */
static volatile uint16_t _queueIndexTail[NUM_PRIORITIES];

/** Stores the maximum length the queue has reached at priority levels. This is used for
    load leveling to check how close the queue has been to being compeletely full
*/
static uint16_t _queueMaxLength[NUM_PRIORITIES];

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _CheckQueueLength(EventPriority_t priority);

/***************************************************************************//**
    Initializes the queue.
*//****************************************************************************/
void EventQueue_Init(void)
{
	volatile int16_t priority;

	for (priority = (int16_t)EventPriority_LOWEST; priority <= (int16_t) EventPriority_HIGHEST; ++priority)
	{
		_queueIndexHead[priority] = 0u;
		_queueIndexTail[priority] = 0u;
		_queueMaxLength[priority] = 0u;
	}
}

/***************************************************************************//**
    Adds an event to the queue at a given priority.

    \param[in] newEvent - Event to add to the queue.
    \param[in] priority - Priority of the event.

    \note This does disable interrupts so that if it events are added in interrupt
          context and general application context then there won't be a race condition.
*//****************************************************************************/
void EventQueue_Add(Event_t newEvent,
                    EventPriority_t priority)
{
	uint16_t indexHead;
	IntState_t interruptState;

	/* interrupts must be disabled here to prevent events from being overwritten */
	interruptState = Interrupt_Lock();

	/* add the event to the queue */
	indexHead = _queueIndexHead[priority];
	_eventQueue[priority][indexHead] = newEvent;

	/* update the queue head and tail. Determine the next position in the queue */
	if (indexHead == (QUEUE_SIZE - 1u))
	{
		indexHead = 0u;
	}
	else
	{
		++indexHead;
	}

	/* if the queue is not full, update head index. Otherwise assert because the
	   queue should never fill all the way up.
	*/
	if(indexHead != _queueIndexTail[priority])
	{
		_queueIndexHead[priority] = indexHead;
	}
	else
	{
		ASSERT(false, ErrorLedMode_SOFTWARE);
	}

	/* now that the event is added, unlock the interrupts */
	Interrupt_Unlock(interruptState);
}

/***************************************************************************//**
    Gets the next event on the queue.

    \param[out] nextEvent - The event removed from the queue.

    \return True if an event was retrieved.  False if the entire queue
            was empty.
*//****************************************************************************/
bool EventQueue_GetNext(Event_t *nextEvent)
{
	uint16_t indexTail;
	volatile int16_t priority;
	bool eventAvailable = false;

	/* search all priority levels, starting at the highest */
	for (priority = (int16_t) EventPriority_HIGHEST; priority >= (int16_t) EventPriority_LOWEST; --priority)
	{
		/* check that the queue is not empty */
		indexTail = _queueIndexTail[priority];
		if(indexTail != _queueIndexHead[priority])
		{
			eventAvailable = true;

			/* return the oldest event */
			*nextEvent = _eventQueue[priority][indexTail];
			_CheckQueueLength((EventPriority_t) priority);

			/* increment to the next oldest event */
			if (indexTail == (QUEUE_SIZE - 1))
			{
				indexTail = 0u;
			}
			else
			{
				++indexTail;
			}

			_queueIndexTail[priority] = indexTail;

			break;
		}
	}

	return eventAvailable;
}

/***************************************************************************//**
    Determines if the event queue is empty.

    \return True if the entire event queue is empty.  Otherwise false.
*//****************************************************************************/
bool EventQueue_IsEmpty(void)
{
	uint16_t indexTail;
	volatile int16_t  priority;
	bool queueEmpty = true;

	/* Check the queue for each priority level */
	for (priority = (int16_t) EventPriority_LOWEST; priority <= (int16_t) EventPriority_HIGHEST; ++priority)
	{
		indexTail = _queueIndexTail[priority];
		if (indexTail != _queueIndexHead[priority])
		{
			queueEmpty = false;
			break;
		}
	}

	return queueEmpty;
}

/***************************************************************************//**
    Get the maximum length of the queue at the given priority level.

    \param[in] priority - Selected priority level.

    \return The maximum length of the queue.
*//****************************************************************************/
uint16_t EventQueue_GetMaxLength(EventPriority_t priority)
{
    return _queueMaxLength[priority];
}

/***************************************************************************//**
    Checks the queue length at the given priority level.

    \param[in] priority - Selected priority level.
*//****************************************************************************/
static void _CheckQueueLength(EventPriority_t priority)
{
    uint16_t indexHead = _queueIndexHead[priority];
    uint16_t indexTail = _queueIndexTail[priority];
    uint16_t length;

    /* Calculate length of queue */
    if (indexHead > indexTail)
    {
        length = indexHead - indexTail;
    }
    else
    {
        length = (((uint16_t) QUEUE_SIZE) - indexTail) + indexHead;
    }

    /* Track the maximum length */
    if (length > _queueMaxLength[priority])
    {
        _queueMaxLength[priority] = length;
    }
}




