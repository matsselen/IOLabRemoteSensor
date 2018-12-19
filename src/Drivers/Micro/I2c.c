/***************************************************************************//**
    \file I2c.c
    \brief I2C Module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions to configure an I2C port to
       communicate to various peripheral devices.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/I2c.c$
    $Revision: 3$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "I2c.h"

#include <Assert.h>
#include <Device.h>
#include <Gpio.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Number of possible I2C ports */
#define I2C_NUM_PORTS               (2)

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Look-up table for I2C control 1 registers. */
static volatile uint8_t* const _i2cControlOneRegisters[I2C_NUM_PORTS] =
{
    &UCB0CTL1,
    &UCB1CTL1
};

/** Look-up table for I2C transmit buffer registers. */
static volatile uint8_t* const _i2cTransmitBufferRegisters[I2C_NUM_PORTS] =
{
    &UCB0TXBUF,
    &UCB1TXBUF
};

/** Look-up table for I2C receive buffer registers. */
static volatile const uint8_t* const _i2cReceiveBufferRegisters[I2C_NUM_PORTS] =
{
    &UCB0RXBUF,
    &UCB1RXBUF
};

/** Look-up table for I2C interrupt flag registers. */
static volatile uint8_t* const _i2cInterruptFlagRegisters[I2C_NUM_PORTS] =
{
    &UCB0IFG,
    &UCB1IFG
};

/** Look-up table for GPIO port number of I2C SCL ports. */
static const int16_t _i2cGpioSCLPorts[I2C_NUM_PORTS] = {3, 4};

/** Look-up table for GPIO port number of I2C SDA ports. */
static const int16_t _i2cGpioSDAPorts[I2C_NUM_PORTS] = {3, 4};

/** Look-up table for GPIO pinmask of I2C SCL ports. */
static const uint8_t _i2cGpioSCLPinmasks[I2C_NUM_PORTS] =
{
    (uint8_t)(1u << 1),
    (uint8_t)(1u << 2)
};

/** Look-up table for GPIO pinmask of I2C SDA ports. */
static const uint8_t _i2cGpioSDAPinmasks[I2C_NUM_PORTS] =
{
    (uint8_t)(1u << 0),
    (uint8_t)(1u << 1)
};

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Tracks if a I2C port is initialized. */
static bool _i2cPortInitialized[I2C_NUM_PORTS] = {false, false};

