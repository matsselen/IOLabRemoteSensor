/***************************************************************************//**
    \file DataBuffer.c
    \brief Data buffer is the memory space reserved to store sensor data

    \b Project 1814 IOLab Remote Sensor Application

    \b Description The data buffer contains memory space to store sensor data and
       can be written to and read from. The buffer contains locking mechanisms to
       ensure that data is not compromised and is valid.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/DataBuffer.c$
    $Revision: 11$
    $Date: Friday, October 24, 2014 10:53:10 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include "DataBuffer.h"

#include <Assert.h>

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------LOCAL DEFINITIONS------------------------------------*/
/** The number of bytes offset from the start of the subarray to the start of data */
#define SUBARRAY_DATA_BYTE_OFFSET     ( 2u )

/** The subarray length byte offset is how many places to look after the subarray
    ID to find the length byte of that subarray.
*/
#define SUBARRAY_LENGTH_BYTE_OFFSET   ( 1u )
/** The subarray ID byte offset is how many places to look after the subarray
    starting index.
*/
#define SUBARRAY_ID_BYTE_OFFSET       ( 0u )

/** The number of subarrays byte offset is how far into the array the number of
    subarrays are stored.
 */
#define NUM_SUBARRAY_OFFSET           ( 1u )

/** The number of useful bytes offset is how many bytes of the array can store useful
    data based on the subarray configuration. The number of useful bytes byte itself
    is not included in the count of useful bytes.
*/
#define NUM_USEFUL_BYTES_OFFSET       ( 0u )

/** The number of bytes of overhead each subarray generates */
#define SUBARRAY_BYTES_OVERHEAD       ( 2u )

/** The number of bytes of the array that are used for overhead */
#define ARRAY_OVERHEAD                ( 2u )

/** The value of an invalid subarray Index */
#define INVALID_INDEX                 ( -1 )

/** Defines the overflow bit as msb of Byte */
#define OVERFLOW_BIT                  ((uint8_t)(1<<7u ))

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/
/** Stores the pointer to one of the two data arrays to store data into. */
static uint8_t* _array0     = NULL;
/** Stores the pointer to one of the two data arrays to store data into. */
static uint8_t* _array1     = NULL;
/** Keeps the pointer to the array that is actively getting data written to it */
static uint8_t* _writeArray = NULL;

/** Stores the size of the data arrays */
static uint16_t _arraySize  = 0u;

/** Stores the size of the allocated bytes of the arrays */
static uint16_t _allocatedBytesSize = 0u;

/** Stores the starting index of each subarray within the larger array */
static uint16_t _subArrayStartIndex[MAX_NUM_SUB_ARRAYS] = { 0u };
/** Stores the length of each subarray */
static uint16_t _subArrayEndIndex[MAX_NUM_SUB_ARRAYS] = { 0u };
/** Stores the number of active subarrays that are in use */
static uint16_t _numActiveSubs = 0u;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _InitializeData(uint8_t array[]);
static int16_t _IndexOfSubarrayId(uint8_t subarrayId);

/***************************************************************************//**
    Initialized the data buffer and points this module to the memory to use as
    the data buffers.

    \param[in] array0 - the first array to be used to pingpong between.
    \param[in] array1 - the second array to be used to pingpong between.
    \param[in] arraySize - the size of the array0 and array1 in bytes.

    \note array0 and array1 must be the same size.

*//****************************************************************************/
void DataBuffer_Init(
	uint8_t array0[],
	uint8_t array1[],
	uint16_t arraySize)
{
	ASSERT(arraySize >= MIN_BUFFER_SIZE, ErrorLedMode_SOFTWARE);

	_array0 = &array0[0];
	_array1 = &array1[0];
	_arraySize = arraySize;

	_writeArray = _array0;
}

