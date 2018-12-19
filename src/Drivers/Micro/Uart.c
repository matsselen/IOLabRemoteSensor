/***************************************************************************//**
    \file Uart.c
    \brief UART driver module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions and macros for configuring the
       UART and transmitting and receiving data over that port.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Uart.c$
    $Revision: 7$
    $Date: Friday, October 24, 2014 10:53:12 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Uart.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

#include <Device.h>
#include <Gpio.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Number of possible UART ports */
#define UART_NUM_PORTS              (2)

/** Maximum length of a string that can be sent during one UART function call.
    Prevents infinite loops from non-null-terminated strings.
*/
#define UART_MAX_STRING_LENGTH      (50u)

/** Number of bytes that can be stored in the UART transmit buffer.
    Must be a power of 2.
*/
#define UART_TX_BUFFER_SIZE         (512u)
/** Number of bytes that can be stored in the UART receive buffer.
    Must be a power of 2.
*/
#define UART_RX_BUFFER_SIZE         (128u)

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Look-up table for UART control 1 registers. */
static volatile uint8_t* const _uartControlOneRegisters[UART_NUM_PORTS] =
{
    &UCA0CTL1,
    &UCA1CTL1
};
/** Look-up table for UART transmit buffer registers. */
static volatile uint8_t* const _uartTransmitBufferRegisters[UART_NUM_PORTS] =
{
    &UCA0TXBUF,
    &UCA1TXBUF
};
/** Look-up table for UART receive buffer registers */
static volatile const uint8_t* const _uartReceiveBufferRegisters[UART_NUM_PORTS] =
{
    &UCA0RXBUF,
    &UCA1RXBUF
};
/** Look-up table for UART interrupt enable registers. */
static volatile uint8_t* const _uartInterruptEnableRegisters[UART_NUM_PORTS] =
{
    &UCA0IE,
    &UCA1IE
};
/** Look-up table for UART interrupt flag registers. */
static volatile const uint8_t* const _uartInterruptFlagRegisters[UART_NUM_PORTS] =
{
    &UCA0IFG,
    &UCA1IFG
};
#if 0 /* Not used */
/** Look-up table for UART status registers. */
static volatile const uint8_t* const _uartStatusRegisters[UART_NUM_PORTS] =
{
    &UCA0STAT,
    &UCA1STAT
};
#endif

/** Look-up table for GPIO port number of UART ports. */
static const int16_t _uartGpioPorts[UART_NUM_PORTS] = {3, 4};
/** Look-up table for GPIO pinmask of transmit line for UART ports. */
static const uint8_t _uartGpioTransmitPinmasks[UART_NUM_PORTS] =
{
    (uint8_t)(1u << 3),
    (uint8_t)(1u << 4)
};
/** Look-up table for GPIO pinmask of receive line for UART ports. */
static const uint8_t _uartGpioReceivePinmasks[UART_NUM_PORTS] =
{
    (uint8_t)(1u << 4),
    (uint8_t)(1u << 5)
};

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Tracks if a UART port is open. */
static bool _uartPortIsOpen[UART_NUM_PORTS] = {false, false};

/** Stores the bytes to transmit over the UART for each port. */
static uint8_t _uartBufferTx[UART_NUM_PORTS][UART_TX_BUFFER_SIZE];
/** Stores the bytes received over the UART for each port. */
static uint8_t _uartBufferRx[UART_NUM_PORTS][UART_RX_BUFFER_SIZE];

/** Tracks the head of the transmit buffer. */
static volatile uint16_t _uartBufferTxHead[UART_NUM_PORTS] = {0u, 0u};
/** Tracks the tail of the transmit buffer. */
static volatile uint16_t _uartBufferTxTail[UART_NUM_PORTS] = {0u, 0u};
/** Tracks the head of the receive buffer. */
static volatile uint16_t _uartBufferRxHead[UART_NUM_PORTS] = {0u, 0u};
/** Tracks the tail of the receive buffer. */
static volatile uint16_t _uartBufferRxTail[UART_NUM_PORTS] = {0u, 0u};
/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _HandleUartTransmitInterrupt(UartPort_t port);
static void _HandleUartReceiveInterrupt(UartPort_t port);
#ifdef UART_ENABLE_PORT_0
__interrupt void _UartA0Isr(void);
#endif
#ifdef UART_ENABLE_PORT_1
__interrupt void _UartA1Isr(void);
#endif

