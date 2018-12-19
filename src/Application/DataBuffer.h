/***************************************************************************//**
    \file DataBuffer.h
    \brief Data buffer is the memory space reserved to store sensor data

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The data buffer contains memory space to store sensor data and
       can be written to and read from. The buffer contains locking mechanisms to
       ensure that data is not compromised and is valid.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/DataBuffer.h$
    $Revision: 7$
    $Date: Friday, October 24, 2014 10:53:10 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DATABUFFER_H_
#define DATABUFFER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the maximum number of subarrays that can be handled */
#define MAX_NUM_SUB_ARRAYS	(33u)
/** Defines the minimum size acceptable for the buffer arrays */
#define MIN_BUFFER_SIZE		(100u)

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void DataBuffer_Init(
	uint8_t array0[],
	uint8_t array1[],
	uint16_t arraySize);
bool DataBuffer_SetData(
	uint16_t startIndex,
	uint16_t length,
	const uint8_t  data[]);
void DataBuffer_GetIdleBuffer(
    uint16_t *bufferLength,
    uint8_t **buffer);
uint16_t DataBuffer_RotateBuffer(void);
bool DataBuffer_CreateSubarray(
	uint8_t  subArrayId,
	uint16_t length);
bool DataBuffer_AddToSubarray8Bit(
	uint8_t  subArrayId,
	uint16_t length,
	const uint8_t data[]);
bool DataBuffer_AddToSubarray16Bit(
	uint8_t  subArrayId,
	uint16_t length,
	const uint16_t data[]);
void DataBuffer_ClearSubarrays(void);
bool DataBuffer_SubarrayExists(uint8_t subArrayId);


#endif /* DATABUFFER_H_ */
