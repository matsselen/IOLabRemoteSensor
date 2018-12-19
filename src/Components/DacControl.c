/***************************************************************************//**
    \file DacControl.c
    \brief Controls the hardware DAC

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the hardware DAC.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/DacControl.c$
    $Revision: 1$
    $Date: Friday, August 10, 2012 5:44:25 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "DacControl.h"
#include <Dac.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode the DAC is in */
DacMode_t _dacMode = DacMode_INVALID;
/** Tracks what amplitude the DAC is set at */
DacAmplitude_t _dacAmplitude = DEFAULT_DAC_AMPLITUDE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _DacControl_SetAmplitude(void);

/***************************************************************************//**
    Initialized the DAC and driver.
*//****************************************************************************/
void DacControl_Init()
{
	DacInit();
}

/***************************************************************************//**
    Sets a configuration value to the DAC

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void DacControl_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case DacKey_MODE:
			if ((((DacMode_t)value) >= DacMode_FIRST) && (((DacMode_t)value) < DacMode_LAST))
			{
				_dacMode = (DacMode_t)(value);
				switch (_dacMode)
				{
					case DacMode_DISABLE:
						DacSetVoltage(0u);
						break;

					case DacMode_DC:
						_DacControl_SetAmplitude();
						break;


					default:
						DEBUG("dac unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid dac mode");
			}
			break;

		case DacKey_AMPLITUDE:
			if ((((DacAmplitude_t)value) >= DacAmplitude_FIRST) && (((DacAmplitude_t)value) < DacAmplitude_LAST))
			{
				_dacAmplitude = (DacAmplitude_t)(value);
				_DacControl_SetAmplitude();
			}
			else
			{
				DEBUG("invalid dac amplitude");
			}
			break;


		default:
			break;
	}
}

/***************************************************************************//**
    Sets the output of the DAC to the amplitude stored in _dacAmplitude.
*//****************************************************************************/
static void _DacControl_SetAmplitude(void)
{
	/* scale the amplitude to take up the entire UINT8 range */
	if (_dacMode == DacMode_DC)
	{
		DacSetVoltage(((uint8_t)_dacAmplitude) * UINT8_T_MAX / (((uint8_t)DacAmplitude_LAST) - 1u));
	}
}
