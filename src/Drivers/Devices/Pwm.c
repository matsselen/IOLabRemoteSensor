/***************************************************************************//**
    \file Pwm.c
    \brief Pulse width modulation module driver.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The pulse width modulation module contains functions
       definitions and macros that set up and control the MSP430 timer B
       peripheral used to generate pulse width modulated signals.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Pwm.c$
    $Revision: 7$
    $Date: Friday, July 17, 2015 2:23:01 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Pwm.h"
#include <Assert.h>
#include <Debug.h>
#include <Serial.h>
#include <Device.h>
#include <Timer.h>
#include <Board.h>
#include <Gpio.h>
#include <Clock.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** The PWM clock is the PWM timer source divided by the timer divider. the SMCLK divided by 20.
    The timer source is defined in the init to be SMCLK, which is 24MHz. The
    divider is defined in the init to be 20.
    Change this define if the init is changed.
*/
#define PWM_CLK         (1200000ul)

/** number of PWM channels */
#define NUM_CHANNELS	7

/** maximum duty cycle that can be specified for a PWM channel */
#define MAX_DUTY		100

/** define the default frequency during initialization to be 1kHz, which
 * corresponds to a maxCount of 1200.
 */
#define DEFAULT_MAX_COUNT	1200

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Array of Timer B0 compare/capture (CC) modules */
static volatile TimerB0CC_t const _ccModules[NUM_CHANNELS] =
{
	TimerB0CC_0,
	TimerB0CC_1,
	TimerB0CC_2,
	TimerB0CC_3,
	TimerB0CC_4,
	TimerB0CC_5,
	TimerB0CC_6
};

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Stores the maximum number the timer will count up to before resetting,
 * which defines the period of the PWM signal.
 */
static uint16_t _maxCount = DEFAULT_MAX_COUNT;

/** Array to keep track of the duty cycles of channels */
static uint8_t _dutyCycles[NUM_CHANNELS] = {0};

/** Array to keep track of what PWM channels are in use */
//static volatile bool _openChannels[NUM_CHANNELS] = {false};
static bool _openChannels[NUM_CHANNELS] = {false};

/** tracks if the PWM driver has been initialized */
static bool _initialized = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static uint16_t _DutyToCount(uint8_t duty);
static bool _AnyChannelsEnabled(void);

/***************************************************************************//**
	Initialize the PWM with default settings:

*//****************************************************************************/
void Pwm_Init(void)
{
	if(_initialized == false)
	{
		Timer_B0_Configure(TimerSource_SMCLK, TimerDivider_20, TimerCounterLength_16BITS, TimerRunMode_UP, _maxCount);
		_initialized = true;
	}
}

/***************************************************************************//**
    Set the frequency of the PWM in fixed point tenths of Hertz.

    \param[in] freq - Fixed point frequency at which to set the PWM.  Uses a
                      x10 fixed point, so 123.4Hz = 1234.

    \note With TimerSource = SMCLK = 24MHz, and TimerDivider = 20, the frequency
    range is 18 to 37500 Hz
*//****************************************************************************/
void Pwm_SetFrequency(uint32_t freq)
{
	uint8_t channelNumber = 0;

	/* convert frequency to compare value */
	_maxCount = (uint16_t)((((uint32_t)(PWM_CLK*10u) + (uint32_t)(freq/2))/freq));

	/* load compare value into the period register */
	Timer_B0_Configure(TimerSource_SMCLK, TimerDivider_20, TimerCounterLength_16BITS, TimerRunMode_UP, _maxCount);

	/* update the duty cycles of any enabled channels */
	for(channelNumber = 0; channelNumber < NUM_CHANNELS; channelNumber++)
	{
		if( _openChannels[channelNumber])
		{
			Pwm_SetDuty((PwmChannel_t)channelNumber, _dutyCycles[channelNumber]);
		}
	}

	if(_AnyChannelsEnabled())
	{
		TIMER_B0_START(TimerRunMode_UP);
	}
}

