/***************************************************************************//**
    \file Gpio.h
    \brief GPIO configuration and control macros and functions

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The functions and macros defined in this file provide
       access to the configuration and control registers for the GPIO pins
       on the MSP430. This includes access to the interrupt control registers
       for ports 1 and 2.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Gpio.h$
    $Revision: 5$
    $Date: Friday, July 17, 2015 2:23:13 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_

/* Include device header since the macros map to device register */
#include <Device.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the possible GPIO types. */
typedef enum GpioType
{
    GpioType_INPUT,
    GpioType_OUTPUT,
    GpioType_PERIPHERAL_INPUT,
    GpioType_PERIPHERAL_OUTPUT
} GpioType_t;

/** Defines the possible GPIO values. */
typedef enum GpioValue
{
    GpioValue_LOW = false,
    GpioValue_HIGH = true,
    GpioValue_DONT_CARE = GpioValue_LOW
} GpioValue_t;

/** Defines the possible GPIO drive strengths. */
typedef enum GpioDriveStrength
{
    GpioDriveStrength_REDUCED = 0,
    GpioDriveStrength_FULL = 1,
    GpioDriveStrength_DEFAULT = GpioDriveStrength_REDUCED
} GpioDriveStrength_t;

/** Defines the possible resistor types. */
typedef enum GpioResistorType
{
    GpioResistorType_PULLDOWN,
    GpioResistorType_PULLUP
} GpioResistorType_t;

/** Defines the possible interrupt edges. */
typedef enum GpioEdge
{
    GpioEdge_RISING,
    GpioEdge_FALLING
} GpioEdge_t;

/** Defines a GPIO ISR function pointer */
typedef void (*GpioIsrPointer_t)(void);

/*-----------------------PUBLIC MACROS----------------------------------------*/
/*lint -esym(960, 19.12) */
#pragma CHECK_MISRA("none")

/** \name Digital I/O value control and test macros.
    \{
*/
/***************************************************************************//**
    \brief Set a GPIO pin to high.

    Assumes the GPIO pin is already configured as an output.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_SET(port, pin)                 GPIO_SET_PREP(port, (pin))

/***************************************************************************//**
    \brief Clear a GPIO pin to low.

    Assumes the GPIO pin is already configured as an output.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_CLEAR(port, pin)               GPIO_CLEAR_PREP(port, (pin))

/***************************************************************************//**
    \brief Toggle the value of a GPIO pin.

    Assumes the GPIO pin is already configured as an output.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_TOGGLE(port, pin)              GPIO_TOGGLE_PREP(port, (pin))

/***************************************************************************//**
    \brief Check if a GPIO pin is high.

    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask

    \return True if the pin is high, false otherwise.
*//****************************************************************************/
#define GPIO_IS_HIGH(port, pin)             GPIO_IS_HIGH_PREP(port, (pin))

/***************************************************************************//**
    \brief Check if a GPIO pin is low.

    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask

    \return True if the pin is low, false otherwise.
*//****************************************************************************/
#define GPIO_IS_LOW(port, pin)              GPIO_IS_LOW_PREP(port, (pin))

/***************************************************************************//**
    \brief Check the status of multiple GPIO pins on one port.

    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pins - Pin(s) mask

    \return Current state of the requested pins.
*//****************************************************************************/
#define GPIO_PORT_GET(port, pins)           GPIO_PORT_GET_PREP(port, (pins))
/** \} */

/** \name Digital I/O interrupt control and test macros.

    Each pin of ports 1 and 2 have individual interrupt flag, enable, and edge
    select bits.  These feed into two ISRs, one for port 1 and one for port 2.
    The individual flags must be looked at within the ISRs to determine which
    pin caused the interrupt.

    \{
*/
/***************************************************************************//**
    \brief Enable the interrupt for a GPIO pin.

    Only valid for ports 1 and 2.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_INT_ENABLE(port, pin)          GPIO_INT_ENABLE_PREP(port, (pin))

/***************************************************************************//**
    \brief Disable the interrupt for a GPIO pin.

    Only valid for ports 1 and 2.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_INT_DISABLE(port, pin)         GPIO_INT_DISABLE_PREP(port, (pin))

/***************************************************************************//**
    \brief Clear the interrupt flag for a GPIO pin.

    Only valid for ports 1 and 2.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask
*//****************************************************************************/
#define GPIO_INT_CLEAR(port, pin)           GPIO_INT_CLEAR_PREP(port, (pin))

