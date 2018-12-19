/***************************************************************************//**
    \file BuzzerControl.c
    \brief Controls the buzzer

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the buzzer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header4Control.c$
    $Revision: 6$
    $Date: Friday, October 24, 2014 10:53:11 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <PwmFrequencies.h>
#include <PwmDutyCycles.h>
#include <Pwm.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>
#include <Gpio.h>
#include <Board.h>
#include <Device.h>
#include <Header4Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the possible frequency ranges for Header4 */
typedef enum FrequencyRange
{
	FrequencyRange_LOW,
	FrequencyRange_MID,
	FrequencyRange_HIGH
} FrequencyRange_t;

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode the buzzer is in */
static Header4Mode_t _header4Mode = Header4Mode_INVALID;

/** Tracks what low frequency the buzzer was last set to */
static Header4LowFrequency_t _header4LowFrequency = DEFAULT_HEADER4_LOWFREQUENCY;

/** Tracks what mid frequency the buzzer was last set to */
static Header4MidFrequency_t _header4MidFrequency = DEFAULT_HEADER4_MIDFREQUENCY;

/** Tracks what high frequency the buzzer was last set to */
static Header4HighFrequency_t _header4HighFrequency = DEFAULT_HEADER4_HIGHFREQUENCY;

/** Tracks what duty cycle header pin 4 is set to */
static Header4DutyCycle_t _header4Duty = DEFAULT_HEADER4_DUTYCYCLE;

/** Tracks if the header pin is enabled */
static bool _header4Enabled = false;

static FrequencyRange_t range = FrequencyRange_MID;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header4Control_SetFrequency(void);
static void _Header4Control_SetDutyCycle(void);

/***************************************************************************//**
    Initialized the Buzzer and PWM driver.
*//****************************************************************************/
void Header4Control_Init()
{
}

/***************************************************************************//**
    Sets a configuration value to header pin 4

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header4Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header4Key_MODE:
			if ((((Header4Mode_t)value) >= Header4Mode_FIRST) && (((Header4Mode_t)value) < Header4Mode_LAST))
			{
				_header4Mode = (Header4Mode_t)(value);
				switch (_header4Mode)
				{
					case Header4Mode_DISABLE:
						Pwm_Disable(PwmChannel_5);
						_header4Enabled = false;
						break;

					case Header6Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN,
										   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header4Mode_OUTPUT:
						Gpio_Configure(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN,
						                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header4Control_SetFrequency();
						break;

					default:
						DEBUG("header header4 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header4 mode");
			}
			break;

		case Header4Key_LOWFREQUENCY:
			if ((((Header4LowFrequency_t)value) >= Header4LowFrequency_FIRST) && (((Header4LowFrequency_t)value) < Header4LowFrequency_LAST))
			{
				_header4LowFrequency = (Header4LowFrequency_t)(value);
				range = FrequencyRange_LOW;
				_Header4Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header4 low frequency");
			}
			break;

		case Header4Key_MIDFREQUENCY:
			if ((((Header4MidFrequency_t)value) >= Header4MidFrequency_FIRST) && (((Header4MidFrequency_t)value) < Header4MidFrequency_LAST))
			{
				_header4MidFrequency = (Header4MidFrequency_t)(value);
				range = FrequencyRange_MID;
				_Header4Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header4 mid frequency");
			}
			break;

		case Header4Key_HIGHFREQUENCY:
			if ((((Header4HighFrequency_t)value) >= Header4HighFrequency_FIRST) && (((Header4HighFrequency_t)value) < Header4HighFrequency_LAST))
			{
				_header4HighFrequency = (Header4HighFrequency_t)(value);
				range = FrequencyRange_HIGH;
				_Header4Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header4 high frequency");
			}
			break;

		case Header4Key_DUTYCYCLE:
			if ((((Header4DutyCycle_t)value) >= Header4DutyCycle_FIRST) && (((Header4DutyCycle_t)value) < Header4DutyCycle_LAST))
			{
				_header4Duty = (Header4DutyCycle_t)(value);
				_Header4Control_SetDutyCycle();
			}
			else
			{
				DEBUG("invalid header4 duty cycle");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header4Control_SetFrequency(void)
{
	uint32_t freq = 0;
	uint8_t duty = 0;

	switch(range)
	{
		case FrequencyRange_LOW:
			freq = _lowFrequencies[_header4LowFrequency];
			break;

		case FrequencyRange_MID:
			freq = _midFrequencies[_header4MidFrequency];
			break;

		case FrequencyRange_HIGH:
			freq = _highFrequencies[_header4HighFrequency];
			break;

		default:
			break;
	}

	Pwm_SetFrequency(freq);

	if((_header4Enabled == false) && (_header4Mode == Header4Mode_OUTPUT))
	{
		duty = _dutyCycles[_header4Duty];
		Pwm_Enable(PwmChannel_5, duty);
		_header4Enabled = true;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header4Control_SetDutyCycle(void)
{
	uint8_t duty = 0;

	duty = _dutyCycles[_header4Duty];
	Pwm_SetDuty(PwmChannel_5, duty);

}