/***************************************************************************//**
    Sets the memory to the values in data starting at startIndex and ending at
    startIndex+length.

    \param[in] startIndex- the index of the array to start inserting data at.
    \param[in] length - the number of pieces of data in data[].
    \param[in] data - the array of data to set in the data buffer array.

	\retval - True if the data could be set. False otherwise. Will return false
	          if the index or length would cause an out of bounds array access.
*//****************************************************************************/
bool DataBuffer_SetData(
	uint16_t startIndex,
	uint16_t length,
	const uint8_t  data[])
{
	bool setSuccess = false;
	uint16_t writeIndex;
	uint16_t dataIndex = 0;

	ASSERT((_writeArray != NULL) && (data != NULL), ErrorLedMode_SOFTWARE);

	if ((startIndex + length) < (_arraySize - 1u))
	{
		for (writeIndex = startIndex; writeIndex < (startIndex + length); ++writeIndex)
		{
			_writeArray[writeIndex]= data[dataIndex++];
		}

		setSuccess = true;
	}

	return setSuccess;
}

/***************************************************************************//**
    Gets a reference to the idle buffer.

    \param[out] bufferLength - the length of the returned buffer
    \param[out] buffer - the pointer to the array of data in the buffer.

*//****************************************************************************/
void DataBuffer_GetIdleBuffer(
    uint16_t *bufferLength,
    uint8_t **buffer)
{
    ASSERT(buffer != NULL, ErrorLedMode_SOFTWARE);

    *bufferLength = _arraySize;
    /* set the buffer to the array we are not writing to */
    if (_writeArray == _array0)
    {
        *buffer = &_array1[0u];
    }
    else
    {
        *buffer = &_array0[0u];
    }
}

/***************************************************************************//**
    Rotates which buffer the new data is going to and resets the old buffer.

	\retval The value of the now active buffer, 0 or 1.

    \note DO NOT CALL FROM INTERRUPT CONTEXT!!! There is currently no way to
          rotate the buffers from interrupt context and guarantee data integrity
          of the data within the buffers.
*//****************************************************************************/
uint16_t DataBuffer_RotateBuffer(void)
{
	uint16_t writeArrayNumber;

	if (_writeArray == _array0)
	{
		_InitializeData(_array1);
		_writeArray = _array1;
		writeArrayNumber = 1u;

	}
	else /* _writeArray == _array1 */
	{
		_InitializeData(_array0);
		_writeArray = _array0;
		writeArrayNumber = 0u;
	}

	return writeArrayNumber;
}

/***************************************************************************//**
    If there is room, it creates a subarray of length spaces. The subarray will
    always be appended to the end of the active subarrays.

    \param[in] subarrayId- the identifier for the subarray.
    \param[in] length - the maximum number of pieces of data to store in the subarray.

	\retval - True if the subarray could be created. False otherwise. Will return
	false if the length would cause an out of bounds array access. Or if the
	subarray Id is already used.

*//****************************************************************************/
bool DataBuffer_CreateSubarray(
	uint8_t  subArrayId,
	uint16_t length)
{
	bool creationSuccess = false;
	uint16_t subarrayEndIndex;

	ASSERT((_array0 != NULL) && (_array1 != NULL), ErrorLedMode_SOFTWARE);

	/* only add if the subarray ID already doesn't exist */
	if (_IndexOfSubarrayId(subArrayId) == INVALID_INDEX)
	{
		/* make sure there is room in the array for another subarray of length */
		if ((_numActiveSubs == 0) &&
			((SUBARRAY_BYTES_OVERHEAD + length + ARRAY_OVERHEAD) < _arraySize))
		{

			_subArrayStartIndex[0] = NUM_SUBARRAY_OFFSET + 1u;
			subarrayEndIndex = _subArrayStartIndex[0] + SUBARRAY_BYTES_OVERHEAD + length - 1u;
			_subArrayEndIndex[0] = subarrayEndIndex;
			_array0[_subArrayStartIndex[0]] = subArrayId;
			_array1[_subArrayStartIndex[0]] = subArrayId;
			_numActiveSubs += 1;
			_array0[NUM_SUBARRAY_OFFSET] = _numActiveSubs;
			_array1[NUM_SUBARRAY_OFFSET] = _numActiveSubs;
			_array0[NUM_USEFUL_BYTES_OFFSET] = subarrayEndIndex;
			_array1[NUM_USEFUL_BYTES_OFFSET] = subarrayEndIndex;
			_allocatedBytesSize = subarrayEndIndex;

			_InitializeData(_array0);
			_InitializeData(_array1);

			creationSuccess = true;
		}
		else if ((_numActiveSubs < (MAX_NUM_SUB_ARRAYS - 1u)) &&
				 (((_subArrayEndIndex[_numActiveSubs - 1u]) + SUBARRAY_BYTES_OVERHEAD + length) < _arraySize))
		{
			_subArrayStartIndex[_numActiveSubs] = _subArrayEndIndex[_numActiveSubs - 1u] + 1u;
			subarrayEndIndex = _subArrayStartIndex[_numActiveSubs] + SUBARRAY_BYTES_OVERHEAD + length - 1u;
			_subArrayEndIndex[_numActiveSubs] = subarrayEndIndex;
			_array0[_subArrayStartIndex[_numActiveSubs]] = subArrayId;
			_array1[_subArrayStartIndex[_numActiveSubs]] = subArrayId;
			_numActiveSubs += 1;
			_array0[NUM_SUBARRAY_OFFSET] = _numActiveSubs;
			_array1[NUM_SUBARRAY_OFFSET] = _numActiveSubs;
			_array0[NUM_USEFUL_BYTES_OFFSET] = subarrayEndIndex;
			_array1[NUM_USEFUL_BYTES_OFFSET] = subarrayEndIndex;
			_allocatedBytesSize = subarrayEndIndex;

			_InitializeData(_array0);
			_InitializeData(_array1);

			creationSuccess = true;
		}
	}
	return creationSuccess;
}

