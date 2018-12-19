/***************************************************************************//**
    \file UiManager.c
    \brief Manages Feedback to the user.

    \b Description Manages feedback to the user controlling the buzzer and the
                   backlight LEDs.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/UiManager.c$
    $Revision: 2$
    $Date: Friday, July 17, 2015 2:22:35 AM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <TimerTick.h>
#include <Buzzer.h>
#include <UiManager.h>
#include <EventQueue.h>
#include <Assert.h>
#include <Debug.h>

/* Includes for the difference sequences */
#include <UiManagerSequenceData.h>
#include "UiSequenceData/StarWarsSequenceData.h"
#include "UiSequenceData/SuccessSequenceData.h"
#include "UiSequenceData/FailSequenceData.h"
#include "UiSequenceData/GirlILeftBehindSequenceData.h"


#define __DISABLE_BACKLIGHT
/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** Define a structure that keeps track of the location in the current
    sequence */
typedef struct CurrentSequence
{
    const UiSequenceData_t *currentSequence;    /** Pointer to the sequence data of the current sequence */
    uint16_t backlightIndex;                    /** The index into the backlight array of the the current sequence */
    uint16_t endOfBacklightStep;                /** The time (in ms) since the start of the sequence when the current
                                                    backlight step is over */
    uint16_t buzzerIndex;                       /** The index into the buzzer array of the current sequence */
    uint16_t endOfBuzzerStep;                   /** The time (in ms) since the start of hte sequence when the current
                                                    buzzer step is over */
    uint16_t elapsedMs;                         /** The current number of milliseconds that have elapsed since the
                                                    beginning of the sequence. */
} CurrentSequence_t;

/** Defines how often the sequence callback should run which determines the
    resolution of the sequence generator.  */
#define SEQUENCE_RESOLUTION_MS      (20u)

/** Define a buzzer duty that is not quiet, but not loud, remembering the SPL is logarithmic */
#define BUZZER_DUTY_MEDIUM_VOLUME  ( BUZZER_DUTY_MAX_VOLUME / 10u )

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/** Stores the handle to the tick timer */
static TickTimeoutHandle_t _uiTimerHandle;
/** Tracks if the UI tick is in use */
static bool _uiTimerActive = false;

/** Maintains a pointer to the active sequence */
CurrentSequence_t _activeSequence =
    {
            .currentSequence = NULL,
            .backlightIndex = 0u,
            .buzzerIndex = 0u,
            .elapsedMs = 0u,
    };

/** Tracks if the callback currently running */
static volatile bool _callbackRunning = false;

/** Tracks the current playback mode */
static PlayMode_t _playMode = PlayMode_SINGLE;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/

static void _UiTickCallBack(void);
static void _PlaySequence(void);
static void _ZeroCurrentSequence(CurrentSequence_t *sequence);

/***************************************************************************//**
    Initializes the UI manager and the dependancies that the UI manager controls
*//****************************************************************************/
void UiManager_Init()
{
#if 0 /* these are being initialized elsewhere in this application */
    Backlight_Init();
    Buzzer_Init();
#endif
}

/***************************************************************************//**
    Plays a preset sequence that the UI manager can indicate an event to the
    user.  The UI Manager can only play one sequence at a time, so it will not
    play another sequence if one is currently in progress.  It does post an event
    to the event queue when the sequence is finished playing so that another
    sequence can be played if needed.

    \param[in] sequence - sequence to play to the user
    \param[in] mode - the play mode to play the sequence with.

    \returns The status if the sequence was played and the reason if it wasn't.
*//****************************************************************************/
PlaySequenceStatus_t UiManager_PlaySequence(UiSequence_t sequence, PlayMode_t mode)
{
    const UiSequenceData_t *newSequence;
    PlaySequenceStatus_t status = PlaySequenceStatus_INVALID;
    uint8_t desiredVolume = BUZZER_DUTY_MAX_VOLUME;

    switch(sequence)
    {
        case UiSequence_SUCCESS:
            newSequence = &successSequence;
            break;

        case UiSequence_FAIL:
            newSequence = &failSequence;
            break;

        case UiSequence_SPECIAL:
            newSequence = &starWarsSequence;
            break;

        case UiSequence_IN_PROGRESS:
            desiredVolume = BUZZER_DUTY_MEDIUM_VOLUME;
            newSequence = &girlILeftBehindSequence;
            break;

        default:
            status = PlaySequenceStatus_UNKNOWN_SEQUENCE;
            break;
    }

    if ((status != PlaySequenceStatus_UNKNOWN_SEQUENCE) &&
        (_uiTimerActive == false))
    {
        Serial_SendStringNumber("UI: ", sequence);
        _activeSequence.currentSequence = newSequence;
        _playMode = mode;
        _ZeroCurrentSequence(&_activeSequence);
        _uiTimerActive = true;
        _uiTimerHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, SEQUENCE_RESOLUTION_MS, &_UiTickCallBack);
        Buzzer_SetDuty(desiredVolume);
        _PlaySequence();
        status = PlaySequenceStatus_SUCCESS;
    }
    else
    {
        DEBUG("UI in use");
        status = PlaySequenceStatus_BUSY;
    }

    return status;
}

