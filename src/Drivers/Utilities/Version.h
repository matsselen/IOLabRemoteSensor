/***************************************************************************//**
    \file Version.h
    \brief Version header

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The version module provides access to version information such
                   as the firmware revision number and the DIE ID.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Version.h$
    $Revision: 33$
    $Date: Friday, July 17, 2015 2:25:47 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

    Mats: Modifying minor firmware version number, starting with 0x20, as I make changes

*******************************************************************************/
#ifndef VERSION_H_
#define VERSION_H_

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Defines the major version of the firmware */
#define USER_FIRMWARE_MAJOR           (0x01u)
/** Defines the minor version of the firmware */

/* Mats: #define USER_FIRMWARE_MINOR           (0x10u) */
/* Mats: Designate my own modifications by bumping the minor version number starting at 0x20 */
/* Mats: version 0x20 is same as 0x10 plus 5 minute Idle Timeout feature */ 
/* Mats: version 0x21 is same as 0x20 plus new 4.8 kHz A7 readout configuration */ 
/* Mats: version 0x22 is same as 0x21 plus self test volume is always medium */ 
/* Mats: version 0x23 is same as 0x22 plus self A7+A8 and A7+A8+A8 high speed configurations */ 
/* Mats: version 0x24 is same as 0x23 plus a high speed 8g accelerometer configurations */ 
#define USER_FIRMWARE_MINOR           (0x24u)

/** Defines the user firmware version with the high byte the major release and the
    low byte the minor release.
 */
#define USER_FIRMWARE_VERSION         ((USER_FIRMWARE_MAJOR<<8)+USER_FIRMWARE_MINOR)

/** Defines the user hardware version with the high byte the major release and the
    low byte the minor release.
 */
#define USER_HARDWARE_VERSION         (0x0003u)

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
uint32_t WaferId(void);
uint16_t WaferXPosition(void);
uint16_t WaferYPosition(void);
uint16_t DeviceId(void);
uint8_t HardwareRevisionMicro(void);
uint8_t FirmwareRevisionMicro(void);
uint16_t UserFirmwareVersion(void);
const char * UserFirmwareVersionString(void);
uint16_t UserHardwareVersion(void);

#endif /* VERSION_H_ */
