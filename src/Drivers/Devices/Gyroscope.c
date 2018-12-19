/***************************************************************************//**
    \file Gyroscope.c
    \brief Driver for the gyroscope sensor

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module interfaces to a SPI gyroscope sensor to read the
                   3-axis data. This driver is designed to work with a
                   ST Micro L3GD20 SPI Gyroscope.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Gyroscope.c$
    $Revision: 6$
    $Date: Thursday, April 11, 2013 12:04:08 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Gyroscope.h"
#include <Clock.h>
#include <Gpio.h>
#include <Device.h>
#include <Board.h>
#include <SpiMaster.h>

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the valid register addresses for the gyroscope registers */
typedef enum GyroscopeReg
{
	/* 0x00 - 0x0E are reserved */
	GyroscopeReg_WHO_AM_I 		= 0x0F,	/// Device identification register
	/* 0x10 - 0x1F are reserved */
	GyroscopeReg_CTRL_REG1		= 0x20, ///
	GyroscopeReg_CTRL_REG2		= 0x21, ///
	GyroscopeReg_CTRL_REG3		= 0x22, ///
	GyroscopeReg_CTRL_REG4		= 0x23, ///
	GyroscopeReg_CTRL_REG5		= 0x24, ///
	GyroscopeReg_REFERENCE 		= 0x25, /// Reference/DataCapture
	GyroscopeReg_OUT_TEMP  		= 0x26, /// Temperature data
	GyroscopeReg_STATUS_REG		= 0x27, ///
	GyroscopeReg_OUT_X_L		= 0x28, /// X-axis angular rate data, low address
	GyroscopeReg_OUT_X_H 		= 0x29, /// X-axis angular rate data, high address
	GyroscopeReg_OUT_Y_L 		= 0x2A, /// Y-axis angular rate data, low address
	GyroscopeReg_OUT_Y_H 		= 0x2B, /// Y-axis angular rate data, high address
	GyroscopeReg_OUT_Z_L 		= 0x2C, /// Z-axis angular rate data, low address
	GyroscopeReg_OUT_Z_H 		= 0x2D, /// Z-axis angular rate data, high address
	GyroscopeReg_FIFO_CTRL_REG	= 0x2E, ///
	GyroscopeReg_FIFO_SRC_REG	= 0x2F, ///
	GyroscopeReg_INT1_CFG		= 0x30, ///
	GyroscopeReg_INT1_SRC 		= 0x31, ///
	GyroscopeReg_INT1_TSH_XH	= 0x32, /// Interrupt threshold
	GyroscopeReg_INT1_TSH_XL	= 0x33, /// Interrupt threshold
	GyroscopeReg_INT1_TSH_YH	= 0x34, /// Interrupt threshold
	GyroscopeReg_INT1_TSH_YL	= 0x35, /// Interrupt threshold
	GyroscopeReg_INT1_TSH_ZH	= 0x36, /// Interrupt threshold
	GyroscopeReg_INT1_TSH_ZL	= 0x37, /// Interrupt threshold
	GyroscopeReg_INT1_DURATION	= 0x38 ///
} GyroscopeReg_t;

/** Defines the chip select enable and disable for the gyroscope */
#define GYROSCOPE_START_SPI_XFER  GPIO_CLEAR(CS_GYRO_PORT, CS_GYRO_PIN);
#define GYROSCOPE_END_SPI_XFER    GPIO_SET(CS_GYRO_PORT, CS_GYRO_PIN);

/** Defines the number of microseconds to delay between de/asserting the chip select
    line and ending/starting the SPI communication to allow the chip to become ready
 */
#define SPI_CS_DELAY_US 1u

/** Defines the number of registers to read a sample */
#define BYTES_PER_SAMPLE 6u

/** Define the expected value of the WHO_AM_I query */
#define WHO_AM_I ((uint8_t)0xD4)

