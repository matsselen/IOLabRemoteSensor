/***************************************************************************//**
    \file GpioManager.c
    \brief Manager for the digital input and output

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The GPIO manager will allow for configuring the input and output
       of different digital pins for controlling peripherals such as push buttons,
       LEDs, or other GPIO.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/GpioManager.c$
    $Revision: 1$
    $Date: Tuesday, April 10, 2012 1:52:01 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include "GpioManager.h"

#include <ProjectTypes.h>
#include <Assert.h>
#include <Debug.h>
#include <Serial.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
