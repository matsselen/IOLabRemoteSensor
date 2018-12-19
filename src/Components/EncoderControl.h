/***************************************************************************//**
    \file EncoderControl.h
    \brief Controls the driver for the Quadrature Encoder.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls the driver for the Quadrature Encoder and manages the
                   settings and configuration.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/EncoderControl.h$
    $Revision: 1$
    $Date: Wednesday, July 25, 2012 11:16:49 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ENCODERCONTROL_H_
#define ENCODERCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void EncControl_Init(void);
void EncControl_SetConfig(uint8_t control);
void EncControl_ReadEnc(void);

#endif /* ENCODERCONTROL_H_ */
