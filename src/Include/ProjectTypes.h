/***************************************************************************//**
    \file ProjectTypes.h
    \brief Definitions for portable types

    \b Project 1814 IOLab Remote Sensor Application

    \b Description Defined types for primitives to allow for portability across
       architectures.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Include/ProjectTypes.h$
    $Revision: 3$
    $Date: Tuesday, April 30, 2013 10:25:38 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef PROJECTTYPES_H_
#define PROJECTTYPES_H_

/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/

#ifndef ARRAY_LENGTH
    /** Macro for determining the number of elements in an array.
        Only valid in context where array is defined.
    */
    #define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef true
    /** Boolean true value. */
    #define true ((bool)(1==1))
#endif

#ifndef false
    /** Boolean false value. */
    #define false ((bool)(0==1))
#endif

#pragma CHECK_MISRA("none")
#ifndef NULL
    /** The NULL pointer. */
    #define NULL ((void*)0)
#endif

#ifndef ptrdiff_t
    /** Pointer difference type */
    typedef __PTRDIFF_T_TYPE__ ptrdiff_t;
#endif
#pragma RESET_MISRA("all")

#ifndef bool
    /** Definition of the boolean type. */
    typedef unsigned int bool;
#endif

#ifndef char_t
    /** Definition of the character type for use with character values */
    typedef char char_t;
#endif

#ifndef int8_t
    /** 8-bit signed integer type. */
    typedef signed char int8_t;
    /** Number of bits in a int8_t. */
    #define INT8_T_BIT (8)
    /** Minimum int8_t value */
    #define INT8_T_MIN ((-0x7F) - 1)
    /** Maximum int8_t value */
    #define INT8_T_MAX (0x7F)
#endif

#ifndef int16_t
    /** 16-bit signed integer type. */
    typedef signed int int16_t;
    /** Number of bits in an int16_t */
    #define INT16_T_BIT (16)
    /** Minimum int16_t value */
    #define INT16_T_MIN ((-0x7FFF) - 1)
    /** Maximum int16_t value */
    #define INT16_T_MAX (0x7FFF)
#endif

#ifndef int32_t
    /** 32-bit signed integer type. */
    typedef signed long int int32_t;
    /** Number of bits in a long */
    #define INT32_T_BIT (32)
    /** Minimum (signed) long value */
    #define INT32_T_MIN ((-0x7FFFFFFF) - 1)
    /** Maximum (signed) long value */
    #define INT32_T_MAX (0x7FFFFFFF)
#endif

#ifndef uint8_t
    /** 8-bit unsigned integer type. */
#ifdef _SOMETHING_
    /*  Disable Lint warning "Use of 'char' without 'signed' or 'unsigned'".
        Due to an eccentricity of the compiler: string literals are arrays of
        chars, but adding either the signed or the unsigned modifier results in
        a compile warning in the code that handles the literals.
    */
    /*lint --e(971)*/
    typedef char uint8_t;
#else
    typedef unsigned char uint8_t;
#endif
    /** Number of bits in a uint8_t */
    #define UINT8_T_BIT (8)
    /** Maximum uint8_t value */
    #define UINT8_T_MIN (0u)
    /** Maximum uint8_t value */
    #define UINT8_T_MAX (0xFFu)
#endif

#ifndef uint16_t
    /** 16-bit unsigned integer type. */
    typedef unsigned int uint16_t;
    /** Number of bits in uint16_t */
    #define UINT16_T_BIT (16)
    /** Minimum uint16_t value */
    #define UINT16_T_MIN (0u)
    /** Maximum uint16_t value */
    #define UINT16_T_MAX (0xFFFFu)
#endif

#ifndef uint32_t
    /** 32-bit unsigned integer type. */
    typedef unsigned long int uint32_t;
    /** Number of bits in a uint32_t */
    #define UINT32_T_BIT (32)
    /** Minimum uint32_t value */
    #define UINT32_T_MIN (0u)
    /** Maximum uint32_t value */
    #define UINT32_T_MAX (0xFFFFFFFFu)
#endif

#ifndef float32_t
    /** 32-bit float type. */
    typedef float float32_t;
    /** Number of bits in a float32_t */
    #define FLOAT_T_BIT (32)
#endif

/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/


#endif /* PROJECTTYPES_H_ */
