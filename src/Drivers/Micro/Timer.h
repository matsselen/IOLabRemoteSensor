/***************************************************************************//**
    \file Timer.h
    \brief Timer module driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The header file for the timer module. This file contains macros
       and functions that start and stop the hardware timers and control the timer
       interrupts.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Timer.h$
    $Revision: 5$
    $Date: Monday, August 06, 2012 7:32:53 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_


/*-----------------------PUBLIC MACROS-----------------------------------*/
/** \name Timer A0 control macros
    \{
*/
/***************************************************************************//**
    \brief Start timer A0 counting from zero.

    \note Timer A0 is not stopped first.

    \param[in] mode - Timer run mode
*//****************************************************************************/
#define TIMER_A0_START(mode)            (TA0CTL = TA0CTL | ((uint16_t) mode) | TACLR)

/** Timer A0 stop */
#define TIMER_A0_STOP()                 (TA0CTL = TA0CTL & ((uint16_t) ~(MC_3)))

/** Timer A reset */
#define TIMER_A0_RESET()                (TA0CTL = TA0CTL | TACLR)
/** \} */

/** \name Timer A0 CCx register set macros
    \{
*/
#define TIMER_A0_CC0_SET(value)         (TA0CCR0 = (value))
#define TIMER_A0_CC0_INCREMENT(value)   (TA0CCR0 += (value))
#define TIMER_A0_CC1_SET(value)         (TA0CCR1 = (value))
#define TIMER_A0_CC2_SET(value)         (TA0CCR2 = (value))
#define TIMER_A0_CC3_SET(value)         (TA0CCR3 = (value))
#define TIMER_A0_CC4_SET(value)         (TA0CCR4 = (value))
/** \} */

/** \name Timer A0 CCx register get macros
    \{
*/
#define TIMER_A0_CC0_GET                ( TA0CCR0 )
#define TIMER_A0_CC0_OVERFLOWED         ((TA0CCTL0 & COV) > 0)
#define TIMER_A0_CC0_PIN_IS_HIGH        ((TA0CCTL0 & CCI) > 0)
#define TIMER_A0_CC0_PIN_IS_LOW         ((TA0CCTL0 & CCI) == 0)
#define TIMER_A0_CC1_GET                ( TA0CCR1 )
#define TIMER_A0_CC1_OVERFLOWED         ((TA0CCTL1 & COV) > 0)
#define TIMER_A0_CC1_PIN_IS_HIGH        ((TA0CCTL1 & CCI) > 0)
#define TIMER_A0_CC1_PIN_IS_LOW         ((TA0CCTL1 & CCI) == 0)
#define TIMER_A0_CC2_GET                ( TA0CCR2 )
#define TIMER_A0_CC2_OVERFLOWED         ((TA0CCTL2 & COV) > 0)
#define TIMER_A0_CC2_PIN_IS_HIGH        ((TA0CCTL2 & CCI) > 0)
#define TIMER_A0_CC2_PIN_IS_LOW         ((TA0CCTL2 & CCI) == 0)
/** \} */

/** \name Timer A0 enable interrupt macros
    \{
*/
#define TIMER_A0_CC0_INT_ENABLE()       (TA0CCTL0 = TA0CCTL0 | (CCIE))
#define TIMER_A0_CC1_INT_ENABLE()       (TA0CCTL1 = TA0CCTL1 | (CCIE))
#define TIMER_A0_CC2_INT_ENABLE()       (TA0CCTL2 = TA0CCTL2 | (CCIE))
#define TIMER_A0_CC3_INT_ENABLE()       (TA0CCTL3 = TA0CCTL3 | (CCIE))
#define TIMER_A0_CC4_INT_ENABLE()       (TA0CCTL4 = TA0CCTL4 | (CCIE))
#define TIMER_A0_TA_INT_ENABLE()        (TA0CTL   = TA0CTL   | (TAIE))
/** \} */

