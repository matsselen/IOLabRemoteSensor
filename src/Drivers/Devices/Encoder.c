/***************************************************************************//**
    \file Encoder.c
    \brief The hardware driver for the quadrature encoder

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The quadrature encoder uses a a direction and count input to
                   count how many pulses and in which direction a specially
                   designed encoder moved.

                   The encoder module uses timer A2 of for hardware counting
                   and direction checking.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Encoder.c$
    $Revision: 5$
    $Date: Wednesday, September 19, 2012 3:42:25 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Encoder.h"
#include <Device.h>
#include <Board.h>
#include <Gpio.h>
#include <Timer.h>
#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the direction options for the timer */
typedef enum EncoderDirection
{
	EncoderDirection_INVALID,
	EncoderDirection_FORWARD,
	EncoderDirection_BACKWARD
} EncoderDirection_t;

/** Defines how many previous direction changes to store */
#define NUM_DIRECTION_CHANGE    ( 2u )

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
static bool _initialized = false;

/** Tracks the count on the timer the last time it was captured. */
static uint16_t _previousCount;
/** Tracks the direction of the timer */
static EncoderDirection_t _previousDirection;

/** Current Encoder Status */
static EncoderStatus_t _currentStatus = EncoderStatus_OKAY;

/** Stores the encoder counts */
static int16_t _encoderCounts[NUM_DIRECTION_CHANGE] = { 0u };
/** Stores the index of hte encoder counts */
static uint16_t _encoderCountIndex = 0u;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Encoder_AddSample(uint16_t counts, EncoderDirection_t direction);
static void _Encoder_ClearBuffer(void);
__interrupt void _TimerA2CC0Isr(void);
__interrupt void _TimerA2CCxIsr(void);

/***************************************************************************//**
    Initializes the encoder module.
*//****************************************************************************/
void Encoder_Init(void)
{
	/* set the timer to use the external pin to clock it, have no divider, and count
	   continuously.
	*/
	Timer_A2_Configure(TimerSource_EXTERNAL, TimerDivider_1, TimerRunMode_CONTINUOUS, 0u);
	/* Set the timer to capture on both edges (rising and falling), use the CCxA input source,
	   DO NOT synchronize the capture with the timer, use capture mode (not compare). Synchronizing
	   will make the capture wait until the next edge of the timer which might never occur depending
	   on the movement of the encoder, so non-synchronous is desired.

	   CC0 is for the direction change and CC1 is for the software capture. Software capture uses
	   VCC and GND as capture inputs and switches between them to generate a capture event.
	*/
	Timer_A2CCX_Configure(TimerA2CC_0, (CM_3 | CCIS__CCIxA | CAP), 0);
	Timer_A2CCX_Configure(TimerA2CC_1, (CM_3 | CCIS1       | CAP), 0);
	_initialized = true;
}

/***************************************************************************//**
    Starts the counting of the encoder.
*//****************************************************************************/
void Encoder_Start(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	Gpio_SetPortU(POWER_ENCODER_ENABLE_PIN);

	_previousCount = 0u;
	_previousDirection = EncoderDirection_INVALID;
	_Encoder_ClearBuffer();

	TIMER_A2_START(TimerRunMode_CONTINUOUS);
	TIMER_A2_CC0_INT_CLEAR();
	TIMER_A2_CC0_INT_ENABLE();
	TIMER_A2_CC1_INT_CLEAR();
	TIMER_A2_CC1_INT_ENABLE();
}

/***************************************************************************//**
    Stops the counting of the encoder.
*//****************************************************************************/
void Encoder_Stop(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	TIMER_A2_STOP();
	TIMER_A2_CC0_INT_DISABLE();
	TIMER_A2_CC0_INT_CLEAR();
	TIMER_A2_CC1_INT_DISABLE();
	TIMER_A2_CC1_INT_CLEAR();
	Gpio_ClearPortU(POWER_ENCODER_ENABLE_PIN);
}

