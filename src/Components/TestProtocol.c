/***************************************************************************//**
    \file TestProtocol.c
    \brief Manages the self-test protocol.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The test protocol module manages the tests that are performed
                   using a PC via the serial interface.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/TestProtocol.c$
    $Revision: 15$
    $Date: Thursday, June 11, 2015 4:16:58 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "TestProtocol.h"
#include <string.h>
#include <Assert.h>
#include <Debug.h>
#include <Serial.h>
#include <Conversion.h>
#include <Gpio.h>
#include <Pwm.h>
#include <Barometer.h>
#include <Accelerometer.h>
#include <Magnetometer.h>
#include <Gyroscope.h>
#include <Encoder.h>
#include <DAC.h>
#include <AdcManager.h>
#include <Ultrasound.h>
#include <Version.h>
#include <CommProtocol.h>
#include <stdio.h>
#include <Interrupt.h>
#include <Device.h>
#include <Board.h>
#include <Watchdog.h>
#include <Clock.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Possible NAK error codes.  The values are fixed to match the protocol
    document.
*/
typedef enum NakErrorCode
{
    NakErrorCode_NO_COMMAND          = 1,
    NakErrorCode_INVALID_COMMAND     = 2,
    NakErrorCode_INVALID_DATA_LENGTH = 3,
    NakErrorCode_INVALID_DATA        = 4,
    NakErrorCode_CANNOT_PERFORM      = 5,
    NakErrorCode_ERROR               = 6
} NakErrorCode_t;

/** Structure used to store a human command */
typedef struct HumanCommand
{
    /** String used indicate the type of command */
    const char_t* commandString;
    /** String printed by the help command that contains the command
        and its possible parameters.  Format:  " cmd param1 param2"
    */
    const char_t* helpString;
    /** Pointer to the function that handles the command */
    void (*handlerFunction)(void);
} HumanCommand_t;

/** Defines the characters used to separate parameters in human-readable
    commands.
*/
#define HUMAN_PARAM_SEPARATORS      (" _\r\n")

/** Number of defined human-readable commands */
#define NUM_HUMAN_COMMANDS          (17u)

/** Maximum length of a human command string */
#define HUMAN_COMMAND_MAX_LENGTH    (4u)

/** Maximum length of a command help string */
#define HELP_STRING_MAX_LENGTH      (25u)


/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Stores the most recently sent packet */
static SerialBuffer_t _packetTx;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _HandleHumanPackets(SerialBuffer_t *packetReceived);
static void _HandleHumanGpoCommand(void);
static void _HandleHumanGpiCommand(void);
static void _HandleHumanAdc1Command(void);
static void _HandleHumanAdcCommand(void);
static void _HandleHumanPwmCommand(void);
static void _HandleHumanAccCommand(void);
static void _HandleHumanMagCommand(void);
static void _HandleHumanBarcCommand(void);
static void _HandleHumanBarCommand(void);
static void _HandleHumanGyroCommand(void);
static void _HandleHumanEncoderCommand(void);
static void _HandleHumanDacCommand(void);
static void _HandleHumanUsGoCommand(void);
static void _HandleHumanStopCommand(void);
static void _HandleHumanVerCommand(void);
static void _HandleHumanRfCommand(void);
static void _HandleHumanBslCommand(void);

static void _SendAckHuman(void);
static void _SendNakHuman(NakErrorCode_t errorCode);

