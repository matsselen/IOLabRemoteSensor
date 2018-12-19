/***************************************************************************//**
    \file PwmFrequencies.h
    \brief contains the settable frequencies for the PWM header pins

    \b Project 1814 IOLab Remote Sensor Application

    \b Description contains the frequencies for the PWM header pins.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/PwmFrequencies.h$
    $Revision: 2$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef PWMFREQUENCIES_H_
#define PWMFREQUENCIES_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** number of frequencies per range */
#define RANGESIZE	32u

/** low frequency range for PWM output */
static uint32_t const _lowFrequencies[RANGESIZE] =
{
	200,
	250,
	300,
	350,
	400,
	450,
	500,
	550,
	600,
	650,
	700,
	750,
	800,
	850,
	900,
	950,
	1000,
	1500,
	2000,
	2500,
	3000,
	3500,
	4000,
	4500,
	5000,
	6000,
	7000,
	8000,
	9000,
	10000,
	11000,
	12000
};

/** mid frequency range for PWM output */
static uint32_t const _midFrequencies[RANGESIZE] =
{
	14000,
	16000,
	18000,
	20000,
	22000,
	24000,
	26000,
	28000,
	30000,
	32000,
	34000,
	36000,
	38000,
	40000,
	42000,
	44000,
	46000,
	48000,
	50000,
	55000,
	60000,
	65000,
	70000,
	75000,
	80000,
	85000,
	90000,
	95000,
	100000,
	105000,
	110000,
	115000
};

/** high frequency range for PWM output */
static uint32_t const _highFrequencies[RANGESIZE] =
{
	120000,
	125000,
	130000,
	135000,
	140000,
	145000,
	150000,
	155000,
	160000,
	165000,
	170000,
	180000,
	190000,
	200000,
	210000,
	220000,
	230000,
	240000,
	250000,
	260000,
	270000,
	280000,
	290000,
	300000,
	310000,
	320000,
	330000,
	340000,
	350000,
	360000,
	370000,
	375000
};

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* PWMFREQUENCIES_H_ */
