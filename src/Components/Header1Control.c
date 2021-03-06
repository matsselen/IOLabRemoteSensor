/***************************************************************************//**
    \file Header1Control.c
    \brief Controls header pin 1

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 1.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header1Control.c$
    $Revision: 1$
    $Date: Thursday, September 13, 2012 9:44:14 PM$
    $Author: netaylor$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>
#include <Gpio.h>
#include <Board.h>
#include <Device.h>
#include <Header1Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode header pin 1 is in */
static Header1Mode_t _header1Mode = Header1Mode_INVALID;

/** Tracks what output value header pin 1 was last set to */
static Header1OutputValue_t _headerOutputValue = DEFAULT_HEADER1_OUTPUT_VALUE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header1Control_SetPinOutput(void);

/***************************************************************************//**
    Initialize header pin 1.
*//****************************************************************************/
void Header1Control_Init()
{

}

/***************************************************************************//**
    Sets a configuration value to header pin 1

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header1Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header1Key_MODE:
			if ((((Header1Mode_t)value) >= Header1Mode_FIRST) && (((Header1Mode_t)value) < Header1Mode_LAST))
			{
				_header1Mode = (Header1Mode_t)(value);
				switch (_header1Mode)
				{
					case Header1Mode_DISABLE:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN,
						                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_headerOutputValue = Header1OutputValue_LOW;
						_Header1Control_SetPinOutput();
						break;

					case Header1Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN,
										   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header1Mode_OUTPUT:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN,
										  GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header1Control_SetPinOutput();
						break;

					default:
						DEBUG("header header1 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header1 mode");
			}
			break;

		case Header1Key_OUTPUT_VALUE:
			if ((((Header1OutputValue_t)value) >= Header1OutputValue_FIRST) && (((Header1OutputValue_t)value) < Header1OutputValue_LAST))
			{
				_headerOutputValue = (Header1OutputValue_t)(value);
				_Header1Control_SetPinOutput();
			}
			else
			{
				DEBUG("invalid header1 low frequency");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header1Control_SetPinOutput(void)
{
	if(_header1Mode != Header1Mode_INPUT)
	{
		if(_headerOutputValue == Header1OutputValue_LOW)
		{
			GPIO_CLEAR(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN);
		}
		else
		{
			GPIO_SET(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN);
		}
	}
}