/*-----------------------LOCAL CONSTANTS-------------------------------------*/
/** Look-up table for human-readable commands */
/* Keep static handling functions in same order as listed below for aesthetic reasons.
   Also keep this definition below teh function prototypes so that the function
   pointers are valid.
*/
static const HumanCommand_t _humanCommands[NUM_HUMAN_COMMANDS] =
{
    {"gpo",  " gpo [port] [pin] [1|0]", &_HandleHumanGpoCommand},
    {"gpi",  " gpi [port] [pin]", &_HandleHumanGpiCommand},
    {"adc1", " adc1 [channel]", &_HandleHumanAdc1Command},
    {"adc",  " adc [channel]", &_HandleHumanAdcCommand},
    {"pwm",  " pwm [chan] [%] [f]", &_HandleHumanPwmCommand},
    {"acc",  " acc", &_HandleHumanAccCommand},
    {"mag",  " mag", &_HandleHumanMagCommand},
    {"barc", " barc", &_HandleHumanBarcCommand},
    {"bar",  " bar", &_HandleHumanBarCommand},
    {"gyro", " gyro", & _HandleHumanGyroCommand},
    {"enc",  " enc", &_HandleHumanEncoderCommand},
    {"dac",  " dac [d]", & _HandleHumanDacCommand},
    {"ugo",  " ugo", & _HandleHumanUsGoCommand},
    {"stop", " stop", &_HandleHumanStopCommand},
    {"ver",  " ver", &_HandleHumanVerCommand},
    {"rf",   " rf", &_HandleHumanRfCommand},
    {"bsl",  "bsl", &_HandleHumanBslCommand}
};

/***************************************************************************//**
    Initializes all the variables and settings for the test protocol module.
*//****************************************************************************/
void TestProtocol_Init(void)
{
	_packetTx.inUse = false;
}

/***************************************************************************//**
    Directs any received packets to the correct handler.
*//****************************************************************************/
void TestProtocol_HandleReceivedPacket(void)
{
    SerialBuffer_t* packetReceived = NULL;
    SerialStatus_t status;
    bool packetIsBinary;

    status = Serial_GetPacket(&packetReceived);
    if (status == SerialStatus_SUCCESS)
    {
        packetIsBinary = packetReceived->isBinary;
        if (packetIsBinary == false)
        {
            _HandleHumanPackets(packetReceived);
        }
    }
}

/***************************************************************************//**
    Handles any of the human-readable packets received.

    \param[in] packetReceived - Pointer to the received packet.
*//****************************************************************************/
static void _HandleHumanPackets(SerialBuffer_t *packetReceived)
{
	char_t*  command;
	uint16_t commandIndex;
	int16_t  compareResult;

	ASSERT(packetReceived != NULL, ErrorLedMode_SOFTWARE);

	command = strtok((char_t*) packetReceived->buffer, HUMAN_PARAM_SEPARATORS);

	if (command == NULL)
	{
		_SendNakHuman(NakErrorCode_NO_COMMAND);
	}
	else
	{
		/* Determine which command it was */
		for (commandIndex = 0u; commandIndex < NUM_HUMAN_COMMANDS; ++commandIndex)
		{
			compareResult = strncmp(command, _humanCommands[commandIndex].commandString,
									HUMAN_COMMAND_MAX_LENGTH);
			if (compareResult == 0)
			{
				break;
			}
		}

		/* if the for-loop didn't break, then the command wasn't found */
		if (commandIndex < NUM_HUMAN_COMMANDS)
		{
			/* call handler for matching command. It is up to the handler to
			   send the Ack.
			*/
			DEBUG(command);
			(*(_humanCommands[commandIndex].handlerFunction))();
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_COMMAND);
		}
	}

	/* Packet has been evaluated and can be released */
	packetReceived->inUse = false;
}
/***************************************************************************//**
    Sets a selected port and pin to a digital output and sets the logic level
    of the output.
*//****************************************************************************/
static void _HandleHumanGpoCommand(void)
{
	/** Create an integer to represent port J */
#define PORT_J_UINT  (99u)
	/** Create an integer to represent port U */
#define PORT_U_UINT  (98u)

	char_t* paramString;
	uint16_t port;
	uint16_t pin;
	uint16_t pinValueInt;
	GpioValue_t pinValue;
	ConversionStatus_t status;

	/* get the first parameter */
	paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

	/* check that the parameter exists */
	if (paramString != NULL)
	{
		status = Conversion_HexStringToUint(paramString, NumberSize_8_BITS, &port);

		/* handle the non-hex ports U and J */
		if (status != ConversionStatus_SUCCESS)
		{
			if ((*paramString == 'J') || (*paramString == 'j'))
			{
				status = ConversionStatus_SUCCESS;
				port = PORT_J_UINT;
			}
			else if ((*paramString == 'U') || (*paramString == 'u'))
			{
				status = ConversionStatus_SUCCESS;
				port = PORT_U_UINT;
			}
		}
		/* if the port conversion was successful, work on the pin */
		if (status == ConversionStatus_SUCCESS)
		{
			paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

			/* check that the parameter exists */
			if (paramString != NULL)
			{
				status = Conversion_HexStringToUint(paramString, NumberSize_4_BITS, &pin);

				/* if the conversion was successful, get the pin value */
				if (status == ConversionStatus_SUCCESS)
				{
					paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

					/* check that the parameter exists */
					if (paramString != NULL)
					{
						status = Conversion_HexStringToUint(paramString, NumberSize_4_BITS, &pinValueInt);

						/* if the conversion was successful, set the output */
						if (status == ConversionStatus_SUCCESS)
						{
							if ((bool)(pinValueInt))
							{
								pinValue = GpioValue_HIGH;
							}
							else
							{
								pinValue = GpioValue_LOW;
							}

							if (port == PORT_J_UINT)
							{
								Gpio_ConfigurePortJ((1<<pin), GpioType_OUTPUT, pinValue,
													GpioDriveStrength_DEFAULT);
							}
							else if (port == PORT_U_UINT)
							{
								Gpio_ConfigurePortU((1<<pin), GpioType_OUTPUT, pinValue);
							}
							else
							{
								Gpio_Configure(port, (1<<pin), GpioType_OUTPUT, pinValue,
											   GpioDriveStrength_DEFAULT);
							}
							_SendAckHuman();
						}
						else
						{
							_SendNakHuman(NakErrorCode_INVALID_DATA);
						}
					}
					else
					{
						_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
					}
				}
				else
				{
					_SendNakHuman(NakErrorCode_INVALID_DATA);
				}
			}
			else
			{
				_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
			}
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_DATA);
		}
	}
	else
	{
		_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
	}
}

