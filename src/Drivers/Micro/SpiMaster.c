/***************************************************************************//**
    \file SpiMaster.c
    \brief SPI Master Module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The SPI master module controls the SPI port and interfaces to
       other SPI slave devices.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/SpiMaster.c$
    $Revision: 7$
    $Date: Monday, July 02, 2012 6:32:27 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "SpiMaster.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

#include <Gpio.h>
#include <Board.h>
#include <Device.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Bit assignment in the transmit register for reading a byte */
#define READ_BIT (0x80u)

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Tracks if the module is initialized */
static volatile bool _initialized = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
/***************************************************************************//**
    Initialize the SPI hardware.
*//****************************************************************************/
void Spi_Init(void)
{
	ASSERT(_initialized == false, ErrorLedMode_SOFTWARE);

    Gpio_Configure(SPI_MASTER_CLK_PORT, SPI_MASTER_CLK_PIN, GpioType_PERIPHERAL_OUTPUT,
                   GpioValue_LOW, GpioDriveStrength_DEFAULT);

    Gpio_Configure(SPI_MASTER_MISO_PORT, SPI_MASTER_MISO_PIN, GpioType_PERIPHERAL_INPUT,
                   GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    Gpio_Configure(SPI_MASTER_MOSI_PORT, SPI_MASTER_MOSI_PIN, GpioType_PERIPHERAL_OUTPUT,
                   GpioValue_LOW, GpioDriveStrength_DEFAULT);

    /* reset the module to configure it */
    UCA0CTL1 = UCSWRST;

    /* set the clock to SMCLK assuming 24MHz */
    UCA0CTL1 |= UCSSEL__SMCLK;

    /* always set the modulation control to 0u when in SPI mode */
    UCA0MCTL = 0u;

    /* assumes that BRCLK is 24MHz, SPI_CLK = BRCLK / UCxxBR */
    UCA0BR1 = 0u;
    UCA0BR0 = 0x04u;

    /* data is captured on the first clock edge and changed on the following edge */
    /* inactive clock state is low, MSB */
    /* MSB first, 8-bit transfer, Master mode 3-pin SPI */

    /* NET TODO: make sure the clock polarity is right to support all 3 spi components */
    UCA0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
//    UCA0CTL0 = UCCKPL | UCMSB | UCMST | UCSYNC;

    /* enable the SPI module */
    UCA0CTL1 &= ~UCSWRST;

    _initialized = true;
}

/***************************************************************************//**
    Transmits a register address and value via SPI.

    \param[in] writeRegister - Selected control register.
    \param[in] dataByte - Data byte to send out over SPI.

    Note: This assumes that the SPI port has been configured as a MASTER device.

*//****************************************************************************/
void Spi_WriteRegister(uint8_t writeRegister, uint8_t dataByte)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    /* wait for the USCI_A0 TX buffer to empty */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* do nothing */
    }

    /* write the register we want to write to */
    UCA0TXBUF = writeRegister;

    /* wait for the USCI_A0 TX buffer to empty */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* do nothing */
    }

    /* Transmit the data */
    UCA0TXBUF = dataByte;

    /* wait for the USCI_A0 TX buffer to empty */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* do nothing */
    }
}

/***************************************************************************//**
    Reads a register via SPI.

    \param[in] readRegister - Selected control register.
    \param[in] dataByte - Pointer to where received byte should be stored.

*//****************************************************************************/
void Spi_ReadRegister(uint8_t readRegister, uint8_t * dataByte)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
    ASSERT(dataByte != NULL, ErrorLedMode_SOFTWARE);

    /* Wait for USCI_B2 TX buffer ready */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Transmit register address */
    UCA0TXBUF = readRegister | READ_BIT;
    /* Wait for USCI_B2 TX buffer ready */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Transmit a junk value in order to read in a byte */
    UCA0TXBUF = 0x00u;
    /* Wait for Tx complete */
    while ((UCA0STAT & UCBUSY) != 0u)
    {
        /* Do nothing */
    }
    *dataByte = UCA0RXBUF;
}

/***************************************************************************//**
    Reads a byte via SPI.

    \param[in] byte - The value written out the spi port

*//****************************************************************************/
extern void Spi_WriteByte(uint8_t byte)
{
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    /* wait for the USCI_A0 TX buffer to empty */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* do nothing */
    }

    /* write the value we want to write*/
    UCA0TXBUF = byte;

    /* wait for the USCI_A0 TX buffer to empty */
    while ((UCA0IFG & UCTXIFG) == 0u)
    {
        /* do nothing */
    }
}

