/***************************************************************************//**
    \file FailSequenceData.h
    \brief Contains the sequence data information for the star wars UI
           sequence.

    \b Description Contains the sequence data information for the star wars
                   "special" UI sequence.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiSequenceData/FailSequenceData.h$
    $Revision: 2$
    $Date: Friday, October 24, 2014 2:10:34 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __FAILSEQUENCEDATA_H__
#define __FAILSEQUENCEDATA_H__

#include <ProjectTypes.h>
#include <UiManagerSequenceData.h>
#include <NoteDurationToMs.h>

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/*-----------------------PUBLIC CONSTANTS-----------------------------------*/

static const BuzzerSequenceStep_t failBuzzerSteps[] =
{
        { /* step 1 */
                .pitch = PitchFrequency_C6,
                .durationMs = NoteDurToMs_HALF,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SIXTEENTH,
                .enabled = true,
        },
        {
                .pitch = PitchFrequency_F5,
                .durationMs = NoteDurToMs_SAME_NOTE_GAP,
                .enabled = false,
        },
};

static const BackLightSequenceStep_t failBacklightSteps[] =
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

static const UiSequenceData_t failSequence =
{
    .buzzerSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(failBuzzerSteps),
        .stepList = failBuzzerSteps,
    },
    .backLightSequence =
    {
        .stepsInSequence = ARRAY_LENGTH(failBacklightSteps),
        .stepList = failBacklightSteps,
    },
};

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __FAILSEQUENCEDATA_H__ */

