/***************************************************************************//**
    \file EncoderControl.c
    \brief Controls the Quadrature Encoder driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the quadrature encoder driver and manages the configuration
                   settings, and timing of the encoder.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/EncoderControl.c$
    $Revision: 2$
    $Date: Monday, July 30, 2012 3:06:41 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "EncoderControl.h"
#include <Encoder.h>
#include <TimerTick.h>
#include <DataBuffer.h>
#include <SensorId.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines how many items are in an encoder sample */
#define NUM_ENCODER_ITEMS     ( 2u )

/** defines how many milliseconds to wait between samples for a 100 Hz rate */
#define TIMEOUT_100HZ         ( 10u )

/** defines how many milliseconds to wait between samples for a 50 Hz rate */
#define TIMEOUT_50HZ          ( 20u )

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the module is initialized or not */
static bool _initialized = false;

/** Handle to the timer tick timeout for the encoder */
static TickTimeoutHandle_t _tickHandle;
/** tracks if the tick handle is in use to make sure not to use an invalid tick handle */
static bool _tickInUse = false;

/** Stores the encoder sample rate setting */
static EncoderSampleRate_t _sampleRate = DEFAULT_ENCODER_SAMPLE_RATE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/

/***************************************************************************//**
    Initializes the encoder controller.
*//****************************************************************************/
void EncControl_Init(void)
{
	Encoder_Init();
	_initialized = true;
}

/***************************************************************************//**
    Sets a configuration value to the encoder

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void EncControl_SetConfig(uint8_t control)
{
	uint8_t key;
	uint8_t value;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(control, &key, &value);

	// TODO: spg - add space in the buffer for the encoder data

	/* handle the key-value setting */
	switch(key)
	{
		case EncoderKey_MODE:
			switch(value)
			{
				case EncoderMode_DISABLE:
					TimerTick_CancelTimeout(_tickHandle);
					_tickInUse = false;
					Encoder_Stop();
					break;

				case EncoderMode_ENABLE:
					Encoder_Start();
					switch(_sampleRate)
					{
						case EncoderSampleRate_50:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_50HZ, &EncControl_ReadEnc);
							_tickInUse = true;
							break;

						case EncoderSampleRate_100:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_100HZ, &EncControl_ReadEnc);
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

		case EncoderKey_SAMPLE_RATE:
			if (((int16_t)(value) >= EncoderSampleRate_FIRST) && (value < EncoderSampleRate_LAST))
			{
				_sampleRate = ((EncoderSampleRate_t)(value));
			}
			else
			{
				ASSERT(false, ErrorLedMode_SOFTWARE);
			}
			break;

		default:
			/* invalid encoder key */
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}
}

/***************************************************************************//**
    Gets a reading from the accelerometer.
*//****************************************************************************/
void EncControl_ReadEnc(void)
{
#if 0
	#define ENCODER_USE_SERIAL
#endif
#ifdef ENCODER_USE_SERIAL
	int index;
#endif
	EncoderStatus_t encoderStatus;
	static int16_t data[NUM_ENCODER_ITEMS] = { 0u };
	uint16_t actualNumberValues;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	encoderStatus = Encoder_Read(data, &actualNumberValues, NUM_ENCODER_ITEMS);

	if (encoderStatus == EncoderStatus_OKAY)
	{
#ifndef ENCODER_USE_SERIAL /* send to data buffer */
		(void) DataBuffer_AddToSubarray16Bit(SensorId_ENCODER, actualNumberValues, ((uint16_t*)(data)));
#else /* send to serial debug */
		for( index = 0; index < actualNumberValues; ++index)
		{
			Serial_SendStringNumber("enc:", data[index]);
		}
#endif
	}
	else
	{
		switch (encoderStatus)
		{
			case EncoderStatus_BUFFER_OVERFLOW:
				Serial_SendString("enc buffer overflow");
				break;
			case EncoderStatus_CAPTURE_OVERFLOW:
				Serial_SendString("enc capture overflow");
				break;
			case EncoderStatus_SW_CAPTURE_OVERFLOW:
				Serial_SendString("enc SW capture overflow");
				break;
			case EncoderStatus_INVALID:
				Serial_SendString("enc invalid");
				break;
			case EncoderStatus_OTHER_ERROR:
				Serial_SendString("enc other");
				break;
			default:
				Serial_SendString("enc unknown");
				break;
		}
	}
}
