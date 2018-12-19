/***************************************************************************//**
    \file Magnetometer.c
    \brief Magnetometer sensor interface module

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides logic to interface with the magnetometer
       (3-axis Hall effect sensor). This logic assumes that the magnetometer in
       use is the Freescale MAG3110 I2C Digital Magnetometer.

       <a href="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MAG3110"
       >Link to Product Webpage</a>

       <a href="http://cache.freescale.com/files/sensors/doc/data_sheet/MAG3110.pdf?pspll=1"
       >Link to Datasheet</a>

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Magnetometer.c$
    $Revision: 6$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Magnetometer.h"

#include <Assert.h>
#include <Debug.h>
#include <Serial.h>
#include <I2c.h>
#include <Gpio.h>
#include <Board.h>
#include <Device.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the possible registers that can be read from or written to on the
    magnetometer.
 */
typedef enum MagReg
{
	MagReg_DR_STATUS = 0x00, /// Data Ready Status    (r  )
	MagReg_OUT_X_MSB = 0x01, /// Sensor data          (r  )
	MagReg_OUT_X_LSB = 0x02, ///                      (r  )
	MagReg_OUT_Y_MSB = 0x03, ///                      (r  )
	MagReg_OUT_Y_LSB = 0x04, ///                      (r  )
	MagReg_OUT_Z_MSB = 0x05, ///                      (r  )
	MagReg_OUT_Z_LSB = 0x06, ///                      (r  )
	MagReg_WHO_AM_I  = 0x07, /// chip identifier byte (r  )
	MagReg_SYSMOD    = 0x08, /// system mode          (r  )
	MagReg_OFF_X_MSB = 0x09, ///                      (r/w)
	MagReg_OFF_X_LSB = 0x0A, ///                      (r/w)
	MagReg_OFF_Y_MSB = 0x0B, ///                      (r/w)
	MagReg_OFF_Y_LSB = 0x0C, ///                      (r/w)
	MagReg_OFF_Z_MSB = 0x0D, ///                      (r/w)
	MagReg_OFF_Z_LSB = 0x0E, ///                      (r/w)
	MagReg_DIE_TEMP  = 0x0F, ///                      (r  )
	MagReg_CTRL_REG1 = 0x10, ///                      (r/w)
	MagReg_CTRL_REG2 = 0x11  ///                      (r/w)
} MagReg_t;

/** Defines the bitmask for the data ready status register */
enum DataReadyStatus
{
	DataReadyStatus_XDR   = 0x01,
	DataReadyStatus_YDR   = 0x02,
	DataReadyStatus_ZDR   = 0x04,
	DataReadyStatus_ZYXDR = 0x08,
	DataReadyStatus_XOW   = 0x10,
	DataReadyStatus_YOW   = 0x20,
	DataReadyStatus_ZOW   = 0x40,
	DataReadyStatus_ZYXOW = 0x80
};

/** Define the expected value of the WHO_AM_I query */
#define WHO_AM_I ((uint8_t)0xC4)

/** Defines the states of the system mode register */
enum SysMod
{
	SysMod_STANDBY            = 0x00,
	SysMod_ACTIVE_RAW_DATA    = 0x01,
	SysMod_ACTIVE_SCALED_DATA = 0x02
};

/** Defines the settings for control register one. The output data rate (ODR)
    is equal to the ADC_RATE/OSR. E.g. ADC_RATE_1280/OSR_16 = 80 Hz ODR
 */
enum CtrlRegOne
{
	ADC_RATE_1280       = 0x00, /// analog sample rate in Hz
	ADC_RATE_640        = 0x20,
	ADC_RATE_320        = 0x40,
	ADC_RATE_160        = 0x60,
	ADC_RATE_80         = 0x80,
	ADC_RATE_40         = 0xA0,
	ADC_RATE_20         = 0xC0,
	ADC_RATE_10         = 0xE0,
	OSR_16              = 0x00, /// over sampling ratio
	OSR_32              = 0x08,
	OSR_64              = 0x10,
	OSR_128             = 0x18,
	FAST_READ_ENABLE    = 0x04,
	TRIGGER_MEASUREMENT = 0x02,
	ACTIVE_MODE_ENABLE  = 0x01
};

enum CtrlRegTwo
{
	Mag_RST      = 0x10, /// resets the magnetic sensor after exposure to large magnetic field
	RAW_ENABLE   = 0x20, /// if enabled, sensor will output raw data not scaled by user offset
	AUTO_MRST_EN = 0x80  /// the magnetic sensor will be automatically reset before each measurement
};

