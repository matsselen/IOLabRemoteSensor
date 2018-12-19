/***************************************************************************//**
    \file Uart.h
    \brief UART module header

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions and macros for configuring the
       UART and transmitting and receiving data over that port.


*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Uart.h$
    $Revision: 3$
    $Date: Thursday, November 29, 2012 8:21:34 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef UART_H_
#define UART_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
#if 1
/** If defined, UART port 0 is enabled. */
#define UART_ENABLE_PORT_0
#endif

#if 1
/** If defined, UART port 1 is enabled. */
#define UART_ENABLE_PORT_1
#endif

/** Enumeration type for UART module errors. */
typedef enum UartError
{
    UartError_OK,
    UartError_NO_DATA,
    UartError_BUFFER_FULL
} UartError_t;

/** The available UART ports */
typedef enum UartPort
{
#ifdef UART_ENABLE_PORT_0
    UartPort_0 = 0u,
#endif
#ifdef UART_ENABLE_PORT_1
    UartPort_1 = 1u
#endif
} UartPort_t;

/** The possible baud rates when configuring a UART port */
typedef enum UartBaudRate
{
	UartBaudRate_9600,
    UartBaudRate_19200,
    UartBaudRate_38400,
    UartBaudRate_57600,
    UartBaudRate_115200,
    UartBaudRate_230400
} UartBaudRate_t;

/** The possible parity options when configuring a UART port */
typedef enum UartParity
{
    UartParity_NONE,
    UartParity_ODD,
    UartParity_EVEN
} UartParity_t;

/** The possible endieness options for a UART port */
typedef enum UartEndieness
{
	UartEndieness_MsbFirst,
	UartEndieness_LsbFirst
} UartEndieness_t;

/** The possible stop bit options for a UART port */
typedef enum UartStopBit
{
	UartStopBit_ONE,
	UartStopBit_TWO
} UartStopBit_t;

/** Defines the UART port configuration structure */
typedef struct UartPortConfig
{
    UartBaudRate_t baudRate;
    UartParity_t parity;
    UartEndieness_t endieness;
    UartStopBit_t stopBits;
    bool enableIrda;
} UartPortConfig_t;


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Uart_Init(
    UartPort_t port,
    const UartPortConfig_t* config);
void Uart_Open(UartPort_t port);
void Uart_Close(UartPort_t port);
bool Uart_IsPortOpen(UartPort_t port);

void Uart_EnableReceive(UartPort_t port);
void Uart_DisableReceive(UartPort_t port);

void Uart_WriteByteBlocking(
    UartPort_t port,
    uint8_t byte);
void Uart_WriteByteBlockingNoAssert(
    UartPort_t port,
    uint8_t byte);
void Uart_WriteByteArrayBlocking(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[]);
void Uart_WriteByteArrayBlockingNoAssert(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[]);
void Uart_WriteByteHexBlocking(
    UartPort_t port,
    uint8_t byte);
void Uart_WriteByteArrayHexBlocking(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[]);
void Uart_WriteStringBlocking(
    UartPort_t port,
    const char_t string[]);

UartError_t Uart_WriteByte(
    UartPort_t port,
    uint8_t byte);
UartError_t Uart_WriteByteArray(
    UartPort_t port,
    uint16_t numToWrite,
    const uint8_t source[]);
UartError_t Uart_WriteByteHex(
    UartPort_t port,
    uint8_t byte);
void Uart_WriteString(
    UartPort_t port,
    const char_t string[]);

UartError_t Uart_ReadByte(
    UartPort_t port,
    uint8_t* destinationByte);
UartError_t Uart_ReadByteArray(
    UartPort_t port,
    uint16_t numToRead,
    uint8_t destination[],
    uint16_t* numRead);
bool Uart_AreBytesAvailable(UartPort_t port);
bool Uart_AreBytesAvailableAnyOpenPort(void);

#endif /* UART_H_ */
