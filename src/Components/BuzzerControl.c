/***************************************************************************//**
    \file BuzzerControl.c
    \brief Controls the buzzer

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Controls and manages the settings for the buzzer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/BuzzerControl.c$
    $Revision: 6$
    $Date: Friday, October 24, 2014 10:53:10 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "BuzzerControl.h"
#include <Pwm.h>
#include <KeyValue.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/*-----------------------LOCAL CONSTANTS--------------------------------------*/
static uint32_t const _lowFrequencies[BuzzerLowFrequency_LAST] =
{
    500,
    600,
    700,
    800,
    900,
    1000,
    1200,
    1500,
    2000,
    2400,
    2500,
    3000,
    3500,
    4000,
    4500,
    4800,
    5000,
    6000,
    7000,
    8000,
    9000,
    9600,
    10000,
    11000,
    12000,
    13000,
    14000,
    15000,
    16000,
    17000,
    18000,
    19000
};

static uint32_t const _highFrequencies[BuzzerHighFrequency_LAST] =
{
    20000,
    21000,
    22000,
    23000,
    23980,
    24000,
    24020,
    25000,
    27500,
    30000,
    32500,
    35000,
    37500,
    40000,
    42500,
    45000,
    47500,
    47980,
    48000,
    48020,
    49000,
    50000,
    52500,
    55000,
    57500,
    60000,
    62500,
    65000,
    67500,
    70000,
    72500,
    75000
};

static uint32_t const _lowPitches[BuzzerLowPitch_LAST] =
{
    346,     /* C#1/Db1  */
    367,     /* D1       */
    389,     /* D#1/Eb1  */
    412,     /* E1       */
    437,     /* F1       */
    462,     /* F#1/Gb1  */
    490,     /* G1       */
    519,     /* G#1/Ab1  */
    550,     /* A1       */
    583,     /* A#1/Bb1  */
    617,     /* B1       */
    654,     /* C2       */
    693,     /* C#2/Db2  */
    734,     /* D2       */
    778,     /* D#2/Eb2  */
    824,     /* E2       */
    873,     /* F2       */
    925,     /* F#2/Gb2  */
    980,     /* G2       */
    1038,    /* G#2/Ab2  */
    1100,    /* A2       */
    1167,    /* A#2/Bb2  */
    1235,    /* B2       */
    1308,    /* C3       */
    1386,    /* C#3/Db3  */
    1468,    /* D3       */
    1556,    /* D#3/Eb3  */
    1648,    /* E3       */
    1746,    /* F3       */
    1850,    /* F#3/Gb3  */
    1960,    /* G3       */
    2077     /* G#3/Ab3  */
};

static uint32_t const _medPitches[BuzzerMidPitch_LAST] =
{
	2200,    /* A3 		*/
	2331,    /* A#3/Bb3	*/
	2469,    /* B3       */
	2616,    /* C4       */
	2772,    /* C#4/Db4  */
	2937,    /* D4       */
	3111,    /* D#4/Eb4  */
	3396,    /* E4       */
	3492,    /* F4       */
	3700,    /* F#4/Gb4  */
	3920,    /* G4       */
	4153,    /* G#4/Ab4  */
	4400,    /* A4       */
	4662,    /* A#4/Bb4  */
	4939,    /* B4       */
	5233,    /* C5       */
	5544,    /* C#5/Db5  */
	5873,    /* D5       */
	6223,    /* D#5/Eb5  */
	6593,    /* E5       */
	6985,    /* F5       */
	7400,    /* F#5/Gb5  */
	7840,    /* G5       */
	8306,    /* G#5/Ab5  */
	8800,    /* A5       */
	9323,    /* A#5/Bb5  */
	9878,    /* B5       */
	10465,   /* C6       */
	11087,   /* C#6/Db6  */
	11747,   /* D6       */
	12445,   /* D#6/Eb6  */
	13185,   /* E6       */
};

