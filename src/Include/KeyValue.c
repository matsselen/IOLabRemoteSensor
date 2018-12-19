/***************************************************************************//**
    \file KeyValue.c
    \brief Contains the definitions of the Key-Value pairs

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Contains the definitions of the Key-Value pairs used for
                   configuring the different sensors in the IOLab system. The
                   keys and values defined in this module are based on the
                   document 1814F08 Data Protocol Key-Value table.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/KeyValue.c$
    $Revision: 3$
    $Date: Monday, August 20, 2012 9:17:15 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "KeyValue.h"

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the bit mask for the value portion of the key-value */
#define VALUE_BIT_MASK   ( 0x1F )

/*-----------------------LOCAL VARIABLES-------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/

/***************************************************************************//**
    Separate the key and the value from the KeyValue pair.

    \param[in] keyValue - The key-value pair
    \param[out] key - the key from the key value pair
    \param[out] value - the value from the key value pair

*//****************************************************************************/
void KeyValue_Separate(uint8_t keyValue, Key_t *key, Value_t *value)
{
	ASSERT(((key != NULL) && (value != NULL)), ErrorLedMode_SOFTWARE);

	*key = (Key_t)(keyValue & ((uint8_t)~(VALUE_BIT_MASK)));
	*value = (Value_t)(keyValue & VALUE_BIT_MASK);
}

/***************************************************************************//**
    Combine the key and the value to form the KeyValue pair.

    \param[out] key - the key from the key value pair
    \param[out] value - the value from the key value pair

    \return The key-value pair made from the input key and value

*//****************************************************************************/
KeyValue_t KeyValue_Combine(Key_t key, Value_t value)
{
	return (KeyValue_t)(((Key_t)(key & ((Key_t)~(VALUE_BIT_MASK)))) | ((Value_t)(value & VALUE_BIT_MASK)));
}
