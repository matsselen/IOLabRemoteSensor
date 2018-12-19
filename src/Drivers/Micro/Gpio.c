/***************************************************************************//**
    \file Gpio.c
    \brief GPIO configuration and control macros and functions

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The routines in this file are used to initialize or
       configure the GPIO pins on the MSP430.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Gpio.c$
    $Revision: 15$
    $Date: Wednesday, April 03, 2013 9:44:20 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "Gpio.h"
#include <Assert.h>
#include <Debug.h>
#include <Serial.h>
#include <Board.h>

/*-----------------------LOCAL DEFINITIONS-----------------------------------*/
/** Minimum port number. */
#define PORT_MIN        (1)
/** Maximum port number. */
#define PORT_MAX        (8)

/** Minimum port number that supports interrupts. */
#define PORT_INT_MIN    (1)
/** Maximum port number that supports interrupts. */
#define PORT_INT_MAX    (2)

/** Number of pins per port */
#define PINS_PER_PORT   (8u)

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Pointers to GPIO output registers. */
static volatile uint8_t* const _gpioOutputRegisters[PORT_MAX+1] =
{
    NULL,
    &P1OUT,
    &P2OUT,
    &P3OUT,
    &P4OUT,
    &P5OUT,
    &P6OUT,
    &P7OUT,
    &P8OUT,
//    &PJOUT_L,
//    &PUOUT
};

/** Pointers to the GPIO input registers */
static volatile uint8_t* const _gpioInputRegisters[PORT_MAX+1] =
{
	NULL,
	&P1IN,
	&P2IN,
	&P3IN,
	&P4IN,
	&P5IN,
	&P6IN,
	&P7IN,
	&P8IN,
//	&PJIN,
//	&PUIN,
};

/** Pointers to GPIO resistor enable registers. */
static volatile uint8_t* const _gpioResistorEnableRegisters[PORT_MAX+1] =
{
    NULL,
    &P1REN,
    &P2REN,
    &P3REN,
    &P4REN,
    &P5REN,
    &P6REN,
    &P7REN,
    &P8REN,
//    &PJREN_L,
//    &PUREN
};

/** Pointers to GPIO interrupt enable registers. */
static volatile uint8_t* const _gpioInterruptEnableRegisters[PORT_INT_MAX+1] =
{
    NULL,
    &P1IE,
    &P2IE
};

/** Pointers to GPIO interrupt edge select registers. */
static volatile uint8_t* const _gpioInterruptEdgeSelectRegisters[PORT_INT_MAX+1] =
{
    NULL,
    &P1IES,
    &P2IES
};

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Stores the port one GPIO ISR pointers */
static GpioIsrPointer_t _gpioIsrListPortOne[PINS_PER_PORT] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
/** Stores the port two GPIO ISR pointers */
static GpioIsrPointer_t _gpioIsrListPortTwo[PINS_PER_PORT] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
__interrupt void _GpioPortOneIsr(void);
__interrupt void _GpioPortTwoIsr(void);
static void _UnlockPortU(void);
static void _LockPortU(void);

