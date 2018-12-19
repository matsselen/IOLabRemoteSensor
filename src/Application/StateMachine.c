/***************************************************************************//**
    \file StateMachine.c
    \brief State logic handling

    \b Project IOLabRemoteSensorApplication

    \b Description Handles the state logic and transition between states.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/StateMachine.c$
    $Revision: 10$
    $Date: Friday, July 17, 2015 2:21:32 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "StateMachine.h"
#include <EventQueue.h>
#include <CommProtocol.h>
#include <TestProtocol.h>
#include <AccelerometerControl.h>
#include <MagnetometerControl.h>
#include <GyroscopeControl.h>
#include <PushButtonControl.h>
#include <LedControl.h>
#include <TimerTick.h>
#include <AdcManager.h>
#include <SelfTest.h>
#include <System.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Possible states for the sensor micro to be in */
typedef enum State
{
	State_INVALID = -1,
	State_FIRST   = 0,
	State_RUN     = State_FIRST,
	State_SELF_TEST,
	State_LAST
} State_t;

/** The number of milliseconds that the button must be pressed before the power
    will be shut off.
 */
#define POWER_OFF_MSEC    ( 1000u )
/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Current system state */
static State_t _currentState = State_RUN;

/** Stores if button one has been pressed or not.  This makes sure that the button
    has been pressed before it was released, and was not pressed before power on
    (in which case the firmware wouldn't see the press event) then released.  */
static bool _buttonOnePressed = false;

/** Handle for the system power off timeout */
static TickTimeoutHandle_t _powerOffTimeoutHandle;
/** Tracks if the power off timeout is in use */
static bool _powerOffTimeoutInUse = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static bool _ProcessEvents_RUN(Event_t event);
static bool _ProcessEvents_SELF_TEST(Event_t event);
static bool _ProcessEvents_DEFAULT(Event_t event);
__inline static void _UnlatchPower(void);
static void _HandlePowerOffRequest(void);

/***************************************************************************//**
    State machine logic that will process all the events from the event queue and
    manage the states of the system
*//****************************************************************************/
void StateMachine_ProcessEvents(void)
{
    bool eventValid = false;
    bool continueProcessingEvent = false;
    Event_t event = Event_INVALID;

    /* check to see if an event is pending. If it is, get it */
    eventValid = EventQueue_GetNext(&event);
    if (eventValid)
    {
        /* state specific logic */
        switch(_currentState)
        {
            case State_RUN:
                continueProcessingEvent = _ProcessEvents_RUN(event);
                break;

            case State_SELF_TEST:
                continueProcessingEvent = _ProcessEvents_SELF_TEST(event);
                break;

            default:
                continueProcessingEvent = _ProcessEvents_DEFAULT(event);
                break;
        }

        if (continueProcessingEvent)
        {
            /* do something here */
        }
    }
}

/***************************************************************************//**
    Process events while in RUN state

    \param[in] event - Pending event

    \return True if the event should be further processed, false otherwise
*//****************************************************************************/
static bool _ProcessEvents_RUN(Event_t event)
{
    RfStatus_t rfStatus;
	bool continueProcessing = false;

	switch(event)
	{
	    /* this event must be immediately before default because it falls through */
		case Event_BUTTON_1_RELEASED:
		    CommProtocol_GetRfStatus(&rfStatus);
		    if ((_buttonOnePressed && PushButtonControl_isButtonPressed(PushButton_ZERO)) &&
		        ((rfStatus.connectionStatus == RfConnectionStatus_NOT_PAIRED) ||
		         (rfStatus.connectionStatus == RfConnectionStatus_PAIRED_NOT_CONNECTED)))
		    {
		        /* Before we enter the self-test, unlatch the system power so that the
		           self-test will automatically end if the power button is released   */
		        _UnlatchPower();
		        _currentState = State_SELF_TEST;
		        EventQueue_Add(Event_START_SELF_TEST, EventPriority_LOWEST);
		    }
		    /* do not break. continue with any other processing of the event */

		default:
		    continueProcessing = _ProcessEvents_DEFAULT(event);
			break;
	}

	return continueProcessing;
}