/** Defines the bitmask for the control register one GyroscopeReg_CTRL_REG1 of the GyroscopeReg_t.

    DATA_RATE - 00: 95  Hz
                01: 190 Hz
                10: 380 Hz
                11: 760 Hz

                OD	BW	ODR(Hz)	Cut-Off
				00	00	95		12.5
				00	01	95  	25
				00	10	95		25
				00	11	95		25
				01	00	190		12_5
				01	01	190		25
				01	10	190		50
				01	11	190		70
				10	00	380		20
				10	01	380		25
				10	10	380		50
				10	11	380		100
				11	00	760		30
				11	01	760		35
				11	10	760		50
				11	11	760		100
*/
typedef enum CtrlRegOne
{
    CtrlRegOne_ODR1		= 0x80,
    CtrlRegOne_ODR0		= 0x40,
    CtrlRegOne_ODR		= CtrlRegOne_ODR1 | CtrlRegOne_ODR0, /// output data rate selection
    CtrlRegOne_BW1		= 0x20,
	CtrlRegOne_BW0		= 0x10,
	CtrlRegOne_BW		= CtrlRegOne_BW1 | CtrlRegOne_BW0, /// bandwidth selection
    CtrlRegOne_PD		= 0x08, /// power-down mode enable
    CtrlRegOne_Zen		= 0x04, /// Z axis enable
    CtrlRegOne_Yen		= 0x02, /// Y axis enable
    CtrlRegOne_Xen		= 0x01, /// X axis enable
    CtrlRegOne_ODR_760	= CtrlRegOne_ODR1 | CtrlRegOne_ODR0,
    CtrlRegOne_ODR_380	= CtrlRegOne_ODR1,
    CtrlRegOne_ODR_190	= CtrlRegOne_ODR0,
    CtrlRegOne_ODR_95	= 0x00
} CtrlRegOne_t;

/** Defines the bitmask for the control register GyroscopeReg_CTRL_REG2 of the GyroscopeReg_t.

	HPM[1:0] High-pass filter mode
	00	Normal mode (reset reading HP_RESET_FILTER)
	01	Reference signal for filtering
	10	Normal mode
	11	Autoreset on interrupt event

	HPCF3-0 ODR=95Hz ODR=190Hz ODR=380Hz ODR=760Hz
	0000	7.2	 	13.5 		27 		51.4
	0001	3.5	 	7.2 		13.5 	27
	0010	1.8	 	3.5 		7.2 	13.5
	0011	0.9		1.8 		3.5 	7.2
	0100	0.45	0.9 		1.8 	3.5
	0101	0.18	0.45 		0.9 	1.8
	0110	0.09	0.18 		0.45 	0.9
	0111	0.045	0.09 		0.18 	0.45
	1000	0.018	0.045 		0.09 	0.18
	1001	0.009	0.018 		0.045 	0.09

*/
typedef enum CtrlRegTwo
{
    CtrlRegTwo_HPM1		= 0x20,
    CtrlRegTwo_HPM0		= 0x10,
    CtrlRegTwo_HPM		= CtrlRegTwo_HPM1 | CtrlRegTwo_HPM0, /// high-pass filter mode selection
    CtrlRegTwo_HPCF3	= 0x08,
	CtrlRegTwo_HPCF2	= 0x04,
	CtrlRegTwo_HPCF1	= 0x02,
    CtrlRegTwo_HPCF0	= 0x01,
    CtrlRegTwo_HPCF		= CtrlRegTwo_HPCF3 | CtrlRegTwo_HPCF3 | CtrlRegTwo_HPCF1 | CtrlRegTwo_HPCF0 /// high-pass filter cutoff frequency selection
} CtrlRegTwo_t;