/***************************************************************************//**
    Read the value of the encoder counts.

    \param[out] counts - the counts and direction of the encoder.
    \param[out] elements - the actual number of samples that were put in counts
    \param[in] maxElements - the maximum number of samples that fit into counts

    \return The status of the encoder over the last sample.
*//****************************************************************************/
EncoderStatus_t Encoder_Read(
		int16_t counts[],
		uint16_t *elements,
		uint16_t maxElements)
{
	EncoderStatus_t status;
	int16_t index;
	bool encoderInteruptEnabled = TIMER_A2_CC0_INT_IS_ENABLED();

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
	ASSERT(counts != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(elements != NULL, ErrorLedMode_SOFTWARE);

	/* trigger a capture to get the most recent count */
	TIMER_A2_CC1_TRIGGER;

	/* disable the encoder interrupts so the data can't be changed
	   while it's being copied.
	*/
	if (encoderInteruptEnabled)
	{
		TIMER_A2_CC0_INT_DISABLE();
	}

	if (maxElements >= NUM_DIRECTION_CHANGE)
	{
		/* copy the encoder data elements then reset the
		   storage buffer.
		*/
		for (index = NUM_DIRECTION_CHANGE - 1; index >= 0; --index)
		{
			counts[index] = _encoderCounts[index];
		}

		if (_encoderCountIndex == 0)
		{
			*elements = 1u;
		}
		else
		{
			*elements = _encoderCountIndex;
		}

		status = _currentStatus;

		_Encoder_ClearBuffer();
	}
	else
	{
		status = EncoderStatus_OTHER_ERROR;
	}

	if (encoderInteruptEnabled)
	{
		TIMER_A2_CC0_INT_ENABLE();
	}

	_currentStatus = EncoderStatus_OKAY;
	return status;
}

/***************************************************************************//**
    Adds an encoder sample to the buffer.

    \param[in] counts - the value of the encoder HW counter
    \param[in] direction - the current direction of the encoder

*//****************************************************************************/
static void _Encoder_AddSample(uint16_t counts, EncoderDirection_t direction)
{
	int16_t *sampleLocation = NULL;

	/* if the direction is the same, we can just add/subtract to the value
	   in that buffer location. If the direction is different, we have to
	   move to the next location in the buffer if there is room.
	*/
	if ((direction == _previousDirection) ||
		((_encoderCountIndex == 0u) && (_previousDirection == EncoderDirection_INVALID)))
	{
		sampleLocation = &_encoderCounts[_encoderCountIndex];
	}
	else
	{
		++_encoderCountIndex;
		if (_encoderCountIndex < NUM_DIRECTION_CHANGE)
		{
			sampleLocation = &_encoderCounts[_encoderCountIndex];
		}
		else
		{
			_currentStatus = EncoderStatus_BUFFER_OVERFLOW;
		}
	}

	/* make sure there was room in the buffer before writing to it */
	if (sampleLocation != NULL)
	{
		/* add the new counts to the buffer with the correct sign */
		if (direction == EncoderDirection_FORWARD)
		{
			*sampleLocation += (int16_t)(counts - _previousCount);
		}
		else if (direction == EncoderDirection_BACKWARD)
		{
			*sampleLocation -= (int16_t)(counts - _previousCount);
		}
	}

	/* update the current values to previous values */
	_previousCount = counts;
	_previousDirection = direction;
}

/***************************************************************************//**
    Clears the storage buffer for the encoder.
*//****************************************************************************/
static void _Encoder_ClearBuffer(void)
{
	volatile int16_t index;

	_encoderCountIndex = 0u;
	for (index = NUM_DIRECTION_CHANGE - 1; index >= 0; --index)
	{
		_encoderCounts[index] = 0;
	}
}

/***************************************************************************//**
    Interrupt service routine for the timer A2 compare/capture 0 interrupt.
    The interrupt flag is cleared in hardware when the ISR is called.

    Read the value of the counter when the direction change occurred and start
    counting in the other direction.

*//****************************************************************************/
#pragma vector=TIMER2_A0_VECTOR
__interrupt void _TimerA2CC0Isr(void)
{
	static volatile uint16_t currentCount;
	static EncoderDirection_t currentDirection;
		currentCount = TIMER_A2_CC0_GET;
	if (TIMER_A2_CC0_OVERFLOWED == false)
	{
		/* the direction is opposite the current state because it was the state
		   change that triggered this interrupt.
		*/
		if (TIMER_A2_CC0_PIN_IS_HIGH)
		{
			currentDirection = EncoderDirection_FORWARD;
		}
		else
		{
			currentDirection = EncoderDirection_BACKWARD;
		}

		_Encoder_AddSample(currentCount, currentDirection);
	}
	else
	{
		TIMER_A2_CC0_CLEAR_OVERFLOW;
		_currentStatus = EncoderStatus_CAPTURE_OVERFLOW;
	}

    _low_power_mode_off_on_exit();
}

/***************************************************************************//**
    Interrupt service routine for the timer A2 compare/capture 1-4 interrupt.
    The interrupt flag is cleared in hardware when the ISR is called.

    Read the value of the counter and store the value.

*//****************************************************************************/
#pragma vector=TIMER2_A1_VECTOR
__interrupt void _TimerA2CCxIsr(void)
{
	static volatile uint16_t currentCount;
	static EncoderDirection_t currentDirection;

	 /* Note: intrinsic allows compiler to optimize switch */
	    switch (_even_in_range(TA2IV, TA2IV_TA2IFG))
	    {
	        case TA2IV_NONE:
	            /* Do nothing.  No interrupt to handle.
	               This case may occur if an interrupt is disabled at the same time
	               as the interrupt flag is set.  The CPU is already committed to
	               running the ISR, but the interrupt has been disabled, which
	               keeps the interrupt vector register from indicating the flag
	               has been set.
	            */
	            break;

	        /* CC1 interrupt flag is set */
	        case TA2IV_TA2CCR1:
	        	if (TIMER_A2_CC1_OVERFLOWED == false)
	        	{
	        		currentCount = TIMER_A2_CC1_GET;
	        		/* the CC0 pin has the direction signal */
	        		if (TIMER_A2_CC0_PIN_IS_HIGH)
	        		{
	        			currentDirection = EncoderDirection_BACKWARD;
	        		}
	        		else
	        		{
	        			currentDirection = EncoderDirection_FORWARD;
	        		}

	        		_Encoder_AddSample(currentCount, currentDirection);
	        	}
	        	else
	        	{
	        		TIMER_A2_CC1_CLEAR_OVERFLOW;
	        		_currentStatus = EncoderStatus_SW_CAPTURE_OVERFLOW;
	        	}
	            break;

	        /* CC2 interrupt flag is set */
	        case TA2IV_TA2CCR2:
	        	/* do nothing */
	            break;

	        /* TA interrupt flag is set */
	        case TA2IV_TA2IFG:
	        	/* do nothing */
	            break;

	        default:
	            _never_executed();
	            break;
	    }

	    _low_power_mode_off_on_exit();
}
