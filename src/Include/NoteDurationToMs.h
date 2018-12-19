/***************************************************************************//**
    \file NoteDurationToMs.h
    \brief Converts note durations to milliseconds.

    \b Description Converts note durations (e.g. quarter, eighth) to
                   milliseconds.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/NoteDurationToMs.h$
    $Revision: 3$
    $Date: Thursday, June 11, 2015 4:16:59 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __NOTEDURATIONTOMS_H__
#define __NOTEDURATIONTOMS_H__

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Defines the tempo, in beats per minute (BPM), that is used to calculate the
    number of milliseconds in a note duration */
#define NOTE_DUR_TO_MS_TEMPO    ( 190ul )

/** Define a constant scaling factor so the integer math doesn't truncate too many
    significant digits */
#define NOTE_DUR_SCALING_FACTOR ( 10000ul )

#define QUARTER_NOTE_MS     ((1000ul * NOTE_DUR_SCALING_FACTOR) / ((NOTE_DUR_TO_MS_TEMPO * NOTE_DUR_SCALING_FACTOR)/60ul))

/** Defines an enumeration to convert note durations to milliseconds.  It assumes
    a tempo of 120 BPM.  */
typedef enum NoteDurToMs
{
    NoteDurToMs_WHOLE            = QUARTER_NOTE_MS * 4ul,
    NoteDurToMs_DOTTED_HALF      = QUARTER_NOTE_MS * 3ul,
    NoteDurToMs_HALF             = QUARTER_NOTE_MS * 2ul,
    NoteDurToMs_DOTTED_QUARTER   = (QUARTER_NOTE_MS + (QUARTER_NOTE_MS / 2ul)),
    NoteDurToMs_QUARTER          = QUARTER_NOTE_MS,
    NoteDurToMs_DOTTED_EIGHTH    = ((QUARTER_NOTE_MS / 2ul) + (QUARTER_NOTE_MS / 4ul)),
    NoteDurToMs_EIGHTH           = (QUARTER_NOTE_MS / 2ul),
    NoteDurToMs_TRIPLET          = (QUARTER_NOTE_MS / 3ul),
    NoteDurToMs_DOTTED_SIXTEENTH = ((QUARTER_NOTE_MS/ 4ul) + (QUARTER_NOTE_MS / 8ul)),
    NoteDurToMs_SIXTEENTH        = (QUARTER_NOTE_MS / 4ul),
    NoteDurToMs_DOTTED_32        = ((QUARTER_NOTE_MS / 8ul) + (QUARTER_NOTE_MS / 16ul)),
    NoteDurToMs_THIRTYSECOND     = (QUARTER_NOTE_MS / 8ul),
    NoteDurToMs_SAME_NOTE_GAP    = 60ul, /** The number of milliseconds to put between two of same note so that
                                             the individual notes are still perceived */
} NoteDurToMs_t;

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __NOTEDURATIONTOMS_H__ */

