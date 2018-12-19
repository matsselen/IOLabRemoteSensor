/***************************************************************************//**
    \file Pmm.c
    \brief Power management module driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The power management module driver will interact with the
       microcontroller registers that control the power modes of the micro and
       peripherals.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Pmm.c$
    $Revision: 4$
    $Date: Wednesday, May 30, 2012 5:48:45 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "Pmm.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

#include <Device.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Maximum level that Vcore can be set to */
#define PMM_MAX_VCORE_LEVEL         (3u)

/** Password required to access PMM registers */
#define PMM_PASSWORD                (0xA5u)

/** Possible results of setting Vcore */
typedef enum PmmStatus
{
    PmmStatus_OKAY,
    PmmStatus_ERROR
} PmmStatus_t;
/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static PmmStatus_t _SetVCore(uint8_t level);
static PmmStatus_t _SetVCoreUp(uint8_t level);
static PmmStatus_t _SetVCoreDown(uint8_t level);

/***************************************************************************//**
    Initialize the power management module.

    Note that high-side refers to Vcc and low-side refers to Vcore.
*//****************************************************************************/
void Pmm_Init(void)
{
    PmmStatus_t status;

    /* set the system to core level 3 to allow full speed operation of 24 MHz with Vdd > 2.7V */
    status = _SetVCore(3u);
    ASSERT(status == PmmStatus_OKAY, ErrorLedMode_SOFTWARE);

    /* Open PMM registers for write access */
    PMMCTL0_H = PMM_PASSWORD;

    /* Disable interrupts and clear flags */
    PMMRIE = 0u;
    PMMIFG = 0u;

    /* Turn off low-side SVS and SVM.
       Mask events just in case.
    */
    SVSMLCTL &= ~(SVMLFP | SVMLE | SVMLOVPE |
                  SVSLFP | SVSLE | SVSMLACE | SVSLMD);
    SVSMLCTL |= SVSMLEVM;

    /* Turn off high-side SVM.
       Turn on high-side SVS.
    */
    SVSMHCTL &= ~(SVMHFP | SVMHE | SVMHOVPE);
    SVSMHCTL |= SVSHE;

    /* Clear high-side delay flag */
    PMMIFG &= ~(SVSMHDLYIFG);

    /* Put high-side SVS in full performance mode with automatic hardware control
       and active in LPM 2/3/4 (causes SVS to enter normal mode in LPM 2/3/4).
       Ensure events are not masked.
    */
    SVSMHCTL |= (SVSHFP | SVSMHACE | SVSHMD);
    SVSMHCTL &= ~(SVSMHEVM);

    /* Wait until high-side SVM (and SVS) is settled */
    while ((PMMIFG & SVSMHDLYIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Clear flags */
    PMMIFG = 0u;

    /* Enable power-on reset (POR) if high-side voltage drops too low */
    PMMRIE = SVSHPE;

    /* Lock PMM registers to prevent write access */
    PMMCTL0_H = 0x00u;
}

/***************************************************************************//**
    Set Vcore to given level.

    \param[in] level - Level to which Vcore needs to be set

    \return PmmStatus_OKAY if successful, otherwise PmmStatus_ERROR
*//****************************************************************************/
static PmmStatus_t _SetVCore(uint8_t level)
{
    uint8_t actualLevel;
    PmmStatus_t status = PmmStatus_OKAY;

    ASSERT(level <= PMM_MAX_VCORE_LEVEL, ErrorLedMode_SOFTWARE);

    /* Get current Vcore level */
    actualLevel = (uint8_t) (PMMCTL0 & PMMCOREV_3);

    /* Increase/decrease level one step at a time */
    while ((level != actualLevel) && (status == PmmStatus_OKAY))
    {
        if (level > actualLevel)
        {
            ++actualLevel;
            status = _SetVCoreUp(actualLevel);
        }
        else
        {
            --actualLevel;
            status = _SetVCoreDown(actualLevel);
        }
    }

    return status;
}

/***************************************************************************//**
    Increase Vcore by one level

    \param[in] level - Level to which Vcore needs to be increased

    \return PmmStatus_OKAY if successful, otherwise PmmStatus_ERROR
*//****************************************************************************/
static PmmStatus_t _SetVCoreUp(uint8_t level)
{
    uint16_t PMMRIE_backup;
    uint16_t SVSMHCTL_backup;
    uint16_t SVSMLCTL_backup;
    PmmStatus_t status = PmmStatus_OKAY;

    ASSERT(level <= PMM_MAX_VCORE_LEVEL, ErrorLedMode_SOFTWARE);

    /* The code flow for increasing the Vcore has been altered to work around
       the erratum FLASH37.
       Please refer to the Errata sheet to know if a specific device is affected
       DO NOT ALTER THIS FUNCTION
    */

    /* Open PMM registers for write access */
    PMMCTL0_H = PMM_PASSWORD;

    /* Disable dedicated interrupts */
    /* Backup all registers */
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE | SVSLPE | SVMHVLRIE |
                SVMHIE | SVSMHDLYIE | SVMLVLRIE | SVMLIE | SVSMLDLYIE);
    SVSMHCTL_backup = SVSMHCTL;
    SVSMLCTL_backup = SVSMLCTL;

    /* Clear flags */
    PMMIFG = 0u;

    /* Set SVM highside to new level and check if a VCore increase is possible */
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);

    /* Wait until SVM highside is settled */
    while ((PMMIFG & SVSMHDLYIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Clear flag */
    PMMIFG &= ~SVSMHDLYIFG;

    /* Check if a VCore increase is possible */
    if ((PMMIFG & SVMHIFG) == SVMHIFG)
    {
        /* Vcc is too low for a Vcore increase */

        /* recover the previous settings */
        PMMIFG &= ~SVSMHDLYIFG;
        SVSMHCTL = SVSMHCTL_backup;

        /* Wait until SVM highside is settled */
        while ((PMMIFG & SVSMHDLYIFG) == 0u)
        {
            /* Do nothing */
        }

        status = PmmStatus_ERROR;
    }
    else
    {
        /* Vcc is high enough for a Vcore increase */

        /* Set also SVS highside to new level */
        SVSMHCTL |= (SVSHRVL0 * level);

        /* Wait until SVM highside is settled */
        while ((PMMIFG & SVSMHDLYIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Clear flag */
        PMMIFG &= ~SVSMHDLYIFG;

        /* Set VCore to new level */
        PMMCTL0_L = (uint8_t) (PMMCOREV0 * level);

        /* Set SVM, SVS low side to new level */
        SVSMLCTL = SVMLE | (SVSMLRRL0 * level) | SVSLE | (SVSLRVL0 * level);

        /* Wait until SVM, SVS low side is settled */
        while ((PMMIFG & SVSMLDLYIFG) == 0u)
        {
            /* Do nothing */
        }

        /* Clear flag */
        PMMIFG &= ~SVSMLDLYIFG;

        /* SVS, SVM low and high side are now set to protect for the new core level */

        /* Restore Low side settings
           Clear all other bits _except_ level settings
        */
        SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

        /* Clear level settings in the backup register, keep all other bits */
        SVSMLCTL_backup &= ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

        /* Restore low-side SVS monitor settings */
        SVSMLCTL |= SVSMLCTL_backup;

        /* Restore High side settings
           Clear all other bits except level settings
        */
        SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

        /* Clear level settings in the backup register, keep all other bits */
        SVSMHCTL_backup &= ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

        /* Restore backup */
        SVSMHCTL |= SVSMHCTL_backup;

        /* Wait until high side and low side are settled */
        while ((PMMIFG & SVSMLDLYIFG) == 0u)
        {
            /* Do nothing */
        }
        while ((PMMIFG & SVSMHDLYIFG) == 0u)
        {
            /* Do nothing */
        }
    }

    /* Clear all flags */
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);

    /* Restore PMM interrupt enable register */
    PMMRIE = PMMRIE_backup;

    /* Lock PMM registers to prevent write access */
    PMMCTL0_H = 0x00u;

    return status;
}

/***************************************************************************//**
    Decrease Vcore by one level

    \param[in] level - Level to which Vcore needs to be decreased

    \return PmmStatus_OKAY if successful, otherwise PmmStatus_ERROR
*//****************************************************************************/
static PmmStatus_t _SetVCoreDown(uint8_t level)
{
    uint16_t PMMRIE_backup;
    uint16_t SVSMHCTL_backup;
    uint16_t SVSMLCTL_backup;

    ASSERT(level <= PMM_MAX_VCORE_LEVEL, ErrorLedMode_SOFTWARE);

    /* The code flow for decreasing the Vcore has been altered to work around
       the erratum FLASH37.
       Please refer to the Errata sheet to know if a specific device is affected
       DO NOT ALTER THIS FUNCTION
    */
    // TODO: spg - find out if this could be cleaned up for different parts

    /* Open PMM registers for write access */
    PMMCTL0_H = PMM_PASSWORD;

    /* Disable dedicated Interrupts
       Backup all registers
    */
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE | SVSLPE | SVMHVLRIE |
                SVMHIE | SVSMHDLYIE | SVMLVLRIE | SVMLIE | SVSMLDLYIE);
    SVSMHCTL_backup = SVSMHCTL;
    SVSMLCTL_backup = SVSMLCTL;

    /* Clear flags */
    PMMIFG &= ~(SVMHIFG | SVSMHDLYIFG | SVMLIFG | SVSMLDLYIFG);

    /* Set SVM, SVS high & low side to new settings in normal mode */
    SVSMHCTL = SVMHE | (SVSMHRRL0 * level) | SVSHE | (SVSHRVL0 * level);
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level) | SVSLE | (SVSLRVL0 * level);

    /* Wait until SVM high side and SVM low side are settled */
    while ((PMMIFG & SVSMHDLYIFG) == 0u)
    {
        /* Do nothing */
    }
    while ((PMMIFG & SVSMLDLYIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Clear flags */
    PMMIFG &= ~(SVSMHDLYIFG + SVSMLDLYIFG);

    /* SVS, SVM low and high side are now set to protect for the new core level */

    /* Set VCore to new level */
    PMMCTL0_L = (uint8_t) (PMMCOREV0 * level);

    /* Restore Low side settings
       Clear all other bits _except_ level settings
    */
    SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    /* Clear level settings in the backup register,keep all other bits */
    SVSMLCTL_backup &= ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    /* Restore low-side SVS monitor settings */
    SVSMLCTL |= SVSMLCTL_backup;

    /* Restore High side settings
       Clear all other bits except level settings
    */
    SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    /* Clear level settings in the backup register, keep all other bits */
    SVSMHCTL_backup &= ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    /* Restore backup */
    SVSMHCTL |= SVSMHCTL_backup;

    /* Wait until high side and low side are settled */
    while ((PMMIFG & SVSMHDLYIFG) == 0u)
    {
        /* Do nothing */
    }
    while ((PMMIFG & SVSMLDLYIFG) == 0u)
    {
        /* Do nothing */
    }

    /* Clear all Flags */
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG | SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG);

    /* Restore PMM interrupt enable register */
    PMMRIE = PMMRIE_backup;

    /* Lock PMM registers to prevent write access */
    PMMCTL0_H = 0x00u;

    return PmmStatus_OKAY;
}