/** \name Timer A0 disable interrupt macros
    \{
*/
#define TIMER_A0_CC0_INT_DISABLE()      (TA0CCTL0 = TA0CCTL0 & ((uint16_t) ~(CCIE)))
#define TIMER_A0_CC1_INT_DISABLE()      (TA0CCTL1 = TA0CCTL1 & ((uint16_t) ~(CCIE)))
#define TIMER_A0_CC2_INT_DISABLE()      (TA0CCTL2 = TA0CCTL2 & ((uint16_t) ~(CCIE)))
#define TIMER_A0_CC3_INT_DISABLE()      (TA0CCTL3 = TA0CCTL3 & ((uint16_t) ~(CCIE)))
#define TIMER_A0_CC4_INT_DISABLE()      (TA0CCTL4 = TA0CCTL4 & ((uint16_t) ~(CCIE)))
#define TIMER_A0_TA_INT_DISABLE()       (TA0CTL   = TA0CTL   & ((uint16_t) ~(TAIE)))
/** \} */

/** \name Timer A0 enable interrupt status macros
    \{
*/
#define TIMER_A0_CC0_INT_IS_ENABLED()   ((TA0CCTL0 & (CCIE)) > 0u)
#define TIMER_A0_CC1_INT_IS_ENABLED()   ((TA0CCTL1 & (CCIE)) > 0u)
#define TIMER_A0_CC2_INT_IS_ENABLED()   ((TA0CCTL2 & (CCIE)) > 0u)
#define TIMER_A0_CC3_INT_IS_ENABLED()   ((TA0CCTL3 & (CCIE)) > 0u)
#define TIMER_A0_CC4_INT_IS_ENABLED()   ((TA0CCTL4 & (CCIE)) > 0u)
#define TIMER_A0_TA_INT_IS_ENABLED()    ((TA0CTL   & (TAIE)) > 0u)
/** \} */

/** \name Timer A0 clear interrupt macros
    \{
*/
#define TIMER_A0_CC0_INT_CLEAR()        (TA0CCTL0 = TA0CCTL0 & ((uint16_t) ~(CCIFG)))
#define TIMER_A0_CC1_INT_CLEAR()        (TA0CCTL1 = TA0CCTL1 & ((uint16_t) ~(CCIFG)))
#define TIMER_A0_CC2_INT_CLEAR()        (TA0CCTL2 = TA0CCTL2 & ((uint16_t) ~(CCIFG)))
#define TIMER_A0_CC3_INT_CLEAR()        (TA0CCTL3 = TA0CCTL3 & ((uint16_t) ~(CCIFG)))
#define TIMER_A0_CC4_INT_CLEAR()        (TA0CCTL4 = TA0CCTL4 & ((uint16_t) ~(CCIFG)))
#define TIMER_A0_TA_INT_CLEAR()         (TA0CTL   = TA0CTL & ((uint16_t) ~(TAIFG)))
/** \} */

/** \name Timer A0 get interrupt flag macros
    \{
*/
#define TIMER_A0_CC0_INT_IS_FLAG_SET()  ((TA0CCTL0 & (CCIFG)) > 0u)
#define TIMER_A0_CC1_INT_IS_FLAG_SET()  ((TA0CCTL1 & (CCIFG)) > 0u)
#define TIMER_A0_CC2_INT_IS_FLAG_SET()  ((TA0CCTL2 & (CCIFG)) > 0u)
#define TIMER_A0_CC3_INT_IS_FLAG_SET()  ((TA0CCTL3 & (CCIFG)) > 0u)
#define TIMER_A0_CC4_INT_IS_FLAG_SET()  ((TA0CCTL4 & (CCIFG)) > 0u)
#define TIMER_A0_TA_INT_IS_FLAG_SET()   ((TA0CTL   & (TAIFG)) > 0u)
/** \} */

/*----------------------------------------------------------------------------*/

/** \name Timer A1 control macros
    \{
*/
/***************************************************************************//**
    \brief Start timer A1 counting from zero.

    \note Timer A1 is not stopped first.

    \param[in] mode - Timer run mode
*//****************************************************************************/
#define TIMER_A1_START(mode)            (TA1CTL = TA1CTL | ((uint16_t) mode) | TACLR)

/** Timer A1 stop */
#define TIMER_A1_STOP()                 (TA1CTL = TA1CTL & ((uint16_t) ~(MC_3)))

/** Timer A1 reset */
#define TIMER_A1_RESET()                (TA1CTL = TA1CTL | TACLR)
/** \} */