/** Defines the bitmask for the control register GyroscopeReg_CTRL_REG3 of the GyroscopeReg_t.

*/
typedef enum CtrlRegThree
{
    CtrlRegThree_I1_Int1	= 0x80,	/// interrupt enable on INT1 pin
    CtrlRegThree_I1_Boot	= 0x40,	/// boot status available on INT1
    CtrlRegThree_H_Lactive	= 0x20,	/// interrupt active configuration on INT1
    CtrlRegThree_PP_OD		= 0x10,	/// push-pull/ open drain
	CtrlRegThree_I2_DRDY	= 0x08,	/// data ready on DRDY/INT2
	CtrlRegThree_I2_WTM		= 0x04,	/// FIFO watermark interrupt on DRDY/INT2
    CtrlRegThree_I2_ORun	= 0x02,	/// FIFO overrun interrupt on DRDY/INT2
    CtrlRegThree_I2_Empty	= 0x01	/// FIFO empty interrupt on DRDY/INT2
} CtrlRegThree_t;


/** Defines the bitmask for the control register GyroscopeReg_CTRL_REG4 of the GyroscopeReg_t.

*/
typedef enum CtrlRegFour
{
    CtrlRegFour_BDU			= 0x80,	/// block data update
    CtrlRegFour_BLE			= 0x40,	/// big/little endian data selection
    CtrlRegFour_FS1			= 0x20,
    CtrlRegFour_FS0			= 0x10,
	CtrlRegFour_FS			= CtrlRegFour_FS1 | CtrlRegFour_FS0,	/// full scale selection
	CtrlRegFour_SIM			= 0x01,	/// SPI serial interface mode selection
	CtrlRegFour_RES_250		= 0x00,
	CtrlRegFour_RES_500		= CtrlRegFour_FS0,
	CtrlRegFour_RES_2000	= CtrlRegFour_FS1
} CtrlRegFour_t;

/** Defines the bitmask for the control register GyroscopeReg_CTRL_REG5 of the GyroscopeReg_t.

*/
typedef enum CtrlRegFive
{
    CtrlRegFive_BOOT		= 0x80,	/// reboot memory content
    CtrlRegFive_FIFO_EN		= 0x40,	/// FIFO enable
    CtrlRegFive_HPen		= 0x10,	/// high-pass filter enable
    CtrlRegFive_INT1_Sel1	= 0x08,
    CtrlRegFive_INT1_Sel0	= 0x04,
    CtrlRegFive_INT1_Sel	= CtrlRegFive_INT1_Sel1 | CtrlRegFive_INT1_Sel0,	/// INT1 selection configuration
	CtrlRegFive_Out_Sel1	= 0x02,
	CtrlRegFive_Out_Sel0	= 0x01,
	CtrlRegFive_Out_Sel		= CtrlRegFive_INT1_Sel1 | CtrlRegFive_INT1_Sel0	/// Out selection configuration
} CtrlRegFive_t;

/** Defines the bitmask for the control register STATUS_REG of the GyroscopeReg_t.

*/
typedef enum StatusReg
{
	StatusReg_ZYXOR	= 0x80,		/// X, Y, Z -axis data overrun
	StatusReg_ZOR	= 0x40,		/// Z axis data overrun
	StatusReg_YOR	= 0x20,		/// Y axis data overrun
	StatusReg_XOR	= 0x10,		/// X axis data overrun
	StatusReg_ZYXDA	= 0x08,		/// X, Y, Z -axis new data available
	StatusReg_ZDA	= 0x04,		/// Z axis new data available
	StatusReg_YDA	= 0x02,		/// Y axis new data available
	StatusReg_XDA	= 0x01		/// X axis new data available
} StatusReg_t;

