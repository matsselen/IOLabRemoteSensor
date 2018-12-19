/***************************************************************************//**
    \file Serial.c
    \brief Serial communication module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module implements a serial communication interface that
    supports both a binary and human readable output.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Serial.c$
    $Revision: 9$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Serial.h"
#include <EventQueue.h>
#include <Assert.h>
#include <Debug.h>

#ifdef SERIAL_ENABLE_PRINTF
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#endif

#include <Uart.h>
#include <string.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the selected UART port used by the serial interface */
#define SERIAL_UART_PORT                (UartPort_1)

#if 0
/** If defined, enables the sending of ACK and NAK bytes in response to
    binary packets that are received.
*/
#define ENABLE_ACK_NAK_BINARY_PACKETS
#endif

#if 0
/** If defined, enables the sending of ACK and NAK bytes in response to
    human-readable packets that are received.
*/
#define ENABLE_ACK_NAK_HUMAN_READABLE_PACKETS
#endif

/** \name Serial Protocol Field Indexes
    \{
*/
#define SERIAL_SOP_INDEX                (0u)
#define SERIAL_LENGTH_MSB_INDEX         (1u)
#define SERIAL_LENGTH_LSB_INDEX         (2u)
#define SERIAL_DCS_MSB_INDEX            (3u)
#define SERIAL_DCS_LSB_INDEX            (4u)
#define SERIAL_HCS_MSB_INDEX            (5u)
#define SERIAL_HCS_LSB_INDEX            (6u)
#define SERIAL_PAYLOAD_INDEX            (7u)
/** \} */

/** Length of a checksum field in bytes. */
#define SERIAL_CHECKSUM_LENGTH          (2u)
/** Length of an entire header (includes SOP) of a binary packet. */
#define SERIAL_HEADER_LENGTH            (7u)

/** \name Serial protocol start- and end-of-packet byte definitions.
    \{
*/
/** Equal to '|' in ASCII. */
#define SERIAL_SOP_BINARY               (0x7Cu)
/** Equal to '<' in ASCII. */
#define SERIAL_SOP_HUMAN                (0x3Cu)
/** Equal to '\n' (newline) in ASCII. */
#define SERIAL_EOP                      (0x0Du)
/** \} */

/** \name Serial protocol acknowledgment and negative-acknowledgment definitions.
    \{
*/
/** Equal to 'A' in ASCII. */
#define SERIAL_ACK                      (0x41u)
/** Equal to 'N' in ASCII. */
#define SERIAL_NAK                      (0x4Eu)
/** \} */

/** Length in bytes of the footer used for human-readable packets */
#define FOOTER_HUMAN_LENGTH             (2u)

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Footer used for human-readable packets.  While the protocol does not
    require the carriage return (0x0D), adding it ensures that human-readable
    packets display correctly on a terminal.
*/
static const uint8_t _footerHuman[FOOTER_HUMAN_LENGTH] = {0x0Au, SERIAL_EOP};

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Stores the most recently received serial payload. */
static SerialBuffer_t _serialPayloadRx;

/** Tracks if the serial interface is open */
static bool _isSerialInterfaceOpen = false;

/** Tracks the current mode of the serial interface */
static SerialMode_t _currentMode = SerialMode_TRANSMIT_ONLY;

/** Tracks the current index of a packet being received */
static uint16_t _currentRxIndex = SERIAL_SOP_INDEX;

#ifdef SERIAL_ENABLE_PRINTF
/** Stores a string to be transmitted */
static char_t _txString[SERIAL_PAYLOAD_MAX_LENGTH];
#endif

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
#if (defined(ENABLE_ACK_NAK_BINARY_PACKETS) || defined (ENABLE_ACK_NAK_HUMAN_READABLE_PACKETS))
static void _SendAck(void);
static void _SendNak(void);
#endif

/***************************************************************************//**
    Initializes the received serial payload buffer and the UART.
*//****************************************************************************/
void Serial_Init(void)
{
    UartPortConfig_t config = {UartBaudRate_115200, UartParity_NONE, UartEndieness_LsbFirst, UartStopBit_ONE, false};

    Uart_Init(SERIAL_UART_PORT, &config);

    _serialPayloadRx.inUse = false;
}

/***************************************************************************//**
    Opens the serial interface.

    \param[in] mode - Selected mode for the serial interface.
*//****************************************************************************/
void Serial_Open(SerialMode_t mode)
{
    bool isUartPortOpen;

    isUartPortOpen = Uart_IsPortOpen(SERIAL_UART_PORT);
    if ((_isSerialInterfaceOpen == false) && (isUartPortOpen == false))
    {
        Uart_Open(SERIAL_UART_PORT);
    }

    if (mode == SerialMode_TRANSMIT_AND_RECEIVE)
    {
        Uart_EnableReceive(SERIAL_UART_PORT);
    }

    /* Flush received packets and reset receive index */
    _serialPayloadRx.inUse = false;
    _currentRxIndex = SERIAL_SOP_INDEX;

    _currentMode = mode;
    _isSerialInterfaceOpen = true;
}

