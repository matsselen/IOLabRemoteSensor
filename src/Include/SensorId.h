/***************************************************************************//**
    \file SensorId.h
    \brief Sensor Id stores the list of Sensor IDs.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Stores the list of sensor IDs to lookup sensor ID numbers.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/SensorId.h$
    $Revision: 5$
    $Date: Thursday, October 02, 2014 9:32:16 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef SENSORID_H_
#define SENSORID_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
typedef enum SensorId
{
	SensorId_INVALID       	= 0,
	SensorId_FIRST         	= 0x01u,
	SensorId_ACCELEROMETER 	= SensorId_FIRST,
	SensorId_MAGNETOMETER  	= 0x02u,
	SensorId_GYROSCOPE     	= 0x03u,
	SensorId_BAROMETER     	= 0x04u,
	SensorId_ULTRASONIC    	= 0x05u,
	SensorId_MICROPHONE    	= 0x06u,
	SensorId_AMBIENT_LIGHT 	= 0x07u,
	SensorId_FORCE_GAUGE   	= 0x08u,
	SensorId_ENCODER       	= 0x09u,
	SensorId_ECG           	= 0x0Au,
	SensorId_BATTERY       	= 0x0Bu,
	SensorId_HIGH_GAIN_IN  	= 0x0Cu,
	SensorId_DIGITALINPUTS 	= 0x0Du,
	SensorId_HEADER_0      	= 0x0Eu,
	SensorId_HEADER_1      	= 0x0Fu,
	SensorId_HEADER_2      	= 0x10u,
	SensorId_HEADER_3      	= 0x11u,
	SensorId_HEADER_4      	= 0x12u,
	SensorId_HEADER_5      	= 0x13u,
	SensorId_HEADER_6      	= 0x14u,
	SensorId_HEADER_7      	= 0x15u,
	SensorId_HEADER_8      	= 0x16u,
	SensorId_HEADER_9      	= 0x17u,
	SensorId_BUZZER        	= 0x18u,
	SensorId_DAC           	= 0x19u,
	SensorId_THERMOMETER    = 0x1Au,
	SensorId_ECG6CHAN       = 0x1Bu,
	SensorId_LAST
} SensorId_t;

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* SENSORID_H_ */