/** Defines the bitmask for the control register FIFO_CTRL_REG of the GyroscopeReg_t.

	FM2 FM1 FM0 FIFO mode
	0 	0 	0 	Bypass mode
	0 	0 	1 	FIFO mode
	0 	1 	0 	Stream mode
	0 	1 	1 	Stream-to-FIFO mode
	1 	0 	0 	Bypass-to-Stream mode

*/
typedef enum FifoCtrlReg
{
    FifoCtrlReg_FM2		= 0x80,
    FifoCtrlReg_FM1		= 0x40,
    FifoCtrlReg_FM0		= 0x20,
    FifoCtrlReg_FM		= FifoCtrlReg_FM2 | FifoCtrlReg_FM1 | FifoCtrlReg_FM0,	/// FIFO mode selection
    FifoCtrlReg_WTM4	= 0x10,
    FifoCtrlReg_WTM3	= 0x08,
    FifoCtrlReg_WTM2	= 0x04,
	FifoCtrlReg_WTM1	= 0x02,
	FifoCtrlReg_WTM0	= 0x01,
	FifoCtrlReg_WTM		= FifoCtrlReg_WTM4 | FifoCtrlReg_WTM3 | FifoCtrlReg_WTM2 | FifoCtrlReg_WTM1 | FifoCtrlReg_WTM0	/// FIFO threshold. Watermark level setting
} FifoCtrlReg_t;

/** Defines the bitmask for the control register FIFO_SRC_REG of the GyroscopeReg_t.

*/
typedef enum FifoSrcReg
{
    FifoSrcReg_WTM		= 0x80,		/// Watermark status. (0: FIFO filling is lower than WTM level; 1: FIFO filling is equal or higher than WTM level)
    FifoSrcReg_OVRN		= 0x40,		/// Overrun bit status
    FifoSrcReg_EMPTY	= 0x20,		/// FIFO empty bit
    FifoSrcReg_FSS4		= 0x10,
    FifoSrcReg_FSS3		= 0x08,
    FifoSrcReg_FSS2		= 0x04,
    FifoSrcReg_FSS1		= 0x02,
    FifoSrcReg_FSS0		= 0x01,
    FifoSrcReg_FSS		= FifoSrcReg_FSS4 | FifoSrcReg_FSS3 | FifoSrcReg_FSS2 | FifoSrcReg_FSS1 | FifoSrcReg_FSS0	/// FIFO stored data level
} FifoSrcReg_t;

/** Defines the bitmask for the control register INT1_CFG of the GyroscopeReg_t.

*/
typedef enum Int1Cfg
{
    Int1Cfg_AND_OR	= 0x80,		/// AND/OR combination of interrupt events. (0: OR combination of interrupt events 1: AND combination of interrupt events)
    Int1Cfg_LIR		= 0x40,		/// Latch interrupt request. (0: interrupt request not latched; 1: interrupt request latched)
    Int1Cfg_ZHIE	= 0x20,		/// Enable interrupt generation on Z high event
    Int1Cfg_ZLIE	= 0x10,		///	Enable interrupt generation on Z low event
    Int1Cfg_YHIE	= 0x08,		/// Enable interrupt generation on Y high event.
    Int1Cfg_YLIE	= 0x04,		/// Enable interrupt generation on Y low event
    Int1Cfg_XHIE	= 0x02,		/// Enable interrupt generation on X high event
    Int1Cfg_XLIE	= 0x01		/// Enable interrupt generation on X low event
} Int1Cfg_t;

/** Defines the bitmask for the control register INT1_SRC of the GyroscopeReg_t.

*/
typedef enum Int1Src
{
    Int1Src_IA	= 0x40,		/// Interrupt active.
    Int1Src_ZH	= 0x20,		/// Z high
    Int1Src_ZL	= 0x10,		///	Z low
    Int1Src_YH	= 0x08,		/// Y high
    Int1Src_YL	= 0x04,		/// Y low
    Int1Src_XH	= 0x02,		/// X high
    Int1Src_XL	= 0x01		/// X low
} Int1Src_t;