/** \name Timer A1 CCx register set macros
    \{
*/
#define TIMER_A1_CC0_SET(value)         (TA1CCR0 = (value))
#define TIMER_A1_CC0_INCREMENT(value)   (TA1CCR0 += (value))
#define TIMER_A1_CC1_SET(value)         (TA1CCR1 = (value))
#define TIMER_A1_CC2_SET(value)         (TA1CCR2 = (value))
/** \} */

/** \name Timer A1 enable interrupt macros
    \{
*/
#define TIMER_A1_CC0_INT_ENABLE()       (TA1CCTL0 = TA1CCTL0 | (CCIE))
#define TIMER_A1_CC1_INT_ENABLE()       (TA1CCTL1 = TA1CCTL1 | (CCIE))
#define TIMER_A1_CC2_INT_ENABLE()       (TA1CCTL2 = TA1CCTL2 | (CCIE))
#define TIMER_A1_TA_INT_ENABLE()        (TA1CTL   = TA1CTL   | (TAIE))
/** \} */

/** \name Timer A1 disable interrupt macros
    \{
*/
#define TIMER_A1_CC0_INT_DISABLE()      (TA1CCTL0 = TA1CCTL0 & ((uint16_t) ~(CCIE)))
#define TIMER_A1_CC1_INT_DISABLE()      (TA1CCTL1 = TA1CCTL1 & ((uint16_t) ~(CCIE)))
#define TIMER_A1_CC2_INT_DISABLE()      (TA1CCTL2 = TA1CCTL2 & ((uint16_t) ~(CCIE)))
#define TIMER_A1_TA_INT_DISABLE()       (TA1CTL   = TA1CTL   & ((uint16_t) ~(TAIE)))
/** \} */

/** \name Timer A1 enable interrupt status macros
    \{
*/
#define TIMER_A1_CC0_INT_IS_ENABLED()   ((TA1CCTL0 & (CCIE)) > 0u)
#define TIMER_A1_CC1_INT_IS_ENABLED()   ((TA1CCTL1 & (CCIE)) > 0u)
#define TIMER_A1_CC2_INT_IS_ENABLED()   ((TA1CCTL2 & (CCIE)) > 0u)
#define TIMER_A1_TA_INT_IS_ENABLED()    ((TA1CTL   & (TAIE)) > 0u)
/** \} */

/** \name Timer A1 clear interrupt macros
    \{
*/
#define TIMER_A1_CC0_INT_CLEAR()        (TA1CCTL0 = TA1CCTL0 & ((uint16_t) ~(CCIFG)))
#define TIMER_A1_CC1_INT_CLEAR()        (TA1CCTL1 = TA1CCTL1 & ((uint16_t) ~(CCIFG)))
#define TIMER_A1_CC2_INT_CLEAR()        (TA1CCTL2 = TA1CCTL2 & ((uint16_t) ~(CCIFG)))
#define TIMER_A1_TA_INT_CLEAR()         (TA1CTL   = TA1CTL & ((uint16_t) ~(TAIFG)))
/** \} */

/** \name Timer A1 get interrupt flag macros
    \{
*/
#define TIMER_A1_CC0_INT_IS_FLAG_SET()  ((TA1CCTL0 & (CCIFG)) > 0u)
#define TIMER_A1_CC1_INT_IS_FLAG_SET()  ((TA1CCTL1 & (CCIFG)) > 0u)
#define TIMER_A1_CC2_INT_IS_FLAG_SET()  ((TA1CCTL2 & (CCIFG)) > 0u)
#define TIMER_A1_TA_INT_IS_FLAG_SET()   ((TA1CTL   & (TAIFG)) > 0u)
/** \} */

/*----------------------------------------------------------------------------*/

/** \name Timer A2 control macros
    \{
*/
/***************************************************************************//**
    \brief Start timer A2 counting from zero.

    \note Timer A2 is not stopped first.

    \param[in] mode - Timer run mode
*//****************************************************************************/
#define TIMER_A2_START(mode)            (TA2CTL = TA2CTL | ((uint16_t) mode) | TACLR)

/** Timer A2 stop */
#define TIMER_A2_STOP()                 (TA2CTL = TA2CTL & ((uint16_t) ~(MC_3)))

