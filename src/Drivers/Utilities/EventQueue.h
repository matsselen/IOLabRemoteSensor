/***************************************************************************//**
    \file EventQueue.h
    \brief Event queue module header file.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The event queue module provides a FIFO buffer for enqueueing
       events in the system.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/EventQueue.h$
    $Revision: 8$
    $Date: Friday, July 17, 2015 2:23:27 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** List of all possible events */
typedef enum Event
{
	Event_INVALID = -1,
	Event_FIRST = 0,
	Event_TIMEOUT = Event_FIRST,
	Event_ADC_CONVERSION_SEQUENCE_COMPLETE,
	Event_SERIAL_RECEIVED_PACKET,
	Event_LED_CONTROL_TIMEOUT,		// TODO: spg - what does this event do?
	Event_STATE_MACHINE_TIMEOUT,	// TODO: spg - what does this event do?
	Event_SPI_SLAVE_TX_COMPLETE,
	Event_SPI_SLAVE_RX_COMPLETE,
	Event_ACCELEROMETER_DATA_READY,
	Event_MAGNETOMETER_DATA_READY,
	Event_GYROSCOPE_DATA_READY,
	Event_BAROMETER_DATA_READY,
	Event_SYSTEM_TICK,
	Event_SHUTDOWN,
	Event_BUTTON_0_PRESSED,
	Event_BUTTON_0_RELEASED,
	Event_BUTTON_1_PRESSED,
    Event_BUTTON_1_RELEASED,
    Event_START_SELF_TEST,
    Event_SELF_TEST,
    Event_SELF_TEST_COMPLETE,
    EventApplication_SEQUENCE_COMPLETE,
	Event_LAST
} Event_t;

/** The possible event priorities */
typedef enum EventPriority
{
	EventPriority_LOWEST = 0,					/** Lowest priority should be used by most events */
	EventPriority_NORMAL,						/** > Lowest priority. */
	EventPriority_HIGH,							/** High priority events should only be used by a Serial Messages */
	EventPriority_HIGHEST = EventPriority_HIGH  /** HIGHEST enum is only to be used in processing functions only */
} EventPriority_t;
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void EventQueue_Init(void);
void EventQueue_Add(Event_t newEvent ,
                    EventPriority_t priority);
bool EventQueue_GetNext(Event_t *nextEvent);
bool EventQueue_IsEmpty(void);

uint16_t EventQueue_GetMaxLength(EventPriority_t priority);
#endif /* EVENTQUEUE_H_ */
