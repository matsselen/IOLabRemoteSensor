/***************************************************************************//**
    \file PwmFrequencies.h
    \brief contains the settable frequencies for the PWM header pins

    \b Project 1814 IOLab Remote Sensor Application

    \b Description contains the frequencies for the PWM header pins.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/PwmDutyCycles.h$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 5:19:14 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef PWMDUTYCYCLES_H_
#define PWMDUTYCYCLES_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** number of duty cycles */
#define RANGESIZE	32u

/** possible duty cycles for PWM output */
static uint16_t const _dutyCycles[RANGESIZE] =
{
	0,
	3,
	6,
	9,
	12,
	15,
	18,
	21,
	25,
	28,
	31,
	34,
	37,
	40,
	43,
	46,
	50,
	55,
	59,
	62,
	65,
	68,
	71,
	75,
	78,
	81,
	84,
	88,
	91,
	94,
	97,
	100
};


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* PWMDUTYCYCLES_H_ */
