/***************************************************************************//**
    \file ConfigurationManager.c
    \brief The configuration manager keeps track of all the sensor configurations

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The configuration manager is the module that will keep track of
       configuration all the sensors to be in the proper sensor mode based on the
       setup commands.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/ConfigurationManager.c$
    $Revision: 37$
    $Date: Thursday, June 11, 2015 4:16:57 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

    Mats: 1/1/2019.  Added a new fixed configuration to read A7 at 4800 kHz
    Mats: 1/12/2019. Added 2 more fixed configurations to 
                     (i)  read A7 and A8 at 2400 Hz, and
                     (ii) read A7, A8, and A9 at 800 Hz 
    Mats: 1/13/2019. Added 8g full scale 800Hz accelerometer configuration (will have calibration issue) 


*******************************************************************************/


#include <ProjectTypes.h>
#include <Assert.h>
#include <Debug.h>
#include <Serial.h>
#include <SensorId.h>
#include <DataBuffer.h>
#include <Accelerometer.h>
#include <AccelerometerControl.h>
#include <Magnetometer.h>
#include <MagnetometerControl.h>
#include <EncoderControl.h>
#include <ForceGaugeControl.h>
#include <GyroscopeControl.h>
#include <BarometerControl.h>
#include <Gyroscope.h>
#include <Ultrasound.h>
#include <DacControl.h>
#include <BuzzerControl.h>
#include <BatteryControl.h>
#include <AmbientLightControl.h>
#include <HighGainControl.h>
#include <EcgControl.h>
#include <Analog7Control.h>
#include <Analog8Control.h>
#include <Analog9Control.h>
#include <Header1Control.h>
#include <Header2Control.h>
#include <Header3Control.h>
#include <Header4Control.h>
#include <Header5Control.h>
#include <Header6Control.h>
#include <DigitalInputControl.h>
#include <MicrophoneControl.h>
#include <ThermometerControl.h>
#include <Ecg6ChanControl.h>
#include <KeyValue.h>
#include <FixedConfigSettings.h>
#include "ConfigurationManager.h"

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
#if 1 /* used to enable and disable the accelerometer */
#define ENABLE_ACCELEROMETER
#endif

/** Defines the number of bytes that are used in a configuration setting */
#define BYTES_PER_CONFIG    ( 2u )

/** Defines the index of the length byte in the fixed configurations */
#define LENGTH_BYTE_OFFSET   ( 1u )
/** Defines the index within a single configuration setting where the sensor ID
    is located
*/
#define SENSOR_ID_INDEX     ( 0u )
/** Defines the index within a single configuration setting where the key-value
    pair is located
*/
#define KEY_VALUE_INDEX     ( 1u )

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** tracks if the module has been properly initialized */
static volatile bool _initialized = false;

/** Stores the setting for the current fixed configuration */
static FixedConfigList_t _currentConfig = FixedConfigList_AMBIENT;

/** Stores the state of the system for data mode */
static bool _dataModeActive = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _ConMan_AllocateBuffer(const uint8_t bufferConfiguration[]);

/***************************************************************************//**
    Initializes the configuration manager. The initialization include initializing
    all the sensors for which the configuration manager manages.
*//****************************************************************************/
void ConMan_Init()
{
	/* initialize sensors */
#ifdef ENABLE_ACCELEROMETER
	AccControl_Init();
#endif
	MagControl_Init();
	EncControl_Init();
	FgControl_Init();
	BarometerControl_Init();
	GyroControl_Init();
	DacControl_Init();
	UltrasoundInit();
	BuzzerControl_Init();
	BatteryControl_Init();
	AmbientLightControl_Init();
	HighGainControl_Init();
	EcgControl_Init();
	Analog7Control_Init();
	Analog8Control_Init();
	Analog9Control_Init();
	Header1Control_Init();
	Header2Control_Init();
	Header3Control_Init();
	Header4Control_Init();
	Header5Control_Init();
	Header6Control_Init();
	DigitalInputControl_Init();
	MicrophoneControl_Init();
	ThermometerControl_Init();
	Ecg6Control_Init();

	_initialized = true;
}