/** defines the port the magnetometer is attached to */
#define MAG_PORT I2cPort_0

/** defines the magnetometer's I2C address */
#define MAG_ADDRESS ((uint8_t)0x0E)

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the magnetometer is initialized */
static bool _initialized = false;


/** Stores the ISR pointer */
static MagIsrPointer_t _magIsrPointer = NULL;

/** Stores the configuration parameters for the _initialized port */
static MagConfig_t _parameters = {MagSampleRate_1280, MagOverSampleRatio_16, MagDataScaling_RAW, MagAutoReset_ENABLED,
                                  MagSampling_Auto, MagResolution_16BIT};
/** Stores the values written to the control registers because the chip always
    reads back 0x00 when reading the register.
 */
static volatile uint8_t _controlRegOne = 0u;
static volatile uint8_t _controlRegTwo = 0u;

/** Tracks if data is ready to be read from the magnetometer */
static volatile bool _dataReady = false;
/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _HandleDataReadyInterrupt(void);
static uint8_t _ReadSingleRegister(MagReg_t reg);
static void _ReadMultipleRegisters(MagReg_t startReg,
                                   uint8_t numRegisters,
                                   uint8_t registers[]);
static I2cError_t _WriteSingleRegister(MagReg_t reg,
                                       uint8_t value);

/***************************************************************************//**
    Initializes the magnetometer, but does not configure or start the sampling.
*//****************************************************************************/
void Magnetometer_Init(void)
{
	/* See if the magnetometer has already been initialized */
	ASSERT(_initialized == false, ErrorLedMode_SOFTWARE);

	/* check to make sure we have the expected magnetometer using the
	   WHO_AM_I id.
	 */
	uint8_t receivedByte = 0;
	receivedByte = _ReadSingleRegister(MagReg_WHO_AM_I);
	ASSERT(receivedByte == WHO_AM_I, ErrorLedMode_SOFTWARE);

	/* now clear the configuration settings to the magnetometer with our
	   default settings
	*/
	_WriteSingleRegister(MagReg_CTRL_REG1, _controlRegOne);

	/* Register the interrupt for the data ready pin
	*/
	Gpio_RegisterIsr(MAGNETOMETER_INTERRUPT_PORT, MAGNETOMETER_INTERRUPT_PIN,
					 &_HandleDataReadyInterrupt, GpioEdge_RISING);

	_initialized = true;
}

