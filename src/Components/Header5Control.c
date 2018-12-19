/***************************************************************************//**
    \file BuzzerControl.c
    \brief Controls the buzzer

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the buzzer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header5Control.c$
    $Revision: 4$
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
#include <Header5Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the possible frequency ranges for Header5 */
typedef enum FrequencyRange
{
	FrequencyRange_LOW,
	FrequencyRange_MID,
	FrequencyRange_HIGH
} FrequencyRange_t;

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode the buzzer is in */
static Header5Mode_t _header5Mode = Header5Mode_INVALID;

/** Tracks what low frequency the buzzer was last set to */
static Header5LowFrequency_t _header5LowFrequency = DEFAULT_HEADER5_LOWFREQUENCY;

/** Tracks what mid frequency the buzzer was last set to */
static Header5MidFrequency_t _header5MidFrequency = DEFAULT_HEADER5_MIDFREQUENCY;

/** Tracks what high frequency the buzzer was last set to */
static Header5HighFrequency_t _header5HighFrequency = DEFAULT_HEADER5_HIGHFREQUENCY;

/** Tracks what duty cycle header pin 5 is set to */
static Header5DutyCycle_t _header5Duty = DEFAULT_HEADER5_DUTYCYCLE;

/** Tracks if the header pin is enabled */
static bool _header5Enabled = false;

static FrequencyRange_t range = FrequencyRange_MID;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header5Control_SetFrequency(void);
static void _Header5Control_SetDutyCycle(void);

/***************************************************************************//**
    Initialized the Buzzer and PWM driver.
*//****************************************************************************/
void Header5Control_Init()
{
	/* NET TODO: do I want to initialize the Pwm here? */
	Pwm_Init();
}

/***************************************************************************//**
    Sets a configuration value to header pin 5

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header5Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header5Key_MODE:
			if ((((Header5Mode_t)value) >= Header5Mode_FIRST) && (((Header5Mode_t)value) < Header5Mode_LAST))
			{
				_header5Mode = (Header5Mode_t)(value);
				switch (_header5Mode)
				{
					case Header5Mode_DISABLE:
						Pwm_Disable(PwmChannel_6);
						_header5Enabled = false;
						break;

					case Header6Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN,
								   	   	   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header5Mode_OUTPUT:
						Gpio_Configure(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN,
										   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header5Control_SetFrequency();
						break;

					default:
						DEBUG("header header5 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header5 mode");
			}
			break;

		case Header5Key_LOWFREQUENCY:
			if ((((Header5LowFrequency_t)value) >= Header5LowFrequency_FIRST) && (((Header5LowFrequency_t)value) < Header5LowFrequency_LAST))
			{
				_header5LowFrequency = (Header5LowFrequency_t)(value);
				range = FrequencyRange_LOW;
				_Header5Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header5 low frequency");
			}
			break;

		case Header5Key_MIDFREQUENCY:
			if ((((Header5MidFrequency_t)value) >= Header5MidFrequency_FIRST) && (((Header5MidFrequency_t)value) < Header5MidFrequency_LAST))
			{
				_header5MidFrequency = (Header5MidFrequency_t)(value);
				range = FrequencyRange_MID;
				_Header5Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header5 mid frequency");
			}
			break;

		case Header5Key_HIGHFREQUENCY:
			if ((((Header5HighFrequency_t)value) >= Header5HighFrequency_FIRST) && (((Header5HighFrequency_t)value) < Header5HighFrequency_LAST))
			{
				_header5HighFrequency = (Header5HighFrequency_t)(value);
				range = FrequencyRange_HIGH;
				_Header5Control_SetFrequency();
			}
			else
			{
				DEBUG("invalid header5 high frequency");
			}
			break;

		case Header5Key_DUTYCYCLE:
			if ((((Header5DutyCycle_t)value) >= Header5DutyCycle_FIRST) && (((Header5DutyCycle_t)value) < Header5DutyCycle_LAST))
			{
				_header5Duty = (Header5DutyCycle_t)(value);
				_Header5Control_SetDutyCycle();
			}
			else
			{
				DEBUG("invalid header5 duty cycle");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header5Control_SetFrequency(void)
{
	uint32_t freq = 0;
	uint8_t duty = 0;

	switch(range)
	{
		case FrequencyRange_LOW:
			freq = _lowFrequencies[_header5LowFrequency];
			break;

		case FrequencyRange_MID:
			freq = _midFrequencies[_header5MidFrequency];
			break;

		case FrequencyRange_HIGH:
			freq = _highFrequencies[_header5HighFrequency];
			break;

		default:
			break;
	}

	Pwm_SetFrequency(freq);

	if((_header5Enabled == false) && (_header5Mode == Header5Mode_OUTPUT))
	{
		duty = _dutyCycles[_header5Duty];
		Pwm_Enable(PwmChannel_6, duty);
		_header5Enabled = true;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header5Control_SetDutyCycle(void)
{
	uint8_t duty = 0;

	duty = _dutyCycles[_header5Duty];
	Pwm_SetDuty(PwmChannel_6, duty);

}
