/***************************************************************************//**
    \file StarWarsSequenceData.h
    \brief Contains the sequence data information for the star wars UI
           sequence.

    \b Description Contains the sequence data information for the star wars
                   "special" UI sequence.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiSequenceData/StarWarsSequenceData.h$
    $Revision: 1$
    $Date: Tuesday, October 21, 2014 4:43:48 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __STARWARSSEQUENCEDATA_H__
#define __STARWARSSEQUENCEDATA_H__

#include <ProjectTypes.h>
#include <UiManagerSequenceData.h>
#include <NoteDurationToMs.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC CONSTANTS-----------------------------------*/

static const BuzzerSequenceStep_t starWarsBuzzerSteps[] =
{
        { /* step 1 */
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_HALF,
                .enabled = true,
        },
        { /* step 2 */
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_HALF,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F6,
                .durationMs = NoteDurToMs_TRIPLET,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E6,
                .durationMs = NoteDurToMs_TRIPLET,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D6,
                .durationMs = NoteDurToMs_TRIPLET,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C7,
                .durationMs = NoteDurToMs_HALF,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_HALF,
                .enabled = true,
        },
};

static const BackLightSequenceStep_t starWarsBacklightSteps[] =
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

static const UiSequenceData_t starWarsSequence =
{
    .buzzerSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(starWarsBuzzerSteps),
        .stepList = starWarsBuzzerSteps,
    },
    .backLightSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(starWarsBacklightSteps),
        .stepList = starWarsBacklightSteps,
    },
};

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __STARWARSSEQUENCEDATA_H__ */

