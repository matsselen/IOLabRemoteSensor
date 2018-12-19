/***************************************************************************//**
    \file Ultrasound.c
    \brief Driver for the Ultrasonic Transducer Circuit

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module interfaces to the Ultrasound Transducer Circuit

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Ultrasound.c$
    $Revision: 3$
    $Date: Monday, August 20, 2012 9:19:05 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Ultrasound.h"
#include <Clock.h>
#include <Gpio.h>
#include <Device.h>
#include <Board.h>

#include <Assert.h>
// TODO: net - remove these as they are for debugging only
#include <Watchdog.h>
#include <Interrupt.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the number of microseconds for the ultrasound transducer signal
 * to be high or low (50% duty cycle)
 */
#define ULTRASOUND_HALFPERIOD_US 12u

/* Defines the time between ultrasound pulses */
#define ULTRASOUND_DELAY_US 30000u

/* Defines number of pulses */
#define ULTRASOUND_NUM_PULSES 8u


/*-----------------------LOCAL VARIABLES--------------------------------------*/


/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
void _TxEnable(void);
void _RxEnable(void);

void UltrasoundInit(void)
{
	/* Initialize to receive mode*/
	_RxEnable();
}


/***************************************************************************//**
 Note: the contents of the following function are for hardware test purposes
 only. This is not how the real transmit function will be implemented.

 NET TODO: Replace the following function with feature code

*//****************************************************************************/
void UltrasoundStartTransmit(void)
{
	uint8_t pulseNum;

	_TxEnable();

	/* Disable interrupts so they don't affect the clock cycle counting */
	INTERRUPT_DISABLE();
	/* Create burst of 8 40kHz pulses every 10ms */
	while (1)
	{
		/* pet the watchdog so it doesn't reset the system in our loop */
		WATCHDOG_RESET();

		/* Change direction pin during delay */
		GPIO_CLEAR(ULTRASONIC_DIRECTION_PORT, ULTRASONIC_DIRECTION_PIN);

		CLOCK_DELAY_US(ULTRASOUND_DELAY_US);

		GPIO_SET(ULTRASONIC_DIRECTION_PORT, ULTRASONIC_DIRECTION_PIN);

		for(pulseNum = 0; pulseNum < ULTRASOUND_NUM_PULSES; pulseNum++)
		{
			GPIO_SET(ULTRASONIC_OUTPUT_PORT, ULTRASONIC_OUTPUT_PIN);
			CLOCK_DELAY_US(ULTRASOUND_HALFPERIOD_US);
			GPIO_CLEAR(ULTRASONIC_OUTPUT_PORT, ULTRASONIC_OUTPUT_PIN);
			CLOCK_DELAY_US(ULTRASOUND_HALFPERIOD_US);
		}
	}
}

/***************************************************************************//**


*//****************************************************************************/
void UltrasoundStopTransmit(void)
{


}

/***************************************************************************//**


*//****************************************************************************/
void _TxEnable(void)
{
	/* DIR_ULTRASONIC_TRANSDUCER is low to transmit */
	GPIO_CLEAR(ULTRASONIC_DIRECTION_PORT, ULTRASONIC_DIRECTION_PIN);

	/* Enable the transducer power supply */
	GPIO_SET(POWER_ULTRASONIC_ENABLE_PORT, POWER_ULTRASONIC_ENABLE_PIN);
}

/***************************************************************************//**


*//****************************************************************************/
void _RxEnable(void)
{
	/* DIR_ULTRASONIC_TRANSDUCER is high to receive */
	GPIO_SET(ULTRASONIC_DIRECTION_PORT, ULTRASONIC_DIRECTION_PIN);

	/* Disable the transducer power supply */
	GPIO_CLEAR(POWER_ULTRASONIC_ENABLE_PORT, POWER_ULTRASONIC_ENABLE_PIN);

}