/***************************************************************************//**
    \brief Check ifi the interrupt flag for a GPIO pin is set.

    Only valid for ports 1 and 2.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask

    \return True if flag is set, false otherwise.
*//****************************************************************************/
#define GPIO_INT_IS_FLAG_SET(port, pin)     GPIO_INT_IS_FLAG_SET_PREP(port, (pin))

/***************************************************************************//**
    \brief Check if the interrupt for a GPIO pin is enabled.

    Only valid for ports 1 and 2.
    Should only be used with the port and pin definitions in Board.h.

    \param[in] port - Port number of pin
    \param[in] pin  - Pin mask

    \return True if the interrupt is enabled, false otherwise.
*//****************************************************************************/
#define GPIO_INT_IS_ENABLED(port, pin)      GPIO_INT_IS_ENABLED_PREP(port, (pin))
/** \} */

/*-----------------------INTERNAL MACROS--------------------------------------*/
/* (The macros above need a second round in the preprocessor) */

/** \name Internal GPIO value control and test macros.
    \{
*/
#define GPIO_SET_PREP(port, pin)                    (P##port##OUT = P##port##OUT | ((uint8_t)  (pin)))
#define GPIO_CLEAR_PREP(port, pin)                  (P##port##OUT = P##port##OUT & ((uint8_t) ~(pin)))
#define GPIO_TOGGLE_PREP(port, pin)                 (P##port##OUT = P##port##OUT ^ ((uint8_t)  (pin)))
#define GPIO_IS_HIGH_PREP(port, pin)                ((((uint16_t) P##port##IN) & (pin)) > 0u)
#define GPIO_IS_LOW_PREP(port, pin)                 ((((uint16_t) P##port##IN) & (pin)) == 0u)
#define GPIO_PORT_GET_PREP(port, pins)              (((uint16_t) P##port##IN) & (pins))
/** \} */


/** \name Internal GPIO interrupt control and test macros.
    \{
*/
#define GPIO_INT_ENABLE_PREP(port, pin)             (P##port##IE  = P##port##IE  | ((uint8_t)  (pin)))
#define GPIO_INT_DISABLE_PREP(port, pin)            (P##port##IE  = P##port##IE  & ((uint8_t) ~(pin)))
#define GPIO_INT_CLEAR_PREP(port, pin)              (P##port##IFG = P##port##IFG & ((uint8_t) ~(pin)))

#define GPIO_INT_IS_FLAG_SET_PREP(port, pin)        ((((uint16_t) P##port##IFG) & (pin)) > 0u)
#define GPIO_INT_IS_ENABLED_PREP(port, pin)         ((((uint16_t) P##port##IE) & (pin)) > 0u)
/** \} */

/*lint +esym(960, 19.12) */
#pragma RESET_MISRA("all")
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Gpio_Init(void);

void Gpio_Configure(
    int16_t port,
    uint8_t pinMask,
    GpioType_t type,
    GpioValue_t value,
    GpioDriveStrength_t driveStrength);

void Gpio_ConfigurePortJ(
    uint8_t pinMask,
    GpioType_t type,
    GpioValue_t value,
    GpioDriveStrength_t driveStrength);

void Gpio_ConfigurePortU(
	    uint8_t pinMask,
	    GpioType_t type,
	    GpioValue_t value);

void Gpio_EnableResistor(
    int16_t port,
    uint8_t pinMask,
    GpioResistorType_t resistorType);

void Gpio_DisableResistor(
    int16_t port,
    uint8_t pinMask);

void Gpio_Set(
    int16_t port,
    uint8_t pinMask);

void Gpio_SetPortU(
    uint8_t pinMask);

void Gpio_SetPortJ(
    uint8_t pinMask);

void Gpio_Clear(
    int16_t port,
    uint8_t pinMask);

void Gpio_ClearPortU(
    uint8_t pinMask);

void Gpio_ClearPortJ(
    uint8_t pinMask);

bool Gpio_GetValue(
    int16_t port,
    uint8_t pinMask);

bool Gpio_GetValuePortU(
    uint8_t pinMask);

bool Gpio_GetValuePortJ(
    uint8_t pinMask);

void Gpio_RegisterIsr(
    int16_t port,
    uint8_t pinMask,
    GpioIsrPointer_t isr,
    GpioEdge_t edge);

void Gpio_UnregisterIsr(
        int16_t port,
        uint8_t pinMask);

void Gpio_SelectInterruptEdge(
    int16_t port,
    uint8_t pinMask,
    GpioEdge_t edge);

GpioEdge_t Gpio_GetInterruptEdge(
    int16_t port,
    uint8_t pinMask);

#endif /* GPIO_H_ */
