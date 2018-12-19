/***************************************************************************//**
    \file SuccessSequenceData.h
    \brief Contains the sequence data information for the star wars UI
           sequence.

    \b Description Contains the sequence data information for the star wars
                   "special" UI sequence.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiSequenceData/SuccessSequenceData.h$
    $Revision: 1$
    $Date: Wednesday, October 22, 2014 4:49:13 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __SUCCESSSEQUENCEDATA_H__
#define __SUCCESSSEQUENCEDATA_H__

#include <ProjectTypes.h>
#include <UiManagerSequenceData.h>
#include <NoteDurationToMs.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC CONSTANTS-----------------------------------*/

static const BuzzerSequenceStep_t successBuzzerSteps[] =
{
        { /* step 1 */
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_HALF,
                .enabled = false,
        },
        { /* step 2 */
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C7,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C7,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C7,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C7,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
};

static const BackLightSequenceStep_t successBacklightSteps[] =
{
        { /* step 1 */
                .backlightOn = false,
                .durationMs = 100,
        },
        { /* step 2 */
                .backlightOn = true,
                .durationMs = 100,
        },
        { /* step 3 */
                .backlightOn = false,
                .durationMs = 100,
        },
};

static const UiSequenceData_t successSequence =
{
    .buzzerSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(successBuzzerSteps),
        .stepList = successBuzzerSteps,
    },
    .backLightSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(successBacklightSteps),
        .stepList = successBacklightSteps,
    },
};

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __SUCCESSSEQUENCEDATA_H__ */