/***************************************************************************//**
    Process events while in SELF_TEST state

    \param[in] event - Pending event

    \return True if the event should be further processed, false otherwise
*//****************************************************************************/
static bool _ProcessEvents_SELF_TEST(Event_t event)
{
    bool continueProcessing = false;

    switch(event)
    {
        case Event_START_SELF_TEST:
            DEBUG("start self test");
            SelfTest_Start();
            break;

        case Event_SELF_TEST:
            SelfTest_Process();
            break;

        case Event_SELF_TEST_COMPLETE:
            TimerTick_RequestTimeout(TickTimeoutType_SINGLE, 30000ul, &_UnlatchPower);
            break;

        default:
            continueProcessing = _ProcessEvents_DEFAULT(event);
            break;
    }

    return continueProcessing;
}

/***************************************************************************//**
    Process events default handler.  Processes events the same, regardless of
    state.

    \param[in] event - Pending event

    \return True if the event should be further processed, false otherwise
*//****************************************************************************/
static bool _ProcessEvents_DEFAULT(Event_t event)
{
    bool continueProcessing = false;

    switch(event)
    {
        case Event_SERIAL_RECEIVED_PACKET:
            TestProtocol_HandleReceivedPacket();
            break;

        case Event_SPI_SLAVE_TX_COMPLETE:
            CommProtocol_TxHandler();
            break;

        case Event_SPI_SLAVE_RX_COMPLETE:
            CommProtocol_RxHandler();
            break;

        case Event_ACCELEROMETER_DATA_READY:
            AccControl_ReadAcc();
            break;

        case Event_MAGNETOMETER_DATA_READY:
            MagControl_ReadMag();
            break;

        case Event_GYROSCOPE_DATA_READY:
            GyroControl_ReadGyro();
            break;

        case Event_SYSTEM_TICK:
            TimerTick_ProcessTimeouts();
            break;

        case Event_ADC_CONVERSION_SEQUENCE_COMPLETE:
            AdcManager_ProcessConversion();
            break;

        case Event_BUTTON_0_PRESSED:
            if (_powerOffTimeoutInUse == false)
            {
                _powerOffTimeoutInUse = true;
                _powerOffTimeoutHandle = TimerTick_RequestTimeout(TickTimeoutType_SINGLE, POWER_OFF_MSEC, _HandlePowerOffRequest);
            }
            break;

        case Event_BUTTON_0_RELEASED:
            if (_powerOffTimeoutInUse)
            {
                _powerOffTimeoutInUse = false;
                TimerTick_CancelTimeout(_powerOffTimeoutHandle);
            }
            break;
        case Event_BUTTON_1_PRESSED:
            DEBUG("button 1 pressed");
            _buttonOnePressed = true;
            break;

        case Event_BUTTON_1_RELEASED:
            DEBUG("button 1 released");
            _buttonOnePressed = false;
            break;

        case Event_SHUTDOWN:
            DEBUG("power off");
            /* Leave time for the system to print debug messages before power is removed */
            TimerTick_RequestTimeout(TickTimeoutType_SINGLE, 30u, System_PowerOff);
            break;

        default:
            /* do nothing */
            break;
    }

    return continueProcessing;
}

/***************************************************************************//**
    Should be called to turn the system off.  Turns power off by unlatching the
    power supply.  The system might still remain on after this is called if the
    user continues holding the power button.
*//****************************************************************************/
__inline static void _UnlatchPower(void)
{
    DEBUG("unlatch power");
    EventQueue_Add(Event_SHUTDOWN, EventPriority_LOWEST);
}

/***************************************************************************//**
    Handles the request for power off based on the power button.
*//****************************************************************************/
static void _HandlePowerOffRequest(void)
{
    LedControl_SetLed(Led_0_RED, LedState_ON);
    _UnlatchPower();
}