/** Timer A2 reset */
#define TIMER_A2_RESET()                (TA2CTL = TA2CTL | TACLR)

/** Timer A2 CC1 trigger software capture. Assumes both rising and falling edge */
#define TIMER_A2_CC1_TRIGGER            (TA2CCTL1 = TA2CCTL1 ^ CCIS0)
/** \} */

/** \name Timer A2 CCx register set macros
    \{
*/
#define TIMER_A2_CC0_SET(value)         (TA2CCR0 = (value))
#define TIMER_A2_CC0_INCREMENT(value)   (TA2CCR0 += (value))
#define TIMER_A2_CC1_SET(value)         (TA2CCR1 = (value))
#define TIMER_A2_CC2_SET(value)         (TA2CCR2 = (value))
/** \} */


/** \name Timer A2 CCx register get macros
    \{
*/
#define TIMER_A2_CC0_GET                (TA2CCR0)
#define TIMER_A2_CC0_OVERFLOWED         ((TA2CCTL0 & COV) > 0)
#define TIMER_A2_CC0_PIN_IS_HIGH        ((TA2CCTL0 & CCI) > 0)
#define TIMER_A2_CC0_PIN_IS_LOW         ((TA2CCTL0 & CCI) == 0)
#define TIMER_A2_CC1_GET                (TA2CCR1)
#define TIMER_A2_CC1_OVERFLOWED         ((TA2CCTL1 & COV) > 0)
#define TIMER_A2_CC1_PIN_IS_HIGH        ((TA2CCTL1 & CCI) > 0)
#define TIMER_A2_CC1_PIN_IS_LOW         ((TA2CCTL1 & CCI) == 0)
#define TIMER_A2_CC2_GET                (TA2CCR2)
#define TIMER_A2_CC2_OVERFLOWED         ((TA2CCTL2 & COV) > 0)
#define TIMER_A2_CC2_PIN_IS_HIGH        ((TA2CCTL2 & CCI) > 0)
#define TIMER_A2_CC2_PIN_IS_LOW         ((TA2CCTL2 & CCI) == 0)
/** \} */

/** \name Timer A2 enable interrupt macros
    \{
*/
#define TIMER_A2_CC0_INT_ENABLE()       (TA2CCTL0 = TA2CCTL0 | (CCIE))
#define TIMER_A2_CC1_INT_ENABLE()       (TA2CCTL1 = TA2CCTL1 | (CCIE))
#define TIMER_A2_CC2_INT_ENABLE()       (TA2CCTL2 = TA2CCTL2 | (CCIE))
#define TIMER_A2_TA_INT_ENABLE()        (TA2CTL   = TA2CTL   | (TAIE))
/** \} */

/** \name Timer A2 disable interrupt macros
    \{
*/
#define TIMER_A2_CC0_INT_DISABLE()      (TA2CCTL0 = TA2CCTL0 & ((uint16_t) ~(CCIE)))
#define TIMER_A2_CC1_INT_DISABLE()      (TA2CCTL1 = TA2CCTL1 & ((uint16_t) ~(CCIE)))
#define TIMER_A2_CC2_INT_DISABLE()      (TA2CCTL2 = TA2CCTL2 & ((uint16_t) ~(CCIE)))
#define TIMER_A2_TA_INT_DISABLE()       (TA2CTL   = TA2CTL   & ((uint16_t) ~(TAIE)))
/** \} */

/** \name Timer A2 enable interrupt status macros
    \{
*/
#define TIMER_A2_CC0_INT_IS_ENABLED()   ((TA2CCTL0 & (CCIE)) > 0u)
#define TIMER_A2_CC1_INT_IS_ENABLED()   ((TA2CCTL1 & (CCIE)) > 0u)
#define TIMER_A2_CC2_INT_IS_ENABLED()   ((TA2CCTL2 & (CCIE)) > 0u)
#define TIMER_A2_TA_INT_IS_ENABLED()    ((TA2CTL   & (TAIE)) > 0u)
/** \} */

