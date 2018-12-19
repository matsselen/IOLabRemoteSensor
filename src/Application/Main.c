/***************************************************************************//**
    \mainpage IOLab Remote Sensor Application Documentation

    \b Project 1814 IOLab Remote Sensor Application

    \b Client Macmillan

    \section general_desc General Description
    These software modules implement the operations for interfacing to the
    sensors in the IOLab remote device. This application will configure the
    sensors and gather data based on the configurations from the RF
    microcontroller and then send the data to the RF microcontroller.

    Additional comments Main

    \section interfaces_desc Interfaces
    Main supported interfaces:
    - LEDs
    - Pushbuttons
    - GPIO
    - Power Latch
    - High Gain Input
    - ADC
    - Force Gauge
    - Ambient Light Sensor
    - Microphone
    - ECG
    - Battery Voltage Monitor
    - Audible Buzzer
    - Ultrasonic Transducer
    - Quadrature Encoder
    - Radio Microcontroller
    - Barometer
    - Thermometer
    - Gyroscope
    - DAC
    - Accelerometer
    - Magnetometer
    - Debug Serial Port

    Additional comments Main
    - Not all of the sensors are implemented in V1

    Copyright &copy; 2012 Indesign LLC

*//****************************************************************************/
/***************************************************************************//**
    \file Main.c
    \brief Remote Sensor Microcontroller main processing

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Initialized all of the hardware and enters the infinite main
       processing loop.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/Main.c$
    $Revision: 25$
    $Date: Friday, October 24, 2014 10:53:10 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#include <ProjectTypes.h>

#include "Main.h"
#include <Device.h>
#include <Board.h>
#include <Interrupt.h>
#include <Serial.h>
#include <Debug.h>
#include <Assert.h>
#include <EventQueue.h>
#include <StateMachine.h>
#include <SensorId.h>
#include <ConfigurationManager.h>
#include <Watchdog.h>
#include <TimerTick.h>
#include <System.h>
#include <Uart.h>
// TODO: spg - remove these includes, they are for development only
#include <I2c.h>
#include <SpiMaster.h>
#include <Version.h>
#include <AdcManager.h>
#include <Pwm.h>
#include <SpiSlave.h>
#include <Dma.h>
#include <CommProtocol.h>
#include <TestProtocol.h>
#include <Gpio.h>
#include <PushButtonControl.h>
#include <LedControl.h>
#include <SelfTest.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
#if 1 /* enables low power operation */
#define ENABLE_LOW_POWER
#endif

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Defines the configuration for the I2C bus */
static const I2cPortConfig_t _i2cConfig =
{
		I2cMode_MASTER,
		I2cAddressing_7_bits,
		I2cBaudRate_400000
};
/** Defines the active i2c port */
static const I2cPort_t _i2cPort = I2cPort_0;

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
void main( void );
static void _ClearStartup(void);

/*lint -save -e957 -e970 */
#pragma CHECK_MISRA("none")
/***************************************************************************//**
    System pre-init function that is the very first code to be run.  Overrides
    the function provided by the TI library.

    \note Only microcontroller registers should be accessed.  Any use of
    variables will probably be overwritten.

    From TI:
    _system_pre_init() is called in the C/C++ startup routine (_c_int100()
    in boot.c) and provides a mechanism for the user to insert application
    specific low level initialization instructions prior to calling main().
    The return value of _system_pre_init() is used to determine whether or
    not C/C++ global data initialization will be performed (return value of
    0 to bypass C/C++ auto-initialization).

    PLEASE NOTE THAT BYPASSING THE C/C++ AUTO-INITIALIZATION ROUTINE MAY
    RESULT IN PROGRAM FAILURE.

    The linker will include the updated version if it is linked in prior to
    linking with the C/C++ runtime library (rts430.lib).

    \retval 0 - if variable auto-init code should NOT be run
    \retval 1 - if variable auto-init code should be run
*//****************************************************************************/
int _system_pre_init(void)
{
    /* The watchdog defaults to ON at boot; set it long enough to get through
       pre-init
    */
	WATCHDOG_PRE_INIT();

    /* Return 1 to enable the variable auto-init code */
    return 1;
}
#pragma RESET_MISRA("all")
/*lint -restore */

/***************************************************************************//**
    Prints the reason this device reset using blocking writes.
    Should only be called during the init sequence.

    \param[in] resetReason - Reset reason to print.
*//****************************************************************************/
static void _PrintResetReason(SystemResetReason_t resetReason)
{
    const char_t* resetReasonString;

    resetReasonString = System_ConvertResetReasonToString(resetReason);
    Serial_SendString("Reset reason:");
    Serial_SendString(resetReasonString);
}

/***************************************************************************//**
    Main function.
*//****************************************************************************/
#pragma FUNC_NEVER_RETURNS(main);
void main( void )
{
	SystemResetReason_t resetReason;
#ifdef ENABLE_LOW_POWER
	bool goToLowPower;
#endif

    /* Gather reason for this reset.  Must be retrieved before Board_Init() */
    resetReason = System_GetResetReason();

	/* must call Board Init first to put the board in a known default state */
	Board_Init();

	/* turn on the LED to indicate power on was successful */
	GPIO_SET(LED0_GREEN_PORT, LED0_GREEN_PIN);

	/* Initialize the scheduler and system resources */
	EventQueue_Init();
	TimerTick_Init();
	Serial_Init();
	Serial_Open(SerialMode_TRANSMIT_AND_RECEIVE);

	/* initialize the peripherals */
	Spi_Init();
	Dma_Init();
	SpiSlave_Init();
	I2c_Init(_i2cPort, &_i2cConfig);
	I2c_Open(_i2cPort);

	/* initialize the protocols */
	CommProtocol_Init();
	TestProtocol_Init();

	/* initialize the sensors */
	AdcManager_Init();
	Pwm_Init();
	ConMan_Init();

	/* Initialize controllers */
	LedControl_Init();
	PushButtonControl_Init();
	SelfTest_Init();

	/* Enable interrupts now that everything is initialized */
	INTERRUPT_ENABLE();

	DEBUG(UserFirmwareVersionString());
	_PrintResetReason(resetReason);
	/* Set the system to indicate the startup occurred */
	LedControl_SetLed(Led_0_GREEN, LedState_ON);
	TimerTick_RequestTimeout(TickTimeoutType_SINGLE, 1000u, &_ClearStartup);

	DEBUG("starting loop");
	/* Infinite loop */
	for (;;)
	{
		StateMachine_ProcessEvents();
		Serial_ProcessReceivedBytes();

		if (TimerTick_IsTimerActive())
		{
			WATCHDOG_RESET();
		}

#ifdef ENABLE_LOW_POWER
		/* Disable interrupts before checking the event queue and the UART
		   buffers, so that an event or byte cannot "sneak in" and still
		   allow the micro to enter a low power mode.
		   Interrupts are enabled when entering any low power mode.
		*/

		INTERRUPT_DISABLE();
		goToLowPower = true;

		if (Uart_AreBytesAvailableAnyOpenPort())
		{
			goToLowPower = false;
		}

		if (EventQueue_IsEmpty() == false)
		{
			goToLowPower = false;
		}

		if (goToLowPower)
		{
			_low_power_mode_0();
		}

		INTERRUPT_ENABLE();
#endif

	} /* end of infinite loop */
}

/***************************************************************************//**
    Clear the things that the startup left set.
*//****************************************************************************/
static void _ClearStartup(void)
{
	LedControl_SetLed(Led_0_GREEN, LedState_BLINK_1);
}