/***************************************************************************//**
    If there is room, it adds length of data to subarrayId.

    \param[in] subarrayId- the identifier for the subarray.
    \param[in] length - the actual number of pieces of data to add to the subarray.

	\retval - True if the data could be added and would fit. False otherwise. Will return
	false if the length would cause an out of bounds array access. Or if the
	subarray Id is not found.

*//****************************************************************************/
bool DataBuffer_AddToSubarray8Bit(
	uint8_t  subArrayId,
	uint16_t length,
	const uint8_t data[])
{
	bool addSuccess = false;
	int16_t subarrayIndex;
	uint16_t startIndex;
	uint16_t currentLength;
	uint16_t endIndex;
	uint16_t dataIndex;
	uint16_t writeIndex;

	/* find the index of the subarray id */
	subarrayIndex = _IndexOfSubarrayId(subArrayId);

	if (subarrayIndex != INVALID_INDEX)
	{
		startIndex = _subArrayStartIndex[subarrayIndex];
		currentLength = _writeArray[startIndex + SUBARRAY_LENGTH_BYTE_OFFSET];
		endIndex = _subArrayEndIndex[subarrayIndex];
		if ((startIndex + currentLength + length) < endIndex)
		{
			writeIndex = startIndex + SUBARRAY_DATA_BYTE_OFFSET + currentLength;
			for (dataIndex = 0; dataIndex < length; ++dataIndex)
			{
				_writeArray[writeIndex] = data[dataIndex];
				++writeIndex;
			}
			_writeArray[startIndex + SUBARRAY_LENGTH_BYTE_OFFSET] += length;

			addSuccess = true;
		}
		else
		{
			/* set the overflow bit */
			_writeArray[startIndex + SUBARRAY_ID_BYTE_OFFSET] |= OVERFLOW_BIT;
		}
	}

	return addSuccess;
}

