/***************************************************************************//**
    \file PitchToFrequency.h
    \brief Converts named pitches to their frequencies

    \b Description Converts named pitches to their frequencies.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/PitchToFrequency.h$
    $Revision: 1$
    $Date: Tuesday, October 21, 2014 4:35:36 PM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#ifndef __PITCHTOFREQUENCY_H__
#define __PITCHTOFREQUENCY_H__

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

/** Enumeration to contain a list of named pitches and its corresponding
    frequency, in HertzX10 (integer number representing 0.1Hz, e.g. D1 is
    36.7Hz so it is 367.  */
typedef enum PitchFrequency
{
    PitchFrequency_Cs1_Db1 = 346,     /* C#1/Db1 */
    PitchFrequency_D1      = 367,     /* D1       */
    PitchFrequency_Ds1_Eb1 = 389,     /* D#1/Eb1  */
    PitchFrequency_E1      = 412,     /* E1       */
    PitchFrequency_F1      = 437,     /* F1       */
    PitchFrequency_Fs1_Gb1 = 462,     /* F#1/Gb1  */
    PitchFrequency_G1      = 490,     /* G1       */
    PitchFrequency_Gs1_Ab1 = 519,     /* G#1/Ab1  */
    PitchFrequency_A1      = 550,     /* A1       */
    PitchFrequency_As1_Bb1 = 583,     /* A#1/Bb1  */
    PitchFrequency_B1      = 617,     /* B1       */
    PitchFrequency_C2      = 654,     /* C2       */
    PitchFrequency_Cs2_Db2 = 693,     /* C#2/Db2  */
    PitchFrequency_D2      = 734,     /* D2       */
    PitchFrequency_Ds2_Eb2 = 778,     /* D#2/Eb2  */
    PitchFrequency_E2      = 824,     /* E2       */
    PitchFrequency_F2      = 873,     /* F2       */
    PitchFrequency_Fs2_Gb2 = 925,     /* F#2/Gb2  */
    PitchFrequency_G2      = 980,     /* G2       */
    PitchFrequency_Gs2_Ab2 = 1038,    /* G#2/Ab2  */
    PitchFrequency_A2      = 1100,    /* A2       */
    PitchFrequency_As2_Bb2 = 1167,    /* A#2/Bb2  */
    PitchFrequency_B2      = 1235,    /* B2       */
    PitchFrequency_C3      = 1308,    /* C3       */
    PitchFrequency_Cs3_Db3 = 1386,    /* C#3/Db3  */
    PitchFrequency_D3      = 1468,    /* D3       */
    PitchFrequency_Ds3_Eb3 = 1556,    /* D#3/Eb3  */
    PitchFrequency_E3      = 1648,    /* E3       */
    PitchFrequency_F3      = 1746,    /* F3       */
    PitchFrequency_Fs3_Gb3 = 1850,    /* F#3/Gb3  */
    PitchFrequency_G3      = 1960,    /* G3       */
    PitchFrequency_Gs3_Ab3 = 2077,     /* G#3/Ab3  */
    PitchFrequency_A3      = 2200,    /* A3      */
    PitchFrequency_As3_Bb3 = 2331,    /* A#3/Bb3 */
    PitchFrequency_B3      = 2469,    /* B3       */
    PitchFrequency_C4      = 2616,    /* C4       */
    PitchFrequency_Cs4_Db4 = 2772,    /* C#4/Db4  */
    PitchFrequency_D4      = 2937,    /* D4       */
    PitchFrequency_Ds4_Eb4 = 3111,    /* D#4/Eb4  */
    PitchFrequency_E4      = 3396,    /* E4       */
    PitchFrequency_F4      = 3492,    /* F4       */
    PitchFrequency_Fs4_Gb4 = 3700,    /* F#4/Gb4  */
    PitchFrequency_G4      = 3920,    /* G4       */
    PitchFrequency_Gs4_Ab4 = 4153,    /* G#4/Ab4  */
    PitchFrequency_A4      = 4400,    /* A4       */
    PitchFrequency_As4_Bb4 = 4662,    /* A#4/Bb4  */
    PitchFrequency_B4      = 4939,    /* B4       */
    PitchFrequency_C5      = 5233,    /* C5       */
    PitchFrequency_Cs5_Db5 = 5544,    /* C#5/Db5  */
    PitchFrequency_D5      = 5873,    /* D5       */
    PitchFrequency_Ds5_Eb5 = 6223,    /* D#5/Eb5  */
    PitchFrequency_E5      = 6593,    /* E5       */
    PitchFrequency_F5      = 6985,    /* F5       */
    PitchFrequency_Fs5_Gb5 = 7400,    /* F#5/Gb5  */
    PitchFrequency_G5      = 7840,    /* G5       */
    PitchFrequency_Gs5_Ab5 = 8306,    /* G#5/Ab5  */
    PitchFrequency_A5      = 8800,    /* A5       */
    PitchFrequency_As5_Bb5 = 9323,    /* A#5/Bb5  */
    PitchFrequency_B5      = 9878,    /* B5       */
    PitchFrequency_C6      = 10465,   /* C6       */
    PitchFrequency_Cs6_Db6 = 11087,   /* C#6/Db6  */
    PitchFrequency_D6      = 11747,   /* D6       */
    PitchFrequency_Ds6_Eb6 = 12445,   /* D#6/Eb6  */
    PitchFrequency_E6      = 13185,   /* E6       */
    PitchFrequency_F6      = 13969,   /* F6       */
    PitchFrequency_Fs6_Gb6 = 14800,   /* F#6/Gb6  */
    PitchFrequency_G6      = 15680,   /* G6       */
    PitchFrequency_Gs6_Ab6 = 16612,   /* G#6/Ab6  */
    PitchFrequency_A6      = 17600,   /* A6       */
    PitchFrequency_As6_Bb6 = 18647,   /* A#6/Bb6  */
    PitchFrequency_B6      = 19755,   /* B6       */
    PitchFrequency_C7      = 20930,   /* C7       */
    PitchFrequency_Cs7_Db7 = 22175,   /* C#7/Db7  */
    PitchFrequency_D7      = 23493,   /* D7       */
    PitchFrequency_Ds7_Eb7 = 24890,   /* D#7/Eb7  */
    PitchFrequency_E7      = 26370,   /* E7       */
    PitchFrequency_F7      = 27938,   /* F7       */
    PitchFrequency_Fs7_Gb7 = 29600,   /* F#7/Gb7  */
    PitchFrequency_G7      = 31360,   /* G7       */
    PitchFrequency_Gs7_Ab7 = 33224,   /* G#7/Ab7  */
    PitchFrequency_A7      = 35200,   /* A7       */
    PitchFrequency_As7_Bb7 = 37293,   /* A#7/Bb7  */
    PitchFrequency_B7      = 39511,   /* B7       */
    PitchFrequency_C8      = 41860,   /* C8       */
    PitchFrequency_Cs8_Db8 = 44349,   /* C#8/Db8  */
    PitchFrequency_D8      = 46986,   /* D8       */
    PitchFrequency_Ds8_Eb8 = 49780,   /* D#8/Eb8  */
    PitchFrequency_E8      = 52740,   /* E8       */
    PitchFrequency_F8      = 55877,   /* F8       */
    PitchFrequency_Fs8_Gb8 = 59199,   /* F#8/Gb8  */
    PitchFrequency_G8      = 62719,   /* G8       */
    PitchFrequency_Gs8_Ab8 = 66449,   /* G#8/Ab8  */
    PitchFrequency_A8      = 70400,   /* A8       */
    PitchFrequency_As8_Bb8 = 74586,   /* A#8/Bb8  */
    PitchFrequency_B8      = 79021,   /* B8       */
    PitchFrequency_C9      = 83720    /* C9       */
} PitchFrequency_t;

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/

#endif /* __PITCHTOFREQUENCY_H__ */

