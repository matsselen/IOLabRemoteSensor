/***************************************************************************//**
    \file CommProtocol.c
    \brief Communications protocol module.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The communications protocol module manages the data coming in
                   and going out from the other microcontroller.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Components/CommProtocol.c$
    $Revision: 27$
    $Date: Friday, July 17, 2015 2:21:59 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include <EventQueue.h>
#include "CommProtocol.h"
#include <SpiSlave.h>
#include <Assert.h>
#include <Serial.h>
#include <Debug.h>
#include <DataBuffer.h>
#include <LedControl.h>
#include <SensorId.h>
#include <BatteryControl.h>
#include <ConfigurationManager.h>
#include <System.h>
#include <TimerTick.h>
#include <Version.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the size of the packets used in the protocol. */
#define DATA_PACKET_SIZE        ( 114u )
/** Defines the size of the packets used for the non-data messages */
#define MESSAGE_PACKET_SIZE     ( 30u )

/** Defines the value for the start of message byte */
#define START_OF_MESSAGE        ( 0x02u )
/** Defines where in the packet the start message is located */
#define START_OF_MESSAGE_INDEX  ( 0u )

/** Defines the value for the end of message byte */
#define END_OF_PACKET          ( 0x0Au )
/** Defines where in the data packet the end message is located */
#define END_OF_DATA_PACKET_INDEX    ( DATA_PACKET_SIZE - 1u )
/** Defines where in the message packet the end message is located */
#define END_OF_MESSAGE_PACKET_INDEX ( MESSAGE_PACKET_SIZE - 1u)

/** Defines the possible command values */
#if 1 /* use updated numbering */
typedef enum CommCommand
{
	CommCommand_INVALID           = 0u,
	CommCommand_FIRST             = 0x10u,

	/* local packets types */
	CommCommand_RF_PACKET         = CommCommand_FIRST,
	CommCommand_STATUS            = 0x11u,

	/* dongle packet types */
	CommCommand_START_DATA        = 0x20u,
	CommCommand_STOP_DATA         = 0x21u,
	CommCommand_SET_SENSOR_CONFIG = 0x22u,
	CommCommand_GET_SENSOR_CONFIG = 0x23u,
	CommCommand_SET_OUTPUT_CONFIG = 0x24u,
	CommCommand_GET_OUTPUT_CONFIG = 0x25u,
	CommCommand_SET_FIXED_CONFIG  = 0x26u,
	CommCommand_GET_FIXED_CONFIG  = 0x27u,
	CommCommand_GET_PACKET_CONFIG = 0x28u,
	CommCommand_GET_CALIBRATION   = 0x29u,
	CommCommand_GET_STATUS        = 0x2Au,
	CommCommand_POWER_DOWN        = 0x2Bu,

	CommCommand_LAST
} CommCommand_t;
#else /* use legacy numbering */
typedef enum CommCommand
{
	CommCommand_INVALID           = 0u,
	CommCommand_FIRST             = 0x10u,
	CommCommand_RF_PACKET         = CommCommand_FIRST,
	CommCommand_STATUS            = 0x11u,
	CommCommand_SET_SENSOR_CONFIG = 0x12u,
	CommCommand_GET_SENSOR_CONFIG = 0x13u,
	CommCommand_SET_OUTPUT_CONFIG = 0x14u,
	CommCommand_GET_OUTPUT_CONFIG = 0x15u,
	CommCommand_GET_PACKET_CONFIG = 0x16u,
	CommCommand_GET_CALIBRATION   = 0x17u,
	CommCommand_GET_STATUS        = 0x18u,
	/* 0x19 is unused */
	CommCommand_START_DATA        = 0x1Au,
	CommCommand_STOP_DATA         = 0x1Bu,
	CommCommand_SET_FIXED_CONFIG  = 0x1Cu,
	CommCommand_GET_FIXED_CONFIG  = 0x1Du,
	CommCommand_POWER_DOWN        = CommCommand_INVALID, /* this is unsupported in legacy mode */
	CommCommand_LAST
} CommCommand_t;
#endif
/** Defines where in the packet the command value is located */
#define COMMAND_INDEX               ( 1u )
/** Defines where in the packet the start of the TX data is */
#define TX_PACKET_DATA_START_INDEX  ( COMMAND_INDEX + 1u )
/** Defines where in the packet the valid data length value is located */
#define VALID_DATA_LENGTH_INDEX     ( TX_PACKET_DATA_START_INDEX )
/** Defines where in the packet the start of the RX data is */
#define RX_PACKET_DATA_START_INDEX  ( VALID_DATA_LENGTH_INDEX + 1u )

