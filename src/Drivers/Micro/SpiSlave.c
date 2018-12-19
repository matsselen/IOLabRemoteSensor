/***************************************************************************//**
    \file SpiSlave.c
    \brief SPI Slave Module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module is used when this device is a SPI slave device. The
       functions and macros defined in the module can be used to communicate with
       a SPI master device.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/SpiSlave.c$
    $Revision: 3$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "SpiSlave.h"
#include <Device.h>
#include <Dma.h>
#include <Assert.h>
#include <Gpio.h>
#include <Board.h>
#include <EventQueue.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Defines the settings for using the DMA to receive data */
static const DmaConfig_t _dmaConfigRx = { DmaMode_SINGLE,
                                          DmaTrigger_UCB1RXIFG,
                                          DmaIncr_NONE,
                                          DmaIncr_INCREMENT,
                                          DmaBitWidth_BYTE,
                                          DmaBitWidth_BYTE
                                        };
static const DmaChannel_t _dmaChannelRx = DmaChannel_0;

/** Defines the settings for using the DMA to transmit data */
static const DmaConfig_t _dmaConfigTx = { DmaMode_SINGLE,
                                          DmaTrigger_UCB1TXIFG,
                                          DmaIncr_INCREMENT,
                                          DmaIncr_NONE,
                                          DmaBitWidth_BYTE,
                                          DmaBitWidth_BYTE
                                         };
static const DmaChannel_t _dmaChannelTx = DmaChannel_1;

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Tracks if the transmit buffer is being used */
static volatile bool _bufferTxInUse = false;
/** Tracks if the DMA is finished with transmit buffer or not */
static volatile bool _bufferTxComplete = false;

/** Tracks if the receive buffer is being used */
static volatile bool _bufferRxInUse = false;
/** Tracks if the DMA is finished with the receive buffer or not */
static volatile bool _bufferRxComplete  = false;

/** Tracks if the module has been initialized */
static volatile bool _initialized = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _HandleDmaRxIsr( void );
static void _HandleDmaTxIsr( void );