/***************************************************************************//**
    Sets a selected port and pin to a digital input and reads the logic level
    of that pin.
*//****************************************************************************/
static void _HandleHumanGpiCommand(void)
{
	/** Create an integer to represent port J */
#define PORT_J_UINT  (99u)
	/** Create an integer to represent port U */
#define PORT_U_UINT  (98u)

	char_t* paramString;
	uint16_t port;
	uint16_t pin;
	bool pinValue = true;
	ConversionStatus_t status;
	char_t* valueString = (char_t*) &(_packetTx.buffer[0]);
	SerialStatus_t serialStatus;


	/* get the first parameter */
	paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

	/* check that the parameter exists */
	if (paramString != NULL)
	{
		status = Conversion_HexStringToUint(paramString, NumberSize_8_BITS, &port);

		/* handle the non-hex ports U and J */
		if (status != ConversionStatus_SUCCESS)
		{
			if ((*paramString == 'J') || (*paramString == 'j'))
			{
				status = ConversionStatus_SUCCESS;
				port = PORT_J_UINT;
			}
			else if ((*paramString == 'U') || (*paramString == 'u'))
			{
				status = ConversionStatus_SUCCESS;
				port = PORT_U_UINT;
			}
		}
		/* if the port conversion was successful, work on the pin */
		if (status == ConversionStatus_SUCCESS)
		{
			paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

			/* check that the parameter exists */
			if (paramString != NULL)
			{
				status = Conversion_HexStringToUint(paramString, NumberSize_4_BITS, &pin);

				/* if the conversion was successful, get the pin value */
				if (status == ConversionStatus_SUCCESS)
				{
					/* make sure that the TX packet is available */
					ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

					_packetTx.inUse = true;
					_packetTx.isBinary = false;

					if (port == PORT_J_UINT)
					{
						Gpio_ConfigurePortJ((1<<pin), GpioType_INPUT, GpioValue_DONT_CARE,
											GpioDriveStrength_DEFAULT);
						pinValue = Gpio_GetValuePortJ((1<<pin));
					}
					else if (port == PORT_U_UINT)
					{
						Gpio_ConfigurePortU((1<<pin), GpioType_INPUT, GpioValue_DONT_CARE);
						pinValue = Gpio_GetValuePortU((1<<pin));
					}
					else
					{
						Gpio_Configure(port, (1<<pin), GpioType_INPUT, GpioValue_DONT_CARE,
									   GpioDriveStrength_DEFAULT);
						pinValue = Gpio_GetValue(port, (1<<pin));
					}

					if (pinValue)
					{
						(void) strncpy(valueString, "Value:1",
								       SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
					}
					else
					{
						(void) strncpy(valueString, "Value:0",
									   SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
					}

				    _packetTx.length = strlen(valueString);

				    serialStatus = Serial_SendPacket(&_packetTx);
				    _packetTx.inUse = false;
				    ASSERT(serialStatus == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

				    _SendAckHuman();
				}
				else
				{
					_SendNakHuman(NakErrorCode_INVALID_DATA);
				}
			}
			else
			{
				_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
			}
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_DATA);
		}
	}
	else
	{
		_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
	}
}
/***************************************************************************//**
    Sets a selected analog input and takes a single sample of that ADC channel.
*//****************************************************************************/
static void _HandleHumanAdc1Command(void)
{
	char_t* paramString = NULL;
	uint16_t channel = 0u;
	ConversionStatus_t status;
	char_t* valueString = (char_t*) &(_packetTx.buffer[0]);
	SerialStatus_t serialStatus;
	uint16_t adcSample;

	/* get the channel parameter */
	paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

	/* check that the parameter exists */
	if (paramString != NULL)
	{
		status = Conversion_HexStringToUint(paramString, NumberSize_4_BITS, &channel);

		/* make sure the channel conversion was successful */
		if (status == ConversionStatus_SUCCESS)
		{
			/* make sure that the TX packet is available */
			ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

			_packetTx.inUse = true;
			_packetTx.isBinary = false;

			adcSample = AdcManager_SingleSampleBlocking((AdcManChannel_t) channel);

			(void) snprintf(valueString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
							"A%d:0x%x", channel, adcSample);
		    _packetTx.length = strlen(valueString);

		    serialStatus = Serial_SendPacket(&_packetTx);
		    _packetTx.inUse = false;
		    ASSERT(serialStatus == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

		    _SendAckHuman();
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_DATA);
		}
	}
	else
	{
		_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
	}
}

/***************************************************************************//**
    Sets a selected analog input and takes repeating samples of that ADC channel
*//****************************************************************************/
static void _HandleHumanAdcCommand(void)
{
	DEBUG("adc not implemented");
	_SendNakHuman(NakErrorCode_CANNOT_PERFORM);
}

/***************************************************************************//**
    Sets a selected PWM output and enables it
*//****************************************************************************/
static void _HandleHumanPwmCommand(void)
{
	char_t* paramString = NULL;
	uint16_t channel    = 0u;
	uint16_t frequency  = 0u;
	uint16_t duty       = 0u;
	ConversionStatus_t status;

	/* get the channel parameter */
	paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

	/* check that the parameter exists */
	if (paramString != NULL)
	{
		status = Conversion_HexStringToUint(paramString, NumberSize_4_BITS, &channel);

		/* make sure the channel conversion was successful */
		if (status == ConversionStatus_SUCCESS)
		{
			paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

			/* check that the parameter exists */
			if (paramString != NULL)
			{
				status = Conversion_HexStringToUint(paramString, NumberSize_8_BITS, &duty);

				/* make sure the duty conversion was successful */
				if (status == ConversionStatus_SUCCESS)
				{
					paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

					/* check that the parameter exists */
					if (paramString != NULL)
					{
						status = Conversion_HexStringToUint(paramString, NumberSize_16_BITS, &frequency);

						/* make sure the frequency conversion was successful */
						if (status == ConversionStatus_SUCCESS)
						{

						    /* need to pass in a 1 implied decimal fixed point */
							Pwm_SetFrequency((uint32_t)(frequency*10u));
							Pwm_Enable((PwmChannel_t)channel, duty);
							_SendAckHuman();

						}
						else
						{
							_SendNakHuman(NakErrorCode_INVALID_DATA);
						}
					}
					else
					{
						_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
					}
				}
				else
				{
					_SendNakHuman(NakErrorCode_INVALID_DATA);
				}
			}
			else
			{
				_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
			}
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_DATA);
		}
	}
	else
	{
		_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
	}
}

/***************************************************************************//**
    Enables the accelerometer and starts taking samples of the accelerometer.
*//****************************************************************************/
static void _HandleHumanAccCommand(void)
{
	DEBUG("acc part implemented");

	AccSample_t accSample;
	char_t* valueString = (char_t*) &(_packetTx.buffer[0]);
	SerialStatus_t serialStatus;

	/* make sure that the TX packet is available */
	ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

	_packetTx.inUse = true;
	_packetTx.isBinary = false;

	Accelerometer_TriggerSampleBlocking(&accSample);

	(void) snprintf(valueString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"x:0x%x,y:0x%x,z:0x%x", accSample.xData, accSample.yData, accSample.zData);

    _packetTx.length = strlen(valueString);

	serialStatus = Serial_SendPacket(&_packetTx);
	_packetTx.inUse = false;
	ASSERT(serialStatus == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

	_SendAckHuman();
}

/***************************************************************************//**
    Enables the magnetometer and starts taking samples of the magnetometer.
*//****************************************************************************/
static void _HandleHumanMagCommand(void)
{
	DEBUG("mag part implemented");

	MagSample_t magSample;
	char_t* valueString = (char_t*) &(_packetTx.buffer[0]);
	SerialStatus_t serialStatus;

	/* make sure that the TX packet is available */
	ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

	_packetTx.inUse = true;
	_packetTx.isBinary = false;

	Magnetometer_TriggerSampleBlocking(&magSample);

	(void) snprintf(valueString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"x:0x%x,y:0x%x,z:0x%x", magSample.xData, magSample.yData, magSample.zData);

    _packetTx.length = strlen(valueString);

	serialStatus = Serial_SendPacket(&_packetTx);
	_packetTx.inUse = false;
	ASSERT(serialStatus == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

	_SendAckHuman();

}

/***************************************************************************//**
    Enables the barometer and reads the barometer coefficients.
*//****************************************************************************/
static void _HandleHumanBarcCommand(void)
{
	BarometerCoef_t coef;
    char_t* coefString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);
    _packetTx.inUse = true;
    _packetTx.isBinary = false;

	Barometer_ReadCoefficients(&coef);

	(void) snprintf(coefString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"a0:0x%x,b1:0x%x,b2:0x%x,c12:0x%x", coef.a0, coef.b1, coef.b2, coef.c12);

    _packetTx.length = strlen(coefString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Enables the barometer and starts taking samples of the barometer.
*//****************************************************************************/
static void _HandleHumanBarCommand(void)
{
	BarometerSample_t sample;
    char_t* sampleString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);
    _packetTx.inUse = true;
    _packetTx.isBinary = false;

	Barometer_TriggerSampleBlocking(&sample);

	(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"pres:0x%x,temp:0x%x", sample.pressure, sample.temperature);

    _packetTx.length = strlen(sampleString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Enables the gyroscope and starts taking samples of the gyroscope.
*//****************************************************************************/
static void _HandleHumanGyroCommand(void)
{
	GyroSample_t sample;
    char_t* sampleString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);
    _packetTx.inUse = true;
    _packetTx.isBinary = false;

	Gyroscope_ReadSample(&sample);

	(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"xAxis:0x%x,yAxis:0x%x,zAxis:0x%x", sample.xData, sample.yData, sample.zData);

    _packetTx.length = strlen(sampleString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Enables the gyroscope and starts taking samples of the gyroscope.
*//****************************************************************************/
static void _HandleHumanEncoderCommand(void)
{
/** Defines the number of items to read from the encoder */
#define NUM_ENC_ITEMS   ( 2u )

	static int16_t encoderData[NUM_ENC_ITEMS];
	EncoderStatus_t encoderReadStatus;
	uint16_t encoderNumRead;
    char_t* sampleString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;

	Encoder_Start();
	encoderReadStatus = Encoder_Read(encoderData, &encoderNumRead, NUM_ENC_ITEMS);

	if (encoderReadStatus == EncoderStatus_OKAY)
	{
		if (encoderNumRead == 1)
		{
			(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
							"enc:%x", encoderData[0u]);
		}
		else if (encoderNumRead == 2)
		{
			(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
							"enc:%x,%x", encoderData[0u], encoderData[1u]);
		}
		else
		{
			(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
							"enc:%x,%x overflow", encoderData[0u], encoderData[1u]);
		}
	}
	else
	{
		switch (encoderReadStatus)
		{
			case EncoderStatus_BUFFER_OVERFLOW:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder buffer overflow");
				break;
			case EncoderStatus_CAPTURE_OVERFLOW:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder capture overflow");
				break;
			case EncoderStatus_SW_CAPTURE_OVERFLOW:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder software capture overflow");
				break;
			case EncoderStatus_INVALID:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder invalid");
				break;
			case EncoderStatus_OTHER_ERROR:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder other");
				break;
			default:
				(void) snprintf(sampleString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
								"encoder unknown");
				break;
		}
	}

    _packetTx.length = strlen(sampleString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Enables the DAC and sets voltage.
*//****************************************************************************/
static void _HandleHumanDacCommand(void)
{
/* 	uint8_t voltage = 255u;


    DacSetVoltage(voltage);


    _SendAckHuman(); */


	/** Defines the number of ADC channel samples to use */
	#define NUMBER_ADC_USED 1u

	char_t* paramString = NULL;
	uint16_t d = 0u;
	ConversionStatus_t status;

	/* get the channel parameter */
	paramString = strtok(NULL, HUMAN_PARAM_SEPARATORS);

	/* check that the parameter exists */
	if (paramString != NULL)
	{
		status = Conversion_HexStringToUint(paramString, NumberSize_8_BITS, &d);

		/* make sure the channel conversion was successful */
		if (status == ConversionStatus_SUCCESS)
		{
			DacSetVoltage(d);

		    _SendAckHuman();
		}
		else
		{
			_SendNakHuman(NakErrorCode_INVALID_DATA);
		}
	}
	else
	{
		_SendNakHuman(NakErrorCode_INVALID_DATA_LENGTH);
	}


}
/***************************************************************************//**

*//****************************************************************************/
static void _HandleHumanUsGoCommand(void)
{
	UltrasoundStartTransmit();
    _SendAckHuman();
}

/***************************************************************************//**
    Stops the currently running command.
*//****************************************************************************/
static void _HandleHumanStopCommand(void)
{
	DEBUG("stop not implemented");
	_SendNakHuman(NakErrorCode_CANNOT_PERFORM);
}

/***************************************************************************//**
    Prints the current version numbers of the system
*//****************************************************************************/
static void _HandleHumanVerCommand(void)
{
    char_t* versionString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);
    _packetTx.inUse = true;
    _packetTx.isBinary = false;

	(void) snprintf(versionString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"fw:%s,hw:%d,uh:%d,uf:%d,did:%x,wid:%lx",
					UserFirmwareVersionString(),
					USER_HARDWARE_VERSION,
					HardwareRevisionMicro(),
					FirmwareRevisionMicro(),
					DeviceId(),
					WaferId()
				   );

    _packetTx.length = strlen(versionString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Prints the current RF information
*//****************************************************************************/
static void _HandleHumanRfCommand(void)
{
    char_t* rfVersionString = (char_t*) &(_packetTx.buffer[0]);
    SerialStatus_t status;
    RfStatus_t rfStatus;
    static char_t versionString[6u];
    static char_t *connectionStatus;

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);
    _packetTx.inUse = true;
    _packetTx.isBinary = false;

    CommProtocol_GetRfStatus(&rfStatus);

    /* set the version number string */
	versionString[0] = (char_t)((uint8_t)('0') + (uint8_t)(rfStatus.fwMajor >> 4u));
	versionString[1] = (char_t)((uint8_t)('0') + (uint8_t)(rfStatus.fwMajor & 0x0Fu));
	versionString[2] = '.';
	versionString[3] = (char_t)((uint8_t)('0') + (uint8_t)(rfStatus.fwMinor >> 4u));
	versionString[4] = (char_t)((uint8_t)('0') + (uint8_t)(rfStatus.fwMinor & 0x0Fu));
	versionString[5] = '\0';

	/* set the RF connection status string */
	switch(rfStatus.connectionStatus)
	{
		case RfConnectionStatus_CONNECTED:
			connectionStatus = "connected";
			break;

		case RfConnectionStatus_NOT_PAIRED:
			connectionStatus = "not paired";
			break;

		case RfConnectionStatus_PAIRED_NOT_CONNECTED:
			connectionStatus = "paired not connected";
			break;

		case RfConnectionStatus_PAIRING:
			connectionStatus = "pairing";
			break;

		default:
			connectionStatus = "unhandled status";
			break;
	}

	(void) snprintf(rfVersionString, SERIAL_PAYLOAD_MAX_LENGTH_HUMAN,
					"RF Version:%s, RF State:%s",
					versionString,
					connectionStatus);

    _packetTx.length = strlen(rfVersionString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);

    _SendAckHuman();
}

/***************************************************************************//**
    Puts the device in the serial bootloader mode
*//****************************************************************************/
static void _HandleHumanBslCommand(void)
{
//    _SendAckHuman();

	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	SYSBSLC &= ~(SYSBSLPE+SYSBSLOFF); //disable memory protection

    INTERRUPT_DISABLE();
//    _NOP();
    Gpio_Configure(DEBUG_UART_TX_PORT, DEBUG_UART_TX_PIN, GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(DEBUG_UART_RX_PORT, DEBUG_UART_RX_PIN, GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(BSL_TX_PORT, BSL_TX_PIN, GpioType_PERIPHERAL_OUTPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_Configure(BSL_RX_PORT, BSL_RX_PIN, GpioType_PERIPHERAL_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);

//    CLOCK_DELAY_US(100);

    /* jump to bsl */
    ((void (*)())0x1000)();
}

/***************************************************************************//**
    Attempts to send a human-readable acknowledgement packet.  An assert will
    occur if the send fails.
*//****************************************************************************/
static void _SendAckHuman(void)
{
	SerialStatus_t status = SerialStatus_SUCCESS;
    char_t* ackString = (char_t*) &(_packetTx.buffer[0]);

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

    _packetTx.inUse = true;
	_packetTx.isBinary = false;
	(void) strncpy(ackString, "Ack", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
	_packetTx.length = strlen(ackString);

	status = Serial_SendPacket(&_packetTx);
	_packetTx.inUse = false;
	ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Attempts to send a human-readable negative-acknowledgement packet with
    the given error code.  An assert will occur if the send fails.

    \param[in] errorCode - Error code to include in the packet.
*//****************************************************************************/
static void _SendNakHuman(NakErrorCode_t errorCode)
{
    SerialStatus_t status = SerialStatus_SUCCESS;
    char_t* nakString = (char_t*) &(_packetTx.buffer[0]);

    ASSERT(_packetTx.inUse == false, ErrorLedMode_SOFTWARE);

    _packetTx.inUse = true;
    _packetTx.isBinary = false;

    /* Create NAK string/packet based on error code. */
    switch (errorCode)
    {
        case NakErrorCode_NO_COMMAND:
            (void) strncpy(nakString, "Nak: no command", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        case NakErrorCode_INVALID_COMMAND:
            (void) strncpy(nakString, "Nak: invalid command", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        case NakErrorCode_INVALID_DATA_LENGTH:
            (void) strncpy(nakString, "Nak: invalid data length", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        case NakErrorCode_INVALID_DATA:
            (void) strncpy(nakString, "Nak: invalid data", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        case NakErrorCode_CANNOT_PERFORM:
            (void) strncpy(nakString, "Nak: cannot perform", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        case NakErrorCode_ERROR:
            (void) strncpy(nakString, "Nak: error performing command", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;

        default:
            (void) strncpy(nakString, "Nak", SERIAL_PAYLOAD_MAX_LENGTH_HUMAN);
            break;
    }

    _packetTx.length = strlen(nakString);

    status = Serial_SendPacket(&_packetTx);
    _packetTx.inUse = false;
    ASSERT(status == SerialStatus_SUCCESS, ErrorLedMode_SOFTWARE);
}