static uint32_t const _highPitches[BuzzerHighPitch_LAST] =
{
    13969,   /* F6       */
    14800,   /* F#6/Gb6  */
    15680,   /* G6       */
    16612,   /* G#6/Ab6  */
    17600,   /* A6       */
    18647,   /* A#6/Bb6  */
    19755,   /* B6       */
    20930,   /* C7       */
    22175,   /* C#7/Db7  */
    23493,   /* D7       */
    24890,   /* D#7/Eb7  */
    26370,   /* E7       */
    27938,   /* F7       */
    29600,   /* F#7/Gb7  */
    31360,   /* G7       */
	33224,   /* G#7/Ab7  */
	35200,   /* A7       */
	37293,   /* A#7/Bb7  */
	39511,   /* B7       */
	41860,   /* C8       */
	44349,   /* C#8/Db8  */
	46986,   /* D8       */
	49780,   /* D#8/Eb8  */
	52740,   /* E8       */
	55877,   /* F8       */
	59199,   /* F#8/Gb8  */
	62719,   /* G8       */
	66449,   /* G#8/Ab8  */
	70400,   /* A8       */
	74586,   /* A#8/Bb8  */
	79021,   /* B8       */
	83720    /* C9       */
};

static uint16_t const _dutyCycles[BuzzerDutyCycle_LAST] =
{
	0,
	3,
	6,
	9,
	12,
	15,
	18,
	21,
	25,
	28,
	31,
	34,
	37,
	40,
	43,
	46,
	50,
	55,
	59,
	62,
	65,
	68,
	71,
	75,
	78,
	81,
	84,
	88,
	91,
	94,
	97,
	100
};

/** Possible output types for the buzzer */
typedef enum BuzzerOutput
{
	BuzzerOutput_LOW_FREQUENCY,
	BuzzerOutput_HIGH_FREQUENCY,
	BuzzerOutput_LOWPITCH,
	BuzzerOutput_MEDPITCH,
	BuzzerOutput_HIGHPITCH
} BuzzerOutput_t;

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks what mode the buzzer is in */
static BuzzerMode_t _buzzerMode = BuzzerMode_INVALID;

/** Tracks the output type for the buzzer */
static BuzzerOutput_t _buzzerOutputType = BuzzerOutput_HIGH_FREQUENCY;

/** Tracks what low frequency the buzzer is set to */
static BuzzerLowFrequency_t _buzzerLowFrequency = DEFAULT_BUZZER_LOW_FREQUENCY;

/** Tracks what high frequency the buzzer is set to */
static BuzzerHighFrequency_t _buzzerHighFrequency = DEFAULT_BUZZER_HIGH_FREQUENCY;

/** Tracks what low pitch the buzzer was last set to */
static BuzzerLowPitch_t _buzzerLowPitch = DEFAULT_BUZZER_LOWPITCH;

/** Tracks what med pitch the buzzer was last set to */
static BuzzerMidPitch_t _buzzerMedPitch = DEFAULT_BUZZER_MIDPITCH;

/** Tracks what high pitch the buzzer was last set to */
static BuzzerHighPitch_t _buzzerHighPitch = DEFAULT_BUZZER_HIGHPITCH;

/** Tracks what duty cycle the buzzer is set to */
static BuzzerDutyCycle_t _buzzerDuty = DEFAULT_BUZZER_DUTYCYCLE;

/** Tracks what duty cycle the buzzer is set to */
static bool _buzzerEnabled = false;
/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _BuzzerControl_SetDutyCycle(void);
static void _BuzzerControl_SetOutput(void);

/***************************************************************************//**
    Initialized the Buzzer and PWM driver.
*//****************************************************************************/
void BuzzerControl_Init()
{
	Pwm_Init();
}