/***************************************************************************//**
    Configures the magnetometer, but does not start the sampling.

    \param[in] config - Structure of selected magnetometer settings.

    \return True if the configuration was successful, false otherwise. The
            configuration will fail if the magnetometer is not in standby when
            configured.
*//****************************************************************************/
bool Magnetometer_Config(const MagConfig_t* config)
{
	bool success = false;
	uint8_t systemMode = 0;
	I2cError_t i2cResult = I2cError_OK;

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	/* store the initialization data */
	_parameters.sampleRate = config->sampleRate;
	_parameters.overSampleRatio = config->overSampleRatio;
	_parameters.dataScaling = config->dataScaling;
	_parameters.autoReset = config->autoReset;
	_parameters.samplingMode = config->samplingMode;
	_parameters.resolution = config->resolution;

	/* setup the configuration registers */
	_controlRegOne = 0u;
	_controlRegTwo = 0u;
	switch(config->sampleRate)
	{
		case MagSampleRate_1280:
			_controlRegOne |= ADC_RATE_1280;
			break;

		case MagSampleRate_640:
			_controlRegOne |= ADC_RATE_640;
			break;

		case MagSampleRate_320:
			_controlRegOne |= ADC_RATE_320;
			break;

		case MagSampleRate_160:
			_controlRegOne |= ADC_RATE_160;
			break;

		case MagSampleRate_80:
			_controlRegOne |= ADC_RATE_80;
			break;

		case MagSampleRate_40:
			_controlRegOne |= ADC_RATE_40;
			break;

		case MagSampleRate_20:
			_controlRegOne |= ADC_RATE_20;
			break;

		case MagSampleRate_10:
			_controlRegOne |= ADC_RATE_10;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->overSampleRatio)
	{
		case MagOverSampleRatio_128:
			_controlRegOne |= OSR_128;
			break;

		case MagOverSampleRatio_64:
			_controlRegOne |= OSR_64;
			break;

		case MagOverSampleRatio_32:
			_controlRegOne |= OSR_32;
			break;

		case MagOverSampleRatio_16:
			_controlRegOne |= OSR_16;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->dataScaling)
	{
		case MagDataScaling_RAW:
			_controlRegTwo |= RAW_ENABLE;
			break;

		case MagDataScaling_SCALED:
			/* register should already be 0 for this bit */
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->autoReset)
	{
		case MagAutoReset_ENABLED:
			_controlRegTwo |= AUTO_MRST_EN;
			break;

		case MagAutoReset_DISABLED:
			/* register should already be 0 for this bit */
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->samplingMode)
	{
		case MagSampling_Auto:
			/* don't set this bit until the magnetometer is started
			   because data will start being collected as soon as this
			   bit is set.
			 */
			break;

		case MagSampling_Triggered:
			/* register should already be 0 for this bit */
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->resolution)
	{
		case MagResolution_16BIT:
			/* register should already be 0 for this bit */
			break;

		case MagResolution_8BIT:
			_controlRegOne |= FAST_READ_ENABLE;
			// TODO: spg - this mode is untested. Test it sometime.
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	/* make sure the magnetometer is in idle mode, or the settings won't take */
	systemMode = _ReadSingleRegister(MagReg_SYSMOD);

	if (systemMode == SysMod_STANDBY)
	{
		/* now send the configuration settings to the magnetometer */
		/* control reg 1 */
		i2cResult = _WriteSingleRegister(MagReg_CTRL_REG1, _controlRegOne);

		/* only write the next register if the first was successful. */
		if (i2cResult == I2cError_OK)
		{
			/* control reg 2 */
			_WriteSingleRegister(MagReg_CTRL_REG2, _controlRegTwo);

			/* the config is only successful if all the registers were successful. */
			if (i2cResult == I2cError_OK)
			{
				success = true;
			}
		}
	}

	return success;
}

/***************************************************************************//**
    Starts the magnetometer sampling at its configured data rate.
*//****************************************************************************/
void Magnetometer_Start(void)
{
	I2cError_t i2cError = I2cError_OK;

	/* make sure the magnetometer is initialized */
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);
	ASSERT(_parameters.samplingMode == MagSampling_Auto, ErrorLedMode_SOFTWARE);

	/* make sure the accelerometer is in standby or the register settings
	   won't take.
	 */
	i2cError = _WriteSingleRegister(MagReg_CTRL_REG1, _controlRegOne);
	ASSERT(i2cError == I2cError_OK, ErrorLedMode_SOFTWARE);

	/* read X axis MSB to clear the data ready interrupt */
	(void) _ReadSingleRegister(MagReg_OUT_X_MSB);

	/* set the interrupt to trigger when data is ready */
	GPIO_INT_ENABLE(MAGNETOMETER_INTERRUPT_PORT, MAGNETOMETER_INTERRUPT_PIN);

	/* set the magnetometer to active */
	_controlRegOne |= ACTIVE_MODE_ENABLE;

	/* write the control settings back to the magnetometer */
	i2cError = _WriteSingleRegister(MagReg_CTRL_REG1, _controlRegOne);
	ASSERT(i2cError == I2cError_OK, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Stops the data collecting of the magnetometer.
*//****************************************************************************/
void Magnetometer_Stop(void)
{
	/* set the interrupt to trigger when data is ready */
	GPIO_INT_DISABLE(MAGNETOMETER_INTERRUPT_PORT, MAGNETOMETER_INTERRUPT_PIN);

	/* set the magnetometer to standby */
	_controlRegOne &= ~ACTIVE_MODE_ENABLE;

	/* write the control settings back to the magnetometer */
	_WriteSingleRegister(MagReg_CTRL_REG1, _controlRegOne);
}

/***************************************************************************//**
    Triggers a sample from the magnetometer and blocks until the sample
    is returned.

	\param[in] sample - structure to hold the sample data once it is collected

	\return True if the sample was aquired, false otherwise
*//****************************************************************************/
bool Magnetometer_TriggerSampleBlocking(MagSample_t* sample)
{
	#define BYTES_IN_SAMPLE 6u

	bool sampleSuccess = false;
	volatile uint8_t controlRegOne = 0;
	volatile uint8_t dataReady = 0;
	uint8_t sampleArray[BYTES_IN_SAMPLE];
	volatile uint8_t index;
	for(index = 0; index < BYTES_IN_SAMPLE; index++)
	{
		sampleArray[index] = 0;
	}

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	controlRegOne = _ReadSingleRegister(MagReg_CTRL_REG1);
	controlRegOne |= TRIGGER_MEASUREMENT;
	_WriteSingleRegister(MagReg_CTRL_REG1, controlRegOne);

	volatile uint16_t limitCounter = UINT16_T_MAX;
	while ((dataReady & DataReadyStatus_ZYXDR) == false)
	{
		dataReady = _ReadSingleRegister(MagReg_DR_STATUS);
		limitCounter--;
		ASSERT(limitCounter > 0, ErrorLedMode_SOFTWARE);
	}

	_ReadMultipleRegisters(MagReg_OUT_X_MSB, BYTES_IN_SAMPLE, sampleArray);

	sample->xData = (int16_t)((((uint16_t)sampleArray[0u])<<8u) + sampleArray[1u]);
	sample->yData = (int16_t)((((uint16_t)sampleArray[2u])<<8u) + sampleArray[3u]);
	sample->zData = (int16_t)((((uint16_t)sampleArray[4u])<<8u) + sampleArray[5u]);

	sampleSuccess = true;
	return sampleSuccess;
}

/***************************************************************************//**
    Resets the sensor in the magnetometer and blocks until the sensor is reset.
*//****************************************************************************/
void Magnetometer_ResetSensorBlocking(void)
{
	I2cError_t i2cStatus = I2cError_OK;
	uint8_t volatile controlRegTwoCopy = _controlRegTwo;
	uint16_t volatile timeoutCounter;

	/* set the sensor reset bit in control register two */
	controlRegTwoCopy |= Mag_RST;

	i2cStatus = _WriteSingleRegister(MagReg_CTRL_REG2, controlRegTwoCopy);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	timeoutCounter = 1000;
	while ((controlRegTwoCopy & Mag_RST) && (timeoutCounter > 0))
	{
		controlRegTwoCopy = _ReadSingleRegister(MagReg_CTRL_REG2);
		timeoutCounter--;
	}

	ASSERT(timeoutCounter > 0, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Writes the sensor calibration data to the magnetometer.

	\param[in] calibration - structure to hold the calibration data to be written
*//****************************************************************************/
void Magnetometer_SetCalibration(const MagSample_t* calibration)
{
	I2cError_t i2cStatus = I2cError_OK;
	volatile uint8_t calValue = 0;

	ASSERT(calibration != NULL, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->xData >> 8);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_X_MSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->xData);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_X_LSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->yData >> 8);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_Y_MSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->yData);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_Y_LSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->zData >> 8);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_Z_MSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);

	calValue = (uint8_t)(calibration->zData);
	i2cStatus = _WriteSingleRegister(MagReg_OFF_Z_LSB, calValue);
	ASSERT(i2cStatus == I2cError_OK, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Reads a sample from the magnetometer if a sample was ready.

	\param[in] sample - structure to hold the sample data once it is collected

	\return True if the sample was ready, false otherwise
*//****************************************************************************/
bool Magnetometer_ReadSample(MagSample_t* sample)
{
	bool sampleSuccess = false;
	uint8_t sampleArray[BYTES_IN_SAMPLE] = {0u, 0u, 0u, 0u, 0u, 0u};

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	if(_dataReady)
	{
		/* clear the data ready flag before reading the data because the chip
		   interrupt will be cleared when the first byte is read. If the data
		   ready flag is cleared after the first byte is read, it could be a new
		   data ready that is being cleared.
		 */
		_dataReady = false;

		_ReadMultipleRegisters(MagReg_OUT_X_MSB, BYTES_IN_SAMPLE, sampleArray);
		sample->xData = (int16_t)((((uint16_t)sampleArray[0u])<<8u) + sampleArray[1u]);
		sample->yData = (int16_t)((((uint16_t)sampleArray[2u])<<8u) + sampleArray[3u]);
		sample->zData = (int16_t)((((uint16_t)sampleArray[4u])<<8u) + sampleArray[5u]);
		sampleSuccess = true;
	}

	return sampleSuccess;
}

/***************************************************************************//**
    Reads a sample from the magnetometer if a sample was ready.

	\param[in]  maxLength - the maximum number of items that can be written to data[]
	\param[out] actuaLength - the actual number of items written to data[]
	\param[out] data - the array that the data will be stored in.

*//****************************************************************************/
void Magnetometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[])
{
	ASSERT(data != NULL, ErrorLedMode_SOFTWARE);

	if(_dataReady)
	{
		/* clear the data ready flag before reading the data because the chip
		   interrupt will be cleared when the first byte is read. If the data
		   ready flag is cleared after the first byte is read, it could be a new
		   data ready that is being cleared.
		 */
		_dataReady = false;

		_ReadMultipleRegisters(MagReg_OUT_X_MSB, BYTES_IN_SAMPLE, data);
		*actualLength = BYTES_IN_SAMPLE;
	}
	else
	{
		*actualLength = 0u;
	}
}
/***************************************************************************//**
    Reads the die temperature of the device.

    \returns Temperature in °C as a signed 8-bit integer number

*//****************************************************************************/
int8_t Magnetometer_ReadTemp(void)
{
    int8_t temperature;
    temperature = (int8_t)_ReadSingleRegister(MagReg_DIE_TEMP);
    return temperature;
}

