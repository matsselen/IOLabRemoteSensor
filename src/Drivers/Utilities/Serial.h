/***************************************************************************//**
    \file Serial.h
    \brief Serial interface module header

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The serial interface is used to transmit serial data over a
    serial port.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Serial.h$
    $Revision: 6$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef SERIAL_H_
#define SERIAL_H_

#if 0
#define SERIAL_ENABLE_PRINTF
#endif

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** The maximum length in bytes of the payload field. */
#define SERIAL_PAYLOAD_MAX_LENGTH           (50u)
/** The maximum length in bytes of the payload field for a human readable
    packet.  The length is reduced by one to make space for a NULL character
    at the end of the payload, making it a valid string.
*/
#define SERIAL_PAYLOAD_MAX_LENGTH_HUMAN     (SERIAL_PAYLOAD_MAX_LENGTH - 1u)

/** Possible results of serial operations. */
typedef enum SerialStatus
{
    SerialStatus_SUCCESS,
    SerialStatus_RECEIVE_DISABLED,
    SerialStatus_NO_PACKET,
    SerialStatus_TX_BUFFER_FULL
} SerialStatus_t;

/** Possible serial interface modes. */
typedef enum SerialMode
{
    SerialMode_TRANSMIT_ONLY,
    SerialMode_TRANSMIT_AND_RECEIVE
} SerialMode_t;

/** Defines a struct used to store serial payloads. */
typedef struct SerialBuffer
{
    /** Tracks if the buffer is currently in use. */
    bool inUse;
    /** Specifies if the buffer contains a binary payload. */
    bool isBinary;
    /** Specifies the length of the payload in the buffer. */
    uint16_t length;
    /** Stores the serial payload. */
    uint8_t buffer[SERIAL_PAYLOAD_MAX_LENGTH];
} SerialBuffer_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Serial_Init(void);
void Serial_Open(SerialMode_t mode);
void Serial_Close(void);

void Serial_ProcessReceivedBytes(void);

SerialStatus_t Serial_GetPacket(SerialBuffer_t** payloadReceived);
SerialStatus_t Serial_SendPacket(const SerialBuffer_t* payloadToTransmit);
void Serial_SendString(const char_t* string);
void Serial_SendStringNumber(const char_t* string,
                             uint16_t number);
void Serial_SendStringNumberU32(const char_t* string,
                             uint32_t number);
void Serial_SendString3Numbers(const char_t* string,
                               uint16_t number1,
                               uint16_t number2,
                               uint16_t number3);
#ifdef SERIAL_ENABLE_PRINTF
void Serial_Printf(const char_t* string, ...);
#endif
//void Serial_SendPacketBlockingNoAssert(const SerialBuffer_t* payloadToTransmit);
void Serial_SendStringBlocking(const char_t* string);
void Serial_SendStringBlockingNoAssert(const char_t* string);

#endif /* SERIAL_H_ */
