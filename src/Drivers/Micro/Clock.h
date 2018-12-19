/***************************************************************************//**
    \file Clock.h
    \brief Clock configuration module header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header file for the clock configuration module.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Clock.h$
    $Revision: 3$
    $Date: Tuesday, May 19, 2015 4:01:47 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef CLOCK_H_
#define CLOCK_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Approx number of clock cycles per microsecond. */
#define CLOCK_CYCLES_PER_MICROSECOND    (24ul)

/*-----------------------PUBLIC MACROS----------------------------------------*/

/** Software delay of x microseconds. */
/* Note that the intrinsic _delay_cycles() accepts a uint32_t argument */
#define CLOCK_DELAY_US(x)               __delay_cycles(((uint32_t) (x)) * CLOCK_CYCLES_PER_MICROSECOND)

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

void Clock_Init(void);

#endif /* CLOCK_H_ */