/** \name Timer A2 clear macros
    \{
*/
#define TIMER_A2_CC0_INT_CLEAR()        (TA2CCTL0 = TA2CCTL0 & ((uint16_t) ~(CCIFG)))
#define TIMER_A2_CC0_CLEAR_OVERFLOW     (TA2CCTL0 = TA2CCTL0 & ((uint16_t) ~(COV)))
#define TIMER_A2_CC1_INT_CLEAR()        (TA2CCTL1 = TA2CCTL1 & ((uint16_t) ~(CCIFG)))
#define TIMER_A2_CC1_CLEAR_OVERFLOW     (TA2CCTL1 = TA2CCTL1 & ((uint16_t) ~(COV)))
#define TIMER_A2_CC2_INT_CLEAR()        (TA2CCTL2 = TA2CCTL2 & ((uint16_t) ~(CCIFG)))
#define TIMER_A2_CC2_CLEAR_OVERFLOW     (TA2CCTL0 = TA2CCTL2 & ((uint16_t) ~(COV)))
#define TIMER_A2_TA_INT_CLEAR()         (TA2CTL   = TA2CTL & ((uint16_t) ~(TAIFG)))
/** \} */

/** \name Timer A2 get interrupt flag macros
    \{
*/
#define TIMER_A2_CC0_INT_IS_FLAG_SET()  ((TA2CCTL0 & (CCIFG)) > 0u)
#define TIMER_A2_CC1_INT_IS_FLAG_SET()  ((TA2CCTL1 & (CCIFG)) > 0u)
#define TIMER_A2_CC2_INT_IS_FLAG_SET()  ((TA2CCTL2 & (CCIFG)) > 0u)
#define TIMER_A2_TA_INT_IS_FLAG_SET()   ((TA2CTL   & (TAIFG)) > 0u)
/** \} */

/*----------------------------------------------------------------------------*/

/** \name Timer B0 control macros
    \{
*/
/***************************************************************************//**
    \brief Start timer B0 counting from zero.

    \note Timer B0 is not stopped first.

    \param[in] mode - Timer run mode
*//****************************************************************************/
#define TIMER_B0_START(mode)            (TB0CTL = TB0CTL | ((uint16_t) mode) | TBCLR)

/** Timer B0 stop */
#define TIMER_B0_STOP()                 (TB0CTL = TB0CTL & ((uint16_t) ~(MC_3)))

/** Timer B reset */
#define TIMER_B0_RESET()                (TB0CTL = TB0CTL | TACLR)
/** \} */

/** \name Timer B0 CCx register get macros
    \{
*/
#define TIMER_B0_CC0_GET()              (TB0CCR0)
#define TIMER_B0_CC1_GET()              (TB0CCR1)
#define TIMER_B0_CC2_GET()              (TB0CCR2)
#define TIMER_B0_CC3_GET()              (TB0CCR3)
#define TIMER_B0_CC4_GET()              (TB0CCR4)
#define TIMER_B0_CC5_GET()              (TB0CCR5)
#define TIMER_B0_CC6_GET()              (TB0CCR6)
/** \} */

/** \name Timer B0 CCx register set macros
    \{
*/
#define TIMER_B0_CC0_SET(value)         (TB0CCR0 = (value))
#define TIMER_B0_CC0_INCREMENT(value)   (TB0CCR0 += (value))
#define TIMER_B0_CC1_SET(value)         (TB0CCR1 = (value))
#define TIMER_B0_CC2_SET(value)         (TB0CCR2 = (value))
#define TIMER_B0_CC3_SET(value)         (TB0CCR3 = (value))
#define TIMER_B0_CC4_SET(value)         (TB0CCR4 = (value))
#define TIMER_B0_CC5_SET(value)         (TB0CCR5 = (value))
#define TIMER_B0_CC6_SET(value)         (TB0CCR6 = (value))
/** \} */

/** \name Timer B0 enable interrupt macros
    \{
*/
#define TIMER_B0_CC0_INT_ENABLE()       (TB0CCTL0 = TB0CCTL0 | (CCIE))
#define TIMER_B0_CC1_INT_ENABLE()       (TB0CCTL1 = TB0CCTL1 | (CCIE))
#define TIMER_B0_CC2_INT_ENABLE()       (TB0CCTL2 = TB0CCTL2 | (CCIE))
#define TIMER_B0_CC3_INT_ENABLE()       (TB0CCTL3 = TB0CCTL3 | (CCIE))
#define TIMER_B0_CC4_INT_ENABLE()       (TB0CCTL4 = TB0CCTL4 | (CCIE))
#define TIMER_B0_CC5_INT_ENABLE()       (TB0CCTL5 = TB0CCTL5 | (CCIE))
#define TIMER_B0_CC6_INT_ENABLE()       (TB0CCTL6 = TB0CCTL6 | (CCIE))
#define TIMER_B0_TA_INT_ENABLE()        (TB0CTL   = TB0CTL   | (TAIE))
/** \} */

