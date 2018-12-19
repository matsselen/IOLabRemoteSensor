/***************************************************************************//**
    \file Dma.c
    \brief Configuration and control functions and macros for the DMA

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Provides configuration and control for the direct memory
                   access (DMA) controller on the MSP430F5xx family.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Dma.c$
    $Revision: 3$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include <Device.h>
#include "Dma.h"

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the number of DMA channels available */
#define DMA_NUM_CHANNELS     3u

/** Defines the number of trigger registers there are */
#define DMA_NUM_TRIGGER_REGISTERS    4u
/*-----------------------LOCAL CONSTANTS--------------------------------------*/
/** Look up table for DMA control registers */
static volatile uint16_t* const _dmaControlRegisters[DMA_NUM_CHANNELS] =
{
		&DMA0CTL,
		&DMA1CTL,
		&DMA2CTL
};

/** Look up table for the DMA trigger registers */
static volatile uint16_t* const _dmaTriggerRegisters[DMA_NUM_TRIGGER_REGISTERS] =
{
		&DMACTL0,
		&DMACTL1,
		&DMACTL2,
		&DMACTL3
};

/** Look up table for the DMA source address registers */
static volatile uint32_t* const _dmaSourceAddressRegisters[DMA_NUM_CHANNELS] =
{
		/* the compiler uses SRF20(DMAxSA) typedef to __SFR_FARPTR * which can
		   be cast into a pointer for accessing the data
		*/
		(uint32_t*)(&DMA0SA),
		(uint32_t*)(&DMA1SA),
		(uint32_t*)(&DMA2SA)
};

/** Look up table for the DMA destination address registers */
static volatile uint32_t* const _dmaDestinationAddressRegisters[DMA_NUM_CHANNELS] =
{
		/* the compiler uses SRF20(DMAxDA) typedef to __SFR_FARPTR * which can
		   be cast into a pointer for accessing the data
		*/
		(uint32_t*)(&DMA0DA),
		(uint32_t*)(&DMA1DA),
		(uint32_t*)(&DMA2DA)
};

/** Look up table for the DMA size registers */
static volatile uint16_t* const _dmaSizeRegisters[DMA_NUM_CHANNELS] =
{
		&DMA0SZ,
		&DMA1SZ,
		&DMA2SZ
};
/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the DMA has been initialized */
static volatile bool _dmaInitialized = false;

/** Tracks if the DMA channel has been configured */
static volatile bool _dmaConfigured[DMA_NUM_CHANNELS] = { false, false, false };

/** Tracks if the DMA channel is armed */
static volatile bool _dmaArmed[DMA_NUM_CHANNELS] = { false, false, false };

/** Stores the DMA ISR pointers */
static DmaIsrPointer_t _dmaIsrList[DMA_NUM_CHANNELS] = { NULL, NULL, NULL };

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
__interrupt void _DmaIsr(void);

/***************************************************************************//**
    Initializes the DMA.
*//****************************************************************************/
void Dma_Init( void )
{
	/* Disable the DMA from interrupting read-modify-write operations by the CPU.
	   This is a workaround for DMA4 Errata, in which a write to a 20-bit address
	   is corrupted by any DMA transfer.
	*/
	DMACTL4 = DMARMWDIS;
	_dmaInitialized = true;
}

/***************************************************************************//**
    Configures the DMA to be ready to use.

    \param[in] channel - the DMA channel to configure
    \param[in] config - the configuration struct of configuration options

*//****************************************************************************/
void Dma_Configure(DmaChannel_t channel,
                   const DmaConfig_t *config)
{
	/** Defines the number of channels per trigger register */
	#define DMA_CHANNELS_PER_TRIG_REG  2

	ASSERT(_dmaInitialized, ErrorLedMode_SOFTWARE);
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);
	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);

	volatile uint16_t* dmaCtrlReg = _dmaControlRegisters[channel];

	/* each trigger register is for 2 channels, so the trigger register is
	   the DMA channel / 2. If the channel number is odd the configuration is
	   in the high byte.
	*/
	volatile uint16_t* dmaTrigReg = _dmaTriggerRegisters[channel/DMA_CHANNELS_PER_TRIG_REG];

	/* Ensure that the DMA is disabled before changing its configuration */
	*dmaCtrlReg = 0;

	/* set the DMA mode */
	*dmaCtrlReg |= config->mode;

	/* set the DMA source and destination increment and size */
	*dmaCtrlReg |= (((uint16_t)(config->sourceIncr)) * DMASRCINCR0) +
                   (((uint16_t)(config->sourceBitWidth)) * DMASRCBYTE) +
                   (((uint16_t)(config->destinationIncr)) * DMADSTINCR0) +
                   (((uint16_t)(config->destinationBitWidth)) * DMADSTBYTE);

	/* set the trigger source */
	if ((channel % 2) == 0) /* is even */
	{
		/* even channels are in the low byte of the register. Make sure old value
		   is clear before setting new value
		*/
		*dmaTrigReg = ((uint16_t)(*dmaTrigReg & 0xFF00)) | ((uint16_t)(config->trigger));
	}
	else /* is odd */
	{
		/* odd channels are in the high byte of the register. Make sure old value
		   is clear before setting new value
		*/
		*dmaTrigReg = ((uint16_t)(*dmaTrigReg & 0x00FF)) | (((uint16_t)(config->trigger))<<8u);
	}

	_dmaConfigured[channel] = true;
}