/***************************************************************************//**
    Parses the sensor configuration command sends the configuration data to
    the appropriate sensors to manage the settings.

    \param[in] configLength - the number of valid bytes in the configuration data
    \param[in] config[] - array of the data contained in the configuration

*//****************************************************************************/
void ConMan_SetSensorConfig(
		uint16_t configLength,
		const uint8_t config[])
{
	uint16_t index;
	KeyValue_t keyValue;
	SensorId_t sensorId;

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);

	for (index = 0u; (index + KEY_VALUE_INDEX) < configLength; index += BYTES_PER_CONFIG)
	{
		sensorId = (SensorId_t)config[index + SENSOR_ID_INDEX];
		keyValue = (KeyValue_t)config[index + KEY_VALUE_INDEX];

		switch(sensorId)
		{
			case SensorId_ACCELEROMETER:
#ifdef ENABLE_ACCELEROMETER
				AccControl_SetConfig(keyValue);
#endif
				break;

			case SensorId_AMBIENT_LIGHT:
				AmbientLightControl_SetConfig(keyValue);
				break;

			case SensorId_BAROMETER:
				BarometerControl_SetConfig(keyValue);
				break;

			case SensorId_BATTERY:
				BatteryControl_SetConfig(keyValue);
				break;

			case SensorId_ECG:
				EcgControl_SetConfig(keyValue);
				break;

			case SensorId_ENCODER:
				EncControl_SetConfig(keyValue);
				break;

			case SensorId_FORCE_GAUGE:
				FgControl_SetConfig(keyValue);
				break;

			case SensorId_GYROSCOPE:
				GyroControl_SetConfig(keyValue);
				break;

			case SensorId_HEADER_0:
				break;

			case SensorId_HEADER_1:
				Header1Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_2:
				Header2Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_3:
				Header3Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_4:
				Header4Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_5:
				Header5Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_6:
				Header6Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_7:
				Analog7Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_8:
				Analog8Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_9:
				Analog9Control_SetConfig(keyValue);
				break;

			case SensorId_HIGH_GAIN_IN:
				HighGainControl_SetConfig(keyValue);
				break;

			case SensorId_MAGNETOMETER:
				MagControl_SetConfig(keyValue);
				break;

			case SensorId_MICROPHONE:
				MicrophoneControl_SetConfig(keyValue);
				break;

			case SensorId_DIGITALINPUTS:
				DigitalInputControl_SetConfig(keyValue);
				break;

			case SensorId_ULTRASONIC:
				break;

			case SensorId_THERMOMETER:
			    ThermometerControl_SetConfig(keyValue);
			    break;

			case SensorId_ECG6CHAN:
			    Ecg6Control_SetConfig(keyValue);
			    break;

			default:
				DEBUG("unhandled sensor");
				break;
		}
	}
}

/***************************************************************************//**
    Selects the fixed configuration and sends the configuration data to
    the appropriate sensors to manage the settings.

    \param[in] configLength - the number of valid bytes in the configuration data
    \param[in] config[] - array of the data contained in the configuration

*//****************************************************************************/
void ConMan_SetFixedConfig(
		uint16_t configLength,
		const uint8_t config[])
{
	FixedConfigList_t configSetting;

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);

	configSetting = (FixedConfigList_t)config[0u];

    Serial_SendStringNumber("FixCfg:", configSetting);

	if ((configSetting >= FixedConfigList_FIRST) &&
		(configSetting < FixedConfigList_LAST))
	{
		_currentConfig = configSetting;
	}
	else
	{
		DEBUG("invalid fixed conf");
	}
}


/***************************************************************************//**
    Fetches and returns the current fixed configuration setting.

    \returns The setting for the current fixed configuration.

*//****************************************************************************/
FixedConfigList_t ConMan_GetFixedConfig(void)
{
	return _currentConfig;
}

