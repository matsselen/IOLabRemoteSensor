/***************************************************************************//**
    \file Version.c
    \brief Version information about the device and firmware.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The version module stores version information about the device
                   and the firmware.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Version.c$
    $Revision: 3$
    $Date: Tuesday, June 04, 2013 2:35:17 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Version.h"
#include <Device.h>
#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL CONSTANTS  ------------------------------------*/
/** Define a local array that has a fixed sequence of bytes before the version
    number so that the version number can be located in the binary code image */
static const uint8_t VERSION_ARRAY[8u] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, USER_FIRMWARE_MAJOR, USER_FIRMWARE_MINOR };

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** define macros to use the version number from the version number array so that
    the array doesn't get left out by the linker */
#define VERSION_MAJOR	(VERSION_ARRAY[6u])
#define VERSION_MINOR	(VERSION_ARRAY[7u])

/*-----------------------LOCAL VARIABLES-------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
/***************************************************************************//**
    Reads the 4 byte Wafer Id from the microcontroller.

    \return The 4-byte Wafer Id of the microcontroller.
*//****************************************************************************/
uint32_t WaferId(void)
{
	static const uint32_t* const waferIdLocation = (uint32_t*)DEVICE_WAFER_ID_ADDRESS;
	return *waferIdLocation;
}

/***************************************************************************//**
    Reads the 2 byte Wafer X Position from the microcontroller.

    \return The 2-byte Wafer X Position of the microcontroller.
*//****************************************************************************/
uint16_t WaferXPosition(void)
{
	static const uint16_t* const xPosition = (uint16_t*)DEVICE_WAFER_X_POS;
	return *xPosition;
}

/***************************************************************************//**
    Reads the 2 byte Wafer Y position from the microcontroller.

    \return The 2-byte Wafer Y position from the microcontroller.
*//****************************************************************************/
uint16_t WaferYPosition(void)
{
	static const uint16_t* const yPosition = (uint16_t*)DEVICE_WAFER_Y_POS;
	return *yPosition;
}

/***************************************************************************//**
    Reads the 2 byte Device Id from the microcontroller.

    MSP430F5329 should be 0x811B
    MSP430F5328 should be 0x811A
    MSP430F5327 should be 0x8119
    MSP430F5326 should be 0x8118
    MSP430F5325 should be 0x8117
    MSP430F5324 should be 0x8116

    \return The 2-byte Device Id of the microcontroller.
*//****************************************************************************/
uint16_t DeviceId(void)
{
	static const uint16_t* const deviceIdLocation = (uint16_t*)DEVICE_ID_ADDRESS;
	return *deviceIdLocation;
}

/***************************************************************************//**
    Reads the microcontroller hardware revision.

    \return The 1-byte hardware revision.
*//****************************************************************************/
uint8_t HardwareRevisionMicro(void)
{
	static const uint8_t* const deviceHwLocation = (uint8_t*)DEVICE_HW_REV_ADDRESS;
	return *deviceHwLocation;
}

/***************************************************************************//**
    Reads the microcontroller firmware revision.

    \return The 1-byte firmware revision.
*//****************************************************************************/
uint8_t FirmwareRevisionMicro(void)
{
	static const uint8_t* const deviceFwLocation = (uint8_t*)DEVICE_FW_REV_ADDRESS;
	return *deviceFwLocation;
}

/***************************************************************************//**
    Reads the 2 byte firmware version of the code. The high byte is the major
    revision version and the low byte is the minor revision version.

    \return The 2-byte firmware version.
*//****************************************************************************/
uint16_t UserFirmwareVersion(void)
{
	return (uint16_t) USER_FIRMWARE_VERSION;
}

const char * UserFirmwareVersionString(void)
{
	static char_t versionString[6];
	versionString[0] = (char_t)((uint8_t)('0') + (uint8_t)(VERSION_MAJOR >> 4u));
	versionString[1] = (char_t)((uint8_t)('0') + (uint8_t)(VERSION_MAJOR & 0x0Fu));
	versionString[2] = '.';
	versionString[3] = (char_t)((uint8_t)('0') + (uint8_t)(VERSION_MINOR >> 4u));
	versionString[4] = (char_t)((uint8_t)('0') + (uint8_t)(VERSION_MINOR & 0x0Fu));
	versionString[5] = '\0';
	return &versionString[0];
}

/***************************************************************************//**
    Reads the 2 byte hardware version of the PCB. The high byte is the major
    revision version and the low byte is the minor revision version.

    \return The 2-byte hardware version.
*//****************************************************************************/
uint16_t UserHardwareVersion(void)
{
	return (uint16_t) USER_HARDWARE_VERSION;
}
