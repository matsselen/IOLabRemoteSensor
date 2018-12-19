/***************************************************************************//**
    \file Timer.c
    \brief Hardware timer module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Provides functions to setup the hardware timers. The control
       of the timers and their interrupts are handled using the macros defined
       in the header file.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Timer.c$
    $Revision: 5$
    $Date: Tuesday, August 07, 2012 11:51:57 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <Device.h>
#include "Timer.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Number of compare/capture modules within timer A0 */
#define TIMER_A0_NUM_CC_MODULES         (5u)
/** Number of compare/capture modules within timer A1 */
#define TIMER_A1_NUM_CC_MODULES         (3u)
/** Number of compare/capture modules within timer A2 */
#define TIMER_A2_NUM_CC_MODULES         (3u)
/** Number of compare/capture modules within timer B0 */
#define TIMER_B0_NUM_CC_MODULES         (7u)

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
/***************************************************************************//**
    Configure timer A0 using the given parameters.  If the selected run mode
    is either Up or Up/Down, the compare/capture 0 module is also configured
    and the timer counts to \p value.  If the selected run mode is Continuous,
    the timer counts up to 2^16-1.  The timer is not started.

    \param[in] source  - Timer source
    \param[in] divider - Timer divider
    \param[in] runMode - Timer mode
    \param[in] value   - Value for timer to count up to
*//****************************************************************************/
void Timer_A0_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value)
{
    uint16_t dividerInteger;

    /* Ensure timer A is stopped */
    TA0CTL = 0u;

    /* Set timer source */
    if (source == TimerSource_SMCLK)
    {
        TA0CTL = TA0CTL | TASSEL__SMCLK;
    }
    else if (source == TimerSource_EXTERNAL)
    {
        TA0CTL = TA0CTL | TASSEL__TACLK;
    }
    else
    {
        TA0CTL = TA0CTL | TASSEL__ACLK;
    }

    /* Set input source divider */
    dividerInteger = (uint16_t) divider;
    if ((dividerInteger % 8u) == 0u)
    {
        TA0CTL = TA0CTL | ID__8;
        TA0EX0 = ((dividerInteger / 8u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 4u) == 0u)
    {
        TA0CTL = TA0CTL | ID__4;
        TA0EX0 = ((dividerInteger / 4u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 2u) == 0u)
    {
        TA0CTL = TA0CTL | ID__2;
        TA0EX0 = ((dividerInteger / 2u) - 1u) & 0x07u;
    }
    else
    {
        TA0CTL = TA0CTL | ID__1;
        TA0EX0 = (dividerInteger - 1u) & 0x07u;
    }

    if ((runMode == TimerRunMode_UP) ||
        (runMode == TimerRunMode_UP_DOWN))
    {
        /* Set upper bound */
        TA0CCR0 = value;

        /* Put compare/capture 0 module in compare mode,
           clear interrupt pending flag, and disable interrupt
        */
        TA0CCTL0 = 0u;
    }
}

/***************************************************************************//**
    Configure a timer A0 compare/capture (CC) module.

    \param[in] module - Compare/capture module to configure
    \param[in] config - Configuration value to set control register to
    \param[in] value - Initial compare/capture value
*//****************************************************************************/
void Timer_A0CCX_Configure(
    TimerA0CC_t module,
    uint16_t config,
    uint16_t value)
{
    /** Look-up table of timer A0 compare/capture registers */
    static volatile uint16_t* const _timerA0CCRegisters[TIMER_A0_NUM_CC_MODULES] =
    {
        &TA0CCR0,
        &TA0CCR1,
        &TA0CCR2,
        &TA0CCR3,
        &TA0CCR4
    };
    /** Look-up table of timer A0 compare/capture control registers */
    static volatile uint16_t* const _timerA0CCControlRegisters[TIMER_A0_NUM_CC_MODULES] =
    {
        &TA0CCTL0,
        &TA0CCTL1,
        &TA0CCTL2,
        &TA0CCTL3,
        &TA0CCTL4
    };

    volatile uint16_t* ccReg = _timerA0CCRegisters[module];
    volatile uint16_t* ccControlReg = _timerA0CCControlRegisters[module];

    *ccReg = value;
    *ccControlReg = config;
}

/***************************************************************************//**
    Configure timer A1 using the given parameters.  If the selected run mode
    is either Up or Up/Down, the compare/capture 0 module is also configured
    and the timer counts to \p value.  If the selected run mode is Continuous,
    the timer counts up to 2^16-1.  The timer is not started.

    \param[in] source  - Timer source
    \param[in] divider - Timer divider
    \param[in] runMode - Timer mode
    \param[in] value   - Value for timer to count up to
*//****************************************************************************/
void Timer_A1_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value)
{
    uint16_t dividerInteger;

    /* Ensure timer A is stopped */
    TA1CTL = 0u;

    /* Set timer source */
    if (source == TimerSource_SMCLK)
    {
        TA1CTL = TA1CTL | TASSEL__SMCLK;
    }
    else if (source == TimerSource_EXTERNAL)
    {
        TA1CTL = TA1CTL | TASSEL__TACLK;
    }
    else
    {
        TA1CTL = TA1CTL | TASSEL__ACLK;
    }

    /* Set input source divider */
    dividerInteger = (uint16_t) divider;
    if ((dividerInteger % 8u) == 0u)
    {
        TA1CTL = TA1CTL | ID__8;
        TA1EX0 = ((dividerInteger / 8u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 4u) == 0u)
    {
        TA1CTL = TA1CTL | ID__4;
        TA1EX0 = ((dividerInteger / 4u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 2u) == 0u)
    {
        TA1CTL = TA1CTL | ID__2;
        TA1EX0 = ((dividerInteger / 2u) - 1u) & 0x07u;
    }
    else
    {
        TA1CTL = TA1CTL | ID__1;
        TA1EX0 = (dividerInteger - 1u) & 0x07u;
    }

    if ((runMode == TimerRunMode_UP) ||
        (runMode == TimerRunMode_UP_DOWN))
    {
        /* Set upper bound */
        TA1CCR0 = value;

        /* Put compare/capture 0 module in compare mode,
           clear interrupt pending flag, and disable interrupt
        */
        TA1CCTL0 = 0u;
    }
}

/***************************************************************************//**
    Configure a timer A1 compare/capture (CC) module.

    \param[in] module - Compare/capture module to configure
    \param[in] config - Configuration value to set control register to
    \param[in] value - Initial compare/capture value
*//****************************************************************************/
void Timer_A1CCX_Configure(
    TimerA1CC_t module,
    uint16_t config,
    uint16_t value)
{
    /** Look-up table of timer A1 compare/capture registers */
    static volatile uint16_t* const _timerA1CCRegisters[TIMER_A1_NUM_CC_MODULES] =
    {
        &TA1CCR0,
        &TA1CCR1,
        &TA1CCR2
    };
    /** Look-up table of timer A1 compare/capture control registers */
    static volatile uint16_t* const _timerA1CCControlRegisters[TIMER_A1_NUM_CC_MODULES] =
    {
        &TA1CCTL0,
        &TA1CCTL1,
        &TA1CCTL2
    };

    volatile uint16_t* ccReg = _timerA1CCRegisters[module];
    volatile uint16_t* ccControlReg = _timerA1CCControlRegisters[module];

    *ccReg = value;
    *ccControlReg = config;
}

/***************************************************************************//**
    Configure timer A2 using the given parameters.  If the selected run mode
    is either Up or Up/Down, the compare/capture 0 module is also configured
    and the timer counts to \p value.  If the selected run mode is Continuous,
    the timer counts up to 2^16-1.  The timer is not started.

    \param[in] source  - Timer source
    \param[in] divider - Timer divider
    \param[in] runMode - Timer mode
    \param[in] value   - Value for timer to count up to
*//****************************************************************************/
void Timer_A2_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value)
{
    uint16_t dividerInteger;

    /* Ensure timer A is stopped */
    TA2CTL = 0u;

    /* Set timer source */
    if (source == TimerSource_SMCLK)
    {
        TA2CTL = TA2CTL | TASSEL__SMCLK;
    }
    else if (source == TimerSource_EXTERNAL)
    {
        TA2CTL = TA2CTL | TASSEL__TACLK;
    }
    else
    {
        TA2CTL = TA2CTL | TASSEL__ACLK;
    }

    /* Set input source divider */
    dividerInteger = (uint16_t) divider;
    if ((dividerInteger % 8u) == 0u)
    {
        TA2CTL = TA2CTL | ID__8;
        TA2EX0 = ((dividerInteger / 8u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 4u) == 0u)
    {
        TA2CTL = TA2CTL | ID__4;
        TA2EX0 = ((dividerInteger / 4u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 2u) == 0u)
    {
        TA2CTL = TA2CTL | ID__2;
        TA2EX0 = ((dividerInteger / 2u) - 1u) & 0x07u;
    }
    else
    {
        TA2CTL = TA2CTL | ID__1;
        TA2EX0 = (dividerInteger - 1u) & 0x07u;
    }

    if ((runMode == TimerRunMode_UP) ||
        (runMode == TimerRunMode_UP_DOWN))
    {
        /* Set upper bound */
        TA2CCR0 = value;

        /* Put compare/capture 0 module in compare mode,
           clear interrupt pending flag, and disable interrupt
        */
        TA2CCTL0 = 0u;
    }
}

/***************************************************************************//**
    Configure a timer A2 compare/capture (CC) module.

    \param[in] module - Compare/capture module to configure
    \param[in] config - Configuration value to set control register to
    \param[in] value - Initial compare/capture value
*//****************************************************************************/
void Timer_A2CCX_Configure(
    TimerA2CC_t module,
    uint16_t config,
    uint16_t value)
{
    /** Look-up table of timer A2 compare/capture registers */
    static volatile uint16_t* const _timerA2CCRegisters[TIMER_A2_NUM_CC_MODULES] =
    {
        &TA2CCR0,
        &TA2CCR1,
        &TA2CCR2
    };
    /** Look-up table of timer A2 compare/capture control registers */
    static volatile uint16_t* const _timerA2CCControlRegisters[TIMER_A2_NUM_CC_MODULES] =
    {
        &TA2CCTL0,
        &TA2CCTL1,
        &TA2CCTL2
    };

    volatile uint16_t* ccReg = _timerA2CCRegisters[module];
    volatile uint16_t* ccControlReg = _timerA2CCControlRegisters[module];

    *ccReg = value;
    *ccControlReg = config;
}

/***************************************************************************//**
    Configure timer B0 using the given parameters.  If the selected run mode
    is either Up or Up/Down, the compare/capture 0 module is also configured
    and the timer counts to \p value.  If the selected run mode is Continuous,
    a range of either 16, 12, 10, or 8 bits is selected.  The timer is not
    started.

    \param[in] source  - Timer source
    \param[in] divider - Timer divider
    \param[in] counterLength - Length of timer counter
    \param[in] runMode - Timer mode
    \param[in] value   - Value for timer to count up to
*//****************************************************************************/
void Timer_B0_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerCounterLength_t counterLength,
    TimerRunMode_t runMode,
    uint16_t value)
{
    uint16_t dividerInteger;

    /* Ensure timer B is stopped */
    TB0CTL = 0u;

    /* Set timer source */
    if (source == TimerSource_SMCLK)
    {
        TB0CTL = TB0CTL | TBSSEL__SMCLK;
    }
    else
    {
        TB0CTL = TB0CTL | TBSSEL__ACLK;
    }

    /* Set input source divider */
    dividerInteger = (uint16_t) divider;
    if ((dividerInteger % 8u) == 0u)
    {
        TB0CTL = TB0CTL | ID__8;
        TB0EX0 = ((dividerInteger / 8u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 4u) == 0u)
    {
        TB0CTL = TB0CTL | ID__4;
        TB0EX0 = ((dividerInteger / 4u) - 1u) & 0x07u;
    }
    else if ((dividerInteger % 2u) == 0u)
    {
        TB0CTL = TB0CTL | ID__2;
        TB0EX0 = ((dividerInteger / 2u) - 1u) & 0x07u;
    }
    else
    {
        TB0CTL = TB0CTL | ID__1;
        TB0EX0 = (dividerInteger - 1u) & 0x07u;
    }

    /* If run mode counts up to a CC0 value, leave counter length at 16 bits.
       Otherwise, set to provided counter length.
    */
    if ((runMode == TimerRunMode_UP) ||
        (runMode == TimerRunMode_UP_DOWN))
    {
        /* Set upper bound */
        TB0CCR0 = value;

        /* Put compare/capture 0 module in compare mode,
           clear interrupt pending flag, and disable interrupt
        */
        TB0CCTL0 = 0u;
    }
    else
    {
        TB0CTL = TB0CTL | ((uint16_t) counterLength);
    }
}

/***************************************************************************//**
    Configure a timer B0 compare/capture (CC) module.

    \param[in] module - Compare/capture module to configure
    \param[in] config - Configuration value to set control register to
    \param[in] value - Initial compare/capture value
*//****************************************************************************/
void Timer_B0CCX_Configure(
    TimerB0CC_t module,
    uint16_t config,
    uint16_t value)
{
    /** Look-up table of timer B0 compare/capture registers */
    static volatile uint16_t* const _timerB0CCRegisters[TIMER_B0_NUM_CC_MODULES] =
    {
        &TB0CCR0,
        &TB0CCR1,
        &TB0CCR2,
        &TB0CCR3,
        &TB0CCR4,
        &TB0CCR5,
        &TB0CCR6
    };
    /** Look-up table of timer B0 compare/capture control registers */
    static volatile uint16_t* const _timerB0CCControlRegisters[TIMER_B0_NUM_CC_MODULES] =
    {
        &TB0CCTL0,
        &TB0CCTL1,
        &TB0CCTL2,
        &TB0CCTL3,
        &TB0CCTL4,
        &TB0CCTL5,
        &TB0CCTL6
    };

    volatile uint16_t* ccReg = _timerB0CCRegisters[module];
    volatile uint16_t* ccControlReg = _timerB0CCControlRegisters[module];

    *ccReg = value;
    *ccControlReg = config;
}

