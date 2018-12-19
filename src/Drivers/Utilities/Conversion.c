/***************************************************************************//**
    \file Conversion.c
    \brief Conversion module for performing generic conversion operations

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The conversion module contains functions and macros that will
                   perform generic conversion such as string to integer type
                   conversions.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Utilities/Conversion.c$
    $Revision: 2$
    $Date: Tuesday, June 05, 2012 6:06:20 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Conversion.h"

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** Defines the maximum 12-bit unsigned number */
#define UINT_12_MAX  (0x0FFF)

/** Defines the maximum 4-bit unsigned number */
#define UINT_4_MAX   (0x0F)

/** Defines the maximum 12-bit signed number */
#define INT_12_MAX   (0x07FF)

/** Defines the maximum 4-bit signed number */
#define INT_4_MAX    (0x07)

/** Defines the minimum 12-bit signed number */
#define INT_12_MIN   ((-0x07FF) - 1)

/** Defines the minimum 4-bit signed number */
#define INT_4_MIN    ((-0x07) - 1)
/*-----------------------LOCAL VARIABLES-------------------------------------*/

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
ConversionStatus_t _CheckForOverflowUnsigned(NumberSize_t size,
                                             uint16_t number);
ConversionStatus_t _CheckForOverflowSigned(NumberSize_t size,
                                           int16_t number);

ConversionStatus_t Conversion_HexStringToUint( const char_t string[],
                                NumberSize_t size,
                                uint16_t* result)
{
	uint16_t index  = 0u;
	uint16_t number = 0u;
	uint8_t  digit;
	ConversionStatus_t status = ConversionStatus_FAILURE;

	ASSERT((string != NULL) && (result != NULL), ErrorLedMode_SOFTWARE);

	/* Skip spaces, hyphens, and underscores */
	digit = (uint8_t) string[index];
	while ((digit == ((uint8_t) ' ')) || (digit == ((uint8_t) '-')) ||
		   (digit == ((uint8_t) '_')))
	{
		++index;
		digit = (uint8_t) string[index];
	}

	/* Convert each digit in the number */
	digit = (uint8_t) string[index];
	while (((digit >= ((uint8_t) '0')) && (digit <= ((uint8_t) '9'))) ||
		   ((digit >= ((uint8_t) 'a')) && (digit <= ((uint8_t) 'f'))) ||
		   ((digit >= ((uint8_t) 'A')) && (digit <= ((uint8_t) 'F'))))
	{
		/* Finding at least one number character means the conversion
		   will succeed if the result is within range.
		*/
		status = ConversionStatus_SUCCESS;

		/* Check for possible overflow before multiplying */
		if (number >= (UINT16_T_MAX / 16u))
		{
			status = ConversionStatus_FAILURE;
			break;
		}
		number *= 16u;

		if ((digit >= ((uint8_t) '0')) && (digit <= ((uint8_t) '9')))
		{
			digit -= ((uint8_t) '0');
		}
		else if ((digit >= ((uint8_t) 'a')) && (digit <= ((uint8_t) 'f')))
		{
			digit -= ((uint8_t) 'a');
			digit += 10u;
		}
		else if ((digit >= ((uint8_t) 'A')) && (digit <= ((uint8_t) 'F')))
		{
			digit -= ((uint8_t) 'A');
			digit += 10u;
		}
		else
		{
			/* do nothing */
		}
		number += digit;

		++index;
		digit = (uint8_t) string[index];
	}

	*result = number;

	/* verify that the number is in range and did not overflow */
	if (status == ConversionStatus_SUCCESS)
	{
		status = _CheckForOverflowUnsigned(size, number);
	}

	return status;
}

/***************************************************************************//**
    Swaps the bytes of the input variable.

    \param[in] number - The number to swap the bytes in

    \return The byte swapped version of number
*//****************************************************************************/
uint16_t Conversion_SwapBytes(uint16_t number)
{
	uint16_t swappedLowByte = (uint16_t)(number >> 8u);
	uint16_t swappedHighByte = (uint16_t)(number << 8u);
	number = (uint16_t)(swappedHighByte | swappedLowByte);
	return number;
}

/***************************************************************************//**
    Checks to see if an unsigned number is out of range for its specified number
    of bits.

    \param[in] size - The enumeration for the max number of bits in the number
    \param[in] number - the number to check for overflow

    \return ConversionStatus_SUCCESS if the number is within limits
    \return ConversionStatus_FAILURE if the number is not within limits
*//****************************************************************************/
ConversionStatus_t _CheckForOverflowUnsigned(NumberSize_t size,
                                     uint16_t number)
{
	ConversionStatus_t status = ConversionStatus_SUCCESS;

	switch(size)
	{
		case NumberSize_4_BITS:
		{
			if (number > UINT_4_MAX)
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_8_BITS:
		{
			if (number > UINT8_T_MAX)
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_12_BITS:
		{
			if (number > UINT_12_MAX)
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_16_BITS:
			/* no handling necessary because number is UINT16 and can't
			   detect 16-bit overflow.
			*/
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	return status;
}

/***************************************************************************//**
    Checks to see if a signed number is out of range for its specified number of
    bits.

    \param[in] size - The enumeration for the max number of bits in the number
    \param[in] number - the number to check for overflow

    \return ConversionStatus_SUCCESS if the number is within limits
    \return ConversionStatus_FAILURE if the number is not within limits
*//****************************************************************************/
ConversionStatus_t _CheckForOverflowSigned(NumberSize_t size,
                                           int16_t number)
{
	ConversionStatus_t status = ConversionStatus_SUCCESS;

	switch(size)
	{
		case NumberSize_4_BITS:
		{
			if ((number > INT_4_MAX) || (number < INT_4_MIN))
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_8_BITS:
		{
			if ((number > INT8_T_MAX) || (number < INT8_T_MIN))
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_12_BITS:
		{
			if ((number > INT_12_MAX) || (number < INT_12_MIN))
			{
				status = ConversionStatus_FAILURE;
			}
			break;
		}

		case NumberSize_16_BITS:
			/* no handling necessary because number is INT16 and can't
			   detect 16-bit overflow.
			*/
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	return status;
}