/** Tracks if a I2C port is open. */
static bool _i2cPortOpen[I2C_NUM_PORTS] = {false, false};

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
/***************************************************************************//**
    Initialize the I2C hardware.

    \param[in] port - Selection of which I2C hardware port to initialize.
    \param[in] config - Structure of selected I2C HW settings.
*//****************************************************************************/
void I2c_Init(
    I2cPort_t port,
    const I2cPortConfig_t* config)
{
    /** Look-up table for I2C control 0 registers. */
    static volatile uint8_t* const _i2cControlZeroRegisters[I2C_NUM_PORTS] =
    {
        &UCB0CTL0,
        &UCB1CTL0
    };
    /** Look-up table for I2C baud rate control registers. */
    static volatile uint16_t* const _i2cBaudRateRegisters[I2C_NUM_PORTS] =
    {
        &UCB0BRW,
        &UCB1BRW
    };
    /** I2C Port Configuration parameters.  These get configured here */
    static I2cPortConfig_t _i2cPortParameters[I2C_NUM_PORTS] =
    {
        {I2cMode_MASTER, I2cAddressing_7_bits, I2cBaudRate_100000},
        {I2cMode_MASTER, I2cAddressing_7_bits, I2cBaudRate_100000}
    };

    I2cMode_t selectedModeType;
    I2cAddressing_t selectedAddressing;
    I2cBaudRate_t selectedBaudRate;
    volatile uint8_t* controlZeroReg = _i2cControlZeroRegisters[port];
    bool portInitialized = _i2cPortInitialized[port];

    /* See if port has already been initialized by another component */
    if (portInitialized)
    {
        /* This can be OK, if all port parameters are identical */
        ASSERT(_i2cPortParameters[port].addressing == config->addressing, ErrorLedMode_SOFTWARE);
        ASSERT(_i2cPortParameters[port].baudRate == config->baudRate, ErrorLedMode_SOFTWARE);
        ASSERT(_i2cPortParameters[port].modeType == config->modeType, ErrorLedMode_SOFTWARE);
    }
    else
    {
        /* Begin by storing initialization data */
        _i2cPortInitialized[port] = true;
        _i2cPortParameters[port].addressing = config->addressing;
        _i2cPortParameters[port].baudRate = config->baudRate;
        _i2cPortParameters[port].modeType = config->modeType;

        /* Reset module */
        *_i2cControlOneRegisters[port] = UCSWRST;

        /* Configure Control Register Zero fixed values: I2C mode, Synchronous */
        *controlZeroReg = (UCMODE_3 | UCSYNC);

        /* Configure Control Register Zero value: Addressing */
        selectedAddressing = config->addressing;
        switch (selectedAddressing)
        {
            case I2cAddressing_7_bits:
                /* 7-bit addressing for both "Own" and "Slave" devices. */
                /* Do Nothing */
                break;

            case I2cAddressing_10_bits:
                /* 10-bit addressing for both "Own" and "Slave" devices. */
                *controlZeroReg |= (UCA10 | UCSLA10);
                break;

            default:
                ASSERT(false, ErrorLedMode_SOFTWARE);
                break;
        }

        /* Configure Control Register Zero value: Mode */
        selectedModeType = config->modeType;
        switch (selectedModeType)
        {
            case I2cMode_SLAVE:
                /* Set up in Slave mode. */
                /* Do Nothing */
                break;

            case I2cMode_MASTER:
                /* Set up in Master mode. */
                *controlZeroReg |= UCMST;
                break;

            default:
                ASSERT(false, ErrorLedMode_SOFTWARE);
                break;
        }

        /* Select ACLK as clock source,
           keep module in reset

           ACLK = 12 MHz
        */
        *_i2cControlOneRegisters[port] = UCSSEL__ACLK | UCSWRST;

        /* Configure value: Baud Rate */
        selectedBaudRate = config->baudRate;
        switch (selectedBaudRate)
        {
            case I2cBaudRate_400000:
                /* 12MHz / 120 = 400,000     120 = 0x78 */
                *_i2cBaudRateRegisters[port] = (uint16_t) 0x0078u;
                break;

            case I2cBaudRate_100000:
                /* 12MHz / 30 = 100,000     30 = 0x1E */
                *_i2cBaudRateRegisters[port] = (uint16_t) 0x001Eu;
                break;

            default:
                ASSERT(false, ErrorLedMode_SOFTWARE);
                break;
        }
    }
}

/***************************************************************************//**
    Open the I2C port.

    \param[in] port - Selected I2C port.
*//****************************************************************************/
void I2c_Open(I2cPort_t port)
{
    bool portOpen = _i2cPortOpen[port];
    bool portInitialized = _i2cPortInitialized[port];
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];

    ASSERT(portInitialized, ErrorLedMode_SOFTWARE);
    ASSERT(portOpen == false, ErrorLedMode_SOFTWARE);

    /* Reset module */
    *controlReg = *controlReg | UCSWRST;

    /* Configure pins to be used by peripheral */
    Gpio_Configure(_i2cGpioSCLPorts[port], _i2cGpioSCLPinmasks[port],
                   GpioType_PERIPHERAL_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(_i2cGpioSDAPorts[port], _i2cGpioSDAPinmasks[port],
                   GpioType_PERIPHERAL_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);

    /* Release module from reset */
    *controlReg = *controlReg & ((uint8_t) ~(UCSWRST));

    _i2cPortOpen[port] = true;
}

