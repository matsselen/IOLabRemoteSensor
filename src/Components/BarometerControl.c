/***************************************************************************//**
    \file BarometerControl.c
    \brief Controls the barometer driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the barometer driver and controls the configuration
                   for it.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BarometerControl.c$
    $Revision: 4$
    $Date: Friday, July 17, 2015 2:21:45 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "BarometerControl.h"
#include <Barometer.h>
#include <TimerTick.h>
#include <DataBuffer.h>
#include <SensorId.h>
#include <KeyValue.h>
#include <Debug.h>
#include <Assert.h>
#include <Serial.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** defines how many milliseconds to wait between samples for a 100 Hz rate */
#define TIMEOUT_100HZ         ( 10u )

/** defines how many milliseconds to wait between samples for a 50 Hz rate */
#define TIMEOUT_50HZ          ( 20u )

/** defines how many milliseconds to wait between samples for a 10 Hz rate */
#define TIMEOUT_10HZ          ( 100u )

/** defines how many milliseconds to wait between samples for a 1 Hz rate */
#define TIMEOUT_1HZ          ( 1000u )

/** Define the maximum number of data bytes the barometer will return. */
#define BAROMETER_MAX_DATA_BYTES  (4u)

/** Define a typedef for a struct to hold a barometer data sample */
typedef struct BarometerDataSample
{
    uint8_t data[BAROMETER_MAX_DATA_BYTES];
} BarometerSampleArray_t;

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** tracks if the controller has been initialized */
static bool _initialized = false;

/** Handle to the timer tick timeout for the barometer */
static TickTimeoutHandle_t _tickHandle;
/** tracks if the tick handle is in use to make sure not to use an invalid tick handle */
static bool _tickInUse = false;

/** Stores the barometer sample rate setting */
static BarometerSampleRate_t _sampleRate = DEFAULT_BAROMETER_SAMPLE_RATE;

/** Keeps a copy of the last valid barometer reading.  Occassionally the
    barometer will return bad data (all zeros or FFs) that is likely caused by a
    glitch on the barometer because we are reading it too fast or the SPI clock
    rate is too high.  Regardless of the reason, buffer the last value and
    report the last good value when reading samples if the new sample is bad. */
static BarometerSampleArray_t _lastGoodSample = { .data = 0u };

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

static bool _BarometerArrayGlitched(BarometerSampleArray_t *sample);

/***************************************************************************//**
    Initializes the barometer controller.
*//****************************************************************************/
void BarometerControl_Init(void)
{
	Barometer_Init();
	_initialized = true;
}

/***************************************************************************//**
    Sets a configuration value to the Barometer

    \param[in] keyValue - the key/value pair with the configuration information

*//****************************************************************************/
void BarometerControl_SetConfig(uint8_t keyValue)
{
	uint8_t key;
	uint8_t value;

	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	KeyValue_Separate(keyValue, &key, &value);

	// TODO: net - add space in the buffer for the encoder data

	/* handle the key-value setting */
	switch(key)
	{
		case BarometerKey_MODE:
			switch(value)
			{
				case BarometerMode_DISABLE:
					TimerTick_CancelTimeout(_tickHandle);
					_tickInUse = false;
					break;

				case BarometerMode_ENABLE:
					Barometer_StartSampleConversion();
					switch(_sampleRate)
					{
						case BarometerSampleRate_1:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_1HZ, &BarometerControl_ReadBar);
							_tickInUse = true;
							break;
						case BarometerSampleRate_10:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_10HZ, &BarometerControl_ReadBar);
							_tickInUse = true;
							break;
						case BarometerSampleRate_50:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_50HZ, &BarometerControl_ReadBar);
							_tickInUse = true;
							break;

						case BarometerSampleRate_100:
							if (_tickInUse)
							{
								TimerTick_CancelTimeout(_tickHandle);
							}
							_tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, TIMEOUT_100HZ, &BarometerControl_ReadBar);
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

		case BarometerKey_SAMPLE_RATE:
			if (((int16_t)(value) >= BarometerSampleRate_FIRST) && (value < BarometerSampleRate_LAST))
			{
				_sampleRate = ((BarometerSampleRate_t)(value));
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
    Gets a reading from the Barometer.
*//****************************************************************************/
void BarometerControl_ReadBar(void)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

#if 1 /* use data buffer for data */
	static BarometerSampleArray_t barometerSample;
	bool glitched;
	uint16_t sampleSize = 0u;

	Barometer_ReadSampleArray(BAROMETER_MAX_DATA_BYTES, &sampleSize, barometerSample.data);

	Serial_SendString3Numbers("Bar:", (uint16_t)(((uint16_t)barometerSample.data[0])<<8u)+((uint16_t)barometerSample.data[1]),
	                                  (uint16_t)(((uint16_t)barometerSample.data[2])<<8u)+((uint16_t)barometerSample.data[3]),
	                                  0u);

	glitched = _BarometerArrayGlitched(&barometerSample);

	if (sampleSize > 0 && (glitched == false))
	{
		DataBuffer_AddToSubarray8Bit(SensorId_BAROMETER, sampleSize, barometerSample.data);
		memcpy((void*)&_lastGoodSample, (void*)&barometerSample, sizeof(BarometerSampleArray_t));
	}
	else
	{
	    DEBUG("barometer glitched");
        DataBuffer_AddToSubarray8Bit(SensorId_BAROMETER, sampleSize, _lastGoodSample.data);
	}

#else /* use serial print for data */
	bool sampleSuccess = false;
	static BarometerSample_t sample;
	sampleSuccess = Barometer_ReadSample(&sample);
	if (sampleSuccess)
	{
		Serial_SendString3Numbers("bar:", sample.pressure, sample.temperature, 0);
	}
#endif

	/* Start new sample conversion if sampling is still active */
	if(_tickInUse)
	{
		Barometer_StartSampleConversion();
	}

}

/***************************************************************************//**
    Reads the calibration values from the barometer.

    \param[in] maxCalibrationLength - the maximum number of bytes allocated for calibration[]
    \param[out] actualCalibrationLength - the number of bytes of calibration data placed in calibration[]
    \param[out] calibration[] - the calibration data from the barometer.

*//****************************************************************************/
void BarometerControl_ReadCalibration(
		uint16_t  maxCalibrationLength,
		uint16_t *actualCalibrationLength,
		uint8_t   calibration[])
{
	/** Defines how many bytes are needed to store the calibration data */
#define NUM_BAROMETER_CALIBRATION_BYTES   ( 8u )

	ASSERT((calibration != NULL) && (actualCalibrationLength != NULL) &&
	        (maxCalibrationLength >= NUM_BAROMETER_CALIBRATION_BYTES), ErrorLedMode_SOFTWARE);

	Barometer_ReadCoeffArray(NUM_BAROMETER_CALIBRATION_BYTES, actualCalibrationLength, calibration);
}

/***************************************************************************//**
    Checks if the barometer data array is from a glitched reading.

    \param[in] sample - sample array from the barometer.
    \returns True if the sample was a glitched sample.

    The sample is defined as glitched of all values are 0x00 or 0xFF.
*//****************************************************************************/
static bool _BarometerArrayGlitched(BarometerSampleArray_t *sample)
{
    bool glitched = true;
    uint16_t index;

    for (index = 0; index < ARRAY_LENGTH(sample->data); ++index)
    {
        if ((sample->data[index] > 0x00) &&
            (sample->data[index] < 0xFF))
        {
            glitched = false;
            break;
        }
    }

    return glitched;
}
