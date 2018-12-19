/***************************************************************************//**
    \file Encoder.h
    \brief Hardware driver for the Quadrature Encoder.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The quadrature encoder uses a a direction and count input to
                   count how many pulses and in which direction a specially
                   designed encoder moved.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Encoder.h$
    $Revision: 2$
    $Date: Monday, July 30, 2012 10:47:14 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ENCODER_H_
#define ENCODER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the status options for the encoder */
typedef enum EncoderStatus
{
	EncoderStatus_INVALID,
	EncoderStatus_OKAY,
	EncoderStatus_CAPTURE_OVERFLOW,
	EncoderStatus_SW_CAPTURE_OVERFLOW,
	EncoderStatus_BUFFER_OVERFLOW,
	EncoderStatus_OTHER_ERROR
} EncoderStatus_t;
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Encoder_Init(void);
void Encoder_Start(void);
void Encoder_Stop(void);
EncoderStatus_t Encoder_Read(
		int16_t counts[],
		uint16_t *elements,
		uint16_t maxElements);

#endif /* ENCODER_H_ */