/***************************************************************************//**
    Arms the DMA to make a DMA transaction as soon as it is enabled and triggered.

    \param[in] channel - the DMA channel to arm
    \param[in] sourceAddress - the address that is the source of the DMA transfer
    \param[in] destinationAddress - the address that is the destination of the DMA transfer
    \param[in] dmaTransferSize - the number of DMA transfers to make in a transaction

	\note This function does not enable the DMA. Use Dma_Enable or the enable macro
	      to enable the DMA.
*//****************************************************************************/
void Dma_Arm(DmaChannel_t channel,
             void* sourceAddress,
             void* destinationAddress,
             uint16_t dmaTransferSize)
{
	ASSERT(_dmaInitialized, ErrorLedMode_SOFTWARE);
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);
	ASSERT(_dmaConfigured[channel], ErrorLedMode_SOFTWARE);
	ASSERT((sourceAddress != NULL) && (destinationAddress != NULL), ErrorLedMode_SOFTWARE);

	*_dmaSourceAddressRegisters[channel] = (uint32_t)(sourceAddress);
	*_dmaDestinationAddressRegisters[channel] = (uint32_t)(destinationAddress);
	*_dmaSizeRegisters[channel] = dmaTransferSize;

	_dmaArmed[channel] = true;
}

/***************************************************************************//**
    Enables the DMA to perform a transaction as soon as it is triggered.

    \param[in] channel - the DMA channel to enable

	\note This function provides better safety checking (verifying that the DMA
	      is configured and armed) than the macro but the enable macro takes
	      fewer clock cycles.
*//****************************************************************************/
void Dma_Enable(DmaChannel_t channel)
{
	ASSERT(_dmaInitialized, ErrorLedMode_SOFTWARE);
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);
	ASSERT(_dmaConfigured[channel], ErrorLedMode_SOFTWARE);
	ASSERT(_dmaArmed[channel], ErrorLedMode_SOFTWARE);

	*_dmaControlRegisters[channel] |= DMAEN;
}

/***************************************************************************//**
    Disables the DMA.

    \param[in] channel - the DMA channel to disable

	\note This function provides better safety checking (verifying that the DMA
	      channel is valid) than the macro but the disable macro takes fewer
	      clock cycles.
*//****************************************************************************/
void Dma_Disable(DmaChannel_t channel)
{
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);

	*_dmaControlRegisters[channel] &= ~DMAEN;
}

/***************************************************************************//**
    Triggers the DMA to start a transaction.

    \param[in] channel - the DMA channel to trigger
*//****************************************************************************/
void Dma_Trigger(DmaChannel_t channel)
{
	ASSERT(_dmaInitialized, ErrorLedMode_SOFTWARE);
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);
	ASSERT(_dmaConfigured[channel], ErrorLedMode_SOFTWARE);
	ASSERT(_dmaArmed[channel], ErrorLedMode_SOFTWARE);

	*_dmaControlRegisters[channel] |= DMAREQ;
}

/***************************************************************************//**
    Register an ISR for a DMA channel.

    \param[in] channel - the DMA channel for the ISR
    \param[in] isrPtr - pointer to the ISR
*//****************************************************************************/
void Dma_RegisterIsr(DmaChannel_t channel,
                     DmaIsrPointer_t isrPtr)
{
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);
	ASSERT(_dmaIsrList[channel] == NULL, ErrorLedMode_SOFTWARE);

	/* disable interrupts for selected channel */
	*_dmaControlRegisters[channel] &= ~DMAIE;

	/* set the ISR pointer */
	_dmaIsrList[channel] = isrPtr;
}

/***************************************************************************//**
    Unregister an ISR for a DMA channel.

    \param[in] channel - the DMA channel for the ISR to unregister
*//****************************************************************************/
void Dma_UnregisterIsr(DmaChannel_t channel)
{
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);

	/* disable the interrupt */
	*_dmaControlRegisters[channel] &= ~DMAIE;

	/* erase the ISR pointer */
	_dmaIsrList[channel] = NULL;
}

/***************************************************************************//**
    Enables the ISR for a DMA channel.

    \param[in] channel - the DMA channel for the ISR

    \note This function provides better safety checking (verifying that the DMA
	      channel is valid) than the macro but the macro takes fewer
	      clock cycles.
*//****************************************************************************/
void Dma_EnableIsr(DmaChannel_t channel)
{
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);

	*_dmaControlRegisters[channel] |= DMAIE;
}

/***************************************************************************//**
    Disables the ISR for a DMA channel.

    \param[in] channel - the DMA channel for the ISR

    \note This function provides better safety checking (verifying that the DMA
	      channel is valid) than the macro but the macro takes fewer
	      clock cycles.
*//****************************************************************************/
void Dma_DisableIsr(DmaChannel_t channel)
{
	ASSERT(channel < DMA_NUM_CHANNELS, ErrorLedMode_SOFTWARE);

	*_dmaControlRegisters[channel] &= ~DMAIE;
}

/***************************************************************************//**
    Interrupt service routine for the DMA.  The interrupt flag of the interrupt
    specified in DMAIV is cleared when DMAIV is read.
*//****************************************************************************/
#pragma vector=DMA_VECTOR
__interrupt void _DmaIsr(void)
{
    switch (_even_in_range(DMAIV, DMAIV_DMA2IFG))
    {
        case DMAIV_DMA0IFG:
        	if (_dmaIsrList[0] != NULL)
        	{
        		(*_dmaIsrList[0])();
        	}
            break;

        case DMAIV_DMA1IFG:
        	if (_dmaIsrList[1] != NULL)
			{
				(*_dmaIsrList[1])();
			}
            break;

        case DMAIV_DMA2IFG:
        	if (_dmaIsrList[2] != NULL)
			{
				(*_dmaIsrList[2])();
			}
            break;

        case DMAIV_NONE:
        	/* If the interrupt is being disabled as the interrupt is firing, the ISR
        	   could get called when no flags are set. Need to handle that here.
        	*/
        	break;

        default:
            _never_executed();
            break;
    }

    _low_power_mode_off_on_exit();
}