/***************************************************************************//**
    Close the I2C port.

    \param[in] port - Selected I2C port.
*//****************************************************************************/
void I2c_Close(I2cPort_t port)
{
    bool portOpen = _i2cPortOpen[port];
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];

    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* Reset module */
    *controlReg = *controlReg | UCSWRST;

    /* Configure pins as generic digital I/O */
    Gpio_Configure(_i2cGpioSCLPorts[port], _i2cGpioSCLPinmasks[port],
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(_i2cGpioSDAPorts[port], _i2cGpioSDAPinmasks[port],
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    _i2cPortOpen[port] = false;
}

/***************************************************************************//**
    Transmit the Start Bit on the I2C port, immediately followed by sending
    the selected Slave Address out to the I2C port.

    This assumes that the I2C port has been configured as a MASTER device.
    Slave devices do not send out the Start bit.

    \param[in] port - Selected I2C port.
    \param[in] address - Slave address to communicate with.
    \param[in] operation - Select I2C Write or I2C Read.

    \return True if the Start Bit and address were sent OK.  Otherwise, false.
*//****************************************************************************/
I2cError_t I2c_TransmitStart(
    I2cPort_t port,
    uint8_t address,
    I2cOperation_t operation)
{
    /** Look-up table for I2C slave address registers. */
    static volatile uint16_t* const _i2cSlaveAddressRegisters[I2C_NUM_PORTS] =
    {
        &UCB0I2CSA,
        &UCB1I2CSA
    };

    I2cError_t error = I2cError_OK;
    bool portOpen = _i2cPortOpen[port];
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];
    volatile uint8_t* flagReg = _i2cInterruptFlagRegisters[port];

    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* Set slave address */
    *_i2cSlaveAddressRegisters[port] = address;

    /* Send start bit. The slave address byte will also be sent. */
    if (operation == I2cOperation_WRITE)
    {
        /* Set up bits to Write to slave device */
        *controlReg = *controlReg | (UCTR | UCTXSTT);
    }
    else
    {
        /* Set up bits to Read from slave device */
        *controlReg = *controlReg & ((uint8_t) ~(UCTR));
        *controlReg = *controlReg | UCTXSTT;

        /* Wait for start bit and slave address transmission to finish. */
        volatile uint16_t limitCounter = 0;
        while ( (*controlReg & UCTXSTT) != 0u)
        {
            /* Do Nothing */
        	limitCounter++;
        	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
        }

        /* If address was NACK'd, reset I2C port and return error */
        if ( (*flagReg & UCNACKIFG) != 0u)
        {
            *controlReg = *controlReg | UCTXSTP;
            *flagReg = *flagReg & ((uint8_t) ~(UCNACKIFG));
            error = I2cError_NACK;
        }
    }

    return error;
}

/***************************************************************************//**
    Transmits a byte via I2C.
    This is used to send multiple bytes on I2C, as the Stop Bit is NOT sent.

    This assumes that the I2C port has been configured as a MASTER device.

    \param[in] port - Selected I2C port.
    \param[in] byte - Data byte to send out over I2C.

    \return True if the byte was sent OK.  Otherwise, false.
*//****************************************************************************/
I2cError_t I2c_TransmitByte(
    I2cPort_t port,
    uint8_t byte)
{
    I2cError_t error = I2cError_OK;
    bool portOpen = _i2cPortOpen[port];
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];
    volatile uint8_t* flagReg = _i2cInterruptFlagRegisters[port];

    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* Send byte */
    *_i2cTransmitBufferRegisters[port] = byte;

    /* Wait until send is complete or byte is NACK'd */
    volatile uint16_t limitCounter = 0;
    while ( (*flagReg & (UCTXIFG | UCNACKIFG)) == 0u)
    {
        /* Do Nothing */
    	limitCounter++;
    	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
    }

    /* If byte was NACK'd, must clean up I2C port by sending STOP */
    if ( (*flagReg & UCNACKIFG) != 0u)
    {
        *controlReg = *controlReg | UCTXSTP;
        *flagReg = *flagReg & ((uint8_t) ~(UCNACKIFG));
        error = I2cError_NACK;
    }

    return error;
}

/***************************************************************************//**
    Transmits a byte via I2C, then send the I2C Stop bit.  This is required
    to end the transaction with the slave device.

    This assumes that the I2C port has been configured as a MASTER device.

    \param[in] port - Selected I2C port.
    \param[in] byte - Data byte to send out over I2C.

    \return True if the byte and Stop Bit was sent OK.  Otherwise, false.
*//****************************************************************************/
I2cError_t I2c_TransmitByteEnd(
    I2cPort_t port,
    uint8_t byte)
{
    I2cError_t error = I2cError_OK;
    bool portOpen = _i2cPortOpen[port];
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];
    volatile uint8_t* flagReg = _i2cInterruptFlagRegisters[port];

    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* Send byte */
    *_i2cTransmitBufferRegisters[port] = byte;

    /* Wait until send is complete or byte is NACK'd */
    volatile uint16_t limitCounter = 0;
    while ( (*flagReg & (UCTXIFG | UCNACKIFG)) == 0u)
    {
        /* Do Nothing */
    	limitCounter++;
    	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
    }

    /* If byte was NACK'd, must clean up I2C port by sending STOP */
    if ( (*flagReg & UCNACKIFG) != 0u)
    {
        *controlReg = *controlReg | UCTXSTP;
        *flagReg = *flagReg & ((uint8_t) ~(UCNACKIFG));
        error = I2cError_NACK;
    }
    else
    {
        /* Byte was sent successfully */

        /* Now send STOP bit */
        *controlReg = *controlReg | UCTXSTP;

        /* Wait for STOP bit to be sent */
        limitCounter = 0;
        while ( (*controlReg & UCTXSTP) != 0u)
        {
            /* Do Nothing */
        	limitCounter++;
        	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
        }

        /* Once again, if STOP was NACK'd, must clean up logic */
        if ( (*flagReg & UCNACKIFG) != 0u)
        {
            *flagReg = *flagReg & ((uint8_t) ~(UCNACKIFG));
            error = I2cError_STOP;
        }
    }

    return error;
}