/***************************************************************************//**
    Parses the output configuration command and sends the configuration data to
    the appropriate sensors to manage the settings.

    \param[in] configLength - the number of valid bytes in the configuration data
    \param[in] config[] - array of the data contained in the configuration

*//****************************************************************************/
void ConMan_SetOutputConfig(
		uint16_t configLength,
		const uint8_t config[])
{
	uint16_t index;
	KeyValue_t keyValue;
	SensorId_t sensorId;

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);

	for (index = 0u; (index + KEY_VALUE_INDEX) < configLength; index += BYTES_PER_CONFIG)
	{
		sensorId = (SensorId_t)config[index + SENSOR_ID_INDEX];
		keyValue = (KeyValue_t)config[index + KEY_VALUE_INDEX];
		switch(sensorId)
		{
			case SensorId_BUZZER:
				BuzzerControl_SetConfig(keyValue);
				break;

			case SensorId_DAC:
				DacControl_SetConfig(keyValue);
				break;

			case SensorId_HEADER_0:
				break;

			case SensorId_HEADER_1:
				Header1Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_2:
				Header2Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_3:
				Header3Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_4:
				Header4Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_5:
				Header5Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_6:
				Header6Control_SetConfig(keyValue);
				break;

			case SensorId_HEADER_7:
				break;

			case SensorId_HEADER_8:
				break;

			case SensorId_HEADER_9:
				break;

			default:
				DEBUG("unhandled out conf");
				break;
		}
	}
}

/***************************************************************************//**
    Returns the current configuration of the data packet.

    \param[out] packetLength - the number of bytes in the packet config array

    \returns Pointer to a static constant array containing the packet config

*//****************************************************************************/
const uint8_t* ConMan_GetPacketConfig(uint16_t *packetLength)
{
	const uint8_t *packet = NULL;

	switch(_currentConfig)
	{

		case FixedConfigList_GYRO:
			packet = _gyroBuffer;
			*packetLength = BUFFER_SIZE_GYRO;
			break;

		case FixedConfigList_ACCEL:
			packet = _accelBuffer;
			*packetLength = BUFFER_SIZE_ACCEL;
			break;

		/* Mats: new configuration to read A7 at 4800 Hz */
		case FixedConfigList_ANALOG_HI1:
			packet = _analog_hi1Buffer;
			*packetLength = BUFFER_SIZE_ANALOG_HI1;
			break;

		/* Mats: new configuration to read A7, A8 at 2400 Hz */
		case FixedConfigList_ANALOG_HI2:
			packet = _analog_hi2Buffer;
			*packetLength = BUFFER_SIZE_ANALOG_HI2;
			break;

		/* Mats: new configuration to read A7, A8, A9 at 800 Hz */
		case FixedConfigList_ANALOG_HI3:
			packet = _analog_hi3Buffer;
			*packetLength = BUFFER_SIZE_ANALOG_HI3;
			break;

		case FixedConfigList_IMU:
			packet = _imuBuffer;
			*packetLength = BUFFER_SIZE_IMU;
			break;

		case FixedConfigList_MINIMOTION:
			packet = _miniMotionBuffer;
			*packetLength = BUFFER_SIZE_MINIMOTION;
			break;

		case FixedConfigList_PENDULUM:
			packet = _pendulumBuffer;
			*packetLength = BUFFER_SIZE_PENDULUM;
			break;

		case FixedConfigList_AMBIENT:
			packet = _ambientBuffer;
			*packetLength = BUFFER_SIZE_AMBIENT;
			break;

		case FixedConfigList_ECG:
			packet = _ecgBuffer;
			*packetLength = BUFFER_SIZE_ECG;
			break;

		case FixedConfigList_HEADER:
			packet = _headerBuffer;
			*packetLength = BUFFER_SIZE_HEADER;
			break;

        case FixedConfigList_SLOW_MIC:
            packet = _slowMicrophoneBuffer;
            *packetLength = BUFFER_SIZE_SLOW_MICROPHONE;
            break;

        case FixedConfigList_MAGNETIC:
            packet = _magneticBuffer;
            *packetLength = BUFFER_SIZE_MAGNETIC;
            break;

        case FixedConfigList_ANLG_HEAD:
            packet = _anlgHeadBuffer;
            *packetLength = BUFFER_SIZE_ANLG_HEAD;
            break;

        case FixedConfigList_ECG6:
            packet = _ecg6Buffer;
            *packetLength = BUFFER_SIZE_ECG6;
            break;

        case FixedConfigList_HEADER_3V3:
            packet = _header3V3Buffer;
            *packetLength = BUFFER_SIZE_HEADER_3V3;
            break;

		case FixedConfigList_GYRO_HI:
			packet = _gyroHiBuffer;
			*packetLength = BUFFER_SIZE_GYRO_HI;
			break;

		case FixedConfigList_ACCEL_HI:
			packet = _accelHiBuffer;
			*packetLength = BUFFER_SIZE_ACCEL_HI;
			break;

		/* Mats: new HI8G configuration has same packet configuration as the regular one above */
		case FixedConfigList_ACCEL_HI8G:
			packet = _accelHiBuffer;
			*packetLength = BUFFER_SIZE_ACCEL_HI;
			break;

		case FixedConfigList_IMU_HI:
			packet = _imuHiBuffer;
			*packetLength = BUFFER_SIZE_IMU_HI;
			break;

		case FixedConfigList_MOTION:
			packet = _motionBuffer;
			*packetLength = BUFFER_SIZE_MOTION;
			break;

		case FixedConfigList_SPORTS:
			packet = _sportsBuffer;
			*packetLength = BUFFER_SIZE_SPORTS;
			break;

		case FixedConfigList_PENDULUM_HI:
			packet = _pendulumHiBuffer;
			*packetLength = BUFFER_SIZE_PENDULUM_HI;
			break;

		case FixedConfigList_KITCHENSINK:
			packet = _kitchenSinkBuffer;
			*packetLength = BUFFER_SIZE_KITCHENSINK;
			break;

		case FixedConfigList_MICROPHONE:
			packet = _microphoneBuffer;
			*packetLength = BUFFER_SIZE_MICROPHONE;
			break;

		case FixedConfigList_ALS_HI:
			packet = _alsHiBuffer;
			*packetLength = BUFFER_SIZE_ALS_HI;
			break;

		case FixedConfigList_ALS_ACC:
			packet = _alsAccBuffer;
			*packetLength = BUFFER_SIZE_ALS_ACC;
			break;

		case FixedConfigList_FORCE_ACC:
			packet = _forceAccBuffer;
			*packetLength = BUFFER_SIZE_FORCE_ACC;
			break;

		case FixedConfigList_ALS_MIC:
		    packet = _alsMicBuffer;
		    *packetLength = BUFFER_SIZE_ALS_MIC;
		    break;

        case FixedConfigList_HI_ECG:
            packet = _hiEcgBuffer;
            *packetLength = BUFFER_SIZE_HI_ECG;
            break;

        case FixedConfigList_HI_GAIN:
            packet = _hiGainBuffer;
            *packetLength = BUFFER_SIZE_HI_GAIN;
            break;

        case FixedConfigList_HI_FORCE:
            packet = _hiForceBuffer;
            *packetLength = BUFFER_SIZE_HI_FORCE;
            break;

        case FixedConfigList_HI_ANLG_HED:
            packet = _hiAnlgHeadBuffer;
            *packetLength = BUFFER_SIZE_HI_ANLG_HEAD;
            break;

        case FixedConfigList_HI_ECG6:
            packet = _hiEcg6Buffer;
            *packetLength = BUFFER_SIZE_HI_ECG6;
            break;

        case FixedConfigList_SELFTEST:
            packet = _selfTestBuffer;
            *packetLength = BUFFER_SIZE_SELFTEST;
            break;

		default:
			DEBUG("unhandled get pkt cfg");
			*packetLength = 0u;
			break;
	}

	return packet;
}

