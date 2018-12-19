/***************************************************************************//**
    \file Header2Control.c
    \brief Controls header pin 2

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 2.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header2Control.c$
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
#include <Header2Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode header pin 2 is in */
static Header2Mode_t _header2Mode = Header2Mode_INVALID;

/** Tracks what output value header pin 2 was last set to */
static Header2OutputValue_t _headerOutputValue = DEFAULT_HEADER2_OUTPUT_VALUE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header2Control_SetPinOutput(void);

/***************************************************************************//**
    Initialize header pin 2.
*//****************************************************************************/
void Header2Control_Init()
{

}

/***************************************************************************//**
    Sets a configuration value to header pin 2

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header2Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header2Key_MODE:
			if ((((Header2Mode_t)value) >= Header2Mode_FIRST) && (((Header2Mode_t)value) < Header2Mode_LAST))
			{
				_header2Mode = (Header2Mode_t)(value);
				switch (_header2Mode)
				{
					case Header2Mode_DISABLE:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN,
						                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_headerOutputValue = Header2OutputValue_LOW;
						_Header2Control_SetPinOutput();
						break;

					case Header2Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN,
										   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header2Mode_OUTPUT:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN,
										  GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header2Control_SetPinOutput();
						break;

					default:
						DEBUG("header header2 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header2 mode");
			}
			break;

		case Header2Key_OUTPUT_VALUE:
			if ((((Header2OutputValue_t)value) >= Header2OutputValue_FIRST) && (((Header2OutputValue_t)value) < Header2OutputValue_LAST))
			{
				_headerOutputValue = (Header2OutputValue_t)(value);
				_Header2Control_SetPinOutput();
			}
			else
			{
				DEBUG("invalid header2 low frequency");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header2Control_SetPinOutput(void)
{
	if(_header2Mode != Header2Mode_INPUT)
	{
		if(_headerOutputValue == Header2OutputValue_LOW)
		{
			GPIO_CLEAR(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN);
		}
		else
		{
			GPIO_SET(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN);
		}
	}
}