/***************************************************************************//**
    Receives a byte via I2C.

    This assumes that the I2C port has been configured as a MASTER device.

    \param[in] port - Selected I2C port.
    \param[in] byte - Pointer to where received byte should be stored.
*//****************************************************************************/
void I2c_ReceiveByte(
    I2cPort_t port,
    uint8_t* byte)
{
    bool portOpen = _i2cPortOpen[port];
    volatile uint8_t* flagReg = _i2cInterruptFlagRegisters[port];

    ASSERT(byte != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* Wait until byte is received */
    volatile uint16_t limitCounter = 0;
    while ( (*flagReg & UCRXIFG) == 0u)
    {
        /* Do Nothing */
    	limitCounter++;
    	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
    }

    /* Read byte. */
    *byte = *_i2cReceiveBufferRegisters[port];
}

/***************************************************************************//**
    Receives a byte via I2C, then send the I2C Stop bit.  This is required
    to end the transaction with the slave device.

    This assumes that the I2C port has been configured as a MASTER device.

    \param[in] port - Selected I2C port.
    \param[in] byte - Pointer to where received byte should be stored.
*//****************************************************************************/
void I2c_ReceiveByteEnd(
    I2cPort_t port,
    uint8_t* byte)
{
    bool portOpen = _i2cPortOpen[port];
    bool stopHasBeenSent = false;
    volatile uint8_t* controlReg = _i2cControlOneRegisters[port];
    volatile uint8_t* flagReg = _i2cInterruptFlagRegisters[port];
    volatile uint8_t junk;

    ASSERT(byte != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(portOpen, ErrorLedMode_SOFTWARE);

    /* See if byte is being received */
    if ( (*flagReg & UCRXIFG) == 0u)
    {
        /* Reading in byte, so send STOP bit now */
        *controlReg = *controlReg | UCTXSTP;
        stopHasBeenSent = true;
    }

    /* Wait until byte is received */
    volatile uint16_t limitCounter = 0;
    while ( (*flagReg & UCRXIFG) == 0u)
    {
        /* Do Nothing */
    	limitCounter++;
    	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
    }

    /* Read byte. */
    *byte = *_i2cReceiveBufferRegisters[port];

    if (stopHasBeenSent == false)
    {
        /* Now send STOP bit */
        *controlReg = *controlReg | UCTXSTP;

        /* Wait for STOP bit to be sent */
        limitCounter = 0;
        while ( (*controlReg & UCTXSTP) != 0u)
        {
            /* Do Nothing */
        	limitCounter++;
        	ASSERT(limitCounter < UINT16_T_MAX, ErrorLedMode_SOFTWARE);
        }

        /* Read byte received when issuing stop bit to clear receive flag. */
        junk = *_i2cReceiveBufferRegisters[port];
    }
}

/***************************************************************************//**
    Delays until the I2C bus becomes idle.  This is useful to ensure that
    the I2C bus is available between rapid bus accesses.

    \param[in] port - Selected I2C port.
*//****************************************************************************/
void I2c_WaitUntilBusIdle(I2cPort_t port)
{
    /** Look-up table for I2C status registers. */
    static volatile uint8_t* const _i2cSlaveStatusRegisters[I2C_NUM_PORTS] =
    {
        &UCB0STAT,
        &UCB1STAT
    };

    volatile uint8_t* statusReg = _i2cSlaveStatusRegisters[port];

    /* Wait for BUSY bit to be cleared */
    volatile uint32_t limitCounter = 0;
    while ( (*statusReg & UCBBUSY) != 0u)
    {
        /* Do Nothing */
    	limitCounter++;
    	ASSERT(limitCounter < 1000000, ErrorLedMode_SOFTWARE);
    }
}

