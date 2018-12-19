/***************************************************************************//**
    \file Barometer.c
    \brief Driver for the barometer sensor

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module interfaces to a SPI barometer sensor to read the
                   barometric pressure and temperature data. This driver is
                   designed to work with a Freescale MPL115A1 SPI Barometer.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Barometer.c$
    $Revision: 4$
    $Date: Tuesday, August 28, 2012 5:15:46 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Barometer.h"
#include <Clock.h>
#include <Gpio.h>
#include <Device.h>
#include <Board.h>
#include <SpiMaster.h>

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the valid register addresses for the barometer registers
    \note The values in the barometer are shifted left 1 bit from the datasheet
          definition (<<1).
 */
typedef enum BarometerReg
{
	BarometerReg_PADC_MSB = 0x00<<1, /// pressure ADC MSB
	BarometerReg_PADC_LSB = 0x01<<1, /// pressure ADC LSB
	BarometerReg_TADC_MSB = 0x02<<1, /// temperature ADC MSB
	BarometerReg_TADC_LSB = 0x03<<1, /// temperature ADC LSB
	BarometerReg_A0_MSB   = 0x04<<1, /// pressure offset coefficient MSB
	BarometerReg_A0_LSB   = 0x05<<1, /// pressure offset coefficient LSB
	BarometerReg_B1_MSB   = 0x06<<1, /// pressure sensitivity coefficient MSB
	BarometerReg_B1_LSB   = 0x07<<1, /// pressure sensitivity coefficient LSB
	BarometerReg_B2_MSB   = 0x08<<1, /// temperature offset coefficient MSB
	BarometerReg_B2_LSB   = 0x09<<1, /// temperature offset coefficient LSB
	BarometerReg_C12_MSB  = 0x0A<<1, /// temperature sensitivity coefficient MSB
	BarometerReg_C12_LSB  = 0x0B<<1, /// temperature sensitivity coefficient LSB
	/* 0x0C - 0x11 are reserved */
	BarometerReg_CONVERT  = 0x12<<1  /// start temperature and pressure conversion
} BarometerReg_t;

/** defines how long the Barometer conversion takes in microseconds */
#define CONVERSION_US 5000u

/** Defines the chip select enable and disable for the barometer */
#define BAROMETER_START_SPI_XFER  GPIO_CLEAR(CS_BAROMETER_PORT, CS_BAROMETER_PIN);
#define BAROMETER_END_SPI_XFER    GPIO_SET(CS_BAROMETER_PORT, CS_BAROMETER_PIN);

/** Defines the value of a "Blank Byte" to write to the barometer */
#define BLANK_BYTE 0u

/** Defines the number of registers to read a sample */
#define BYTES_PER_SAMPLE 4u

/** Defines the number of registers to read the coefficients */
#define NUM_COEFF_BYTES 8u

/** Defines the number of microseconds to delay between de/asserting the chip select
    line and ending/starting the SPI communication to allow the chip to become ready
 */
#define SPI_CS_DELAY_US 1u

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Define a variable that will track if the barometer conversion is in progress */
static volatile bool _conversionInProgress = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
uint8_t _BarReadRegister(BarometerReg_t regAddress);
void _BarWriteRegister(BarometerReg_t regAddress,
                       uint8_t regValue);
void _BarReadMultipleRegisters(BarometerReg_t regStartAddress,
                               uint8_t numberOfRegisters,
                               uint8_t values[]);

void Barometer_Init(void)
{
	/* do nothing */
}
void Barometer_Config(const BarometerConfig_t* config);
void Barometer_Start(void);
void Barometer_Stop(void);