/***************************************************************************//**
    Initializes the digital I/O pins.
*//****************************************************************************/
void Gpio_Init(void)
{
    /* Button/switch pins */
    Gpio_Configure(BUTTON_0_PORT, BUTTON_0_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(BUTTON_1_PORT, BUTTON_1_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_EnableResistor(BUTTON_1_PORT, BUTTON_1_PIN, GpioResistorType_PULLUP);

    /* I2C bus pins */
    Gpio_Configure(I2C_SCL_PORT, I2C_SCL_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(I2C_SDA_PORT, I2C_SDA_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Power control pins */
    Gpio_Configure(POWER_ADC_V1_ENABLE_PORT, POWER_ADC_V1_ENABLE_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(POWER_ULTRASONIC_ENABLE_PORT, POWER_ULTRASONIC_ENABLE_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(POWER_ADC_V2_ENABLE_PORT, POWER_ADC_V2_ENABLE_PIN,
                   GpioType_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(POWER_FORCE_GAUGE_ENABLE_PORT, POWER_FORCE_GAUGE_ENABLE_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_ConfigurePortU(POWER_ENCODER_ENABLE_PIN,
                        GpioType_OUTPUT, GpioValue_LOW);
    Gpio_ConfigurePortU(POWER_LATCH_PIN,
                        GpioType_OUTPUT, GpioValue_HIGH);

    /* Debug UART pins */
    Gpio_Configure(DEBUG_UART_TX_PORT, DEBUG_UART_TX_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(DEBUG_UART_RX_PORT, DEBUG_UART_RX_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* LED output pins */
    Gpio_Configure(LED0_RED_PORT, LED0_RED_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(LED0_GREEN_PORT, LED0_GREEN_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(LED1_RED_PORT, LED1_RED_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(LED1_GREEN_PORT, LED1_GREEN_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);

    /* External header I/O pins */
    Gpio_Configure(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    /* these pins can be analog inputs, so set the analog input to save power */
    Gpio_Configure(EXT_HEADER_6_PORT, EXT_HEADER_6_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_7_PORT, EXT_HEADER_7_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(EXT_HEADER_8_PORT, EXT_HEADER_8_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* SPI master pins (sensor interface) */
    Gpio_Configure(SPI_MASTER_MOSI_PORT, SPI_MASTER_MOSI_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(SPI_MASTER_MISO_PORT, SPI_MASTER_MISO_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(SPI_MASTER_CLK_PORT, SPI_MASTER_CLK_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    Gpio_Configure(CS_BAROMETER_PORT, CS_BAROMETER_PIN,
                   GpioType_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(CS_GYRO_PORT, CS_GYRO_PIN,
                   GpioType_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(CS_DAC_PORT, CS_DAC_PIN,
                   GpioType_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);

    /* SPI Slave pins (RF micro interface) */
    Gpio_Configure(SPI_SLAVE_MOSI_PORT, SPI_SLAVE_MOSI_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(SPI_SLAVE_MISO_PORT, SPI_SLAVE_MISO_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(SPI_SLAVE_CLK_PORT, SPI_SLAVE_CLK_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(CS_RADIO_PORT, CS_RADIO_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Analog Input Pins */
    Gpio_Configure(ADC_FORCE_GAUGE_PORT, ADC_FORCE_GAUGE_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_ECG_1_PORT, ADC_ECG_1_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_ECG_2_PORT, ADC_ECG_2_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_ECG_3_PORT, ADC_ECG_3_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_LIGHT_SENSOR_PORT, ADC_LIGHT_SENSOR_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_HIGH_GAIN_PORT, ADC_HIGH_GAIN_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_MICROPHONE_PORT, ADC_MICROPHONE_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ADC_BATTERY_PORT, ADC_BATTERY_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Ultrasonic Transducer pins     */
    Gpio_Configure(ULTRASONIC_DIRECTION_PORT, ULTRASONIC_DIRECTION_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ULTRASONIC_CAPTURE_BEGIN_PORT, ULTRASONIC_CAPTURE_BEGIN_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ULTRASONIC_CAPTURE_END_PORT, ULTRASONIC_CAPTURE_END_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ULTRASONIC_OUTPUT_PORT, ULTRASONIC_OUTPUT_PIN,
                   GpioType_OUTPUT, GpioValue_LOW, GpioDriveStrength_DEFAULT);

    /* Encoder input pins */
    Gpio_Configure(ENCODER_INPUT_DIRECTION_PORT, ENCODER_INPUT_DIRECTION_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ENCODER_INPUT_COUNTER_PORT, ENCODER_INPUT_COUNTER_PIN,
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Accelerometer interrupt pins */
    Gpio_Configure(ACCELEROMETER_INTERRUPT_0_PORT, ACCELEROMETER_INTERRUPT_0_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(ACCELEROMETER_INTERRUPT_1_PORT, ACCELEROMETER_INTERRUPT_1_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_EnableResistor(ACCELEROMETER_INTERRUPT_0_PORT, ACCELEROMETER_INTERRUPT_0_PIN,
                        GpioResistorType_PULLDOWN);
    Gpio_EnableResistor(ACCELEROMETER_INTERRUPT_1_PORT, ACCELEROMETER_INTERRUPT_1_PIN,
                        GpioResistorType_PULLDOWN);

    /* Magnetometer interrupt pins */
    Gpio_Configure(MAGNETOMETER_INTERRUPT_PORT, MAGNETOMETER_INTERRUPT_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Gyroscope interrupt pins */
    Gpio_Configure(GYRO_INTERRUPT_0_PORT, GYRO_INTERRUPT_0_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(GYRO_INTERRUPT_1_PORT, GYRO_INTERRUPT_1_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Buzzer output pins */
    Gpio_Configure(BUZZER_OUTPUT_PORT, BUZZER_OUTPUT_PIN,
                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Boot strap loader pins */
    Gpio_Configure(BSL_TX_PORT, BSL_TX_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(BSL_RX_PORT, BSL_RX_PIN,
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
}

/***************************************************************************//**
    Configures GPIO pins.

    \param[in] port - Port number of pins.
    \param[in] pinMask - Mask of pins.
    \param[in] type - Type to set the GPIO to.
    \param[in] value - Value to set the GPIO to.
    \param[in] driveStrength - Drive strength to set the GPIO to.
*//****************************************************************************/
void Gpio_Configure(
    int16_t port,
    uint8_t pinMask,
    GpioType_t type,
    GpioValue_t value,
    GpioDriveStrength_t driveStrength)
{
    /** Pointers to GPIO direction registers. */
    static volatile uint8_t* const _gpioDirectionRegisters[PORT_MAX+1] =
    {
        NULL,
        &P1DIR,
        &P2DIR,
        &P3DIR,
        &P4DIR,
        &P5DIR,
        &P6DIR,
        &P7DIR,
        &P8DIR,
    };

    /** Pointers to GPIO drive strength registers. */
    static volatile uint8_t* const _gpioDriveStrengthRegisters[PORT_MAX+1] =
    {
        NULL,
        &P1DS,
        &P2DS,
        &P3DS,
        &P4DS,
        &P5DS,
        &P6DS,
        &P7DS,
        &P8DS,
    };

    /** Pointers to GPIO peripheral select registers. */
    static volatile uint8_t* const _gpioSelectRegisters[PORT_MAX+1] =
    {
        NULL,
        &P1SEL,
        &P2SEL,
        &P3SEL,
        &P4SEL,
        &P5SEL,
        &P6SEL,
        &P7SEL,
        &P8SEL,
    };

    volatile uint8_t* selectReg;
    volatile uint8_t* dirReg;
    volatile uint8_t* outReg;
    volatile uint8_t* dsReg;

    ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

    selectReg = _gpioSelectRegisters[port];
    dirReg = _gpioDirectionRegisters[port];
    outReg = _gpioOutputRegisters[port];
    dsReg = _gpioDriveStrengthRegisters[port];

    /* Set drive strength */
    if (driveStrength == GpioDriveStrength_FULL)
    {
        *dsReg = *dsReg | pinMask;
    }
    else
    {
        *dsReg = *dsReg & ((uint8_t) ~(pinMask));
    }

    /* Set output value */
    if (value == GpioValue_HIGH)
    {
        *outReg = *outReg | pinMask;
    }
    else
    {
        *outReg = *outReg & ((uint8_t) ~(pinMask));
    }

    /* Set type */
    switch (type)
    {
        case GpioType_INPUT:
            *selectReg = *selectReg & ((uint8_t) ~(pinMask));
            *dirReg = *dirReg & ((uint8_t) ~(pinMask));
            break;

        case GpioType_OUTPUT:
            *selectReg = *selectReg & ((uint8_t) ~(pinMask));
            *dirReg = *dirReg | pinMask;
            break;

        case GpioType_PERIPHERAL_INPUT:
            *dirReg = *dirReg & ((uint8_t) ~(pinMask));
            *selectReg = *selectReg | pinMask;
            break;

        case GpioType_PERIPHERAL_OUTPUT:
            *dirReg = *dirReg | pinMask;
            *selectReg = *selectReg | pinMask;
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }
}

/***************************************************************************//**
    Configures GPIO pins for Port J.

    \param[in] pinMask - Mask of pins.
    \param[in] type - Type to set the GPIO to.
    \param[in] value - Value to set the GPIO to.
    \param[in] driveStrength - Drive strength to set the GPIO to.
*//****************************************************************************/
void Gpio_ConfigurePortJ(
    uint8_t pinMask,
    GpioType_t type,
    GpioValue_t value,
    GpioDriveStrength_t driveStrength)
{
    static volatile uint8_t* dirReg = &PJDIR_L;
    static volatile uint8_t* outReg = &PJOUT_L;
    static volatile uint8_t* dsReg  = &PJDS_L;

    /* Set drive strength */
    if (driveStrength == GpioDriveStrength_FULL)
    {
        *dsReg = *dsReg | pinMask;
    }
    else
    {
        *dsReg = *dsReg & ((uint8_t) ~(pinMask));
    }

    /* Set output value */
    if (value == GpioValue_HIGH)
    {
        *outReg = *outReg | pinMask;
    }
    else
    {
        *outReg = *outReg & ((uint8_t) ~(pinMask));
    }

    /* Set type */
    switch (type)
    {
        case GpioType_INPUT:
            *dirReg = *dirReg & ((uint8_t) ~(pinMask));
            break;

        case GpioType_OUTPUT:
            *dirReg = *dirReg | pinMask;
            break;

        case GpioType_PERIPHERAL_INPUT:
        	/* fall through to default because peripheral is not
        	   a valid option for port J.
        	*/
        case GpioType_PERIPHERAL_OUTPUT:
        	/* fall through to default because peripheral is not
			   a valid option for port J.
			*/
        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }
}

/***************************************************************************//**
    Configures GPIO pins for Port U.

    \param[in] pinMask - Mask of pins.
    \param[in] type - Type to set the GPIO to.
    \param[in] value - Value to set the GPIO to.
*//****************************************************************************/
void Gpio_ConfigurePortU(
	    uint8_t pinMask,
	    GpioType_t type,
	    GpioValue_t value)
{
	/* Port U only has 2 pins, so make sure the pin mask is only referring
	   to those two pins.
	*/
	ASSERT((pinMask & ~(PUOUT0 | PUOUT1)) == 0u, ErrorLedMode_SOFTWARE);

	_UnlockPortU();

    /* Set output value */
    if (value == GpioValue_HIGH)
    {
        PUCTL_L |= pinMask;
    }
    else
    {
        PUCTL_L &= ((uint8_t) ~(pinMask));
    }

    /* Set type */
    switch (type)
    {
        case GpioType_INPUT:
        	/* clear the output bit and set the input bit */
            PUCTL &= ~(PUOPE);
            PUCTL |= PUIPE;
            break;

        case GpioType_OUTPUT:
        	/* clear the input bit and set the output bit */
            PUCTL &= ~(PUIPE);
            PUCTL |= PUOPE;
            break;

        case GpioType_PERIPHERAL_INPUT:
        	/* fall through to default because peripheral is not
        	   a valid option for port U.
        	*/
        case GpioType_PERIPHERAL_OUTPUT:
        	/* fall through to default because peripheral is not
			   a valid option for port U.
			*/
        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    _LockPortU();
}
/***************************************************************************//**
    Enable the pull-up/pull-down resistor of GPIO pins.

    \param[in] port - Port number of pins.
    \param[in] pinMask - Mask of pins.
    \param[in] resistorType - Type of resistor to enable.
*//****************************************************************************/
void Gpio_EnableResistor(
    int16_t port,
    uint8_t pinMask,
    GpioResistorType_t resistorType)
{
    volatile uint8_t* renReg;
    volatile uint8_t* outReg;

    ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

    renReg = _gpioResistorEnableRegisters[port];
    outReg = _gpioOutputRegisters[port];

    if (resistorType == GpioResistorType_PULLUP)
    {
        *outReg = *outReg | pinMask;
    }
    else
    {
        *outReg = *outReg & ((uint8_t) ~(pinMask));
    }

    *renReg = *renReg | pinMask;
}

/***************************************************************************//**
    Disable the pull-up/pull-down resistor of GPIO pins.

    \param[in] port - Port number of pins.
    \param[in] pinMask - Mask of pins.
*//****************************************************************************/
void Gpio_DisableResistor(
    int16_t port,
    uint8_t pinMask)
{
    volatile uint8_t* renReg;

    ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

    renReg = _gpioResistorEnableRegisters[port];

    *renReg = *renReg & ((uint8_t) ~(pinMask));
}

/***************************************************************************//**
    Set a GPIO pin to high.  Allows the port parameter to be a variable,
    as compared to the GPIO_SET macro that does not.

    \note Use the GPIO_SET macro if possible for better performance.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_Set(
    int16_t port,
    uint8_t pinMask)
{
    volatile uint8_t* outReg;

    ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

    outReg = _gpioOutputRegisters[port];

    *outReg = *outReg | pinMask;
}

/***************************************************************************//**
    Set a GPIO pin On port U to high.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_SetPortU(
    uint8_t pinMask)
{
	/* Port U only has 2 pins, so make sure the pin mask is only referring
	   to those two pins.
	*/
	ASSERT((pinMask & ~(PUOUT0 | PUOUT1)) == 0u, ErrorLedMode_SOFTWARE);

	_UnlockPortU();
    PUCTL |= pinMask;
    _LockPortU();
}

/***************************************************************************//**
    Set a GPIO pin On port J to high.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_SetPortJ(
    uint8_t pinMask)
{
    PJOUT |= pinMask;
}

/***************************************************************************//**
    Clear a GPIO pin to low.  Allows the port parameter to be a variable,
    as compared to the GPIO_CLEAR macro that does not.

    \note Use the GPIO_CLEAR macro if possible for better performance.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_Clear(
    int16_t port,
    uint8_t pinMask)
{
    volatile uint8_t* outReg;

    ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

    outReg = _gpioOutputRegisters[port];

    *outReg = *outReg & ((uint8_t) ~(pinMask));
}

/***************************************************************************//**
    Clear a Port U GPIO pin to low.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_ClearPortU(
    uint8_t pinMask)
{
	/* Port U only has 2 pins, so make sure the pin mask is only referring
	   to those two pins.
	*/
	ASSERT((pinMask & ~(PUOUT0 | PUOUT1)) == 0u, ErrorLedMode_SOFTWARE);

	_UnlockPortU();
    PUCTL &= ~(pinMask);
    _LockPortU();
}

/***************************************************************************//**
    Clear a Port J GPIO pin to low.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_ClearPortJ(
    uint8_t pinMask)
{
    PJOUT &= ~(pinMask);
}

/***************************************************************************//**
    Read the value of a GPIO pin.  Allows the port parameter to be a variable,
    as compared to the GPIO_IS_HIGH macro that does not.

    \note Use the GPIO_IS_HIGH macro if possible for better performance.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
bool Gpio_GetValue(
    int16_t port,
    uint8_t pinMask)
{
	volatile uint8_t* inReg;
	bool pinValue;

	ASSERT((port >= PORT_MIN) && (port <= PORT_MAX), ErrorLedMode_SOFTWARE);

	inReg = _gpioInputRegisters[port];

	if (*inReg & ((uint8_t)(pinMask)))
	{
		pinValue = true;
	}
	else
	{
		pinValue = false;
	}

	return pinValue;
}

/***************************************************************************//**
    Read the value of a GPIO pin on Port U.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
bool Gpio_GetValuePortU(
    uint8_t pinMask)
{
	/** The input bit mask for Port U is shifted from the standard bit mask. This
	    is how many bits to shift the bitmask left to read the input bits
	*/
#define INPUT_BIT_SHIFT    2

	bool pinValue;

	/* Port U only has 2 pins, so make sure the pin mask is only referring
	   to those two pins.
	*/
	ASSERT((pinMask & ~(PUOUT0 | PUOUT1)) == 0u, ErrorLedMode_SOFTWARE);

	if (PUCTL & ((uint16_t)(pinMask<<INPUT_BIT_SHIFT)))
	{
		pinValue = true;
	}
	else
	{
		pinValue = false;
	}

	return pinValue;
}

/***************************************************************************//**
    Read the value of a GPIO pin on Port J.

    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
bool Gpio_GetValuePortJ(
    uint8_t pinMask)
{
	bool pinValue;

	if (PJIN & ((uint16_t)(pinMask)))
	{
		pinValue = true;
	}
	else
	{
		pinValue = false;
	}

	return pinValue;
}

/***************************************************************************//**
    Register an ISR for the given GPIO pin.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.
    \param[in] isr - Pointer to GPIO ISR.
    \param[in] edge - Edge to select.
*//****************************************************************************/
void Gpio_RegisterIsr(
    int16_t port,
    uint8_t pinMask,
    GpioIsrPointer_t isr,
    GpioEdge_t edge)
{
    volatile uint8_t* ieReg;
    volatile uint8_t* iesReg;
    uint16_t pinNumber = PINS_PER_PORT;
    uint16_t i;

    ASSERT((port >= PORT_INT_MIN) && (port <= PORT_INT_MAX) && (isr != NULL), ErrorLedMode_SOFTWARE);

    /* Determine pin number */
    for (i = 0u; i < PINS_PER_PORT; ++i)
    {
        /* Checking that the shifted mask equals exactly one
           ensures that only one pin is specified in the mask.
        */
        if (pinMask == 1u)
        {
            pinNumber = i;
            break;
        }

        pinMask >>= 1;
    }
    ASSERT(pinNumber < PINS_PER_PORT, ErrorLedMode_SOFTWARE);

    /* Disable interrupt */
    ieReg = _gpioInterruptEnableRegisters[port];
    *ieReg = *ieReg & ((uint8_t) ~(pinMask));

    /* Store ISR pointer */
    if (port == 1)
    {
        ASSERT(_gpioIsrListPortOne[pinNumber] == NULL, ErrorLedMode_SOFTWARE);

        _gpioIsrListPortOne[pinNumber] = isr;
    }
    else
    {
        ASSERT(_gpioIsrListPortTwo[pinNumber] == NULL, ErrorLedMode_SOFTWARE);

        _gpioIsrListPortTwo[pinNumber] = isr;
    }

    /* Select interrupt edge */
    iesReg = _gpioInterruptEdgeSelectRegisters[port];
    if (edge == GpioEdge_FALLING)
    {
        *iesReg = *iesReg | pinMask;
    }
    else
    {
        *iesReg = *iesReg & ((uint8_t) ~(pinMask));
    }
}

/***************************************************************************//**
    Unregister an ISR for the given GPIO pin.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.
*//****************************************************************************/
void Gpio_UnregisterIsr(
    int16_t port,
    uint8_t pinMask)
{
    volatile uint8_t* ieReg;
    uint16_t pinNumber = PINS_PER_PORT;
    uint16_t i;

    ASSERT((port >= PORT_INT_MIN) && (port <= PORT_INT_MAX), ErrorLedMode_SOFTWARE);

    /* Determine pin number */
    for (i = 0u; i < PINS_PER_PORT; ++i)
    {
        /* Checking that the shifted mask equals exactly one
           ensures that only one pin is specified in the mask.
        */
        if (pinMask == 1u)
        {
            pinNumber = i;
            break;
        }

        pinMask >>= 1;
    }
    ASSERT(pinNumber < PINS_PER_PORT, ErrorLedMode_SOFTWARE);

    /* Disable interrupt */
    ieReg = _gpioInterruptEnableRegisters[port];
    *ieReg = *ieReg & ((uint8_t) ~(pinMask));

    /* Erase ISR pointer */
    if (port == 1)
    {
        _gpioIsrListPortOne[pinNumber] = NULL;
    }
    else
    {
        _gpioIsrListPortTwo[pinNumber] = NULL;
    }
}

/***************************************************************************//**
    Select the interrupt edge for GPIO pins.

    \param[in] port - Port number of pins.
    \param[in] pinMask - Mask of pins.
    \param[in] edge - Edge to select.
*//****************************************************************************/
void Gpio_SelectInterruptEdge(
    int16_t port,
    uint8_t pinMask,
    GpioEdge_t edge)
{
    volatile uint8_t* iesReg;

    ASSERT((port >= PORT_INT_MIN) && (port <= PORT_INT_MAX), ErrorLedMode_SOFTWARE);

    iesReg = _gpioInterruptEdgeSelectRegisters[port];

    if (edge == GpioEdge_FALLING)
    {
        *iesReg = *iesReg | pinMask;
    }
    else
    {
        *iesReg = *iesReg & ((uint8_t) ~(pinMask));
    }
}

/***************************************************************************//**
    Get the selected interrupt edge for a GPIO pin.

    \param[in] port - Port number of pin.
    \param[in] pinMask - Mask of pin.

    \return The selected interrupt edge.
*//****************************************************************************/
GpioEdge_t Gpio_GetInterruptEdge(
    int16_t port,
    uint8_t pinMask)
{
    GpioEdge_t edge;
    volatile uint8_t* iesReg;

    ASSERT((port >= PORT_INT_MIN) && (port <= PORT_INT_MAX), ErrorLedMode_SOFTWARE);

    iesReg = _gpioInterruptEdgeSelectRegisters[port];

    if ((*iesReg & pinMask) > 0u)
    {
        edge = GpioEdge_FALLING;
    }
    else
    {
        edge = GpioEdge_RISING;
    }

    return edge;
}

/***************************************************************************//**
    Interrupt service routine for the port 1 GPIO interrupts.  The interrupt
    flag of the interrupt specified in P1IV is cleared when P1IV is read.
*//****************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void _GpioPortOneIsr(void)
{
    /* Note: intrinsic allows compiler to optimize switch */
    switch (_even_in_range(P1IV, P1IV_P1IFG7))
    {
        /* Interrupt flag for pin 1.0 is set */
        case P1IV_P1IFG0:
            if (_gpioIsrListPortOne[0] != NULL)
            {
                (*_gpioIsrListPortOne[0])();
            }
            break;

        /* Interrupt flag for pin 1.1 is set */
        case P1IV_P1IFG1:
            if (_gpioIsrListPortOne[1] != NULL)
            {
                (*_gpioIsrListPortOne[1])();
            }
            break;

        /* Interrupt flag for pin 1.2 is set */
        case P1IV_P1IFG2:
            if (_gpioIsrListPortOne[2] != NULL)
            {
                (*_gpioIsrListPortOne[2])();
            }
            break;

        /* Interrupt flag for pin 1.3 is set */
        case P1IV_P1IFG3:
            if (_gpioIsrListPortOne[3] != NULL)
            {
                (*_gpioIsrListPortOne[3])();
            }
            break;

        /* Interrupt flag for pin 1.4 is set */
        case P1IV_P1IFG4:
            if (_gpioIsrListPortOne[4] != NULL)
            {
                (*_gpioIsrListPortOne[4])();
            }
            break;

        /* Interrupt flag for pin 1.5 is set */
        case P1IV_P1IFG5:
            if (_gpioIsrListPortOne[5] != NULL)
            {
                (*_gpioIsrListPortOne[5])();
            }
            break;

        /* Interrupt flag for pin 1.6 is set */
        case P1IV_P1IFG6:
            if (_gpioIsrListPortOne[6] != NULL)
            {
                (*_gpioIsrListPortOne[6])();
            }
            break;

        /* Interrupt flag for pin 1.7 is set */
        case P1IV_P1IFG7:
            if (_gpioIsrListPortOne[7] != NULL)
            {
                (*_gpioIsrListPortOne[7])();
            }
            break;

        default:
            _never_executed();
            break;
    }

    _low_power_mode_off_on_exit();
}

/***************************************************************************//**
    Interrupt service routine for the port 2 GPIO interrupts.  The interrupt
    flag of the interrupt specified in P2IV is cleared when P2IV is read.
*//****************************************************************************/
#pragma vector=PORT2_VECTOR
__interrupt void _GpioPortTwoIsr(void)
{
    /* Note: intrinsic allows compiler to optimize switch */
    switch (_even_in_range(P2IV, P2IV_P2IFG7))
    {
        /* Interrupt flag for pin 2.0 is set */
        case P2IV_P2IFG0:
            if (_gpioIsrListPortTwo[0] != NULL)
            {
                (*_gpioIsrListPortTwo[0])();
            }
            break;

        /* Interrupt flag for pin 2.1 is set */
        case P2IV_P2IFG1:
            if (_gpioIsrListPortTwo[1] != NULL)
            {
                (*_gpioIsrListPortTwo[1])();
            }
            break;

        /* Interrupt flag for pin 2.2 is set */
        case P2IV_P2IFG2:
            if (_gpioIsrListPortTwo[2] != NULL)
            {
                (*_gpioIsrListPortTwo[2])();
            }
            break;

        /* Interrupt flag for pin 2.3 is set */
        case P2IV_P2IFG3:
            if (_gpioIsrListPortTwo[3] != NULL)
            {
                (*_gpioIsrListPortTwo[3])();
            }
            break;

        /* Interrupt flag for pin 2.4 is set */
        case P2IV_P2IFG4:
            if (_gpioIsrListPortTwo[4] != NULL)
            {
                (*_gpioIsrListPortTwo[4])();
            }
            break;

        /* Interrupt flag for pin 2.5 is set */
        case P2IV_P2IFG5:
            if (_gpioIsrListPortTwo[5] != NULL)
            {
                (*_gpioIsrListPortTwo[5])();
            }
            break;

        /* Interrupt flag for pin 2.6 is set */
        case P2IV_P2IFG6:
            if (_gpioIsrListPortTwo[6] != NULL)
            {
                (*_gpioIsrListPortTwo[6])();
            }
            break;

        /* Interrupt flag for pin 1.7 is set */
        case P2IV_P2IFG7:
            if (_gpioIsrListPortTwo[7] != NULL)
            {
                (*_gpioIsrListPortTwo[7])();
            }
            break;

        default:
            _never_executed();
            break;
    }

    _low_power_mode_off_on_exit();
}

/***************************************************************************//**
    Unlocks the port U registers for for changing
*//****************************************************************************/
static void _UnlockPortU(void)
{
	LDOKEYPID = LDOKEY;
}

/***************************************************************************//**
    Relocks the port U registers to prevent changing them.
*//****************************************************************************/
static void _LockPortU(void)
{
	LDOKEYPID = 0u;
}