/***************************************************************************//**
    If there is room, it adds length of data to subarrayId.

    \param[in] subarrayId- the identifier for the subarray.
    \param[in] length - the actual number of pieces of data to add to the subarray.

	\retval - True if the data could be added and would fit. False otherwise. Will return
	false if the length would cause an out of bounds array access. Or if the
	subarray Id is not found.

*//****************************************************************************/
bool DataBuffer_AddToSubarray16Bit(
	uint8_t  subArrayId,
	uint16_t length,
	const uint16_t data[])
{
	bool addSuccess = false;
	int16_t subarrayIndex;
	uint16_t startIndex;
	uint16_t currentLength;
	uint16_t endIndex;
	uint16_t dataIndex;
	uint16_t writeIndex;

	/* find the index of the subarray id */
	subarrayIndex = _IndexOfSubarrayId(subArrayId);

	if (subarrayIndex != INVALID_INDEX)
	{
		startIndex = _subArrayStartIndex[subarrayIndex];
		currentLength = _writeArray[startIndex + SUBARRAY_LENGTH_BYTE_OFFSET];
		endIndex = _subArrayEndIndex[subarrayIndex];
		/* Length must be doubled because data is Uint16_t and buffer is Uint8_t */
		if ((startIndex + currentLength + (length * 2)) < endIndex)
		{
			writeIndex = startIndex + SUBARRAY_DATA_BYTE_OFFSET + currentLength;
			for (dataIndex = 0; dataIndex < length; ++dataIndex)
			{
				/* Take the MSB first, then the LSB */
				_writeArray[writeIndex] = ((uint8_t)(data[dataIndex] >> 8u));
				++writeIndex;
				_writeArray[writeIndex] = ((uint8_t)(data[dataIndex]));
				++writeIndex;
			}
			/* length is uint16_t so needs to be doubled */
			_writeArray[startIndex + SUBARRAY_LENGTH_BYTE_OFFSET] += length * 2;

			addSuccess = true;
		}
		else
		{
			/* set the overflow bit */
			_writeArray[startIndex + SUBARRAY_ID_BYTE_OFFSET] |= OVERFLOW_BIT;
		}
	}

	return addSuccess;
}

/***************************************************************************//**
    Deletes all the subarrays from the data arrays.
*//****************************************************************************/
void DataBuffer_ClearSubarrays(void)
{
	_numActiveSubs = 0u;
	_array0[NUM_SUBARRAY_OFFSET] = 0u;
	_array1[NUM_SUBARRAY_OFFSET] = 0u;
	_array0[NUM_USEFUL_BYTES_OFFSET] = 0u;
	_array1[NUM_USEFUL_BYTES_OFFSET] = 0u;
	_allocatedBytesSize = 0u;
}

/***************************************************************************//**
    Checks if a subarray by the name of subArrayId exists.

    \param[in] subarrayId- the identifier for the subarray.

	\retval - True if the subarray ID exists. False otherwise.

*//****************************************************************************/
bool DataBuffer_SubarrayExists(uint8_t subArrayId)
{
	int16_t index;
	index = _IndexOfSubarrayId(subArrayId);

	return (index != INVALID_INDEX);
}

/***************************************************************************//**
    Clears the length bytes of the data stored in the buffer to "zero" it out.

    \param[in] array - the array to get initialized.

*//****************************************************************************/
static void _InitializeData(uint8_t array[])
{
	int16_t subArray;

	/* this assumes that _subArrayIndex only has valid indexes for array[] */
	for (subArray = _numActiveSubs - 1; subArray >= 0; --subArray)
	{
		/* clear the length byte */
		array[_subArrayStartIndex[subArray] + SUBARRAY_LENGTH_BYTE_OFFSET] = 0u;
		/* clear the overflow bit */
		array[_subArrayStartIndex[subArray] + SUBARRAY_ID_BYTE_OFFSET] &= ((uint8_t)(~OVERFLOW_BIT));
		/* set the allocated size byte */
		array[NUM_USEFUL_BYTES_OFFSET] = _allocatedBytesSize;
	}
}

/***************************************************************************//**
    Locates the subarray index for a specified subarray ID. If the ID is not found,
    returns INVALID_INDEX.

    \param[in] subarrayId- the identifier for the subarray.

	\retval - The index of the subarray ID. Returns INVALID_INDEX if ID is not found.

*//****************************************************************************/
static int16_t _IndexOfSubarrayId(uint8_t subarrayId)
{
	int16_t index = INVALID_INDEX;
	int16_t subarrayCounter;

	ASSERT(_writeArray != NULL, ErrorLedMode_SOFTWARE);

	for (subarrayCounter = _numActiveSubs - 1; subarrayCounter >= 0; --subarrayCounter)
	{
		if (_writeArray[_subArrayStartIndex[subarrayCounter] + SUBARRAY_ID_BYTE_OFFSET] == subarrayId)
		{
			index = subarrayCounter;
			break;
		}
	}

	return index;
}
