/***************************************************************************//**
    \file SelfTest.h
    \brief Manages the self test mode.

    \b Description Handles the sequencing, configuration, and data management
                   for the self test mode.
*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/SelfTest.h$
    $Revision: 1$
    $Date: Friday, October 03, 2014 9:17:05 AM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __SELFTEST_H__
#define __SELFTEST_H__

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
extern void SelfTest_Init(void);
extern void SelfTest_Start(void);
extern void SelfTest_Process(void);


#endif /* __SELFTEST_H__ */

