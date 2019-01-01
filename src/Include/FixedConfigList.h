/***************************************************************************//**
    \file FixedConfigList.h
    \brief Contains the list of the fixed config options

    \b Description Contains the list of the fixed configuration options.  The
                   list contains all of the possible settings for the fixed
                   configurations in the system.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/FixedConfigList.h$
    $Revision: 2$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

    Mats: 1/1/2019. Added new configuration to read A7 at 4.8 kHz

*******************************************************************************/

#ifndef __FIXEDCONFIGLIST_H__
#define __FIXEDCONFIGLIST_H__

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** Defines a list of the fixed configuration options */
typedef enum FixedConfigList
{
    FixedConfigList_INVALID     = 0x0u,
    FixedConfigList_FIRST       = 0x01u,
    /* Low Data Rate Configurations */
    FixedConfigList_GYRO        = 0x01u,
    FixedConfigList_ACCEL       = 0x02u,
    FixedConfigList_IMU         = 0x03u,
    FixedConfigList_MINIMOTION  = 0x04u,
    FixedConfigList_PENDULUM    = 0x05u,
    FixedConfigList_AMBIENT     = 0x06u,
    FixedConfigList_ECG         = 0x07u,
    FixedConfigList_HEADER      = 0x08u,
    FixedConfigList_SLOW_MIC    = 0x09u,
    FixedConfigList_MAGNETIC    = 0x0Au,
    FixedConfigList_ANLG_HEAD   = 0x0Bu,
    FixedConfigList_HEADER_3V3  = 0x0Cu,

    /* Mats New Configurations */
    FixedConfigList_ANALOG_HI1  = 0x15u,

    /* High Data Rate Configurations */
    FixedConfigList_GYRO_HI     = 0x20u,
    FixedConfigList_ACCEL_HI    = 0x21u,
    FixedConfigList_IMU_HI      = 0x22u,
    FixedConfigList_MOTION      = 0x23u,
    FixedConfigList_SPORTS      = 0x24u,
    FixedConfigList_PENDULUM_HI = 0x25u,
    FixedConfigList_KITCHENSINK = 0x26u,
    FixedConfigList_MICROPHONE  = 0x27u,
    FixedConfigList_ALS_HI      = 0x28u,
    FixedConfigList_ALS_ACC     = 0x29u,
    FixedConfigList_FORCE_ACC   = 0x2Au,
    FixedConfigList_ALS_MIC     = 0x2Bu,
    FixedConfigList_HI_ECG      = 0x2Cu,
    FixedConfigList_HI_GAIN     = 0x2Du,
    FixedConfigList_HI_FORCE    = 0x2Eu,
    FixedConfigList_HI_ANLG_HED = 0x2Fu,
    FixedConfigList_ECG6        = 0x30u,
    FixedConfigList_HI_ECG6     = 0x31u,
    /* Self test configuration */
    FixedConfigList_SELFTEST   = 0x32u,
    FixedConfigList_LAST
} FixedConfigList_t;
/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __FIXEDCONFIGLIST_H__ */

