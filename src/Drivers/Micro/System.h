/***************************************************************************//**
    \file System.h
    \brief System module driver

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This header file provides functions to configure the system
       control module on MSP430x5xx microcontrollers.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/System.h$
    $Revision: 3$
    $Date: Tuesday, October 02, 2012 6:14:15 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef SYSTEM_H_
#define SYSTEM_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Possible system reset reasons */
typedef enum SystemResetReason
{
    SystemResetReason_INVALID = 0x00u,
    SystemResetReason_BROWNOUT = 0x02u,
    SystemResetReason_RESET_PIN = 0x04u,
    SystemResetReason_PMM_SW_BOR = 0x06u,
    SystemResetReason_WAKEUP_FROM_LPMXd5 = 0x08u,
    SystemResetReason_SECURITY_VIOLATION = 0x0Au,
    SystemResetReason_SVSL = 0x0Cu,
    SystemResetReason_SVSH = 0x0Eu,
    SystemResetReason_SVML_OVP = 0x10u,
    SystemResetReason_SVMH_OVP = 0x12u,
    SystemResetReason_PMM_SW_POR = 0x14u,
    SystemResetReason_WATCHDOG = 0x16u,
    SystemResetReason_WDT_PASSWORD_VIOLATION = 0x18u,
    SystemResetReason_FLASH_PASSWORD_VIOLATION = 0x1Au,
    SystemResetReason_PLL_UNLOCK = 0x1Cu,
    SystemResetReason_PERF = 0x1Eu,
    SystemResetReason_PMM_PASSWORD_VIOLATION = 0x20u
} SystemResetReason_t;
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void System_Init(void);

void System_PowerOn(void);
void System_PowerOff(void);

SystemResetReason_t System_GetResetReason(void);
const char_t* System_ConvertResetReasonToString(
		SystemResetReason_t reason);

#endif /* SYSTEM_H_ */