/***************************************************************************//**
    Initialize the SPI slave module.

*//****************************************************************************/
void SpiSlave_Init(void)
{
	/* configure the port pins */
    Gpio_Configure(SPI_SLAVE_CLK_PORT, SPI_SLAVE_CLK_PIN, GpioType_PERIPHERAL_INPUT,
                   GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    Gpio_Configure(SPI_SLAVE_MISO_PORT, SPI_SLAVE_MISO_PIN, GpioType_PERIPHERAL_OUTPUT,
                   GpioValue_LOW, GpioDriveStrength_DEFAULT);

    Gpio_Configure(SPI_SLAVE_MOSI_PORT, SPI_SLAVE_MOSI_PIN, GpioType_PERIPHERAL_INPUT,
                   GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    Gpio_Configure(CS_RADIO_PORT, CS_RADIO_PIN, GpioType_PERIPHERAL_INPUT,
                   GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

    /* put the module in reset to configure it */
    UCB1CTL1 = UCSWRST;

    /* data is captured on the first clock edge and changed on the following edge */
    /* inactive clock state is low, MSB first*/
    /* 8-bit transfer, Slave mode 4-pin SPI, chip select is active low */
    UCB1CTL0 = UCCKPH | UCMSB | UCMODE_2 | UCSYNC;

    /* enable the SPI module */
    UCB1CTL1 &= ~UCSWRST;

    Dma_Configure(_dmaChannelRx, &_dmaConfigRx);
    Dma_Configure(_dmaChannelTx, &_dmaConfigTx);

    Dma_RegisterIsr(_dmaChannelRx, &_HandleDmaRxIsr);
    Dma_RegisterIsr(_dmaChannelTx, &_HandleDmaTxIsr);

    _initialized = true;
}

/***************************************************************************//**
    Transmit a packet over the SPI slave port. The data will not be transmitted
    until the SPI master device clocks the data out. This function will store the
    pointer and size of the packet to be transmitted when the master asks for the
    data.

    \param[in]  packetLength - number of bytes in packet.
    \param[in] packet - packet to transmit.

    \retval SpiSlaveError_OK - Success
    \retval SpiSlaveError_BUFFER_IN_USE - a packet is already waiting to be
            transmitted.

    \note The packet must be > 1 bytes (at least 2 bytes) to transmit.
*//****************************************************************************/
SpiSlaveError_t SpiSlave_LoadPacket(uint16_t packetLength,
                                    const uint8_t packet[])
{
	/** Create pointer to transmit buffer */
	static volatile uint8_t* const spiTxBuffer = &UCB1TXBUF;

	SpiSlaveError_t error = SpiSlaveError_OK;

	/* make sure the packet has > 1 element because the DMA needs to load a packet
	   to start the DMA trigger and have at least one element for the DMA to transfer.
	*/
	ASSERT(packetLength > 1u, ErrorLedMode_SOFTWARE);
	ASSERT(packet != NULL, ErrorLedMode_SOFTWARE);

	if (_bufferTxInUse == false)
	{
		_bufferTxInUse = true;
		_bufferTxComplete = false;
		Dma_Arm(_dmaChannelTx, (void*)&packet[1], (void*)spiTxBuffer, packetLength - 1);
		Dma_EnableIsr(_dmaChannelTx);
		Dma_Enable(_dmaChannelTx);
		*spiTxBuffer = packet[0];
	}
	else
	{
		error = SpiSlaveError_BUFFER_IN_USE;
	}

	return error;
}

/***************************************************************************//**
    Disarms the SPI port transmit by disabling the DMA
*//****************************************************************************/
void SpiSlave_UnloadPacket(void)
{
	Dma_DisableIsr(_dmaChannelTx);
	Dma_Disable(_dmaChannelTx);
	_bufferTxInUse = false;
	_bufferTxComplete = false;
}

/***************************************************************************//**
    Disarms the SPI port transmit by disabling the DMA
*//****************************************************************************/
bool SpiSlave_PacketLoaded(void)
{
	return ((_bufferTxComplete == false) && (_bufferTxInUse));
}
/***************************************************************************//**
    Read a packet received over the SPI slave port into a buffer when it arrives.
    The buffer will be used to store the received SPI information after it
    arrives. The function will store the pointer and size to the buffer and
    return but the SpiSlave_PacketAvailable will tell when the data in the
    buffer becomes valid.

    \param[in]  packetLength - number of bytes in packet to receive.
    \param[out] packet - Received packet.

    \retval SpiSlaveError_OK - Success
    \retval SpiSlaveError_BUFFER_IN_USE - There is already a read buffer waiting
            to be filled.
*//****************************************************************************/
SpiSlaveError_t SpiSlave_ReadPacket(uint16_t packetLength,
                         uint8_t  packet[])
{
	static volatile uint8_t* const spiRxBuffer = &UCB1RXBUF;

	SpiSlaveError_t errorType = SpiSlaveError_OK;

	ASSERT(packetLength > 0u, ErrorLedMode_SOFTWARE);
	ASSERT(packet != NULL, ErrorLedMode_SOFTWARE);

	if(_bufferRxInUse == false)
	{
		_bufferRxInUse = true;
		_bufferRxComplete = false;

		Dma_Arm(_dmaChannelRx, (void*)spiRxBuffer, &packet[0], packetLength);
		Dma_EnableIsr(_dmaChannelRx);
		Dma_Enable(_dmaChannelRx);
	}
	else
	{
		errorType = SpiSlaveError_BUFFER_IN_USE;
	}

	return errorType;
}

/***************************************************************************//**
    Disarms the SPI port from accepting new data. Disables the DMA so that received
    bytes won't be transferred into the memory.
*//****************************************************************************/
void SpiSlave_FlushRead(void)
{
	Dma_DisableIsr(_dmaChannelRx);
	Dma_Disable(_dmaChannelRx);
	_bufferRxInUse = false;
	_bufferRxComplete = false;
}
/***************************************************************************//**
    Determine if a packet is available.

    \return True if a packet is available, otherwise false.
*//****************************************************************************/
bool SpiSlave_PacketAvailable(void)
{
	return (_bufferRxComplete && _bufferRxInUse);
}

/***************************************************************************//**
    Handles the interrupt generated by the DMA when the receive transfer is
    complete.
*//****************************************************************************/
static void _HandleDmaRxIsr( void )
{
	static const Event_t spiRxEvent = Event_SPI_SLAVE_RX_COMPLETE;
	static const EventPriority_t spiRxEventPriority = EventPriority_HIGH;
    _bufferRxComplete = true;
    EventQueue_Add(spiRxEvent, spiRxEventPriority);
}

/***************************************************************************//**
    Handles the interrupt generated by the DMA when the transmit transfer is
    complete.
*//****************************************************************************/
static void _HandleDmaTxIsr( void )
{
	static const Event_t spiTxEvent = Event_SPI_SLAVE_TX_COMPLETE;
	static const EventPriority_t spiTxEventPriority = EventPriority_HIGH;
    _bufferTxComplete = true;
    EventQueue_Add(spiTxEvent, spiTxEventPriority);
}
