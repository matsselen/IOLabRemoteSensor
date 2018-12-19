/***************************************************************************//**
    \file Ultrasound.h
    \brief Header file for Ultrasonic Transducer Circuit driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The Ultrasonic Transducer Circuit driver sets the voltage level
    				out of the DAC

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Ultrasound.h$
    $Revision: 1$
    $Date: Monday, July 02, 2012 6:25:01 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef ULTRASOUND_H_
#define ULTRASOUND_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void UltrasoundInit(void);
void UltrasoundStartTransmit(void);
void UltrasoundStopTransmit(void);

#endif /* ULTRASOUND_H_ */