/** Defines how many bytes of the packet is overhead */
#define BYTES_PACKET_OVERHEAD       ( 3u )

/** Defines where the remote ID index belongs in a transmitted message */
#define TX_REMOTE_ID_INDEX     ( VALID_DATA_LENGTH_INDEX + 1u )
/** Defines where the remote ID index in on the message data received by the message handlers */
#define RX_REMOTE_ID_INDEX     ( 0u )

/** Defines the maximum amount of time the system is allowed to be "not connected" */
#define MAX_NO_CONNECT_TIME_MSEC  ((uint32_t)( 1000u * 60u )) /* 60 seconds in msec */

/** Mats: Defines the maximum amount of time the system is allowed to be "idle" (connected but not recording) */
#define MAX_IDLE_TICK ((uint32_t)( 100u * 300u )) /* 300 sec (5 minutes) at 100 ticks/sec */

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** A buffer to store the protocol data in */
static uint8_t _bufferTxZero[DATA_PACKET_SIZE] = { 0u };
/** A buffer to store the protocol data in */
static uint8_t _bufferTxOne[DATA_PACKET_SIZE]  = { 0u };
/** A buffer to store the non-data messages */
static uint8_t _messageBuffer[MESSAGE_PACKET_SIZE] = { 0u };
/** A pointer to the active write array */
static uint8_t *_activeWriteArray = _bufferTxZero;
/** A buffer to store the received data in */
static uint8_t _bufferRx[DATA_PACKET_SIZE]   = { 0u };

/** Tracks if the protocol is in data mode or not */
static bool _dataMode = false;

/** Mats: This keeps track of how long device is Idle (in 10 ms ticks) **/
static uint32_t idleTick = 0u; 

/** Stores the current RF Status */
static RfStatus_t _rfStatus = { RfConnectionStatus_INVLALID, 0u, 0u };

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _HandleRfPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleStatusPacket(
		uint8_t messageData[],
		uint8_t messageLength);

static void _HandleSetSensorConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetSensorConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleSetOutputConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetOutputConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleSetFixedConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetFixedConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetPacketConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetCalibrationPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandleGetStatusPacket(
		uint8_t messageData[],
		uint8_t messageLength);
static void _HandlePowerDownPacket(
		uint8_t messageData[],
		uint8_t messageLength);

static void _SetTxCommand(
		CommCommand_t command);
static bool _WriteMessageData(
		uint8_t messageStartIndex,
		uint8_t messageLength,
		const uint8_t messageData[]);

static void _HandleRfStatusISR(void);

/***************************************************************************//**
    Initialized the comm protocol.
*//****************************************************************************/
void CommProtocol_Init()
{
	TickIsrHandle_t isrHandle;

	DataBuffer_Init(&_bufferTxZero[TX_PACKET_DATA_START_INDEX], &_bufferTxOne[TX_PACKET_DATA_START_INDEX], DATA_PACKET_SIZE - BYTES_PACKET_OVERHEAD);

	_bufferTxZero[START_OF_MESSAGE_INDEX] = START_OF_MESSAGE;
	_bufferTxOne[START_OF_MESSAGE_INDEX] = START_OF_MESSAGE;
	_bufferTxZero[COMMAND_INDEX] = CommCommand_RF_PACKET;
	_bufferTxOne[COMMAND_INDEX] = CommCommand_RF_PACKET;
	_bufferTxZero[END_OF_DATA_PACKET_INDEX] = END_OF_PACKET;
	_bufferTxOne[END_OF_DATA_PACKET_INDEX] = END_OF_PACKET;

	_messageBuffer[START_OF_MESSAGE_INDEX] = START_OF_MESSAGE;
	_messageBuffer[COMMAND_INDEX] = CommCommand_RF_PACKET;
	_messageBuffer[VALID_DATA_LENGTH_INDEX] = 0u;
	_messageBuffer[END_OF_MESSAGE_PACKET_INDEX] = END_OF_PACKET;

	SpiSlave_ReadPacket(DATA_PACKET_SIZE, _bufferRx);

	SpiSlave_LoadPacket(DATA_PACKET_SIZE, _messageBuffer);
	_activeWriteArray = _messageBuffer;

	isrHandle = TimerTick_RegisterTickIsr(_HandleRfStatusISR);
	TimerTick_EnableTickIsr(isrHandle);
}

