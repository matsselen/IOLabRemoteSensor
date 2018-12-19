/***************************************************************************//**
    \file ConfigurationManager.h
    \brief The configuration manager keeps track of all the sensor configurations

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The configuration manager is the module that will keep track of
       configuration all the sensors to be in the proper sensor mode based on the
       setup commands.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/ConfigurationManager.h$
    $Revision: 17$
    $Date: Friday, October 24, 2014 10:53:10 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include <FixedConfigList.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void ConMan_Init();
void ConMan_SetSensorConfig(
		uint16_t configLength,
		const uint8_t config[]);
void ConMan_SetFixedConfig(
		uint16_t configLength,
		const uint8_t config[]);
FixedConfigList_t ConMan_GetFixedConfig(void);
void ConMan_SetOutputConfig(
		uint16_t configLength,
		const uint8_t config[]);
const uint8_t* ConMan_GetPacketConfig(uint16_t *packetLength);
void ConMan_GetCalibration(
		SensorId_t sensorId,
		uint16_t   maxLength,
		uint16_t  *actualLength,
		uint8_t    calibration[]);
void ConMan_StartData();
void ConMan_StopData();

#endif /* CONFIGURATIONMANAGER_H_ */
