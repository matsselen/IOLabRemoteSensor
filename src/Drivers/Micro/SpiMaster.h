/***************************************************************************//**
    \file SpiMaster.h
    \brief SPI Master Module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The SPI master module controls the SPI port and interfaces to
       other SPI slave devices.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/SpiMaster.h$
    $Revision: 3$
    $Date: Wednesday, May 09, 2012 5:18:49 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef SPIMASTER_H_
#define SPIMASTER_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
extern void Spi_Init(void);
extern void Spi_WriteRegister(uint8_t writeRegister,
                          uint8_t dataByte);
extern void Spi_ReadRegister(uint8_t readRegister,
                         uint8_t *dataByte);
extern void Spi_WriteByte(uint8_t byte);
#endif /* SPIMASTER_H_ */