/** Defines the bitmask for the control register INT1_DURATION of the GyroscopeReg_t.

*/
typedef enum Int1Duration
{
    Int1Duration_WAIT	= 0x80,	/// Wait enable
    Int1Duration_D6	= 0x40,
    Int1Duration_D5	= 0x20,
    Int1Duration_D4	= 0x10,
    Int1Duration_D3	= 0x08,
    Int1Duration_D2	= 0x04,
    Int1Duration_D1	= 0x02,
    Int1Duration_D0 = 0x01,
    Int1Duration_D	= Int1Duration_D6 | Int1Duration_D5 | Int1Duration_D4 | Int1Duration_D3 | Int1Duration_D2 | Int1Duration_D1 | Int1Duration_D0	/// Duration value
} Int1Duration_t;

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Tracks if the gyroscope has been initialized. */
static bool _initialized = false;

/** Stores the ISR pointer */
static GyroIsrPointer_t _gyroIsrPointer = NULL;

/** Store the values written to the control registers so they don't have to be
    read back.
 */
static volatile uint8_t _ctrlRegOne = 0u;
static volatile uint8_t _ctrlRegTwo = 0u;
static volatile uint8_t _ctrlRegThree = 0u;
static volatile uint8_t _ctrlRegFour = 0u;
static volatile uint8_t _ctrlRegFive = 0u;

/** Tracks if data is ready to be read from the gyroscope */
static volatile bool _dataReady = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
uint8_t _GyroReadSingleRegister(GyroscopeReg_t regAddress);
void _GyroWriteRegister(GyroscopeReg_t regAddress,
                       uint8_t regValue);
void _GyroReadMultipleRegisters(GyroscopeReg_t regStartAddress,
                               uint8_t numberOfRegisters,
                               uint8_t values[]);
static void _HandleDataReadyInterrupt(void);
void _HandleOtherInterrupt(void);

/***************************************************************************//**
    Initializes the gyroscope, but does not configure or start sampling
*//****************************************************************************/
void Gyroscope_Init(void)
{
	/* See if the gyroscope has already been initialized */
	ASSERT(_initialized == false, ErrorLedMode_SOFTWARE);

	/* check to make sure we have the expected gyroscope using the
	   WHO_AM_I id.
	 */
	volatile uint8_t receivedByte = 0u;
	receivedByte = _GyroReadSingleRegister(GyroscopeReg_WHO_AM_I);
	ASSERT(receivedByte == WHO_AM_I, ErrorLedMode_SOFTWARE);

	/* register the interrupt for the data ready pin */
	Gpio_RegisterIsr(GYRO_INTERRUPT_1_PORT, GYRO_INTERRUPT_1_PIN,
					 &_HandleDataReadyInterrupt, GpioEdge_RISING);
	Gpio_RegisterIsr(GYRO_INTERRUPT_0_PORT, GYRO_INTERRUPT_0_PIN,
					 &_HandleOtherInterrupt, GpioEdge_RISING);
	_initialized = true;

}

