/***************************************************************************//**
    \file GirlILeftBehindSequenceData.h
    \brief Contains the sequence data information for the Girl I Left Behind UI
           sequence.

    \b Description Contains the sequence data information for the Girl I Left
                   Behind "special" UI sequence.  The song is a public domain
                   Irish/British folk tune.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiSequenceData/GirlILeftBehindSequenceData.h$
    $Revision: 1$
    $Date: Thursday, October 23, 2014 7:28:37 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __GIRLILEFTBEHINDSEQUENCEDATA_H__
#define __GIRLILEFTBEHINDSEQUENCEDATA_H__

#include <ProjectTypes.h>
#include <UiManagerSequenceData.h>
#include <NoteDurationToMs.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC CONSTANTS-----------------------------------*/


static const BuzzerSequenceStep_t girlILeftBehindBuzzerSteps[] =
{
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_Fs6_Gb6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_A5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER - NoteDurToMs_SAME_NOTE_GAP,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER - NoteDurToMs_SAME_NOTE_GAP,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_A5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D6,
                .durationMs = NoteDurToMs_DOTTED_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_Fs6_Gb6,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D6,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_A5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_B5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_A5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_Fs5_Gb5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_A5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_D5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_E5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_Fs5_Gb5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_DOTTED_QUARTER,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_EIGHTH,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_G5,
                .durationMs = NoteDurToMs_QUARTER,
                .enabled = true,
        },
};

static const BackLightSequenceStep_t girlILeftBehindBacklightSteps[] =
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

static const UiSequenceData_t girlILeftBehindSequence =
{
    .buzzerSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(girlILeftBehindBuzzerSteps),
        .stepList = girlILeftBehindBuzzerSteps,
    },
    .backLightSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(girlILeftBehindBacklightSteps),
        .stepList = girlILeftBehindBacklightSteps,
    },
};

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __GIRLILEFTBEHINDSEQUENCEDATA_H__ */