/***************************************************************************//**
    Closes the serial interface.
*//****************************************************************************/
void Serial_Close(void)
{
    Uart_Close(SERIAL_UART_PORT);

    _isSerialInterfaceOpen = false;
}

/***************************************************************************//**
    Processes bytes received from the UART, searching for a valid packet.
*//****************************************************************************/
void Serial_ProcessReceivedBytes(void)
{
	static uint16_t currentPayloadIndex = 0u;
//	static uint16_t payloadLength = 0u;
	bool payloadInUse;
	bool packetFound;
	uint8_t byteRead;
	UartError_t uartError;

	/* Only process bytes if the interface is open, receive mode is enabled, and
	   the payload structure is not in use.
	*/
	payloadInUse = _serialPayloadRx.inUse;
	if ((_isSerialInterfaceOpen) &&
		(_currentMode == SerialMode_TRANSMIT_AND_RECEIVE) &&
		(payloadInUse == false))
	{
		/* loop while bytes are available from the UART and a packet has not
		   been found.
		*/
		packetFound = false;
		while ((Uart_AreBytesAvailable(SERIAL_UART_PORT)) &&
			   (packetFound == false))
		{
			/* get next byte, if read error occurs, exit loop */
			uartError = Uart_ReadByte(SERIAL_UART_PORT, &byteRead);
			if (uartError != UartError_OK)
			{
				break;
			}

			switch(_currentRxIndex)
			{
				case SERIAL_SOP_INDEX:
				{
					/* Look for start of packet byte. If SOP is binary, increment index
					   and initialize field storage value. If SOP is human, set index to
					   payload index since there is not more header for human readable.
					 */
					if (byteRead == SERIAL_SOP_BINARY)
					{
						// TODO: spg - handle Binary SOP
					}
					else if (byteRead == SERIAL_SOP_HUMAN)
					{
						_currentRxIndex = SERIAL_PAYLOAD_INDEX;
						currentPayloadIndex = 0u;
						_serialPayloadRx.isBinary = false;
					}
					else
					{
						/* do nothing */
					}
					break;
				}

				case SERIAL_PAYLOAD_INDEX:
				{
					if (_serialPayloadRx.isBinary)
					{
						// TODO: spg - handle binary packets
					}
					else
					{
						/* for human readable packets:
						   Save payload bytes until EOP byte is received or the payload
						   buffer becomes full. If an EOP is received, add a NULL character
						   to the end of the payload to make it a proper string. If necessary,
						   send an ACK. Return the pointer to the payload. Otherwise, if the
						   buffer is full and no EOP was found, send NAK and go back looking for
						   SOP byte.
						*/
						if (byteRead == SERIAL_EOP)
						{
							_serialPayloadRx.inUse = true;
							_serialPayloadRx.length = currentPayloadIndex;
							_serialPayloadRx.buffer[currentPayloadIndex] = (uint8_t) '\0';

							packetFound = true;
							EventQueue_Add(Event_SERIAL_RECEIVED_PACKET, EventPriority_LOWEST);

							/* restart the index looking for the start of the next packet */
							_currentRxIndex = SERIAL_SOP_INDEX;

							#ifdef ENABLE_ACK_NAK_HUMAN_READABLE_PACKETS
							_SendAck();
							#endif
						}
						else
						{
							if (currentPayloadIndex < SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
							{
								_serialPayloadRx.buffer[currentPayloadIndex] = byteRead;
								++currentPayloadIndex;
							}
							else
							{
								_currentRxIndex = SERIAL_SOP_INDEX;

								#ifdef ENABLE_ACK_NAK_HUMAN_READABLE_PACKETS
								_SendNak();
								#endif
							}
						}

					}

					break;
				}

				default:
					/* should not reach default, flag error */
					ASSERT(false, ErrorLedMode_SOFTWARE);
					break;
			}
		}
	}
}

/***************************************************************************//**
    Attempts to get a serial packet.

    \param[out] payloadReceived - Pointer to the received serial payload.

    \retval SerialStatus_SUCCESS if a valid packet was received.
    \retval SerialStatus_RECEIVE_DISABLED if not in receive mode.
    \retval SerialStatus_NO_PACKET if no packet was available.
*//****************************************************************************/
SerialStatus_t Serial_GetPacket(SerialBuffer_t** payloadReceived)
{
    bool payloadInUse = _serialPayloadRx.inUse;
    SerialStatus_t status;

    ASSERT(payloadReceived != NULL, ErrorLedMode_SOFTWARE);

    *payloadReceived = NULL;

    if (_currentMode != SerialMode_TRANSMIT_AND_RECEIVE)
    {
        status = SerialStatus_RECEIVE_DISABLED;
    }
    else if (payloadInUse == false)
    {
        status = SerialStatus_NO_PACKET;
    }
    else
    {
        *payloadReceived = &_serialPayloadRx;

        status = SerialStatus_SUCCESS;
    }

    return status;
}

/***************************************************************************//**
    Attempts to send a serial packet.

    \param[in] payloadToTransmit - Pointer to the serial payload to transmit.

    \retval SerialStatus_SUCCESS if packet sent successfully.
    \retval SerialStatus_TX_BUFFER_FULL if transmit buffer is full.
*//****************************************************************************/
SerialStatus_t Serial_SendPacket(const SerialBuffer_t* payloadToTransmit)
{
	SerialStatus_t serialStatus = SerialStatus_SUCCESS;
	UartError_t uartResult = UartError_OK;

	ASSERT(payloadToTransmit != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);
	ASSERT(payloadToTransmit->length <= SERIAL_PAYLOAD_MAX_LENGTH, ErrorLedMode_SOFTWARE);

	if (payloadToTransmit->isBinary)
	{
		// TODO: spg - handle binary packets
		/* assert to notify that binary packets aren't working yet */
		ASSERT(false, ErrorLedMode_SOFTWARE);
	}
	else
	{
		/* Send human readable packet in the form SOP - payload - footer. */
		uartResult = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);

		if (uartResult == UartError_OK)
		{
			uartResult = Uart_WriteByteArray(SERIAL_UART_PORT, payloadToTransmit->length,
											 payloadToTransmit->buffer);
		}

		if (uartResult == UartError_OK)
		{
			uartResult = Uart_WriteByteArray(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
		}
	}

	if (uartResult == UartError_BUFFER_FULL)
	{
		serialStatus = SerialStatus_TX_BUFFER_FULL;
	}

	return serialStatus;
}

#if (defined(ENABLE_ACK_NAK_BINARY_PACKETS) || defined (ENABLE_ACK_NAK_HUMAN_READABLE_PACKETS))
/***************************************************************************//**
    Attempts to send an acknowledgment packet.
*//****************************************************************************/
static void _SendAck(void)
{
    UartError_t result;

    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_ACK);
    ASSERT(result == UartError_OK, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Attempts to send a negative acknowledgment packet.
*//****************************************************************************/
static void _SendNak(void)
{
    UartError_t result;

    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_NAK);
    ASSERT(result == UartError_OK, ErrorLedMode_SOFTWARE);
}
#endif

/***************************************************************************//**
    Attempts to send a string as a human-readable serial packet.  If a UART
    error occurs, the entire packet may not be written.

    \param[in] string - Pointer to the string to transmit.
*//****************************************************************************/
void Serial_SendString(const char_t* string)
{
    uint16_t stringLength;
    UartError_t result;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    if (result == UartError_OK)
    {
/* Disable MISRA checks because of compiler error.  Compiler says that return
   value of function violates MISRA rule 10.1
*/
#pragma CHECK_MISRA("none")
        result = Uart_WriteByteArray(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
#pragma RESET_MISRA("all")

        if (result == UartError_OK)
        {
            (void) Uart_WriteByteArray(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
        }
    }
}

/***************************************************************************//**
    Attempts to send a string and number as a human-readable serial packet.  If a
    UART error occurs, the entire packet may not be written. The number will
    always be printed at the end of the string in hex format.

    \param[in] string - Pointer to the string to transmit.
    \param[in] number - number to print at the end of the string.
*//****************************************************************************/
void Serial_SendStringNumber(const char_t* string,
                             uint16_t number)
{
    uint16_t stringLength;
    UartError_t result;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    if (result == UartError_OK)
    {
/* Disable MISRA checks because of compiler error.  Compiler says that return
   value of function violates MISRA rule 10.1
*/
#pragma CHECK_MISRA("none")
        result = Uart_WriteByteArray(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
#pragma RESET_MISRA("all")

		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number>>8));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number));
		}

        if (result == UartError_OK)
        {
            (void) Uart_WriteByteArray(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
        }
    }
}

