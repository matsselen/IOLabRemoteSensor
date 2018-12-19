/***************************************************************************//**
    \file Interrupt.h
    \brief Global interrupt control

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The macros defined in this file provide global control
       of the MCU's interrupts.  The global interrupt enable (GIE) bit must
       be set for any interrupt to fire.  If the GIE bit is clear, all
       interrupts (except the non-maskable interrupts) are suppressed.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Interrupt.h$
    $Revision: 2$
    $Date: Thursday, April 19, 2012 1:56:35 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef INTERRUPT_H_
#define INTERRUPT_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Type used when saving the interrupt state. */
typedef enum IntState
{
    IntState_DISABLED,
    IntState_ENABLED
} IntState_t;

/** Enable global interrupts (set GIE). */
#define INTERRUPT_ENABLE()          _enable_interrupts()

/** Disable global interrupts (clear GIE). */
#define INTERRUPT_DISABLE()         _disable_interrupts()
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
IntState_t Interrupt_Lock(void);
void Interrupt_Unlock(IntState_t prevState);

#endif /* INTERRUPT_H_ */
