/***************************************************************************//**
    \file Dma.h
    \brief Configuration and control functions and macros for the DMA

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Provides configuration and control for the direct memory
                   access (DMA) controller on the MSP430F5xx family.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Dma.h$
    $Revision: 2$
    $Date: Wednesday, May 16, 2012 5:23:11 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef DMA_H_
#define DMA_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines the available DMA channels */
typedef enum DmaChannel
{
	DmaChannel_0 = 0u,
	DmaChannel_1 = 1u,
	DmaChannel_2 = 2u
} DmaChannel_t;

/** Defines the possible DMA triggers for any DMA channel. The are usually part specific
    and these are defined for the MSP430F532x */
typedef enum DmaTrigger
{
	DmaTrigger_DMAREQ    =  0u,
	DmaTrigger_TA0CCR0   =  1u,
	DmaTrigger_TA0CCR2   =  2u,
	DmaTrigger_TA1CCR0   =  3u,
	DmaTrigger_TA1CCR2   =  4u,
	DmaTrigger_TA2CCR0   =  5u,
	DmaTrigger_TA2CCR2   =  6u,
	DmaTrigger_TB0CCR0   =  7u,
	DmaTrigger_TB0CCR2   =  8u,
	/* 9-15 are reserved */
	DmaTrigger_UCA0RXIFG = 16u,
	DmaTrigger_UCA0TXIFG = 17u,
	DmaTrigger_UCB0RXIFG = 18u,
	DmaTrigger_UCB0TXIFG = 19u,
	DmaTrigger_UCA1RXIFG = 20u,
	DmaTrigger_UCA1TXIFG = 21u,
	DmaTrigger_UCB1RXIFG = 22u,
	DmaTrigger_UCB1TXIFG = 23u,
	DmaTrigger_ADC12IFGx = 24u,
	/* 25 - 28 are reserved */
	DmaTrigger_MPY_READY = 29u,
	DmaTrigger_DMAxIFG   = 30u,
	DmaTrigger_DMAEO     = 31u
} DmaTrigger_t;

/** Defines the ways that the DMA can increment after a transfer */
typedef enum DmaIncr
{
	DmaIncr_NONE      = 1u,
	DmaIncr_DECREMENT = 2u,
	DmaIncr_INCREMENT = 3u
} DmaIncr_t;

/** Defines the transfer modes that the DMA can perform */
typedef enum DmaMode
{
	DmaMode_SINGLE                = 0x0000,
	DmaMode_BLOCK                 = 0x1000,
	DmaMode_BURST_BLOCK           = 0x2000,
	DmaMode_SINGLE_REPEATING      = 0x4000,
	DmaMode_BLOCK_REPEATING       = 0x5000,
	DmaMode_BURST_BLOCK_REPEATING = 0x6000
} DmaMode_t;

/** Defines the bit width of the DMA transfer */
typedef enum DmaBitWidth
{
	DmaBitWidth_WORD = 0x00,
	DmaBitWidth_BYTE = 0x01
} DmaBitWidth_t;

/** Defines a structure that holds all the configuration information
    to set up a DMA channel.
 */
typedef struct DmaConfig
{
	DmaMode_t mode;
	DmaTrigger_t trigger;
	DmaIncr_t sourceIncr;
	DmaIncr_t destinationIncr;
	DmaBitWidth_t sourceBitWidth;
	DmaBitWidth_t destinationBitWidth;
} DmaConfig_t;

/** Defines a DMA ISR function pointer */
typedef void (*DmaIsrPointer_t)(void);

/*-----------------------PUBLIC MACROS----------------------------------------*/
/** Define the number for each of the DMA channels */
#define DMA_CHANNEL_0    0
#define DMA_CHANNEL_1    1
#define DMA_CHANNEL_2    2

/** \name DMA enable and disable macros.
    \{
*/
/***************************************************************************//**
    \brief Enables DMA channel

    \param[in] channel - DMA channel number
*//****************************************************************************/
#define DMA_ENABLE(channel)     DMA_ENABLE_PREP(channel)

/***************************************************************************//**
    \brief Disables DMA channel

    \param[in] channel - DMA channel number
*//****************************************************************************/
#define DMA_DISABLE(channel)    DMA_DISABLE_PREP(channel)

/***************************************************************************//**
    \brief Enables interrupts for a DMA channel

    \param[in] channel - DMA channel number
*//****************************************************************************/
#define DMA_INT_ENABLE(channel)    DMA_INT_ENABLE_PREP(channel)

/***************************************************************************//**
    \brief Disables interrupts for a DMA channel

    \param[in] channel - DMA channel number
*//****************************************************************************/
#define DMA_INT_DISABLE(channel)    DMA_INT_DISABLE_PREP(channel)

/** \} */

/*-----------------------INTERNAL MACROS--------------------------------------*/
/* (The macros above need a second round in the preprocessor) */
/** \name Internal DMA enable and disable macros.
    \{
*/
#define DMA_ENABLE_PREP(channel)  (DMA##channel##CTL  = DMA##channel##CTL |  DMAEN)
#define DMA_DISABLE_PREP(channel) (DMA##channel##CTL  = DMA##channel##CTL & ~DMAEN)

#define DMA_INT_ENABLE_PREP(channel)      (DMA##channel##CTL  = DMA##channel##CTL |  DMAIE)
#define DMA_INT_DISABLE_PREP(channel)     (DMA##channel##CTL  = DMA##channel##CTL & ~DMAIE)
/** \} */
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Dma_Init(void);
void Dma_Configure(DmaChannel_t channel,
                   const DmaConfig_t *config);
void Dma_Arm(DmaChannel_t channel,
             void* sourceAddress,
             void* destinationAddress,
             uint16_t dmaTransferSize);
void Dma_Enable(DmaChannel_t channel);
void Dma_Disable(DmaChannel_t channel);
void Dma_Trigger(DmaChannel_t channel);
void Dma_RegisterIsr(DmaChannel_t channel,
                     DmaIsrPointer_t isrPtr);
void Dma_UnregisterIsr(DmaChannel_t channel);
void Dma_EnableIsr(DmaChannel_t channel);
void Dma_DisableIsr(DmaChannel_t channel);
#endif /* DMA_H_ */