/***************************************************************************//**
    Set the duty cycle for a specific channel of the PWM.

    \param[in] channel - Channel to configure
    \param[in] duty - Duty cycle to set the channel to
*//****************************************************************************/
bool Pwm_SetDuty(PwmChannel_t channel, uint8_t duty)
{
	TimerB0CC_t channelModule;
	uint16_t dutyCount;

	/* Verify the specified duty cycle is within range */
	if(duty > MAX_DUTY)
	{
		return false;
	}

	_dutyCycles[(uint8_t)channel] = duty;

	dutyCount = _DutyToCount(duty);
	channelModule = _ccModules[channel];

	Timer_B0CCX_Configure(channelModule, OUTMOD_7, dutyCount);

	return true;
}
/***************************************************************************//**
    Gets the duty cycle for a specific channel of the PWM.

    \param[in] channel - Channel to get the duty cycle of
    \returns The duty cycle setting of that PWM channel
*//****************************************************************************/
uint8_t Pwm_GetDuty(PwmChannel_t channel)
{
    uint8_t duty = 0;

    if (channel < ARRAY_LENGTH(_dutyCycles))
    {
        duty = _dutyCycles[channel];
    }

    return duty;
}

/***************************************************************************//**
    Enable a channel of the PWM.

    \param[in] channel - PWM channel to enable
    \param[in] duty - duty cycle for specified channel, should be between 0 and 100
*//****************************************************************************/
bool Pwm_Enable(PwmChannel_t channel, uint8_t duty)
{
	TimerB0CC_t channelModule;
	uint16_t dutyCount;

	/* Verify the specified duty cycle is within range */
	if(duty > MAX_DUTY)
	{
		return false;
	}

	if(_AnyChannelsEnabled() == false)
	{
		TIMER_B0_START(TimerRunMode_UP);
	}

	dutyCount = _DutyToCount(duty);
	channelModule = _ccModules[channel];

	Timer_B0CCX_Configure(channelModule, OUTMOD_7, dutyCount);

	switch (channel)
	{
		case 0:
			Gpio_Configure(ULTRASONIC_OUTPUT_PORT, ULTRASONIC_OUTPUT_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 1:
			Gpio_Configure(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 2:
			Gpio_Configure(LED0_RED_PORT, LED0_RED_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 3:
			Gpio_Configure(LED0_GREEN_PORT, LED0_GREEN_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 4:
			Gpio_Configure(LED1_RED_PORT, LED1_RED_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 5:
			Gpio_Configure(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 6:
			Gpio_Configure(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN,
					GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		default:
			/* NET Todo: what to do in default? */
			break;
	}

	_openChannels[channel] = true;
	_dutyCycles[(uint8_t)channel] = duty;

	return true;
}

/***************************************************************************//**
    Disable a channel of the PWM.

    \param[in] channel - PWM channel to disable
*//****************************************************************************/
void Pwm_Disable(PwmChannel_t channel)
{
	switch (channel)
	{
		case 0:
			Gpio_Configure(ULTRASONIC_OUTPUT_PORT, ULTRASONIC_OUTPUT_PIN,
				   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
			break;

		case 1:
			Gpio_Configure(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN,
				   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 2:
			Gpio_Configure(LED0_RED_PORT, LED0_RED_PIN,
				   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
			break;

		case 3:
			Gpio_Configure(LED0_GREEN_PORT, LED0_GREEN_PIN,
				   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
			break;

		case 4:
			Gpio_Configure(LED1_RED_PORT, LED1_RED_PIN,
				   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
			break;

		case 5:
			Gpio_Configure(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN,
				   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		case 6:
			Gpio_Configure(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN,
					GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
			break;

		default:
			/* NET Todo: what to do in default? */
			break;
	}

	_openChannels[channel] = false;

	if(_AnyChannelsEnabled() == false)
	{
		TIMER_B0_STOP();
	}
}

/***************************************************************************//**
    Converts the specified duty cycle to counts to be loaded into the register

    \param[in] duty - Duty cycle for PWM.
*//****************************************************************************/
static uint16_t _DutyToCount(uint8_t duty)
{
	uint16_t dutyCount;
	volatile uint32_t temp;

	/* calculate count value for specified duty cycle */
	temp = (uint32_t)((uint32_t)duty * _maxCount);
	dutyCount = (uint16_t)(temp/100ul);

	return dutyCount;
}

/***************************************************************************//**
    Determines if any channels are enabled
*//****************************************************************************/
static bool _AnyChannelsEnabled(void)
{
	uint8_t numOpenChannels = 0;
	uint8_t channelNumber = 0;

	for(channelNumber = 0; channelNumber < NUM_CHANNELS; channelNumber++)
	{
		if( _openChannels[channelNumber])
		{
			numOpenChannels++;
		}
	}

	if(numOpenChannels > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}