/***************************************************************************//**
    Handles the events related to the received events for the Comm Protocol.
*//****************************************************************************/
void CommProtocol_RxHandler(void)
{
	SpiSlaveError_t spiError;

//	DEBUG("rx");
	switch(_bufferRx[COMMAND_INDEX])
	{
		case CommCommand_RF_PACKET:
			_HandleRfPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_STATUS:
			_HandleStatusPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_START_DATA:
			CommProtocol_HandleStartDataPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_STOP_DATA:
			CommProtocol_HandleStopDataPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_SET_SENSOR_CONFIG:
			_HandleSetSensorConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_SENSOR_CONFIG:
			_HandleGetSensorConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_SET_OUTPUT_CONFIG:
			_HandleSetOutputConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_OUTPUT_CONFIG:
			_HandleGetOutputConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_SET_FIXED_CONFIG:
			_HandleSetFixedConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_FIXED_CONFIG:
			_HandleGetFixedConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_PACKET_CONFIG:
			_HandleGetPacketConfigPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_CALIBRATION:
			_HandleGetCalibrationPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_GET_STATUS:
			_HandleGetStatusPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		case CommCommand_POWER_DOWN:
			_HandlePowerDownPacket(&_bufferRx[RX_PACKET_DATA_START_INDEX], _bufferRx[VALID_DATA_LENGTH_INDEX]);
			break;

		default:
			/* unhandled command type */
			Serial_SendStringNumber("unhandled cmd:", _bufferRx[COMMAND_INDEX]);
	}

	SpiSlave_FlushRead();
	spiError = SpiSlave_ReadPacket(DATA_PACKET_SIZE, _bufferRx);
	if (spiError != SpiSlaveError_OK)
	{
		DEBUG("spi read error");
	}
}

/***************************************************************************//**
    Handles the events related to the transmitted events for the Comm Protocol.
*//****************************************************************************/
void CommProtocol_TxHandler(void)
{
//	DEBUG("tx");
	SpiSlave_UnloadPacket();
	if (_dataMode == true)
	{
		if (DataBuffer_RotateBuffer() == 1u)
		{
			SpiSlave_LoadPacket(DATA_PACKET_SIZE, _bufferTxZero);
			_activeWriteArray = _bufferTxOne;
		}
		else
		{
			SpiSlave_LoadPacket(DATA_PACKET_SIZE, _bufferTxOne);
			_activeWriteArray = _bufferTxZero;
		}
	}
	else
	{
		/* TODO: spg - this will write out junk bytes, but must be
					   this long so that the DMA has the correct
					   number of bytes to shift out. Does this cause
					   any problems?
		*/
		SpiSlave_LoadPacket(DATA_PACKET_SIZE, _messageBuffer);
		_activeWriteArray = _messageBuffer;
	}
}

/***************************************************************************//**
    Gets the RF Status information.

    \param[out] status - structure to hold the RF status in

*//****************************************************************************/
void CommProtocol_GetRfStatus(RfStatus_t *status)
{
	*status = _rfStatus;
}

/***************************************************************************//**
    Handles the packet data for the RF Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleRfPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	_SetTxCommand(CommCommand_RF_PACKET);
	/* Do nothing else here */
}

/***************************************************************************//**
    Handles the packet data for the Status Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleStatusPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
#define STATUS_PACKET_EXPECTED_LENGTH  ( 3u )
#define FIRMWARE_MAJOR_INDEX           ( 0u )
#define FIRMWARE_MINOR_INDEX           ( 1u )
#define CONNECTION_STATUS_INDEX        ( 2u )

	ASSERT((messageData != NULL) && (messageLength == STATUS_PACKET_EXPECTED_LENGTH),
	        ErrorLedMode_SOFTWARE);

	_SetTxCommand(CommCommand_STATUS);

	/* Store the RF Status information */
	_rfStatus.fwMajor = messageData[FIRMWARE_MAJOR_INDEX];
	_rfStatus.fwMinor = messageData[FIRMWARE_MINOR_INDEX];
	_rfStatus.connectionStatus = (RfConnectionStatus_t)messageData[CONNECTION_STATUS_INDEX];

	switch(_rfStatus.connectionStatus)
	{
		case RfConnectionStatus_DATA:
			LedControl_SetLed(Led_1_GREEN, LedState_BLINK_FAST_ODD);
			break;

		case RfConnectionStatus_CONNECTED:
			LedControl_SetLed(Led_1_GREEN, LedState_BLINK_SLOW_ODD);
			break;

		case RfConnectionStatus_PAIRED_NOT_CONNECTED:
			LedControl_SetLed(Led_1_GREEN, LedState_BLINK_1);
			break;

		case RfConnectionStatus_PAIRING:
			LedControl_SetLed(Led_1_GREEN, LedState_BLINK_3);
			break;

		case RfConnectionStatus_NOT_PAIRED:
			LedControl_SetLed(Led_1_GREEN, LedState_ON);
			break;

		default:
			LedControl_SetLed(Led_1_GREEN, LedState_OFF);
			LedControl_SetLed(Led_1_RED, LedState_BLINK_FAST_ODD);
			break;
	}
}

