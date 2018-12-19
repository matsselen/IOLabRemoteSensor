/***************************************************************************//**
    \file UiManagerSequenceData.h
    \brief Stores the data for the different sequences in the UI

    \b Description Stores the data for the different sequences in the UI.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiManagerSequenceData.h$
    $Revision: 1$
    $Date: Tuesday, October 21, 2014 4:43:33 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __UIMANAGERSEQUENCEDATA_H__
#define __UIMANAGERSEQUENCEDATA_H__

#include <PitchToFrequency.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Defines the contents of a single step in a buzzer sequence */
typedef struct BuzzerSequenceStep
{
    PitchFrequency_t pitch;
    uint16_t durationMs;
    bool enabled;
} BuzzerSequenceStep_t;

/** Define a structure that holds a buzzer sequence */
typedef struct BuzzerSequence
{
    uint16_t stepsInSequence;
    const BuzzerSequenceStep_t *const stepList;
} BuzzerSequence_t;

/** Define a structure that holds a single step in the backlight sequence */
typedef struct BackLightSequenceStep
{
    bool backlightOn;
    uint16_t durationMs;
} BackLightSequenceStep_t;

/** Define a structure to hold a backlight sequence */
typedef struct BackLightSequence
{
    uint16_t stepsInSequence;
    const BackLightSequenceStep_t *const stepList;
} BackLightSequence_t;

/** Define a structure that will hold the settings for a UI sequence */
typedef struct UiSequenceData
{
    const BuzzerSequence_t buzzerSequence;
    const BackLightSequence_t backLightSequence;
} UiSequenceData_t;

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __UIMANAGERSEQUENCEDATA_H__ */

