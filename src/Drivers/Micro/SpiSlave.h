/***************************************************************************//**
    \file SpiSlave.h
    \brief SPI module using this device as a SPI slave

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides functions to configure the SPI port
       to be a SPI slave device.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/SpiSlave.h$
    $Revision: 3$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef SPISLAVE_H_
#define SPISLAVE_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Enumeration type for SPI slave module errors. */
typedef enum SpiSlaveError
{
    SpiSlaveError_OK,
    SpiSlaveError_BUFFER_IN_USE,
    SpiSlaveError_PACKET_TOO_LONG,
    SpiSlaveError_NO_PACKET,
    SpiSlaveError_WRITE_IN_PROGRESS
} SpiSlaveError_t;
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void SpiSlave_Init(void);

SpiSlaveError_t SpiSlave_LoadPacket(uint16_t packetLength,
                         const uint8_t packet[]);
void SpiSlave_UnloadPacket(void);
bool SpiSlave_PacketLoaded(void);

SpiSlaveError_t SpiSlave_ReadPacket(uint16_t packetLength,
                         uint8_t  packet[]);
void SpiSlave_FlushRead(void);
bool SpiSlave_PacketAvailable(void);

#endif /* SPISLAVE_H_ */
