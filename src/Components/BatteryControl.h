/***************************************************************************//**
    \file BatteryControl.h
    \brief Controls the battery voltage readings.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls reading the battery voltage.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BatteryControl.h$
    $Revision: 2$
    $Date: Thursday, September 06, 2012 7:24:43 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BATTERYCONTROL_H_
#define BATTERYCONTROL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

typedef uint16_t BatteryCounts_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void BatteryControl_Init(void);
void BatteryControl_SetConfig(uint8_t config);
BatteryCounts_t BatteryControl_GetCounts(void);
void BatteryControl_ReadCalibration(
		uint16_t  maxCalibrationLength,
		uint16_t *actualCalibrationLength,
		uint8_t   calibration[]);

#endif /* BATTERYCONTROL_H_ */