/***************************************************************************//**
    Sets a function that will be ran when the data ready interrupt occurs.

	\param[in] isr - pointer to the accelerometer data ready ISR

*//****************************************************************************/
void Magnetometer_RegisterIsr(MagIsrPointer_t isr)
{
	ASSERT(_magIsrPointer == NULL, ErrorLedMode_SOFTWARE);
	_magIsrPointer = isr;
}

/***************************************************************************//**
    Clears the custom ISR for the magnetometer.
*//****************************************************************************/
void Magnetomter_UnregisterIsr(void)
{
	_magIsrPointer = NULL;
}

/***************************************************************************//**
    Handles the GPIO event that indicates that data is ready on the magnetometer.
*//****************************************************************************/
static void _HandleDataReadyInterrupt(void)
{
	_dataReady = true;

	/* call the magnetometer data ready interrupt handler */
	if (_magIsrPointer != NULL)
	{
		(*_magIsrPointer)();
	}
}

/***************************************************************************//**
    Reads a single register from the magnetometer.

	\param[in] reg- the register to read

	\return The value read from the magnetometer register
*//****************************************************************************/
static uint8_t _ReadSingleRegister(MagReg_t reg)
{
	uint8_t registerValue = 0;
	I2cError_t i2cSuccess = I2cError_OK;

	/* read in the register */
	I2c_WaitUntilBusIdle(MAG_PORT);
	i2cSuccess = I2c_TransmitStart(MAG_PORT, MAG_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(MAG_PORT, reg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitStart(MAG_PORT, MAG_ADDRESS, I2cOperation_READ);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	I2c_ReceiveByteEnd(MAG_PORT, &registerValue);
	return registerValue;
}

/***************************************************************************//**
    Reads a multiple consecutive registers from the magnetometer.

	\param[in] startReg - the first register to read of the consecutive registers
	\param[in] numRegisters - The number of consecutive registers to read
	\param[in] registers[] - the array in which the register values will be stored.

	\note Make sure that registers[] has at least numRegisters elements to hold
	      the register data.
*//****************************************************************************/
static void _ReadMultipleRegisters(MagReg_t startReg,
                                   uint8_t numRegisters,
                                   uint8_t registers[])
{
	I2cError_t i2cSuccess = I2cError_OK;
	volatile int16_t regNum;

	ASSERT((registers != NULL) && (numRegisters > 0u), ErrorLedMode_SOFTWARE);

	I2c_WaitUntilBusIdle(MAG_PORT);
	i2cSuccess = I2c_TransmitStart(MAG_PORT, MAG_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(MAG_PORT, startReg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitStart(MAG_PORT, MAG_ADDRESS, I2cOperation_READ);

	for(regNum = 0; regNum < numRegisters; regNum++)
	{
		if (regNum < numRegisters - 1)
		{
			I2c_ReceiveByte(MAG_PORT, &registers[regNum]);
		}
		else
		{
			I2c_ReceiveByteEnd(MAG_PORT, &registers[regNum]);
		}
	}
}

/***************************************************************************//**
    Writes a single register to the magnetometer.

	\param[in] reg - the register to write to
	\param[in] value - The value to write to the register

	\return I2cError_OK if the register could be written successfully. Otherwise
	        return an I2cError.
*//****************************************************************************/
static I2cError_t _WriteSingleRegister(MagReg_t reg,
                                       uint8_t value)
{
	I2cError_t i2cSuccess = I2cError_OK;

	/* write the magnetometer register */
	I2c_WaitUntilBusIdle(MAG_PORT);
	i2cSuccess = I2c_TransmitStart(MAG_PORT, MAG_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(MAG_PORT, reg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByteEnd(MAG_PORT, value);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	return i2cSuccess;
}