/***************************************************************************//**
    Attempts to send a string and number as a human-readable serial packet.  If a
    UART error occurs, the entire packet may not be written. The number will
    always be printed at the end of the string in hex format.

    \param[in] string - Pointer to the string to transmit.
    \param[in] number - number to print at the end of the string.
*//****************************************************************************/
void Serial_SendStringNumberU32(const char_t* string,
                             uint32_t number)
{
    uint16_t stringLength;
    UartError_t result;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    if (result == UartError_OK)
    {
/* Disable MISRA checks because of compiler error.  Compiler says that return
   value of function violates MISRA rule 10.1
*/
#pragma CHECK_MISRA("none")
        result = Uart_WriteByteArray(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
#pragma RESET_MISRA("all")

        if (result == UartError_OK)
        {
            result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number>>24u));
        }

        if (result == UartError_OK)
        {
            result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number>>16u));
        }

        if (result == UartError_OK)
        {
            result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number>>8u));
        }

        if (result == UartError_OK)
        {
            result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number));
        }

        if (result == UartError_OK)
        {
            (void) Uart_WriteByteArray(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
        }
    }
}

/***************************************************************************//**
    Attempts to send a string and number as a human-readable serial packet.  If a
    UART error occurs, the entire packet may not be written. The number will
    always be printed at the end of the string in hex format.

    \param[in] string - Pointer to the string to transmit.
    \param[in] number1 - number to print at the end of the string.
    \param[in] number2 - number to print at the end of the string.
    \param[in] number3 - number to print at the end of the string.
*//****************************************************************************/
void Serial_SendString3Numbers(const char_t* string,
                               uint16_t number1,
                               uint16_t number2,
                               uint16_t number3)
{
    uint16_t stringLength;
    UartError_t result;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    result = Uart_WriteByte(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    if (result == UartError_OK)
    {
/* Disable MISRA checks because of compiler error.  Compiler says that return
   value of function violates MISRA rule 10.1
*/
#pragma CHECK_MISRA("none")
        result = Uart_WriteByteArray(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
#pragma RESET_MISRA("all")

        /* Number 1 */
		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number1>>8));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number1));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByte(SERIAL_UART_PORT, (uint8_t)',');
		}

		/* Number 2 */
		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number2>>8));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number2));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByte(SERIAL_UART_PORT, (uint8_t)',');
		}

		/* Number 3 */
		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number3>>8));
		}

		if (result == UartError_OK)
		{
			result = Uart_WriteByteHex(SERIAL_UART_PORT, (uint8_t)(number3));
		}

		/* end packet */
        if (result == UartError_OK)
        {
            (void) Uart_WriteByteArray(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
        }
    }
}