/***************************************************************************//**
    Starts a sample conversion on the Barometer and blocks until it is complete
    and returns the data from that conversion.

    \param[in] sample - the location to store the result of the sample conversion

*//****************************************************************************/
void Barometer_TriggerSampleBlocking(BarometerSample_t* sample)
{
	volatile bool sampleReadingSuccessful = false;

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	/* start conversion */
	Barometer_StartSampleConversion();

	/* Delay for conversion to complete */
	CLOCK_DELAY_US(CONVERSION_US);

	/* read conversion data */
	sampleReadingSuccessful = Barometer_ReadSample(sample);

	/* the sample reading should be successful because we started it and blocked
	   so there is a problem if the sample reading fails.
	*/
	ASSERT(sampleReadingSuccessful, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Starts a barometer conversion on the sensor.
*//****************************************************************************/
void Barometer_StartSampleConversion(void)
{
	_BarWriteRegister(BarometerReg_CONVERT, BLANK_BYTE);
	_conversionInProgress = true;
}

/***************************************************************************//**
    Reads in the value of the conversion from the barometer

    \param[in] sample- the location to store the sample data.

    \return True if a sample conversion was already in progress and the data
            could be read, false otherwise. True does NOT mean the sample was
            ready, just that the conversion was started before the reading.

*//****************************************************************************/
bool Barometer_ReadSample(BarometerSample_t* sample)
{
	volatile bool sampleSuccess = false;
	uint8_t rawRegisters[BYTES_PER_SAMPLE] = {0u};

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	if (_conversionInProgress)
	{
		/* clear the conversion flag for this sample */
		_conversionInProgress = false;

		/* read the sample data from the barometer */
		_BarReadMultipleRegisters(BarometerReg_PADC_MSB, BYTES_PER_SAMPLE, rawRegisters);

		/* stick the raw samples in the barometer sample */
		sample->pressure = (uint16_t)((((uint16_t)rawRegisters[0u])<<8u) + rawRegisters[1u]);
		sample->temperature = (uint16_t)((((uint16_t)rawRegisters[2u])<<8u) + rawRegisters[3u]);

		sampleSuccess = true;
	}

	return sampleSuccess;
}

/***************************************************************************//**
    Reads in the value of the conversion from the barometer

    \param[in]  maxLength - the number of bytes allocated for data[]
    \param[out] actualLength - the actual number of bytes that were put in data[]
    \param[out] data[] - the pressure and temperature data from the barometer

    \return True if a sample conversion was already in progress and the data
            could be read, false otherwise. True does NOT mean the sample was
            ready, just that the conversion was started before the reading.

*//****************************************************************************/
bool Barometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[])
{
	ASSERT(data != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(maxLength >= BYTES_PER_SAMPLE, ErrorLedMode_SOFTWARE);

	_BarReadMultipleRegisters(BarometerReg_PADC_MSB, BYTES_PER_SAMPLE, data);
	*actualLength = BYTES_PER_SAMPLE;

	return true;

}

/***************************************************************************//**
    Reads in the value of the conversion coefficients from the barometer

    \param[in] coefficients - the location to store the coefficients

*//****************************************************************************/
void Barometer_ReadCoefficients(BarometerCoef_t* coefficients)
{
	uint8_t rawRegisters[NUM_COEFF_BYTES] = {0u};

	ASSERT(coefficients != NULL, ErrorLedMode_SOFTWARE);

	/* read coefficient data */
	_BarReadMultipleRegisters(BarometerReg_A0_MSB, NUM_COEFF_BYTES, rawRegisters);

	/* stick the raw samples in the barometer sample */
	coefficients->a0  = (uint16_t)((((uint16_t)rawRegisters[0u])<<8u) + rawRegisters[1u]);
	coefficients->b1  = (uint16_t)((((uint16_t)rawRegisters[2u])<<8u) + rawRegisters[3u]);
	coefficients->b2  = (uint16_t)((((uint16_t)rawRegisters[4u])<<8u) + rawRegisters[5u]);
	coefficients->c12 = (uint16_t)((((uint16_t)rawRegisters[6u])<<8u) + rawRegisters[7u]);
}


/***************************************************************************//**
    Reads in the conversion coefficients from the barometer

    \param[in]  maxLength - the number of bytes allocated for coefficients[]
    \param[out] actualLength - the actual number of bytes that were put in coefficients[]
    \param[out] coefficients[] - the coefficient data from the barometer

*//****************************************************************************/
void Barometer_ReadCoeffArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   coefficients[])
{
	ASSERT((coefficients != NULL) && (actualLength != NULL), ErrorLedMode_SOFTWARE);
	ASSERT(maxLength >= NUM_COEFF_BYTES, ErrorLedMode_SOFTWARE);

	/* read coefficient data */
	_BarReadMultipleRegisters(BarometerReg_A0_MSB, NUM_COEFF_BYTES, coefficients);
	*actualLength = NUM_COEFF_BYTES;
}

/***************************************************************************//**
    Reads from a specific register using the SPI port.

    \param[in] regAddress - the register address to read from the device

    \return The value read from the register.
*//****************************************************************************/
uint8_t _BarReadRegister(BarometerReg_t regAddress)
{
	uint8_t regValue = 0u;

	BAROMETER_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	Spi_ReadRegister(regAddress, &regValue);
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	BAROMETER_END_SPI_XFER;
	return regValue;
}

/***************************************************************************//**
    Writes a value to a specific register using the SPI port.

    \param[in] regAddress - the register address to write at on device
    \param[in] regValue - the value at regAddress

*//****************************************************************************/
void _BarWriteRegister(BarometerReg_t regAddress,
                       uint8_t regValue)
{
	BAROMETER_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	Spi_WriteRegister(regAddress, regValue);
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	BAROMETER_END_SPI_XFER;
}

/***************************************************************************//**
    Reads from consecutive registers using the SPI port.

    \param[in] regStartAddress - the register address to read from the device
    \param[in] numberOfRegisters - the number of consecutive registers to read
    \param[in] values[] - the location to store the register values

    \note The length of values >= numberOfRegisters
*//****************************************************************************/
void _BarReadMultipleRegisters(BarometerReg_t regStartAddress,
                               uint8_t numberOfRegisters,
                               uint8_t values[])
{
	volatile uint8_t regNum;

	ASSERT((values != NULL) && (numberOfRegisters >0), ErrorLedMode_SOFTWARE);

	BAROMETER_START_SPI_XFER;
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	for(regNum = 0; regNum < numberOfRegisters; regNum++)
	{
		Spi_ReadRegister((regStartAddress + (regNum<<1)), &values[regNum]);
	}
	/* the barometer needs an extra byte to reset the state machine, see
	   the device datasheet */
	Spi_WriteByte(BLANK_BYTE);
	CLOCK_DELAY_US(SPI_CS_DELAY_US);
	BAROMETER_END_SPI_XFER;
}