/***************************************************************************//**
    Handles the packet data for the Start Data Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void CommProtocol_HandleStartDataPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	DEBUG("start data");
	if (_dataMode == false)
	{
		_dataMode = true;
		ConMan_StartData();

		/* init all the buffers */
		_messageBuffer[COMMAND_INDEX] = (uint8_t) CommCommand_START_DATA;
		_messageBuffer[VALID_DATA_LENGTH_INDEX] = 0u;

		_bufferTxZero[COMMAND_INDEX] = (uint8_t) CommCommand_START_DATA;
		_bufferTxZero[VALID_DATA_LENGTH_INDEX] = 0u;

		_bufferTxOne[COMMAND_INDEX] = (uint8_t) CommCommand_START_DATA;
		_bufferTxOne[VALID_DATA_LENGTH_INDEX] = 0u;
	}
	else
	{
		_SetTxCommand(CommCommand_START_DATA);
		_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
	}
}

/***************************************************************************//**
    Handles the packet data for the Stop Data Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void CommProtocol_HandleStopDataPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	DEBUG("stop data");
	if (_dataMode == true)
	{
		_dataMode = false;
		ConMan_StopData();
		idleTick = 0u; /* Mats: Reset the idle counter whenever acquisition stops */

		/* clear all the buffers */
		_messageBuffer[COMMAND_INDEX] = (uint8_t) CommCommand_STOP_DATA;
		_messageBuffer[VALID_DATA_LENGTH_INDEX] = 0u;

		_bufferTxZero[COMMAND_INDEX] = (uint8_t) CommCommand_STOP_DATA;
		_bufferTxZero[VALID_DATA_LENGTH_INDEX] = 0u;

		_bufferTxOne[COMMAND_INDEX] = (uint8_t) CommCommand_STOP_DATA;
		_bufferTxOne[VALID_DATA_LENGTH_INDEX] = 0u;
	}
	else
	{
		_SetTxCommand(CommCommand_STOP_DATA);
		_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
	}
}

/***************************************************************************//**
    Handles the packet data for the Set Sensor Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleSetSensorConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	// TODO: spg - handle packet.
	DEBUG("set sensor cfg");
	_SetTxCommand(CommCommand_SET_SENSOR_CONFIG);
	_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
}

/***************************************************************************//**
    Handles the packet data for the Get Sensor Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetSensorConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	// TODO: spg - handle packet.
	DEBUG("get sensor cfg");
	_SetTxCommand(CommCommand_GET_SENSOR_CONFIG);
	_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
}

/***************************************************************************//**
    Handles the packet data for the Set Output Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleSetOutputConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	DEBUG("set out conf");
	_SetTxCommand(CommCommand_SET_OUTPUT_CONFIG);
	/* define an offset that will skip over the unimportant bytes in the message */
#define OUTPUT_CONFIG_USEFUL_DATA_OFFSET    ( 2u )
    ConMan_SetOutputConfig((uint16_t)messageLength - OUTPUT_CONFIG_USEFUL_DATA_OFFSET,
                           &messageData[OUTPUT_CONFIG_USEFUL_DATA_OFFSET]);
}

/***************************************************************************//**
    Handles the packet data for the Get Output Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetOutputConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	// TODO: spg - handle packet.
	DEBUG("get out conf");
	_SetTxCommand(CommCommand_GET_OUTPUT_CONFIG);
	_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
}

/***************************************************************************//**
    Handles the packet data for the Set Fixed Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleSetFixedConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	DEBUG("set fixed config");
	idleTick = 0u; /* Mats: Reset the idle counter whenever the fixed configuration changes */

	_SetTxCommand(CommCommand_SET_FIXED_CONFIG);
	/** define an offset that will skip over the unimportant bytes in the message */