/***************************************************************************//**
    Returns the configuration data for a specific sensor.

    \param[in]  sensorId - the sensor the configuration is for
    \param[in]  maxLength - the number of bytes allocation fro the calibration array
    \param[out] actualLength - the number of bytes put in the calibration array
	\param[out] calibration[] - the calibration data for sensor

*//****************************************************************************/
void ConMan_GetCalibration(
		SensorId_t sensorId,
		uint16_t   maxLength,
		uint16_t  *actualLength,
		uint8_t    calibration[])
{
	ASSERT((actualLength != NULL) && (calibration != NULL), ErrorLedMode_SOFTWARE);

	switch(sensorId)
	{
		case SensorId_ACCELEROMETER:
			*actualLength = 0u;
			break;

		case SensorId_AMBIENT_LIGHT:
			*actualLength = 0u;
			break;

		case SensorId_BAROMETER:
			BarometerControl_ReadCalibration(maxLength, actualLength, calibration);
			break;

		case SensorId_BATTERY:
			BatteryControl_ReadCalibration(maxLength, actualLength, calibration);
			break;

		case SensorId_ECG:
			*actualLength = 0u;
			break;

		case SensorId_ENCODER:
			*actualLength = 0u;
			break;

		case SensorId_FORCE_GAUGE:
			*actualLength = 0u;
			break;

		case SensorId_GYROSCOPE:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_0:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_1:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_2:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_3:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_4:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_5:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_6:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_7:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_8:
			*actualLength = 0u;
			break;

		case SensorId_HEADER_9:
			*actualLength = 0u;
			break;

		case SensorId_HIGH_GAIN_IN:
			*actualLength = 0u;
			break;

		case SensorId_MAGNETOMETER:
			*actualLength = 0u;
			break;

		case SensorId_MICROPHONE:
			*actualLength = 0u;
			break;

		case SensorId_DIGITALINPUTS:
			*actualLength = 0u;
			break;

		case SensorId_ULTRASONIC:
			*actualLength = 0u;
			break;

		case SensorId_THERMOMETER:
		    ThermometerControl_ReadCalibration(maxLength, actualLength, calibration);
		    break;

		case SensorId_ECG6CHAN:
		    *actualLength = 0u;
		    break;

		default:
			*actualLength = 0u;
			DEBUG("unhandled sensor");
			break;
	}
}

