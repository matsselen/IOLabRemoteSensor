/***************************************************************************//**
    \file DigitalInputControl.c
    \brief Controls reading digital inputs on the header and buttons.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the digital input on the header and buttons and
    				manages the configurations and settings associated with them.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/DigitalInputControl.c$
    $Revision: 4$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "DigitalInputControl.h"
#include <DataBuffer.h>
#include <KeyValue.h>
#include <Board.h>
#include <Gpio.h>
#include <Device.h>
#include <Assert.h>
#include <Serial.h>
#include <SensorId.h>
#include <TimerTick.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** defines how many milliseconds to wait between samples for a 100 Hz rate */
#define TIMEOUT_100HZ         ( 10u )

/** defines how many milliseconds to wait between samples for a 50 Hz rate */
#define TIMEOUT_50HZ          ( 20u )

/** defines how many milliseconds to wait between samples for a 25 Hz rate */
#define TIMEOUT_25HZ          ( 40u )

/** defines how many milliseconds to wait between samples for a 20 Hz rate */
#define TIMEOUT_20HZ          ( 50u )

/** defines how many milliseconds to wait between samples for a 10 Hz rate */
#define TIMEOUT_10HZ          ( 100u )

/** defines how many milliseconds to wait between samples for a 1 Hz rate */
#define TIMEOUT_1HZ          ( 1000u )

/** mask for digital inputs */

/** \name Mask for digital inputs
    \{
*/

/** Button 1 */
#define BUTTON1_MASK   ( 0x80 )

/** Button 0 */
#define BUTTON0_MASK   ( 0x40 )

/** Header Pin 6 */
#define HEADER6_MASK   ( 0x20 )

/** Header Pin 5 */
#define HEADER5_MASK   ( 0x10 )

/** Header Pin 4 */
#define HEADER4_MASK   ( 0x08 )

/** Header Pin 3 */
#define HEADER3_MASK   ( 0x04 )

/** Header Pin 2 */
#define HEADER2_MASK   ( 0x02 )

/** Header Pin 1 */
#define HEADER1_MASK   ( 0x01 )

/** \} */

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module has been initialized */
static bool _initialized = false;

/** Handle to the timer tick timeout for the digital inputs */
static TickTimeoutHandle_t _tickHandle;

/** tracks if the tick handle is in use to make sure not to use an invalid tick handle */
static bool _tickInUse = false;

/** Stores the desired sample rate of the header pin */
static DigitalInputSampleRate_t _sampleRate = DEFAULT_DIGITALINPUT_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _ReadPins(void);

/***************************************************************************//**
    Initializes the the digital input control.
*//****************************************************************************/
void DigitalInputControl_Init(void)
{
	_initialized = true;
}

/***************************************************************************//**
    Takes a configuration and sets it for the digital inputs on the header and
    pushbuttons.

    \param[in] config - the KeyValue configuration setting for the digital inputs.

*//****************************************************************************/
void DigitalInputControl_SetConfig(uint8_t config)
{
	uint8_t key;
	uint8_t value;

    ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(config, &key, &value);

	/* handle the key-value setting */
	switch(key)
	{
		case DigitalInputKey_MODE:
			switch(value)
			{
				case DigitalInputMode_DISABLE:
					TimerTick_CancelTimeout(_tickHandle);
					_tickInUse = false;
					break;

				case DigitalInputMode_ENABLE:
					switch(_sampleRate)
					{
						case DigitalInputSampleRate_1:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_1HZ, &_ReadPins);
							_tickInUse = true;
							break;

						case DigitalInputSampleRate_10:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_10HZ, &_ReadPins);
							_tickInUse = true;
							break;

						case DigitalInputSampleRate_20:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_20HZ, &_ReadPins);
							_tickInUse = true;
							break;

						case DigitalInputSampleRate_25:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_25HZ, &_ReadPins);
							_tickInUse = true;
							break;

						case DigitalInputSampleRate_50:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_50HZ, &_ReadPins);
							_tickInUse = true;
							break;

						case DigitalInputSampleRate_100:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_100HZ, &_ReadPins);
							_tickInUse = true;
							break;

						default:
							ASSERT(false, ErrorLedMode_SOFTWARE);
					}

					break;

				default:
					ASSERT(false, ErrorLedMode_SOFTWARE);
					break;
			}
			break;

		case DigitalInputKey_SAMPLE_RATE:
			if (((int16_t)(value) >= DigitalInputSampleRate_FIRST) && (value < DigitalInputSampleRate_LAST))
			{
				_sampleRate = ((DigitalInputSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid digital input key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}


/***************************************************************************//**
    Takes the digital input data from the header and buttons and stores it in
    the proper location. This function is designed to be called every time the
    timer times out.

    \param[in] data - The sample from the header and the pushbuttons that should
    				  be stored in the proper location.
*//****************************************************************************/
static void _ReadPins(void)
{
	uint8_t data = 0;

	if(GPIO_IS_HIGH(BUTTON_1_PORT, BUTTON_1_PIN))
	{
		data |= BUTTON1_MASK;
	}
	if(GPIO_IS_HIGH(BUTTON_0_PORT, BUTTON_0_PIN))
	{
		data |= BUTTON0_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN))
	{
		data |= HEADER6_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN))
	{
		data |= HEADER5_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN))
	{
		data |= HEADER4_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN))
	{
		data |= HEADER3_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN))
	{
		data |= HEADER2_MASK;
	}
	if(GPIO_IS_HIGH(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN))
	{
		data |= HEADER1_MASK;
	}


#if 0
	#define DIGITALINPUT_USE_SERIAL
#endif

#ifndef DIGITALINPUT_USE_SERIAL
	DataBuffer_AddToSubarray8Bit(SensorId_DIGITALINPUTS, 1u, &data);
#else
	Serial_SendStringNumber("digital inputs:", data);
#endif
}