/** \name Timer B0 disable interrupt macros
    \{
*/
#define TIMER_B0_CC0_INT_DISABLE()      (TB0CCTL0 = TB0CCTL0 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC1_INT_DISABLE()      (TB0CCTL1 = TB0CCTL1 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC2_INT_DISABLE()      (TB0CCTL2 = TB0CCTL2 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC3_INT_DISABLE()      (TB0CCTL3 = TB0CCTL3 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC4_INT_DISABLE()      (TB0CCTL4 = TB0CCTL4 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC5_INT_DISABLE()      (TB0CCTL5 = TB0CCTL5 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_CC6_INT_DISABLE()      (TB0CCTL6 = TB0CCTL6 & ((uint16_t) ~(CCIE)))
#define TIMER_B0_TA_INT_DISABLE()       (TB0CTL   = TB0CTL   & ((uint16_t) ~(TAIE)))
/** \} */

/** \name Timer B0 enable interrupt status macros
    \{
*/
#define TIMER_B0_CC0_INT_IS_ENABLED()   ((TB0CCTL0 & (CCIE)) > 0u)
#define TIMER_B0_CC1_INT_IS_ENABLED()   ((TB0CCTL1 & (CCIE)) > 0u)
#define TIMER_B0_CC2_INT_IS_ENABLED()   ((TB0CCTL2 & (CCIE)) > 0u)
#define TIMER_B0_CC3_INT_IS_ENABLED()   ((TB0CCTL3 & (CCIE)) > 0u)
#define TIMER_B0_CC4_INT_IS_ENABLED()   ((TB0CCTL4 & (CCIE)) > 0u)
#define TIMER_B0_CC5_INT_IS_ENABLED()   ((TB0CCTL5 & (CCIE)) > 0u)
#define TIMER_B0_CC6_INT_IS_ENABLED()   ((TB0CCTL6 & (CCIE)) > 0u)
#define TIMER_B0_TA_INT_IS_ENABLED()    ((TB0CTL   & (TAIE)) > 0u)
/** \} */

/** \name Timer B0 clear interrupt macros
    \{
*/
#define TIMER_B0_CC0_INT_CLEAR()        (TB0CCTL0 = TB0CCTL0 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC1_INT_CLEAR()        (TB0CCTL1 = TB0CCTL1 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC2_INT_CLEAR()        (TB0CCTL2 = TB0CCTL2 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC3_INT_CLEAR()        (TB0CCTL3 = TB0CCTL3 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC4_INT_CLEAR()        (TB0CCTL4 = TB0CCTL4 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC5_INT_CLEAR()        (TB0CCTL5 = TB0CCTL5 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_CC6_INT_CLEAR()        (TB0CCTL6 = TB0CCTL6 & ((uint16_t) ~(CCIFG)))
#define TIMER_B0_TA_INT_CLEAR()         (TB0CTL   = TB0CTL & ((uint16_t) ~(TAIFG)))
/** \} */

/** \name Timer B0 get interrupt flag macros
    \{
*/
#define TIMER_B0_CC0_INT_IS_FLAG_SET()  ((TB0CCTL0 & (CCIFG)) > 0u)
#define TIMER_B0_CC1_INT_IS_FLAG_SET()  ((TB0CCTL1 & (CCIFG)) > 0u)
#define TIMER_B0_CC2_INT_IS_FLAG_SET()  ((TB0CCTL2 & (CCIFG)) > 0u)
#define TIMER_B0_CC3_INT_IS_FLAG_SET()  ((TB0CCTL3 & (CCIFG)) > 0u)
#define TIMER_B0_CC4_INT_IS_FLAG_SET()  ((TB0CCTL4 & (CCIFG)) > 0u)
#define TIMER_B0_CC5_INT_IS_FLAG_SET()  ((TB0CCTL5 & (CCIFG)) > 0u)
#define TIMER_B0_CC6_INT_IS_FLAG_SET()  ((TB0CCTL6 & (CCIFG)) > 0u)
#define TIMER_B0_TA_INT_IS_FLAG_SET()   ((TB0CTL   & (TAIFG)) > 0u)
/** \} */

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Timer sources */
typedef enum TimerSource
{
    TimerSource_SMCLK,
    TimerSource_ACLK,
    TimerSource_EXTERNAL
} TimerSource_t;