/***************************************************************************//**
    Sets the previously or default configured sensors to active mode to begin
    collecting data.
*//****************************************************************************/
void ConMan_StartData()
{
	if (_dataModeActive == true)
	{
		DEBUG("not restarting");
		return;
	}

	DataBuffer_ClearSubarrays();
	switch(_currentConfig)
	{
		case FixedConfigList_GYRO:
			DEBUG("fixed gyro");
			_ConMan_AllocateBuffer(_gyroBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_GYRO - LENGTH_BYTE_OFFSET, &_gyroConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ACCEL:
			DEBUG("fixed accel");
			_ConMan_AllocateBuffer(_accelBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ACCEL - LENGTH_BYTE_OFFSET, &_accelConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;


		/* Mats: new configuration to read A7, A8, A9 at 800 Hz */
		case FixedConfigList_ANALOG_HI3:
			DEBUG("fixed analog_hi3");
			_ConMan_AllocateBuffer(_analog_hi3Buffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ANALOG_HI3 - LENGTH_BYTE_OFFSET, &_analog_hi3Config[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		/* Mats: new configuration to read A7, A8 at 2400 Hz */
		case FixedConfigList_ANALOG_HI2:
			DEBUG("fixed analog_hi2");
			_ConMan_AllocateBuffer(_analog_hi2Buffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ANALOG_HI2 - LENGTH_BYTE_OFFSET, &_analog_hi2Config[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		/* Mats: new configuration to read A7 at 4800 Hz */
		case FixedConfigList_ANALOG_HI1:
			DEBUG("fixed analog_hi1");
			_ConMan_AllocateBuffer(_analog_hi1Buffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ANALOG_HI1 - LENGTH_BYTE_OFFSET, &_analog_hi1Config[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_IMU:
			DEBUG("fixed IMU");
			_ConMan_AllocateBuffer(_imuBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_IMU - LENGTH_BYTE_OFFSET, &_imuConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_MINIMOTION:
			DEBUG("fixed mini motion");
			_ConMan_AllocateBuffer(_miniMotionBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_MINIMOTION - LENGTH_BYTE_OFFSET, &_miniMotionConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_PENDULUM:
			DEBUG("fixed pendulum");
			_ConMan_AllocateBuffer(_pendulumBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_PENDULUM - LENGTH_BYTE_OFFSET, &_pendulumConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_AMBIENT:
			DEBUG("fixed ambient");
			_ConMan_AllocateBuffer(_ambientBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_AMBIENT - LENGTH_BYTE_OFFSET, &_ambientConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ECG:
			DEBUG("fixed ecg");
			_ConMan_AllocateBuffer(_ecgBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ECG - LENGTH_BYTE_OFFSET, &_ecgConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_HEADER:
			DEBUG("fixed header");
			_ConMan_AllocateBuffer(_headerBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_HEADER - LENGTH_BYTE_OFFSET, &_headerConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

        case FixedConfigList_SLOW_MIC:
            DEBUG("fixed slow microphone");
            _ConMan_AllocateBuffer(_slowMicrophoneBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_SLOW_MICROPHONE - LENGTH_BYTE_OFFSET, &_slowMicrophoneConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_MAGNETIC:
            DEBUG("fixed magnetic");
            _ConMan_AllocateBuffer(_magneticBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_MAGNETIC- LENGTH_BYTE_OFFSET, &_magneticConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_ANLG_HEAD:
            DEBUG("fixed anlg head");
            _ConMan_AllocateBuffer(_anlgHeadBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_ANLG_HEAD- LENGTH_BYTE_OFFSET, &_anlgHeadConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_HEADER_3V3:
            DEBUG("fixed header 3.3V");
            _ConMan_AllocateBuffer(_header3V3Buffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HEADER_3V3 - LENGTH_BYTE_OFFSET, &_header3V3Config[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_ECG6:
            DEBUG("fixed ECG6");
            _ConMan_AllocateBuffer(_ecg6Buffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_ECG6- LENGTH_BYTE_OFFSET, &_ecg6Config[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

		case FixedConfigList_GYRO_HI:
			DEBUG("fixed gyro hi");
			_ConMan_AllocateBuffer(_gyroHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_GYRO_HI - LENGTH_BYTE_OFFSET, &_gyroHiConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ACCEL_HI:
			DEBUG("fixed accel hi");
			_ConMan_AllocateBuffer(_accelHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ACCEL_HI - LENGTH_BYTE_OFFSET, &_accelHiConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

        /* Mats: configuring the new fast HI8G uses _accelHi8gConfig, defined in FixedConfigSettings.h  */
		case FixedConfigList_ACCEL_HI8G:
			DEBUG("fixed accel hi 8g");
			_ConMan_AllocateBuffer(_accelHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ACCEL_HI - LENGTH_BYTE_OFFSET, &_accelHi8gConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_IMU_HI:
			DEBUG("fixed imu hi");
			_ConMan_AllocateBuffer(_imuHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_IMU_HI - LENGTH_BYTE_OFFSET, &_imuHiConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_MOTION:
			DEBUG("fixed motion");
			_ConMan_AllocateBuffer(_motionBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_MOTION - LENGTH_BYTE_OFFSET, &_motionConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_SPORTS:
			DEBUG("fixed sports");
			_ConMan_AllocateBuffer(_sportsBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_SPORTS - LENGTH_BYTE_OFFSET, &_sportsConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_PENDULUM_HI:
			DEBUG("fixed pendulum hi");
			_ConMan_AllocateBuffer(_pendulumHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_PENDULUM_HI - LENGTH_BYTE_OFFSET, &_pendulumHiConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_KITCHENSINK:
			DEBUG("fixed kitchen sink");
			_ConMan_AllocateBuffer(_kitchenSinkBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_KITCHENSINK - LENGTH_BYTE_OFFSET, &_kitchenSinkConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_MICROPHONE:
			DEBUG("fixed microphone");
			_ConMan_AllocateBuffer(_microphoneBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_MICROPHONE - LENGTH_BYTE_OFFSET, &_microphoneConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ALS_HI:
			DEBUG("als hi");
			_ConMan_AllocateBuffer(_alsHiBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ALS_HI - LENGTH_BYTE_OFFSET, &_alsHiConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ALS_ACC:
			DEBUG("als acc");
			_ConMan_AllocateBuffer(_alsAccBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_ALS_ACC - LENGTH_BYTE_OFFSET, &_alsAccConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_FORCE_ACC:
			DEBUG("force acc");
			_ConMan_AllocateBuffer(_forceAccBuffer);
			ConMan_SetSensorConfig(CONFIG_SIZE_FORCE_ACC - LENGTH_BYTE_OFFSET, &_forceAccConfig[LENGTH_BYTE_OFFSET]);
			_dataModeActive = true;
			break;

		case FixedConfigList_ALS_MIC:
		    DEBUG("als mic");
		    _ConMan_AllocateBuffer(_alsMicBuffer);
		    ConMan_SetSensorConfig(CONFIG_SIZE_ALS_MIC - LENGTH_BYTE_OFFSET, &_alsMicConfig[LENGTH_BYTE_OFFSET]);
		    _dataModeActive = true;
		    break;

        case FixedConfigList_HI_ECG:
            DEBUG("fixed hi ecg");
            _ConMan_AllocateBuffer(_hiEcgBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HI_ECG - LENGTH_BYTE_OFFSET, &_hiEcgConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_HI_GAIN:
            DEBUG("fixed hi gain");
            _ConMan_AllocateBuffer(_hiGainBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HI_GAIN - LENGTH_BYTE_OFFSET, &_hiGainConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_HI_FORCE:
            DEBUG("fixed hi force");
            _ConMan_AllocateBuffer(_hiForceBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HI_FORCE- LENGTH_BYTE_OFFSET, &_hiForceConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_HI_ANLG_HED:
            DEBUG("fixed hi anlg head");
            _ConMan_AllocateBuffer(_hiAnlgHeadBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HI_ANLG_HEAD- LENGTH_BYTE_OFFSET, &_hiAnlgHeadConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_HI_ECG6:
            DEBUG("fixed hi ECG6");
            _ConMan_AllocateBuffer(_hiEcg6Buffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_HI_ECG6- LENGTH_BYTE_OFFSET, &_hiEcg6Config[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

        case FixedConfigList_SELFTEST:
            DEBUG("fixed self-test");
            _ConMan_AllocateBuffer(_selfTestBuffer);
            ConMan_SetSensorConfig(CONFIG_SIZE_SELFTEST- LENGTH_BYTE_OFFSET, &_selfTestConfig[LENGTH_BYTE_OFFSET]);
            _dataModeActive = true;
            break;

		default:
			DEBUG("unhandled fixed config");
			break;
	}
}

/***************************************************************************//**
	Stops the enabled sensors from collecting data.
*//****************************************************************************/
void ConMan_StopData()
{
	if (_dataModeActive == false)
	{
		DEBUG("not re-stopping");
		return;
	}

	switch(_currentConfig)
	{
		case FixedConfigList_GYRO:
			ConMan_SetSensorConfig(DISABLE_SIZE_GYRO, &_gyroDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ACCEL:
			ConMan_SetSensorConfig(DISABLE_SIZE_ACCEL, &_accelDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		/* Mats: new configuration to read A7, A8, A9 at 800 Hz */
		case FixedConfigList_ANALOG_HI3:
			ConMan_SetSensorConfig(DISABLE_SIZE_ANALOG_HI3, &_analog_hi3Disable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		/* Mats: new configuration to read A7, A8 at 2400 Hz */
		case FixedConfigList_ANALOG_HI2:
			ConMan_SetSensorConfig(DISABLE_SIZE_ANALOG_HI2, &_analog_hi2Disable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		/* Mats: new configuration to read A7 at 4800 Hz */
		case FixedConfigList_ANALOG_HI1:
			ConMan_SetSensorConfig(DISABLE_SIZE_ANALOG_HI1, &_analog_hi1Disable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_IMU:
			ConMan_SetSensorConfig(DISABLE_SIZE_IMU, &_imuDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_MINIMOTION:
			ConMan_SetSensorConfig(DISABLE_SIZE_MINIMOTION, &_miniMotionDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_PENDULUM:
			ConMan_SetSensorConfig(DISABLE_SIZE_PENDULUM, &_pendulumDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_AMBIENT:
			ConMan_SetSensorConfig(DISABLE_SIZE_AMBIENT, &_ambientDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ECG:
			ConMan_SetSensorConfig(DISABLE_SIZE_ECG, &_ecgDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_HEADER:
			ConMan_SetSensorConfig(DISABLE_SIZE_HEADER, &_headerDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

        case FixedConfigList_SLOW_MIC:
            ConMan_SetSensorConfig(DISABLE_SIZE_SLOW_MICROPHONE, &_slowMicrophoneDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_ANLG_HEAD:
            ConMan_SetSensorConfig(DISABLE_SIZE_ANLG_HEAD, &_anlgHeadDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_ECG6:
            ConMan_SetSensorConfig(DISABLE_SIZE_ECG6, &_ecg6Disable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_MAGNETIC:
            ConMan_SetSensorConfig(DISABLE_SIZE_MAGNETIC, &_magneticDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;


        case FixedConfigList_HEADER_3V3:
            ConMan_SetSensorConfig(DISABLE_SIZE_HEADER_3V3, &_header3V3Disable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;

		case FixedConfigList_GYRO_HI:
			ConMan_SetSensorConfig(DISABLE_SIZE_GYRO_HI, &_gyroHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ACCEL_HI:
			ConMan_SetSensorConfig(DISABLE_SIZE_ACCEL_HI, &_accelHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		/* Mats: Stopping acquisition with HI8G looks the same as it does for HI */
		case FixedConfigList_ACCEL_HI8G:
			ConMan_SetSensorConfig(DISABLE_SIZE_ACCEL_HI, &_accelHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_IMU_HI:
			ConMan_SetSensorConfig(DISABLE_SIZE_IMU_HI, &_imuHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_MOTION:
			ConMan_SetSensorConfig(DISABLE_SIZE_MOTION, &_motionDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_SPORTS:
			ConMan_SetSensorConfig(DISABLE_SIZE_SPORTS, &_sportsDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_PENDULUM_HI:
			ConMan_SetSensorConfig(DISABLE_SIZE_PENDULUM_HI, &_pendulumHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_KITCHENSINK:
			ConMan_SetSensorConfig(DISABLE_SIZE_KITCHEN_SINK, &_kitchenSinkDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_MICROPHONE:
			ConMan_SetSensorConfig(DISABLE_SIZE_MICROPHONE, &_microphoneDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ALS_HI:
			ConMan_SetSensorConfig(DISABLE_SIZE_ALS_HI, &_alsHiDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ALS_ACC:
			ConMan_SetSensorConfig(DISABLE_SIZE_ALS_ACC, &_alsAccDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_FORCE_ACC:
			ConMan_SetSensorConfig(DISABLE_SIZE_FORCE_ACC, &_forceAccDisable[LENGTH_BYTE_OFFSET]);
			_dataModeActive = false;
			break;

		case FixedConfigList_ALS_MIC:
		    ConMan_SetSensorConfig(DISABLE_SIZE_ALS_MIC, &_alsMicDisable[LENGTH_BYTE_OFFSET]);
		    _dataModeActive = false;
		    break;

        case FixedConfigList_HI_ECG:
            ConMan_SetSensorConfig(DISABLE_SIZE_HI_ECG, &_hiEcgDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_HI_GAIN:
            ConMan_SetSensorConfig(DISABLE_SIZE_HI_GAIN, &_hiGainDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_HI_FORCE:
            ConMan_SetSensorConfig(DISABLE_SIZE_HI_FORCE, &_hiForceDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_HI_ANLG_HED:
            ConMan_SetSensorConfig(DISABLE_SIZE_HI_ANLG_HEAD, &_hiAnlgHeadDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_HI_ECG6:
            ConMan_SetSensorConfig(DISABLE_SIZE_HI_ECG6, &_hiEcg6Disable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

        case FixedConfigList_SELFTEST:
            ConMan_SetSensorConfig(DISABLE_SIZE_SELFTEST, &_selfTestDisable[LENGTH_BYTE_OFFSET]);
            _dataModeActive = false;
            break;

		default:
			DEBUG("invalid fix conf");
			break;
	}

	DataBuffer_ClearSubarrays();
}

/***************************************************************************//**
	Takes the buffer configuration information and allocates space in the buffer
	for it.

	\param[in] bufferConfiguration[] - array of configuration data that whose first
	                                   byte contains the number of bytes in the array
	                                   and the second byte is the number of sensor
	                                   configurations to follow.
*//****************************************************************************/
void _ConMan_AllocateBuffer(const uint8_t bufferConfiguration[])
{
	uint16_t index;

	ASSERT(bufferConfiguration != NULL, ErrorLedMode_SOFTWARE);

	/* the first byte is the number of bytes in the array, and the second byte is the
	   number of sensors to allocate space for.
	*/
	for (index = 2u; (index + 1u) <= bufferConfiguration[0u]; index += 2u)
	{
		DataBuffer_CreateSubarray(bufferConfiguration[index], (uint16_t)bufferConfiguration[index + 1u]);
	}

}

