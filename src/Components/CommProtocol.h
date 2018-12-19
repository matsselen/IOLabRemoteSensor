/***************************************************************************//**
    \file CommProtocol.h
    \brief Header for the communications protocol module.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header for the communications protocol module that will manage
                   the protocol and data for the communications to the other micro.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/CommProtocol.h$
    $Revision: 5$
    $Date: Friday, October 24, 2014 2:10:33 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef COMMPROTOCOL_H_
#define COMMPROTOCOL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the valid states for the RF Connection Status */
typedef enum RfConnectionStatus
{
	RfConnectionStatus_INVLALID   = 0u,
	RfConnectionStatus_NOT_PAIRED = 0x20u,
	RfConnectionStatus_FIRST = RfConnectionStatus_NOT_PAIRED,
	RfConnectionStatus_PAIRING     = 0x21u,
	RfConnectionStatus_PAIRED_NOT_CONNECTED = 0x22u,
	RfConnectionStatus_CONNECTED  = 0x23u,
	RfConnectionStatus_DATA       = 0x24u,
	RfConnectionStatus_LAST
} RfConnectionStatus_t;

/** Defines a structure to hold the RF Status information in */
typedef struct RfStatus
{
	RfConnectionStatus_t connectionStatus;
	uint8_t fwMajor;
	uint8_t fwMinor;
} RfStatus_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void CommProtocol_Init();
void CommProtocol_RxHandler(void);
void CommProtocol_TxHandler(void);
void CommProtocol_GetRfStatus(RfStatus_t *status);
void CommProtocol_HandleStartDataPacket(
        uint8_t messageData[],
        uint8_t messageLength);
void CommProtocol_HandleStopDataPacket(
        uint8_t messageData[],
        uint8_t messageLength);

#endif /* COMMPROTOCOL_H_ */
