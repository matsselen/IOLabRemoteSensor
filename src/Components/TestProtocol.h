/***************************************************************************//**
    \file TestProtocol.h
    \brief Test Protocol header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Header file for the test protocol module.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/TestProtocol.h$
    $Revision: 1$
    $Date: Friday, May 25, 2012 4:27:30 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef TESTPROTOCOL_H_
#define TESTPROTOCOL_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/


/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void TestProtocol_Init(void);
void TestProtocol_HandleReceivedPacket(void);

#endif /* TESTPROTOCOL_H_ */