/***************************************************************************//**
    Commands the UI manager to stop playing the current sequence.
*//****************************************************************************/
void UiManager_StopSequence(void)
{
    if (_callbackRunning)
    {
        TimerTick_CancelTimeoutFromCallback();
    }
    else
    {
        TimerTick_CancelTimeout(_uiTimerHandle);
    }

    _uiTimerActive = false;
    _activeSequence.currentSequence = NULL;

    Buzzer_Disable();
}

/***************************************************************************//**
    Queries the UiManager to check if a sequence is currently in progress.

    \returns True if a sequence is in progress.  False otherwise.
*//****************************************************************************/
bool UiManager_SequenceInProgress(void)
{
    return _uiTimerActive;
}

/***************************************************************************//**
    Handles the tick and processing needed to generate UI sequences.
*//****************************************************************************/
static void _UiTickCallBack(void)
{
    _callbackRunning = true;
    _PlaySequence();
    _callbackRunning = false;
}

/***************************************************************************//**
    Handles the playing of a sequence.  Turns on and off the appropriate outputs
    and sets the outputs to their proper values.
*//****************************************************************************/
static void _PlaySequence(void)
{
    volatile bool backlightStillRunning;
    volatile bool buzzerStillRunning;
    const BackLightSequenceStep_t *backlightStep;
    const BuzzerSequenceStep_t *buzzerStep;
    static bool buzzerEnabled = false;

    ASSERT(_activeSequence.currentSequence != NULL, ErrorLedMode_SOFTWARE);

    if (_activeSequence.endOfBacklightStep <= _activeSequence.elapsedMs)
    {
        /* make sure we aren't on the last step */
        if (_activeSequence.backlightIndex < _activeSequence.currentSequence->backLightSequence.stepsInSequence)
        {
            backlightStep = &_activeSequence.currentSequence->backLightSequence.stepList[_activeSequence.backlightIndex];
            _activeSequence.endOfBacklightStep = _activeSequence.elapsedMs + backlightStep->durationMs;
            if (backlightStep->backlightOn)
            {
#ifndef __DISABLE_BACKLIGHT
                Backlight_Enable();
#endif
            }
            else
            {
#ifndef __DISABLE_BACKLIGHT
                Backlight_Disable();
#endif
            }

            ++_activeSequence.backlightIndex;
            backlightStillRunning = true;
        }
        else
        {
            /* we are on the last step and its timer has expired */
            backlightStillRunning = false;
#ifndef __DISABLE_BACKLIGHT
            Backlight_Enable();
#endif
        }
    }
    else
    {
        backlightStillRunning = true;
    }

    if (_activeSequence.endOfBuzzerStep <= _activeSequence.elapsedMs)
    {
        /* make sure we aren't on the last step */
        if (_activeSequence.buzzerIndex < _activeSequence.currentSequence->buzzerSequence.stepsInSequence)
        {
            buzzerStep = &_activeSequence.currentSequence->buzzerSequence.stepList[_activeSequence.buzzerIndex];
            _activeSequence.endOfBuzzerStep = _activeSequence.elapsedMs + buzzerStep->durationMs;
            if (buzzerStep->enabled)
            {
                Buzzer_SetFrequency(buzzerStep->pitch);
                /* don't re-enable the buzzer if it is already enabled */
                if (buzzerEnabled == false)
                {
                    buzzerEnabled = true;
                    Buzzer_Enable();
                }
            }
            else
            {
                buzzerEnabled = false;
                Buzzer_Disable();
            }

            ++_activeSequence.buzzerIndex;
            buzzerStillRunning = true;
        }
        else
        {   /* we are on the last step and its timer has expired */
            buzzerStillRunning = false;
            buzzerEnabled = false;
            Buzzer_Disable();
        }
    }
    else
    {
        buzzerStillRunning = true;
    }

    /* If the backlight and the buzzer are done, then the sequence is complete */
    if ((backlightStillRunning == false) &&
        (buzzerStillRunning == false))
    {
        if (_playMode == PlayMode_SINGLE)
        {
            if (_callbackRunning)
            {
                TimerTick_CancelTimeoutFromCallback();
            }
            else
            {
                TimerTick_CancelTimeout(_uiTimerHandle);
            }
            _uiTimerActive = false;
            _activeSequence.currentSequence = NULL;

            /* Enqueue and event to notify the system that the sequence is complete in case anything
               was waiting for the sequence to complete.          */
            EventQueue_Add(EventApplication_SEQUENCE_COMPLETE, EventPriority_LOWEST);
        }
        else
        {
            /* Reset the sequence so it will play again */
            _ZeroCurrentSequence(&_activeSequence);
        }
    }
    else
    {
        _activeSequence.elapsedMs += SEQUENCE_RESOLUTION_MS;
    }
}

/***************************************************************************//**
    Zeros the elements of the current sequence.

    \param[in/out] sequence - the CurrentSequence_t to be zeroed.
*//****************************************************************************/
static void _ZeroCurrentSequence(CurrentSequence_t *sequence)
{
    sequence->backlightIndex = 0u;
    sequence->endOfBacklightStep = 0u;
    sequence->buzzerIndex = 0u;
    sequence->endOfBuzzerStep = 0u;
    sequence->elapsedMs = 0u;
}