#define FIXED_CONFIG_USEFUL_DATA_OFFSET    ( 1u )
    ConMan_SetFixedConfig((uint16_t)messageLength - FIXED_CONFIG_USEFUL_DATA_OFFSET,
                           &messageData[FIXED_CONFIG_USEFUL_DATA_OFFSET]);
}

/***************************************************************************//**
    Handles the packet data for the Get Fixed Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetFixedConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	uint8_t fixedConfig;

	DEBUG("get fixed config");
	_SetTxCommand(CommCommand_GET_FIXED_CONFIG);

	fixedConfig = (uint8_t)ConMan_GetFixedConfig();

	/* increase the length byte by 1 to allow for the remote ID from the message data
	   and write that number of useful bytes to the write array.
	*/
	_activeWriteArray[VALID_DATA_LENGTH_INDEX]= 1u + 1u;
	_activeWriteArray[TX_REMOTE_ID_INDEX] = messageData[RX_REMOTE_ID_INDEX];
	_activeWriteArray[TX_REMOTE_ID_INDEX + 1u] = fixedConfig;
}

/***************************************************************************//**
    Handles the packet data for the Get Packet Configuration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetPacketConfigPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	const uint8_t *configData = NULL;
	uint16_t configLength = 0;
	uint8_t  emptyConfig = 0;

	DEBUG("packet config");

	_SetTxCommand(CommCommand_GET_PACKET_CONFIG);

	configData = ConMan_GetPacketConfig(&configLength);

	if ((configData != NULL) && (configLength > 0))
	{
		/* increase the length byte by 1 to allow for the remote ID from the message data
		   and write that number of useful bytes to the write array.
		*/
		_activeWriteArray[VALID_DATA_LENGTH_INDEX]= configData[0] + 1u;
		_activeWriteArray[TX_REMOTE_ID_INDEX] = messageData[RX_REMOTE_ID_INDEX];
		_WriteMessageData(TX_REMOTE_ID_INDEX + 1u, configLength - 1u, &configData[1u]);
	}
	else
	{
		DEBUG("bad config");
		_activeWriteArray[VALID_DATA_LENGTH_INDEX]= 2u;
		_activeWriteArray[TX_REMOTE_ID_INDEX] = messageData[RX_REMOTE_ID_INDEX];
		_activeWriteArray[TX_REMOTE_ID_INDEX + 1u] = emptyConfig;
	}
}

/***************************************************************************//**
    Handles the packet data for the Get Calibration Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetCalibrationPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
	DEBUG("get calib");
	_SetTxCommand(CommCommand_GET_CALIBRATION);

	static uint8_t calibrationData[8u];
	uint16_t calibrationLength = 0;
	uint8_t  shortLength;
	uint8_t sensorId = 0;

	if (messageLength >= 2u)
	{
		sensorId = messageData[1u];
		ConMan_GetCalibration((SensorId_t)sensorId, 8u, &calibrationLength, calibrationData);
		if (calibrationLength <= UINT8_T_MAX)
		{
			shortLength = (uint8_t)calibrationLength;
		}
		else
		{
			DEBUG("conman calibration length too long");
		}
		/* increase the length byte by 1 to allow for the remote ID from the message data
		   and write that number of useful bytes to the write array.
		*/
		_activeWriteArray[VALID_DATA_LENGTH_INDEX] = shortLength + 3u;
		_activeWriteArray[TX_REMOTE_ID_INDEX] = messageData[RX_REMOTE_ID_INDEX];
		_activeWriteArray[TX_REMOTE_ID_INDEX + 1u] = sensorId;
		_activeWriteArray[TX_REMOTE_ID_INDEX + 2u] = shortLength;
		_WriteMessageData(TX_REMOTE_ID_INDEX + 3u, calibrationLength, calibrationData);
	}
	else
	{
		DEBUG("message data too short");
		_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;
	}


}