#ifdef SERIAL_ENABLE_PRINTF
/***************************************************************************//**
    Attempts to send a printf formatted string with a variable list of
    arguments.

    \param[in] string - Pointer to the string to transmit.
    \param[in] ... - a var args list of inputs into the format string
*//****************************************************************************/
void Serial_Printf(const char_t* string, ...)
{
    va_list argList;
    va_start(argList, string);

    (void) vsnprintf(_txString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
                    string, argList);
    va_end(argList);

    Serial_SendString(_txString);
}
#endif

/***************************************************************************//**
    Attempts to send a string as a human-readable serial packet and blocks
    until the send is complete.

    \param[in] string - Pointer to the string to transmit.
*//****************************************************************************/
void Serial_SendStringBlocking(const char_t* string)
{
    uint16_t stringLength;

    ASSERT(string != NULL, ErrorLedMode_SOFTWARE);
    ASSERT(_isSerialInterfaceOpen, ErrorLedMode_SOFTWARE);

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    Uart_WriteByteBlocking(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    Uart_WriteByteArrayBlocking(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
    Uart_WriteByteArrayBlocking(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
}

/***************************************************************************//**
    Attempts to send a string as a human-readable serial packet and blocks
    until the send is complete.

	\note This function does not use an assert for safety checking so that it
	can be used in the assert handler.

    \param[in] string - Pointer to the string to transmit.
*//****************************************************************************/
void Serial_SendStringBlockingNoAssert(const char_t* string)
{
    uint16_t stringLength;

    /* Limit string length */
    stringLength = strlen(string);
    if (stringLength > SERIAL_PAYLOAD_MAX_LENGTH_HUMAN)
    {
        stringLength = SERIAL_PAYLOAD_MAX_LENGTH_HUMAN;
    }

    /* Send human-readable packet:  SOP - payload - footer. */
    Uart_WriteByteBlocking(SERIAL_UART_PORT, SERIAL_SOP_HUMAN);
    Uart_WriteByteArrayBlocking(SERIAL_UART_PORT, stringLength, (const uint8_t*) string);
    Uart_WriteByteArrayBlocking(SERIAL_UART_PORT, FOOTER_HUMAN_LENGTH, _footerHuman);
}