/***************************************************************************//**
    Initialize the selected UART module.

    \param[in] port - Selected UART port.
    \param[in] config - Pointer to the configuration information for the port.
*//****************************************************************************/
void Uart_Init(
    UartPort_t port,
    const UartPortConfig_t* config)
{
    /** Look-up table for UART control 0 registers. */
    static volatile uint8_t* const _uartControlZeroRegisters[UART_NUM_PORTS] =
    {
        &UCA0CTL0,
        &UCA1CTL0
    };
    /** Look-up table for UART baud rate control registers. */
    static volatile uint16_t* const _uartBaudRateRegisters[UART_NUM_PORTS] =
    {
        &UCA0BRW,
        &UCA1BRW
    };
    /** Look-up table for UART modulation control registers. */
    static volatile uint8_t* const _uartModulationRegisters[UART_NUM_PORTS] =
    {
        &UCA0MCTL,
        &UCA1MCTL
    };
    /** Look-up table for UART auto baud rate control registers. */
    static volatile uint8_t* const _uartAutoBaudRateRegisters[UART_NUM_PORTS] =
    {
        &UCA0ABCTL,
        &UCA1ABCTL
    };
    /** Look-up table for UART IrDA transmit control registers. */
    static volatile uint8_t* const _uartIrdaTransmitRegisters[UART_NUM_PORTS] =
    {
        &UCA0IRTCTL,
        &UCA1IRTCTL
    };
    /** Look-up table for UART IrDA receive control registers. */
    static volatile uint8_t* const _uartIrdaReceiveRegisters[UART_NUM_PORTS] =
    {
        &UCA0IRRCTL,
        &UCA1IRRCTL
    };

    UartBaudRate_t selectedBaudRate;
    UartParity_t selectedParity;
    UartEndieness_t selectedEndieness;
    UartStopBit_t selectedStopBit;

    /* Reset module */
    *_uartControlOneRegisters[port] = UCSWRST;

    /* 8-bit data, UART mode, asynchronous mode*/
    *_uartControlZeroRegisters[port] = 0u;

    selectedParity = config->parity;
    switch (selectedParity)
    {
        case UartParity_EVEN:
            /* Enable parity, Even parity, */
            *_uartControlZeroRegisters[port] |= UCPEN | UCPAR;
            break;

        case UartParity_ODD:
        	/* Enable parity, Even parity, 8-bit data, UART mode, asynchronous mode*/
            *_uartControlZeroRegisters[port] |= UCPEN;
            *_uartControlZeroRegisters[port] &= ~UCPAR;
            break;

        case UartParity_NONE:
        	/* Enable parity, Even parity, 8-bit data, UART mode, asynchronous mode*/
            *_uartControlZeroRegisters[port] &= ~UCPEN;
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    selectedEndieness = config->endieness;
    switch(selectedEndieness)
    {
        case UartEndieness_LsbFirst:
            /* Set LSB first */
            *_uartControlZeroRegisters[port] &= ~UCMSB;
            break;

        case UartEndieness_MsbFirst:
            /* set MSB first */
            *_uartControlZeroRegisters[port] |= UCMSB;
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    selectedStopBit = config->stopBits;
    switch(selectedStopBit)
    {
        case UartStopBit_ONE:
            *_uartControlZeroRegisters[port] &= ~UCSPB;
            break;

        case UartStopBit_TWO:
            *_uartControlZeroRegisters[port] |= UCSPB;
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }

    /* Select ACLK as clock source,
       disable Rx error and Rx break interrupts,
       USCI module is not dormant,
       next frame is normal data,
       keep module in reset
    */
    *_uartControlOneRegisters[port] = UCSSEL__ACLK | UCSWRST;

    /* Disable auto baud rate detection. */
    *_uartAutoBaudRateRegisters[port] = 0u;

    if (config->enableIrda)
    {
        /* Disable receive filter
           Polarity set such that a low pulse indicates a light pulse was seen
        */
        *_uartIrdaReceiveRegisters[port] = UCIRRXPL;

        /* Set transmit pulse length to 3/16 of bit time
           Use BITCLK16
           Enable IrDA encoder/decoder
        */
        *_uartIrdaTransmitRegisters[port] = (5u * UCIRTXPL0) | UCIRTXCLK | UCIREN;
    }
    else
    {
        *_uartIrdaReceiveRegisters[port] = 0u;
        *_uartIrdaTransmitRegisters[port] = 0u;
    }

    /* Set baud rate (clock source = ACLK = 10 MHz).
       In oversampling mode (UCOS16 = 1), baud rate regs (UCBRx) = INT(N/16)
       and first stage modulator (UCBRFx) = round(((N/16) - INT(N/16)) * 16)
       where N = (ACLK / baud rate).
    */
    selectedBaudRate = config->baudRate;
    switch (selectedBaudRate)
    {
        case UartBaudRate_230400:
            *_uartBaudRateRegisters[port] = 3u;
            *_uartModulationRegisters[port] = (4u * UCBRF0) | UCOS16;
            break;

        case UartBaudRate_115200:
            *_uartBaudRateRegisters[port] = 6u;
            *_uartModulationRegisters[port] = (8u * UCBRF0) | UCOS16;
            break;

        case UartBaudRate_57600:
            *_uartBaudRateRegisters[port] = 13u;
            *_uartModulationRegisters[port] = (0u * UCBRF0) | UCOS16;
            break;

        case UartBaudRate_38400:
            *_uartBaudRateRegisters[port] = 19u;
            *_uartModulationRegisters[port] = (9u * UCBRF0) | UCOS16;
            break;

        case UartBaudRate_19200:
            *_uartBaudRateRegisters[port] = 39u;
            *_uartModulationRegisters[port] = (1u * UCBRF0) | UCOS16;
            break;

        case UartBaudRate_9600:
            *_uartBaudRateRegisters[port] = 78u;
            *_uartModulationRegisters[port] = (2u * UCBRF0) | UCOS16;
            break;

        default:
            ASSERT(false, ErrorLedMode_SOFTWARE);
            break;
    }
}

/***************************************************************************//**
    Open the UART port. You should not open the same UART port more than once.

    \param[in] port - Selected UART port.
*//****************************************************************************/
void Uart_Open(UartPort_t port)
{
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* controlReg = _uartControlOneRegisters[port];

    ASSERT(portIsOpen == false, ErrorLedMode_SOFTWARE);

    /* Reset module */
    *controlReg = *controlReg | UCSWRST;

    /* Configure pins to be used by peripheral */
    Gpio_Configure(_uartGpioPorts[port], _uartGpioTransmitPinmasks[port],
                   GpioType_PERIPHERAL_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(_uartGpioPorts[port], _uartGpioReceivePinmasks[port],
                   GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* Enable a pull-up resister on the receive pin to prevent a floating input
       from causing invalid bytes to be received.
    */
    Gpio_EnableResistor(_uartGpioPorts[port], _uartGpioReceivePinmasks[port], GpioResistorType_PULLUP);

    /* Flush transmit and receive buffers */
    _uartBufferTxHead[port] = _uartBufferTxTail[port];
    _uartBufferRxTail[port] = _uartBufferRxHead[port];

    /* Release module from reset */
    *controlReg = *controlReg & ((uint8_t) ~(UCSWRST));

    _uartPortIsOpen[port] = true;
}
/***************************************************************************//**
    Close the UART port.

    \param[in] port - Selected UART port.
*//****************************************************************************/
void Uart_Close(UartPort_t port)
{
    volatile uint8_t* controlReg = _uartControlOneRegisters[port];

    /* Reset module */
    *controlReg = *controlReg | UCSWRST;

    /* Configure pins as generic digital I/O */
    Gpio_Configure(_uartGpioPorts[port], _uartGpioTransmitPinmasks[port],
                   GpioType_OUTPUT, GpioValue_HIGH, GpioDriveStrength_DEFAULT);
    Gpio_Configure(_uartGpioPorts[port], _uartGpioReceivePinmasks[port],
                   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_DisableResistor(_uartGpioPorts[port], _uartGpioReceivePinmasks[port]);

    _uartPortIsOpen[port] = false;
}

/***************************************************************************//**
    Determines if a UART port is open.

    \param[in] port - Selected UART port.

    \return True if the port is open.  Otherwise, false.
*//****************************************************************************/
bool Uart_IsPortOpen(UartPort_t port)
{
    return _uartPortIsOpen[port];
}

/***************************************************************************//**
    Enable the receive interrupt for the UART port.

    \param[in] port - Selected UART port.

    \note Port must be open before enabling the receives.
*//****************************************************************************/
void Uart_EnableReceive(UartPort_t port)
{
    bool portIsOpen = _uartPortIsOpen[port];
    volatile const uint8_t* rxBufReg = _uartReceiveBufferRegisters[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile uint8_t junkByte;

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Dummy read of receive register to flush any extra bytes received
       previously and to clear receive interrupt.
    */
    junkByte = *rxBufReg;

    *intEnableReg = *intEnableReg | UCRXIE;
}

/***************************************************************************//**
    Disable the receive interrupt for the UART port.

    \param[in] port - Selected UART port.

    \note Port must be open before disabling the receives.
*//****************************************************************************/
void Uart_DisableReceive(UartPort_t port)
{
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    *intEnableReg = *intEnableReg & ((uint8_t) ~(UCRXIE));
}

/***************************************************************************//**
    Transmit a byte via the UART.  This function blocks until the byte
    has been written to the UART.

    \param[in] port - Selected UART port.
    \param[in] byte - Byte to transmit.
*//****************************************************************************/
void Uart_WriteByteBlocking(
    UartPort_t port,
    uint8_t byte)
{
    bool portIsOpen = _uartPortIsOpen[port];

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    Uart_WriteByteBlockingNoAssert(port, byte);
}

/***************************************************************************//**
    Transmit a byte via the UART.  This function blocks until the byte
    has been written to the UART.

    \note No asserts are used.

    \param[in] port - Selected UART port.
    \param[in] byte - Byte to transmit.
*//****************************************************************************/
void Uart_WriteByteBlockingNoAssert(
    UartPort_t port,
    uint8_t byte)
{
    bool txIntWasEnabled;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile const uint8_t* intFlagReg = _uartInterruptFlagRegisters[port];

    if (portIsOpen)
    {
        /* Save transmit interrupt state */
        if ((*intEnableReg & UCTXIE) > 0u)
        {
            txIntWasEnabled = true;
        }
        else
        {
            txIntWasEnabled = false;
        }

        /* Disable transmit interrupt */
        *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));

        /* Wait for TX buffer to be ready to receive new byte */
        while ((*intFlagReg & UCTXIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Write character */
        *_uartTransmitBufferRegisters[port] = byte;

        /* Restore transmit interrupt. */
        if (txIntWasEnabled)
        {
            *intEnableReg = *intEnableReg | UCTXIE;
        }
    }
}

/***************************************************************************//**
    Transmit a byte array via the UART.  This function blocks until the entire
    buffer has been written to the UART.

    \param[in] port - Selected UART port.
    \param[in] numToWrite - Number of bytes to transmit.
    \param[in] source - Pointer to byte array to transmit.
*//****************************************************************************/
void Uart_WriteByteArrayBlocking(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[])
{
    bool portIsOpen = _uartPortIsOpen[port];

    ASSERT((source != NULL) && (numToWrite > 0u), ErrorLedMode_SOFTWARE);
    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    Uart_WriteByteArrayBlockingNoAssert(port, numToWrite, source);
}

/***************************************************************************//**
    Transmit a byte array via the UART.  This function blocks until the entire
    buffer has been written to the UART.

    \note No asserts are used.

    \param[in] port - Selected UART port.
    \param[in] numToWrite - Number of bytes to transmit.
    \param[in] source - Pointer to byte array to transmit.
*//****************************************************************************/
void Uart_WriteByteArrayBlockingNoAssert(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[])
{
    uint16_t i;
    bool txIntWasEnabled;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile const uint8_t* intFlagReg = _uartInterruptFlagRegisters[port];

    if ((source != NULL) && (numToWrite > 0u) && (portIsOpen))
    {
        /* Save transmit interrupt state */
        if ((*intEnableReg & UCTXIE) > 0u)
        {
            txIntWasEnabled = true;
        }
        else
        {
            txIntWasEnabled = false;
        }

        /* Disable transmit interrupt */
        *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));

        for (i = 0u; i < numToWrite; ++i)
        {
            /* Wait for TX buffer to be ready to receive new byte */
            while ((*intFlagReg & UCTXIFG) == 0u)
            {
                /* Do nothing */
            }

            /* Write character */
            *_uartTransmitBufferRegisters[port] = source[i];
        }

        /* Restore transmit interrupt. */
        if (txIntWasEnabled)
        {
            *intEnableReg = *intEnableReg | UCTXIE;
        }
    }
}

/***************************************************************************//**
    Transmit a byte via the UART as a hex string.  This function blocks until
    the entire hex string has been written to the UART.

    \param[in] port - Selected UART port.
    \param[in] byte - Byte to transmit as a hex string.
*//****************************************************************************/
void Uart_WriteByteHexBlocking(
    UartPort_t port,
    uint8_t byte)
{
    uint8_t nibble;
    bool txIntWasEnabled;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile const uint8_t* intFlagReg = _uartInterruptFlagRegisters[port];

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Save transmit interrupt state */
    if ((*intEnableReg & UCTXIE) > 0u)
    {
        txIntWasEnabled = true;
    }
    else
    {
        txIntWasEnabled = false;
    }

    /* Disable transmit interrupt */
    *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));

    /* Wait for TX buffer ready to receive new byte */
    while ((*intFlagReg & UCTXIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Write high nibble of byte */
    nibble = (byte >> 4u) & 0x0Fu;
    if (nibble > 9u)
    {
        *_uartTransmitBufferRegisters[port] = nibble + 0x37u;
    }
    else
    {
        *_uartTransmitBufferRegisters[port] = nibble + 0x30u;
    }

    /* Wait for TX buffer ready to receive new byte */
    while ((*intFlagReg & UCTXIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Write low nibble of byte */
    nibble = byte & 0x0Fu;
    if (nibble > 9u)
    {
        *_uartTransmitBufferRegisters[port] = nibble + 0x37u;
    }
    else
    {
        *_uartTransmitBufferRegisters[port] = nibble + 0x30u;
    }

    /* Restore transmit interrupt. */
    if (txIntWasEnabled)
    {
        *intEnableReg = *intEnableReg | UCTXIE;
    }
}

/***************************************************************************//**
    Transmit a byte array via the UART as a hex string.  A hyphen is inserted
    between each byte.  This function blocks until the entire hex string has
    been written to the UART.

    \param[in] port - Selected UART port.
    \param[in] numToWrite - Number of bytes to transmit.
    \param[in] source - Pointer to byte array to transmit as a hex string
*//****************************************************************************/
void Uart_WriteByteArrayHexBlocking(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[])
{
    uint16_t i;
    uint8_t nibble;
    bool txIntWasEnabled;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile const uint8_t* intFlagReg = _uartInterruptFlagRegisters[port];

    ASSERT((source != NULL) && (numToWrite > 0u), ErrorLedMode_SOFTWARE);
    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Save transmit interrupt state */
    if ((*intEnableReg & UCTXIE) > 0u)
    {
        txIntWasEnabled = true;
    }
    else
    {
        txIntWasEnabled = false;
    }

    /* Disable transmit interrupt */
    *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));

    for (i = 0u; i < numToWrite; ++i)
    {
        /* Wait for TX buffer ready to receive new byte */
        while ((*intFlagReg & UCTXIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Write high nibble of byte */
        nibble = source[i];
        nibble = (nibble >> 4u) & 0x0Fu;
        if (nibble > 9u)
        {
            *_uartTransmitBufferRegisters[port] = nibble + 0x37u;
        }
        else
        {
            *_uartTransmitBufferRegisters[port] = nibble + 0x30u;
        }

        /* Wait for TX buffer ready to receive new byte */
        while ((*intFlagReg & UCTXIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Write low nibble of byte */
        nibble = source[i];
        nibble = nibble & 0x0Fu;
        if (nibble > 9u)
        {
            *_uartTransmitBufferRegisters[port] = nibble + 0x37u;
        }
        else
        {
            *_uartTransmitBufferRegisters[port] = nibble + 0x30u;
        }

        /* Don't write separation character after last byte */
        if (i < (numToWrite - 1u))
        {
            /* Wait for TX buffer ready to receive new byte */
            while ((*intFlagReg & UCTXIFG) == 0u)
            {
                /* Do nothing */
            }

            /* Write a hyphen character. */
            *_uartTransmitBufferRegisters[port] = 0x2Du;
        }
    }

    /* Restore transmit interrupt. */
    if (txIntWasEnabled)
    {
        *intEnableReg = *intEnableReg | UCTXIE;
    }
}

/***************************************************************************//**
    Transmit a null-terminated string via the UART.  This function blocks
    until the entire string has been written to the UART.

    \param[in] port - Selected UART port.
    \param[in] string - String to transmit.
*//****************************************************************************/
void Uart_WriteStringBlocking(
    UartPort_t port,
    const char_t string[])
{
    uint16_t i;
    uint8_t character;
    bool txIntWasEnabled;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile const uint8_t* intFlagReg = _uartInterruptFlagRegisters[port];

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Save transmit interrupt state */
    if ((*intEnableReg & UCTXIE) > 0u)
    {
        txIntWasEnabled = true;
    }
    else
    {
        txIntWasEnabled = false;
    }

    /* Disable transmit interrupt */
    *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));

    i = 0u;
    character = (uint8_t) string[i];
    while ((character != ((uint8_t) '\0')) && (i < UART_MAX_STRING_LENGTH))
    {
        /* Wait for TX buffer to be ready to receive new byte */
        while ((*intFlagReg & UCTXIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Write character */
        *_uartTransmitBufferRegisters[port] = character;
        ++i;

        /* Get next character */
        character = (uint8_t) string[i];
    }

    /* Restore transmit interrupt. */
    if (txIntWasEnabled)
    {
        *intEnableReg = *intEnableReg | UCTXIE;
    }
}

/***************************************************************************//**
    Transmit a byte via the UART.  The byte is loaded into the transmit buffer
    to be transmitted using the UART Tx interrupt.

    \param[in] port - Selected UART port.
    \param[in] byte - Byte to transmit.

    \return If the buffer does not have enough space available,
            UartError_BUFFER_FULL.  Otherwise, UartError_OK.
*//****************************************************************************/
UartError_t Uart_WriteByte(
    UartPort_t port,
    uint8_t byte)
{
    uint16_t newHead;
    UartError_t error = UartError_OK;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Calculate new head index value */
    newHead = _uartBufferTxHead[port];
    newHead = (newHead + 1u) & (UART_TX_BUFFER_SIZE - 1u);

    /* If buffer is not full, load byte and enable transmit interrupt.
       Otherwise indicate that the buffer is full.
    */
    if (newHead != _uartBufferTxTail[port])
    {
        _uartBufferTx[port][_uartBufferTxHead[port]] = byte;
        _uartBufferTxHead[port] = newHead;

        *intEnableReg = *intEnableReg | UCTXIE;
    }
    else
    {
        error = UartError_BUFFER_FULL;
    }

    return error;
}

/***************************************************************************//**
    Transmit a byte array via the UART.  The byte array is loaded into
    the transmit buffer to be transmitted using the UART Tx interrupt.

    \param[in] port - Selected UART port.
    \param[in] numToWrite - Number of bytes to transmit.
    \param[in] source - Pointer to byte array to transmit.

    \return If the buffer does not have enough space available,
            UartError_BUFFER_FULL.  Otherwise, UartError_OK.
*//****************************************************************************/
UartError_t Uart_WriteByteArray(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[])
{
    uint16_t i;
    uint16_t tmpHead;
    uint16_t tmpTail;
    uint16_t bytesAvailable;
    UartError_t error = UartError_OK;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];

    ASSERT((source != NULL) && (numToWrite > 0u), ErrorLedMode_SOFTWARE);
    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Calculate the number of bytes available in the transmit buffer. */
    tmpHead = _uartBufferTxHead[port];
    tmpTail = _uartBufferTxTail[port];
    if (tmpTail > tmpHead)
    {
        bytesAvailable = (tmpTail - tmpHead) - 1u;
    }
    else
    {
        bytesAvailable = ((((uint16_t) UART_TX_BUFFER_SIZE) - tmpHead) + tmpTail) - 1u;
    }

    /* If buffer has enough available space, load byte array and enable
       transmit interrupt.  Otherwise, indicate that the buffer is full.
    */
    if (bytesAvailable >= numToWrite)
    {
        for (i = 0u; i < numToWrite; ++i)
        {
            _uartBufferTx[port][tmpHead] = source[i];
            tmpHead = (tmpHead + 1u) & (UART_TX_BUFFER_SIZE - 1u);
        }

        _uartBufferTxHead[port] = tmpHead;

        *intEnableReg = *intEnableReg | UCTXIE;
    }
    else
    {
        error = UartError_BUFFER_FULL;
    }

    return error;
}

/***************************************************************************//**
    Transmit a byte via the UART in hex format.  The byte (in hex format) is
    loaded into the transmit buffer to be transmitted using the UART Tx
    interrupt.

    \param[in] port - Selected UART port.
    \param[in] byte - Byte to transmit in hex.

    \return If the buffer does not have enough space available,
            UartError_BUFFER_FULL.  Otherwise, UartError_OK.
*//****************************************************************************/
UartError_t Uart_WriteByteHex(
    UartPort_t port,
    uint8_t byte)
{
    uint8_t nibble;
    uint16_t tmpHead;
    uint16_t tmpTail;
    uint16_t bytesAvailable;
    UartError_t error = UartError_OK;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];

    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Calculate the number of bytes available in the transmit buffer. */
    tmpHead = _uartBufferTxHead[port];
    tmpTail = _uartBufferTxTail[port];
    if (tmpTail > tmpHead)
    {
        bytesAvailable = (tmpTail - tmpHead) - 1u;
    }
    else
    {
        bytesAvailable = ((((uint16_t) UART_TX_BUFFER_SIZE) - tmpHead) + tmpTail) - 1u;
    }

    /* If buffer has enough available space, load converted byte and enable
       transmit interrupt.  Otherwise, indicate that the buffer is full.
    */
    if (bytesAvailable >= 2u)
    {
        /* Load high nibble of byte */
        nibble = (byte >> 4u) & 0x0Fu;
        if (nibble > 9u)
        {
            nibble = nibble + 0x37u;
        }
        else
        {
            nibble = nibble + 0x30u;
        }
        _uartBufferTx[port][tmpHead] = nibble;
        tmpHead = (tmpHead + 1u) & (UART_TX_BUFFER_SIZE - 1u);

        /* Load low nibble of byte */
        nibble = byte & 0x0Fu;
        if (nibble > 9u)
        {
            nibble = nibble + 0x37u;
        }
        else
        {
            nibble = nibble + 0x30u;
        }
        _uartBufferTx[port][tmpHead] = nibble;
        tmpHead = (tmpHead + 1u) & (UART_TX_BUFFER_SIZE - 1u);

        /* Save new head index */
        _uartBufferTxHead[port] = tmpHead;

        *intEnableReg = *intEnableReg | UCTXIE;
    }
    else
    {
        error = UartError_BUFFER_FULL;
    }

    return error;
}

/***************************************************************************//**
    Transmit a null-terminated string via the UART.  The string is loaded
    into the transmit buffer to be transmitted using the UART Tx interrupt.

    Only the number of characters that will fit into the buffer will be loaded.
    All other characters in the string will be discarded.  This means that the
    string may not be sent if the buffer is full.

    \param[in] port - Selected UART port.
    \param[in] string - String to transmit
*//****************************************************************************/
void Uart_WriteString(
    UartPort_t port,
    const char_t string[])
{
    uint16_t i;
    uint16_t tmpHead;
    uint16_t tmpTail;
    uint16_t numToWrite;
    uint16_t bytesAvailable;
    bool portIsOpen = _uartPortIsOpen[port];
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    uint8_t character;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(portIsOpen, ErrorLedMode_SOFTWARE);

    /* Find length of string */
    i = 0u;
    character = (uint8_t) string[i];
    while ((character != ((uint8_t) '\0')) &&
           (i < UART_MAX_STRING_LENGTH))
    {
        ++i;
        character = (uint8_t) string[i];
    }
    numToWrite = i;

    /* Calculate the number of bytes available in the transmit buffer */
    tmpHead = _uartBufferTxHead[port];
    tmpTail = _uartBufferTxTail[port];
    if (tmpTail > tmpHead)
    {
        bytesAvailable = (tmpTail - tmpHead) - 1u;
    }
    else
    {
        bytesAvailable = ((((uint16_t) UART_TX_BUFFER_SIZE) - tmpHead) + tmpTail) - 1u;
    }

    /* Limit the number of bytes to write to the number of bytes available */
    if (bytesAvailable < numToWrite)
    {
        numToWrite = bytesAvailable;
    }

    if (numToWrite > 0u)
    {
        /* Load bytes into buffer and enable transmit interrupt */
        for (i = 0u; i < numToWrite; ++i)
        {
            _uartBufferTx[port][tmpHead] = (uint8_t) string[i];
            tmpHead = (tmpHead + 1u) & (UART_TX_BUFFER_SIZE - 1u);
        }

        _uartBufferTxHead[port] = tmpHead;

        *intEnableReg = *intEnableReg | UCTXIE;
    }
}

/***************************************************************************//**
    Reads one byte out of the receive buffer if available.

    \param[in] port - Selected UART port.
    \param[out] destinationByte - Pointer to where the read byte should be stored.

    \return If no bytes are available, UartError_NO_DATA.
            Otherwise, UartError_OK.
*//****************************************************************************/
UartError_t Uart_ReadByte(
    UartPort_t port,
    uint8_t* destinationByte)
{
    uint16_t tmpTail;
    UartError_t error = UartError_OK;

    ASSERT(destinationByte != NULL, ErrorLedMode_SOFTWARE);

    /* If buffer is not empty, store byte.
       Otherwise, indicate that not data is available.
    */
    tmpTail = _uartBufferRxTail[port];
    if (tmpTail != _uartBufferRxHead[port])
    {
        *destinationByte = _uartBufferRx[port][tmpTail];
        _uartBufferRxTail[port] = (tmpTail + 1u) & (UART_RX_BUFFER_SIZE - 1u);
    }
    else
    {
        error = UartError_NO_DATA;
    }

    return error;
}

/***************************************************************************//**
    Read bytes out of the receive buffer.

    \param[in] port - Selected UART port.
    \param[in] numToRead - Number of bytes to read if available.
    \param[out] destination - Pointer to where the read bytes should be stored.
    \param[out] numRead - Pointer where the number of bytes read out of the
                          receive buffer should be stored.

    \return If no bytes are available, UartError_NO_DATA.
            Otherwise, UartError_OK.
*//****************************************************************************/
UartError_t Uart_ReadByteArray(
    UartPort_t port,
    uint16_t numToRead,
    uint8_t destination[],
    uint16_t* numRead)
{
    uint16_t bytesRead;
    uint16_t tmpTail;
    UartError_t error = UartError_OK;

    ASSERT((destination != NULL) && (numRead != NULL) && (numToRead > 0u), ErrorLedMode_SOFTWARE);

    /* If buffer is not empty, read bytes.
       Otherwise, indicate that buffer is empty.
    */
    tmpTail = _uartBufferRxTail[port];
    if (tmpTail != _uartBufferRxHead[port])
    {
        /* Read bytes until the receive buffer is empty
           or the requested number of bytes has been read.
        */
        bytesRead = 0u;
        while ((tmpTail != _uartBufferRxHead[port]) &&
               (bytesRead < numToRead))
        {
            destination[bytesRead] = _uartBufferRx[port][tmpTail];

            ++bytesRead;
            tmpTail = (tmpTail + 1u) & (UART_RX_BUFFER_SIZE - 1u);
            _uartBufferRxTail[port] = tmpTail;
        }

        *numRead = bytesRead;
    }
    else
    {
        *numRead = 0u;
        error = UartError_NO_DATA;
    }

    return error;
}

/***************************************************************************//**
    Determines if bytes are available in the UART receive buffer.

    \param[in] port - Selected UART port.

    \return True if bytes are available, False is the receive buffer is empty.
*//****************************************************************************/
bool Uart_AreBytesAvailable(UartPort_t port)
{
    bool areBytesAvailable;
    uint16_t tmpTail;

    tmpTail = _uartBufferRxTail[port];
    if (tmpTail == _uartBufferRxHead[port])
    {
        areBytesAvailable = false;
    }
    else
    {
        areBytesAvailable = true;
    }

    return areBytesAvailable;
}

/***************************************************************************//**
    Determines if bytes are available in the UART receive buffer for any of
    the open UART ports.

    \return True if bytes are available, False is the receive buffers are empty.
*//****************************************************************************/
bool Uart_AreBytesAvailableAnyOpenPort(void)
{
	bool bytesAreAvailable = false;
	uint16_t portNumber;

	for (portNumber = 0u; portNumber < UART_NUM_PORTS; ++portNumber)
	{
		if ((_uartPortIsOpen[portNumber]) &&
			(_uartBufferRxTail[portNumber] != _uartBufferRxHead[portNumber]))
		{
			bytesAreAvailable = true;
			break;
		}
	}

	return bytesAreAvailable;
}
/***************************************************************************//**
    Handles all UART transmit interrupts.  Called from an interrupt service
    routine, so must complete quickly.

    The next byte in the transmit buffer (if available) is written.
    If no bytes are available, the transmit interrupt is disabled.

    \param[in] port - UART port on which the interrupt occurred
*//****************************************************************************/
static void _HandleUartTransmitInterrupt(UartPort_t port)
{
    uint16_t tmpTail;
    volatile uint8_t* intEnableReg = _uartInterruptEnableRegisters[port];
    volatile uint8_t* txBufReg = _uartTransmitBufferRegisters[port];

    tmpTail = _uartBufferTxTail[port];
    if (tmpTail == _uartBufferTxHead[port])
    {
        *intEnableReg = *intEnableReg & ((uint8_t) ~(UCTXIE));
    }
    else
    {
        *txBufReg = _uartBufferTx[port][tmpTail];

        _uartBufferTxTail[port] = (tmpTail + 1u) & (UART_TX_BUFFER_SIZE - 1u);
    }
}

/***************************************************************************//**
    Handles all UART receive interrupts.  Called from an interrupt service
    routine, so must complete quickly.

    The received byte is read into the receive buffer if the buffer is not full.
    Note that UART overrun errors are ignored.  Framing and parity errors will
    not overwrite the UART Rx buffer if not enabled.

    \param[in] port - UART port on which the interrupt occurred
*//****************************************************************************/
static void _HandleUartReceiveInterrupt(UartPort_t port)
{
    uint16_t tmpHead;
    volatile const uint8_t* rxBufReg = _uartReceiveBufferRegisters[port];

    tmpHead = _uartBufferRxHead[port];
    _uartBufferRx[port][tmpHead] = *rxBufReg;

    tmpHead = (tmpHead + 1u) & (UART_RX_BUFFER_SIZE - 1u);
    if (tmpHead != _uartBufferRxTail[port])
    {
        _uartBufferRxHead[port] = tmpHead;
    }
    else
    {
        ASSERT(false, ErrorLedMode_SOFTWARE);
    }
}

#ifdef UART_ENABLE_PORT_0
/***************************************************************************//**
    Interrupt service routine for UART port 0.
*//****************************************************************************/
#pragma vector=USCI_A0_VECTOR
__interrupt void _UartA0Isr(void)
{
    uint8_t interruptEnabledFlags = UCA0IE;
    uint8_t interruptFlags = UCA0IFG;

    if (((interruptEnabledFlags & UCTXIE) > 0u) && ((interruptFlags & UCTXIFG) > 0u))
    {
        _HandleUartTransmitInterrupt(UartPort_0);
    }
    else if (((interruptEnabledFlags & UCRXIE) > 0u) && ((interruptFlags & UCRXIFG) > 0u))
    {
        _HandleUartReceiveInterrupt(UartPort_0);
//        _low_power_mode_off_on_exit();
        // TODO: spg - investigate low power modes
    }
    else
    {
        ASSERT(false, ErrorLedMode_SOFTWARE);
    }
    _low_power_mode_off_on_exit();
}
#endif

#ifdef UART_ENABLE_PORT_1
/***************************************************************************//**
    Interrupt service routine for UART port 1.
*//****************************************************************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void _UartA1Isr(void)
{
    uint8_t interruptEnabledFlags = UCA1IE;
    uint8_t interruptFlags = UCA1IFG;

    if (((interruptEnabledFlags & UCTXIE) > 0u) && ((interruptFlags & UCTXIFG) > 0u))
    {
        _HandleUartTransmitInterrupt(UartPort_1);
    }
    else if (((interruptEnabledFlags & UCRXIE) > 0u) && ((interruptFlags & UCRXIFG) > 0u))
    {
        _HandleUartReceiveInterrupt(UartPort_1);
//        _low_power_mode_off_on_exit();
        // TODO: spg - investigate low power modes
    }
    else
    {
        ASSERT(false, ErrorLedMode_SOFTWARE);
    }
    _low_power_mode_off_on_exit();
}
#endif