/***************************************************************************//**
    Reads in the value of the data from the gyroscope

    \param[in] sample- the location to store the sample data.

*//****************************************************************************/
void Gyroscope_Config(const GyroConfig_t* config)
{

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	/* setup the configuration registers */
	_ctrlRegOne = 0u;
	_ctrlRegFour = 0u;

	switch(config->odr)
	{
		case GyroOdr_760:
			_ctrlRegOne |= CtrlRegOne_ODR_760;
			break;

		case GyroOdr_380:
			_ctrlRegOne |= CtrlRegOne_ODR_380;
			break;

		case GyroOdr_190:
			_ctrlRegOne |= CtrlRegOne_ODR_190;
			break;

		case GyroOdr_95:
			_ctrlRegOne |= CtrlRegOne_ODR_95;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->resolution)
	{
		case GyroResolution_2000:
			_ctrlRegFour |= CtrlRegFour_RES_2000;
			break;

		case GyroResolution_500:
			_ctrlRegFour |= CtrlRegFour_RES_500;
			break;

		case GyroResolution_250:
			_ctrlRegFour |= CtrlRegFour_RES_250;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	/* enable the data ready interrupt */
	_ctrlRegThree |= CtrlRegThree_I2_DRDY;


	/* block data update when MSB and LSB read, Data MSB @ lower address */
	_ctrlRegFour |= (CtrlRegFour_BDU | CtrlRegFour_BLE);


	/* send the configuration settings */
	_GyroWriteRegister(GyroscopeReg_CTRL_REG1, _ctrlRegOne);

	_GyroWriteRegister(GyroscopeReg_CTRL_REG3, _ctrlRegThree);

	_GyroWriteRegister(GyroscopeReg_CTRL_REG4, _ctrlRegFour);

}

/***************************************************************************//**
	Starts the gyroscope sampling at its configured data rate.

*//****************************************************************************/
void Gyroscope_Start(void)
{

    static uint8_t dummyData[BYTES_PER_SAMPLE];

    /* make sure the accelerometer is initialized */
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

    /* make sure the gyro is disabled first */
    _GyroWriteRegister(GyroscopeReg_CTRL_REG1, 0x07);

    /* read the data to clear the data ready pin */
    _GyroReadMultipleRegisters(GyroscopeReg_OUT_X_L, BYTES_PER_SAMPLE, dummyData);

    /* set the interrupt to trigger when data is ready */
    GPIO_INT_ENABLE(GYRO_INTERRUPT_1_PORT, GYRO_INTERRUPT_1_PIN);

	/* set the gyroscope to active mode */
	_ctrlRegOne |= ( CtrlRegOne_PD | CtrlRegOne_Zen | CtrlRegOne_Yen | CtrlRegOne_Xen);
	_GyroWriteRegister(GyroscopeReg_CTRL_REG1, _ctrlRegOne);

}

/***************************************************************************//**
	Stops the data collecting of the gyroscope.

*//****************************************************************************/
void Gyroscope_Stop(void)
{
	/* clear the data ready interrupt trigger */
	GPIO_INT_DISABLE(GYRO_INTERRUPT_1_PORT, GYRO_INTERRUPT_1_PIN);

	/* put the gyroscope into power down mode */
	_ctrlRegOne &= ~CtrlRegOne_PD;

	/* write the settings to the gyroscope */
	_GyroWriteRegister(GyroscopeReg_CTRL_REG1, _ctrlRegOne);

}

/***************************************************************************//**
    Reads in the value of the data from the gyroscope

    \param[in] sample- the location to store the sample data.

*//****************************************************************************/
bool Gyroscope_ReadSample(GyroSample_t* sample)
{
	bool sampleObtained = false;
	uint8_t rawRegisters[BYTES_PER_SAMPLE] = {0u};

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	/* read the sample data from the gyroscope */
	_GyroReadMultipleRegisters(GyroscopeReg_OUT_X_L, BYTES_PER_SAMPLE, rawRegisters);

	/* stick the raw samples in the gyroscope sample */
	sample->xData = (uint16_t)((((uint16_t)rawRegisters[0u])<<8u) + rawRegisters[1u]);
	sample->yData = (uint16_t)((((uint16_t)rawRegisters[2u])<<8u) + rawRegisters[3u]);
	sample->zData = (uint16_t)((((uint16_t)rawRegisters[4u])<<8u) + rawRegisters[5u]);
	sampleObtained = true;
	return sampleObtained;
}

/***************************************************************************//**
    Reads a sample from the gyroscope if a sample was ready.

	\param[in]  maxLength - the maximum length of bytes data[] can hold
	\param[out] actualLength - the actual number of bytes written to data[]
	\param[in]  data - an array to hold the data from the accelerometer reading

*//****************************************************************************/
void Gyroscope_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[])
{
	ASSERT(data != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(maxLength >= BYTES_PER_SAMPLE, ErrorLedMode_SOFTWARE);

	if (_dataReady)
	{
		/* clear the data ready flag before reading the data because the chip
		   interrupt will be cleared when the first byte is read. If the data
		   ready flag is cleared after the first byte is read, it could be a new
		   data ready that is being cleared.
		 */
		_dataReady = false;

		_GyroReadMultipleRegisters(GyroscopeReg_OUT_X_L, BYTES_PER_SAMPLE, data);
		*actualLength = BYTES_PER_SAMPLE;
	}
	else
	{
		*actualLength = 0u;
	}
}

/***************************************************************************//**
    Reads a temperature sample from the gyroscope

    \returns Temperature in °C as a signed 8-bit integer number
*//****************************************************************************/
int8_t Gyroscope_ReadTemperature(void)
{
    int8_t temperature;
    temperature = (int8_t)_GyroReadSingleRegister(GyroscopeReg_OUT_TEMP);
    return temperature;
}

/***************************************************************************//**
    Sets a function that will be ran when the data ready interrupt occurs.

	\param[in] isr - pointer to the gyroscope data ready ISR

*//****************************************************************************/
void Gyroscope_RegisterIsr(GyroIsrPointer_t isr)
{
	ASSERT(_gyroIsrPointer == NULL, ErrorLedMode_SOFTWARE);

	_gyroIsrPointer = isr;
}

/***************************************************************************//**
    Clears the custom ISR for the gyroscope.
*//****************************************************************************/
void Gyroscope_UnregisterIsr(void)
{
	_gyroIsrPointer = NULL;
}

/***************************************************************************//**
    Handles the GPIO event that indicates that data is ready on the gyroscope.
*//****************************************************************************/
static void _HandleDataReadyInterrupt(void)
{
	_dataReady = true;
	/* call the accelerometer data ready interrupt handler */
	if (_gyroIsrPointer != NULL )
	{
		(*_gyroIsrPointer)();
	}

}

/***************************************************************************//**
    Handles the GPIO event that indicates that a special event on the
    gyroscope.
*//****************************************************************************/
void _HandleOtherInterrupt(void)
{
	// TODO: net - handle other interrupt.
}

/***************************************************************************//**
    Reads from a specific register using the SPI port.

    \param[in] regAddress - the register address to read from the device

    \return The value read from the register.
*//****************************************************************************/
uint8_t _GyroReadSingleRegister(GyroscopeReg_t regAddress)
{
	uint8_t regValue = 0u;

	GYROSCOPE_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	Spi_ReadRegister(regAddress, &regValue);
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	GYROSCOPE_END_SPI_XFER;
	return regValue;
}

/***************************************************************************//**
    Writes a value to a specific register using the SPI port.

    \param[in] regAddress - the register address to write at on device
    \param[in] regValue - the value at regAddress

*//****************************************************************************/
void _GyroWriteRegister(GyroscopeReg_t regAddress,
                       uint8_t regValue)
{
	GYROSCOPE_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	Spi_WriteRegister(regAddress, regValue);
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	GYROSCOPE_END_SPI_XFER;
}

/***************************************************************************//**
    Reads from consecutive registers using the SPI port.

    \param[in] regStartAddress - the register address to read from the device
    \param[in] numberOfRegisters - the number of consecutive registers to read
    \param[in] values[] - the location to store the register values

    \note The length of values >= numberOfRegisters
*//****************************************************************************/
void _GyroReadMultipleRegisters(GyroscopeReg_t regStartAddress,
                               uint8_t numberOfRegisters,
                               uint8_t values[])
{
	volatile uint8_t regNum;

	ASSERT((values != NULL) && (numberOfRegisters >0), ErrorLedMode_SOFTWARE);


	for(regNum = 0; regNum < numberOfRegisters; regNum++)
	{
		GYROSCOPE_START_SPI_XFER;
		CLOCK_DELAY_US(SPI_CS_DELAY_US);
		Spi_ReadRegister((regStartAddress + regNum), &values[regNum]);
		CLOCK_DELAY_US(SPI_CS_DELAY_US);
		GYROSCOPE_END_SPI_XFER;
		CLOCK_DELAY_US(SPI_CS_DELAY_US);
	}

}