/***************************************************************************//**
    Handles the packet data for the Get Status Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
void _HandleGetStatusPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{
#define LENGTH_STATUS_REPLY   ( 6u )
	uint16_t batteryVoltage;

	DEBUG("get status");
	_SetTxCommand(CommCommand_GET_STATUS);

	/* increase the length byte by 1 to allow for the remote ID from the message data
	   and write that number of useful bytes to the write array.
	*/
	_activeWriteArray[VALID_DATA_LENGTH_INDEX]= LENGTH_STATUS_REPLY + 1u;

	_activeWriteArray[TX_REMOTE_ID_INDEX] = messageData[RX_REMOTE_ID_INDEX];
	/* sensor micro firmware version */
	_activeWriteArray[TX_REMOTE_ID_INDEX + 1u] = USER_FIRMWARE_MAJOR;
	_activeWriteArray[TX_REMOTE_ID_INDEX + 2u] = USER_FIRMWARE_MINOR;
	/* rf micro firmware version */
	_activeWriteArray[TX_REMOTE_ID_INDEX + 3u] = _rfStatus.fwMajor;
	_activeWriteArray[TX_REMOTE_ID_INDEX + 4u] = _rfStatus.fwMinor;
	/* battery voltage */
	batteryVoltage = (uint16_t)BatteryControl_GetCounts();
	_activeWriteArray[TX_REMOTE_ID_INDEX + 5u] = (uint8_t)(batteryVoltage >> 8u);
	_activeWriteArray[TX_REMOTE_ID_INDEX + 6u] = (uint8_t)(batteryVoltage);
}

/***************************************************************************//**
    Handles the packet data for the Power Down Command.

    \param[in] messageData[] - array of the data contained in the message
    \param[in] messageLength - the number of valid bytes in the message data

*//****************************************************************************/
static void _HandlePowerDownPacket(
		uint8_t messageData[],
		uint8_t messageLength)
{

	DEBUG("power down message received");
	_SetTxCommand(CommCommand_POWER_DOWN);
	_activeWriteArray[VALID_DATA_LENGTH_INDEX] = 0u;

	/* Allow the system to use the default shutdown procedure */
	EventQueue_Add(Event_SHUTDOWN, EventPriority_LOWEST);
}

/***************************************************************************//**
    Sets the command byte to the proper value based on the type of command.

    \param[in] command - the type of command to set for the TX Packet

*//****************************************************************************/
static void _SetTxCommand(
		CommCommand_t command)
{
	_activeWriteArray[COMMAND_INDEX] = (uint8_t)command;
}

/***************************************************************************//**
    Takes message data and writes it to the active write array.

    \param[in] messageStartIndex - the array index where the data should start at
    \param[in] messageLength - the number of bytes of the message to write
    \param[in] messageData - the data of the message to write to the array

    \returns True if the data would fit, false otherwise

*//****************************************************************************/
bool _WriteMessageData(
		uint8_t messageStartIndex,
		uint8_t messageLength,
		const uint8_t messageData[])
{
	bool messageFits = false;
	uint8_t index;

	if ((messageStartIndex + messageLength) <= MESSAGE_PACKET_SIZE)
	{
		messageFits = true;

		for (index = 0u; index < messageLength; ++index)
		{
			_activeWriteArray[index + messageStartIndex] = messageData[index];
		}
	}
	else
	{
		DEBUG("message won't fit");
	}

	return messageFits;
}

/***************************************************************************//**
    Processes system tasks based on the state of the RF connection and the time
    spent in that state. This should be called by the timer tick at regular
    intervals.
*//****************************************************************************/
static void _HandleRfStatusISR(void)
{
	static uint32_t noConnectTime = 0u;

	if ((_rfStatus.connectionStatus >= RfConnectionStatus_FIRST) &&
		(_rfStatus.connectionStatus <= RfConnectionStatus_PAIRED_NOT_CONNECTED))
	{
		noConnectTime += TIMERTICK_MSEC_PER_TICK;
		idleTick = 0u;  /* Mats: Keep the Idle counter zeroed until the device is connected */
						/* (This may be redundant) */

		if (noConnectTime >= MAX_NO_CONNECT_TIME_MSEC)
		{
		    EventQueue_Add(Event_SHUTDOWN, EventPriority_NORMAL);
		}
	}

	/* Mats: Keep track of how long we are idle and shut down after MAX_IDLE_TICK ticks*/
	else if (_rfStatus.connectionStatus == RfConnectionStatus_CONNECTED)
	{
		/* Mats: Advance the idleTick counter only if we aren't recording data */
		/* Probably not be needed since it seems this routine is not called during acquisition ? */
		if (_dataMode == false) 
		{
			idleTick += 1;
		}

		if (idleTick >= MAX_IDLE_TICK)
		{
			EventQueue_Add(Event_SHUTDOWN, EventPriority_NORMAL);
		}
	}

	else
	{
		noConnectTime = 0u;
	}
}
