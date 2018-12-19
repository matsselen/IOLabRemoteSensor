/***************************************************************************//**
    \file Dac.h
    \brief Header file for DAC driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The DAC driver sets the voltage level
    				out of the DAC

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Dac.h$
    $Revision: 1$
    $Date: Monday, July 02, 2012 6:25:01 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DAC_H_
#define DAC_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void DacInit(void);
void DacConfig(void);
void DacSetVoltage(uint8_t voltage);

#endif /* DAC_H_ */
