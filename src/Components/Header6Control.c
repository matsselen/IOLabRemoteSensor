/***************************************************************************//**
    \file Header6Control.c
    \brief Controls header pin 6

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 6.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header6Control.c$
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
#include <Header6Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode header pin 6 is in */
static Header6Mode_t _header6Mode = Header6Mode_INVALID;

/** Tracks what output value header pin 6 was last set to */
static Header6OutputValue_t _headerOutputValue = DEFAULT_HEADER6_OUTPUT_VALUE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header6Control_SetPinOutput(void);

/***************************************************************************//**
    Initialize header pin 6.
*//****************************************************************************/
void Header6Control_Init()
{

}

/***************************************************************************//**
    Sets a configuration value to header pin 6

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header6Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header6Key_MODE:
			if ((((Header6Mode_t)value) >= Header6Mode_FIRST) && (((Header6Mode_t)value) < Header6Mode_LAST))
			{
				_header6Mode = (Header6Mode_t)(value);
				switch (_header6Mode)
				{
					case Header6Mode_DISABLE:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN,
						                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_headerOutputValue = Header6OutputValue_LOW;
						_Header6Control_SetPinOutput();
						break;

					case Header6Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN,
										   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header6Mode_OUTPUT:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN,
										  GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header6Control_SetPinOutput();
						break;

					default:
						DEBUG("header header6 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header6 mode");
			}
			break;

		case Header6Key_OUTPUT_VALUE:
			if ((((Header6OutputValue_t)value) >= Header6OutputValue_FIRST) && (((Header6OutputValue_t)value) < Header6OutputValue_LAST))
			{
				_headerOutputValue = (Header6OutputValue_t)(value);
				_Header6Control_SetPinOutput();
			}
			else
			{
				DEBUG("invalid header6 low frequency");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header6Control_SetPinOutput(void)
{
	if(_header6Mode != Header6Mode_INPUT)
	{
		if(_headerOutputValue == Header6OutputValue_LOW)
		{
			GPIO_CLEAR(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN);
		}
		else
		{
			GPIO_SET(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN);
		}
	}
}
