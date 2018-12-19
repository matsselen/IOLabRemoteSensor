/***************************************************************************//**
    \file UiManager.h
    \brief Manages Feedback to the user.

    \b Description Manages feedback to the user controlling the buzzer and the
                   backlight LEDs.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiManager.h$
    $Revision: 1$
    $Date: Thursday, October 23, 2014 5:20:17 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Define a list of sequences that the UI can indicate */
typedef enum UiSequence
{
    UiSequence_SUCCESS,
    UiSequence_FAIL,
    UiSequence_SPECIAL,
    UiSequence_IN_PROGRESS,
} UiSequence_t;

/** Defines the possible results of the UiManager_PlaySequence function. */
typedef enum PlaySequenceStatuses
{
    PlaySequenceStatus_INVALID = -1,
    PlaySequenceStatus_FIRST   = 0u,
    PlaySequenceStatus_SUCCESS = PlaySequenceStatus_FIRST,
    PlaySequenceStatus_BUSY,
    PlaySequenceStatus_UNKNOWN_SEQUENCE,
    PlaySequenceStatus_LAST,
} PlaySequenceStatus_t;

/** Defines the play modes that the sequence can play with */
typedef enum PlayModes
{
    PlayMode_SINGLE,
    PlayMode_REPEAT
} PlayMode_t;

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

void UiManager_Init();
PlaySequenceStatus_t UiManager_PlaySequence(UiSequence_t sequence, PlayMode_t mode);
void UiManager_StopSequence(void);
bool UiManager_SequenceInProgress(void);

#endif /* __UIMANAGER_H__ */