/** Timer clock dividers */
typedef enum TimerDivider
{
    TimerDivider_1 = 1u,
    TimerDivider_2 = 2u,
    TimerDivider_3 = 3u,
    TimerDivider_4 = 4u,
    TimerDivider_5 = 5u,
    TimerDivider_6 = 6u,
    TimerDivider_7 = 7u,
    TimerDivider_8 = 8u,

    TimerDivider_10 = 10u,
    TimerDivider_12 = 12u,
    TimerDivider_14 = 14u,
    TimerDivider_16 = 16u,

    TimerDivider_20 = 20u,
    TimerDivider_24 = 24u,
    TimerDivider_28 = 28u,
    TimerDivider_32 = 32u,

    TimerDivider_40 = 40u,
    TimerDivider_48 = 48u,
    TimerDivider_56 = 56u,
    TimerDivider_64 = 64u
} TimerDivider_t;

/** Timer run modes */
typedef enum TimerRunMode
{
    TimerRunMode_UP = MC__UP,
    TimerRunMode_CONTINUOUS = MC__CONTINUOUS,
    TimerRunMode_UP_DOWN = MC__UPDOWN
} TimerRunMode_t;

/** Timer counter lengths */
typedef enum TimerCounterLength
{
    TimerCounterLength_16BITS = CNTL__16,
    TimerCounterLength_12BITS = CNTL__12,
    TimerCounterLength_10BITS = CNTL__10,
    TimerCounterLength_8BITS = CNTL__8
} TimerCounterLength_t;

/** Timer A0 compare/capture (CC) modules */
typedef enum TimerA0CC
{
    TimerA0CC_0 = 0,
    TimerA0CC_1 = 1,
    TimerA0CC_2 = 2,
    TimerA0CC_3 = 3,
    TimerA0CC_4 = 4
} TimerA0CC_t;

/** Timer A1 compare/capture (CC) modules */
typedef enum TimerA1CC
{
    TimerA1CC_0 = 0,
    TimerA1CC_1 = 1,
    TimerA1CC_2 = 2
} TimerA1CC_t;

/** Timer A2 compare/capture (CC) modules */
typedef enum TimerA2CC
{
    TimerA2CC_0 = 0,
    TimerA2CC_1 = 1,
    TimerA2CC_2 = 2
} TimerA2CC_t;

/** Timer B0 compare/capture (CC) modules */
typedef enum TimerB0CC
{
    TimerB0CC_0 = 0,
    TimerB0CC_1 = 1,
    TimerB0CC_2 = 2,
    TimerB0CC_3 = 3,
    TimerB0CC_4 = 4,
    TimerB0CC_5 = 5,
    TimerB0CC_6 = 6
} TimerB0CC_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
extern void Timer_A0_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value);
extern void Timer_A0CCX_Configure(
    TimerA0CC_t module,
    uint16_t config,
    uint16_t value);

extern void Timer_A1_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value);
extern void Timer_A1CCX_Configure(
    TimerA1CC_t module,
    uint16_t config,
    uint16_t value);

extern void Timer_A2_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerRunMode_t runMode,
    uint16_t value);
extern void Timer_A2CCX_Configure(
    TimerA2CC_t module,
    uint16_t config,
    uint16_t value);

extern void Timer_B0_Configure(
    TimerSource_t source,
    TimerDivider_t divider,
    TimerCounterLength_t counterLength,
    TimerRunMode_t runMode,
    uint16_t value);
extern void Timer_B0CCX_Configure(
    TimerB0CC_t module,
    uint16_t config,
    uint16_t value);

#endif /* TIMER_H_ */
