/***************************************************************************//**
    \file I2c.h
    \brief I2C Module header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions to configure an I2C port to
       communicate to various peripheral devices.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/I2c.h$
    $Revision: 2$
    $Date: Friday, May 04, 2012 5:41:20 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef I2C_H_
#define I2C_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
#if 1
/** If defined, I2C port 0 is enabled. */
#define I2C_ENABLE_PORT_0
#endif

#if 0
/** If defined, I2C port 1 is enabled. */
#define I2C_ENABLE_PORT_1
#endif


/** Enumeration type for I2C module errors. */
typedef enum I2cError
{
    I2cError_OK,
    I2cError_NACK,
    I2cError_STOP
} I2cError_t;

/** The available I2C ports */
typedef enum I2cPort
{
#ifdef I2C_ENABLE_PORT_0
    I2cPort_0 = 0u
#endif
#ifdef I2C_ENABLE_PORT_1
    ,I2cPort_1 = 1u
#endif
} I2cPort_t;

/** The possible operations when using the I2C port */
typedef enum I2cOperation
{
    I2cOperation_WRITE,
    I2cOperation_READ
} I2cOperation_t;

/** The possible Modes when configuring a I2C port */
typedef enum I2cMode
{
    I2cMode_SLAVE,
    I2cMode_MASTER
} I2cMode_t;

/** The possible addressing modes when configuring a I2C port */
typedef enum I2cAddressing
{
    I2cAddressing_7_bits,
    I2cAddressing_10_bits
} I2cAddressing_t;

/** The possible baud rates when configuring a I2C port */
typedef enum I2cBaudRate
{
    I2cBaudRate_100000,
    I2cBaudRate_400000
} I2cBaudRate_t;

/** Defines the I2C port configuration structure */
typedef struct I2cPortConfig
{
    I2cMode_t modeType;
    I2cAddressing_t addressing;
    I2cBaudRate_t baudRate;
} I2cPortConfig_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void I2c_Init(
    I2cPort_t port,
    const I2cPortConfig_t* config);
void I2c_Open(I2cPort_t port);
void I2c_Close(I2cPort_t port);

I2cError_t I2c_TransmitStart(
    I2cPort_t port,
    uint8_t address,
    I2cOperation_t operation);

I2cError_t I2c_TransmitByte(
    I2cPort_t port,
    uint8_t byte);
I2cError_t I2c_TransmitByteEnd(
    I2cPort_t port,
    uint8_t byte);

void I2c_ReceiveByte(
    I2cPort_t port,
    uint8_t* byte);
void I2c_ReceiveByteEnd(
    I2cPort_t port,
    uint8_t* byte);

void I2c_WaitUntilBusIdle(I2cPort_t port);
#endif /* I2C_H_ */
