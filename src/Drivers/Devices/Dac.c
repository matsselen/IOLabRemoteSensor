/***************************************************************************//**
    \file Dac.c
    \brief Driver for the DAC

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module interfaces to a SPI DAC to set the
                   output voltage. This driver is designed to work with a
                   TI DAC5311 SPI DAC.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Dac.c$
    $Revision: 1$
    $Date: Monday, July 02, 2012 6:25:01 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Dac.h"
#include <Clock.h>
#include <Gpio.h>
#include <Device.h>
#include <Board.h>
#include <SpiMaster.h>

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the chip select enable and disable for the gyroscope */
#define DAC_START_SPI_XFER  GPIO_CLEAR(CS_DAC_PORT, CS_DAC_PIN);
#define DAC_END_SPI_XFER    GPIO_SET(CS_DAC_PORT, CS_DAC_PIN);

/** Defines the number of microseconds to delay between de/asserting the chip select
    line and ending/starting the SPI communication to allow the chip to become ready
 */
#define SPI_CS_DELAY_US 1u


/*-----------------------LOCAL VARIABLES--------------------------------------*/


/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

void DacInit(void)
{
	/* do nothing */
}
void DacConfig(void);

/***************************************************************************//**
    Writes a value to a specific register using the SPI port.

    \param[in] voltage - the decimal equivalent of binary code used to
    set the voltage, given by the equation voltage = Vout/AVDD*2^8

*//****************************************************************************/
void DacSetVoltage(uint8_t voltage)
{
	uint8_t highByte = 0u;
	uint8_t lowByte = 0u;


	highByte = voltage>>2u;
	lowByte = voltage<<6u;
/*	lowByte = 0xFF; */


	DAC_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	Spi_WriteByte(highByte);
	Spi_WriteByte(lowByte);
/*	Spi_WriteByte(0x13);
	Spi_WriteByte(0x80); */
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	DAC_END_SPI_XFER;

}