/***************************************************************************//**
    Sets a configuration value to the buzzer

    \param[in] config - the key/value pair with the configuration information

*//****************************************************************************/
void BuzzerControl_SetConfig(uint8_t config)
{
	Key_t key;
	Value_t value;

	KeyValue_Separate(config, &key, &value);

	switch(key)
	{
		case BuzzerKey_MODE:
			if ((((BuzzerMode_t)value) >= BuzzerMode_FIRST) && (((BuzzerMode_t)value) < BuzzerMode_LAST))
			{
				_buzzerMode = (BuzzerMode_t)(value);
				switch (_buzzerMode)
				{
					case BuzzerMode_DISABLE:
						Pwm_Disable(PwmChannel_1);
						_buzzerEnabled = false;
						break;

					case BuzzerMode_ENABLE:
						_BuzzerControl_SetOutput();
						break;

					default:
						DEBUG("buzzer unhandled mode");
						break;
				}
			}
			else
			{
				DEBUG("invalid buzzer mode");
			}
			break;

        case BuzzerKey_LOW_FREQUENCY:
            if ((((BuzzerLowFrequency_t)value) >= BuzzerLowFrequency_FIRST) && (((BuzzerLowFrequency_t)value) < BuzzerLowFrequency_LAST))
            {
                _buzzerLowFrequency = (BuzzerLowFrequency_t)(value);
                _buzzerOutputType = BuzzerOutput_LOW_FREQUENCY;
                _BuzzerControl_SetOutput();
            }
            else
            {
                DEBUG("invalid buzzer low frequency");
            }
            break;

		case BuzzerKey_HIGH_FREQUENCY:
			if ((((BuzzerHighFrequency_t)value) >= BuzzerHighFrequency_FIRST) && (((BuzzerHighFrequency_t)value) < BuzzerHighFrequency_LAST))
			{
				_buzzerHighFrequency = (BuzzerHighFrequency_t)(value);
				_buzzerOutputType = BuzzerOutput_HIGH_FREQUENCY;
				_BuzzerControl_SetOutput();
			}
			else
			{
				DEBUG("invalid buzzer high frequency");
			}
			break;

		case BuzzerKey_LOWPITCH:
			if ((((BuzzerLowPitch_t)value) >= BuzzerLowPitch_FIRST) && (((BuzzerLowPitch_t)value) < BuzzerLowPitch_LAST))
			{
				_buzzerLowPitch = (BuzzerLowPitch_t)(value);
				_buzzerOutputType = BuzzerOutput_LOWPITCH;
				_BuzzerControl_SetOutput();
			}
			else
			{
				DEBUG("invalid buzzer pitch low");
			}
			break;

        case BuzzerKey_MIDPITCH:
            if ((((BuzzerMidPitch_t)value) >= BuzzerMidPitch_FIRST) && (((BuzzerMidPitch_t)value) < BuzzerMidPitch_LAST))
            {
                _buzzerMedPitch = (BuzzerMidPitch_t)(value);
                _buzzerOutputType = BuzzerOutput_MEDPITCH;
                _BuzzerControl_SetOutput();
            }
            else
            {
                DEBUG("invalid buzzer pitch med");
            }
            break;

		case BuzzerKey_HIGHPITCH:
			if ((((BuzzerHighPitch_t)value) >= BuzzerHighPitch_FIRST) && (((BuzzerHighPitch_t)value) < BuzzerHighPitch_LAST))
			{
				_buzzerHighPitch = (BuzzerHighPitch_t)(value);
				_buzzerOutputType = BuzzerOutput_HIGHPITCH;
				_BuzzerControl_SetOutput();
			}
			else
			{
				DEBUG("invalid buzzer pitch high");
			}
			break;

		case BuzzerKey_DUTYCYCLE:
			if ((((BuzzerDutyCycle_t)value) >= BuzzerDutyCycle_FIRST) && (((BuzzerDutyCycle_t)value) < BuzzerDutyCycle_LAST))
			{
				_buzzerDuty = (BuzzerDutyCycle_t)(value);
				_BuzzerControl_SetDutyCycle();
			}
			else
			{
				DEBUG("invalid buzzer duty cycle");
			}
			break;

		default:
			DEBUG("invalid buzzer key");
			break;
	}
}

/***************************************************************************//**
    Sets the output of the DAC to the amplitude stored in _dacAmplitude.
*//****************************************************************************/
static void _BuzzerControl_SetDutyCycle(void)
{
	uint8_t duty = 0;

	duty = _dutyCycles[_buzzerDuty];
	Pwm_SetDuty(PwmChannel_1, duty);

}

/***************************************************************************//**
    Sets the output of the buzzer
*//****************************************************************************/
static void _BuzzerControl_SetOutput(void)
{
	uint32_t out = 0;
	uint8_t duty = 0;


	switch (_buzzerOutputType)
	{
        case BuzzerOutput_LOW_FREQUENCY:
            out = _lowFrequencies[_buzzerLowFrequency];
            break;

		case BuzzerOutput_HIGH_FREQUENCY:
			out = _highFrequencies[_buzzerHighFrequency];
			break;

		case BuzzerOutput_LOWPITCH:
			out = _lowPitches[_buzzerLowPitch];
			break;

        case BuzzerOutput_MEDPITCH:
            out = _medPitches[_buzzerMedPitch];
            break;

		case BuzzerOutput_HIGHPITCH:
			out = _highPitches[_buzzerHighPitch];
			break;

		default:
			break;
	}

	Pwm_SetFrequency(out);

	if((_buzzerEnabled == false) && (_buzzerMode == BuzzerMode_ENABLE))
	{
		duty = _dutyCycles[_buzzerDuty];
		Pwm_Enable(PwmChannel_1, duty);
		_buzzerEnabled = true;
	}
}
