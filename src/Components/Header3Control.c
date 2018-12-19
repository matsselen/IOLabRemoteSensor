/***************************************************************************//**
    \file Header3Control.c
    \brief Controls header pin 3

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for header pin 3.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/Header3Control.c$
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
#include <Header3Control.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode header pin 3 is in */
static Header3Mode_t _header3Mode = Header3Mode_INVALID;

/** Tracks what output value header pin 3 was last set to */
static Header3OutputValue_t _headerOutputValue = DEFAULT_HEADER3_OUTPUT_VALUE;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _Header3Control_SetPinOutput(void);

/***************************************************************************//**
    Initialize header pin 3.
*//****************************************************************************/
void Header3Control_Init()
{

}

/***************************************************************************//**
    Sets a configuration value to header pin 3

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void Header3Control_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case Header3Key_MODE:
			if ((((Header3Mode_t)value) >= Header3Mode_FIRST) && (((Header3Mode_t)value) < Header3Mode_LAST))
			{
				_header3Mode = (Header3Mode_t)(value);
				switch (_header3Mode)
				{
					case Header3Mode_DISABLE:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN,
						                   GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_headerOutputValue = Header3OutputValue_LOW;
						_Header3Control_SetPinOutput();
						break;

					case Header3Mode_INPUT:
						/* configure pin as input */
						Gpio_Configure(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN,
										   GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						break;

					case Header3Mode_OUTPUT:
						/*configure pin as output */
						Gpio_Configure(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN,
										  GpioType_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
						_Header3Control_SetPinOutput();
						break;

					default:
						DEBUG("header header3 unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid header3 mode");
			}
			break;

		case Header3Key_OUTPUT_VALUE:
			if ((((Header3OutputValue_t)value) >= Header3OutputValue_FIRST) && (((Header3OutputValue_t)value) < Header3OutputValue_LAST))
			{
				_headerOutputValue = (Header3OutputValue_t)(value);
				_Header3Control_SetPinOutput();
			}
			else
			{
				DEBUG("invalid header3 low frequency");
			}
			break;

		default:
			break;
	}
}

/***************************************************************************//**

*//****************************************************************************/
static void _Header3Control_SetPinOutput(void)
{
	if(_header3Mode != Header3Mode_INPUT)
	{
		if(_headerOutputValue == Header3OutputValue_LOW)
		{
			GPIO_CLEAR(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN);
		}
		else
		{
			GPIO_SET(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN);
		}
	}
}
