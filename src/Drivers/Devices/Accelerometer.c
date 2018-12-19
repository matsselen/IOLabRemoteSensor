/***************************************************************************//**
    \file Accelerometer.c
    \brief Accelerometer sensor interface module.

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This module provides logic to interface with the 3-axis
                   accelerometer. This logic assumes that the accelerometer in
                   use is the Freescale MMA8451Q I2C digital accelerometer.

                   <a href="http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8451Q&fsrch=1&sr=5"
                   >Link to Product Webpage</a>

                   <a href="http://cache.freescale.com/files/sensors/doc/data_sheet/MMA8451Q.pdf?fpsp=1&WT_TYPE=Data%20Sheets&WT_VENDOR=FREESCALE&WT_FILE_FORMAT=pdf&WT_ASSET=Documentation"
                   >Link to Product Datasheet</a>

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Devices/Accelerometer.c$
    $Revision: 11$
    $Date: Wednesday, September 19, 2012 3:27:44 PM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/


#include <ProjectTypes.h>
#include "Accelerometer.h"

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
    accelerometer.
 */
typedef enum AccReg
{
	AccReg_STATUS           = 0x00, /// (R  ) Real time status
	AccReg_OUT_X_MSB        = 0x01, /// (R  ) MSB of X sample, root pointer to XYZ FIFO
	AccReg_OUT_X_LSB        = 0x02, /// (R  ) LSB of X sample
	AccReg_OUT_Y_MSB        = 0x03, /// (R  ) MSB of Y sample
	AccReg_OUT_Y_LSB        = 0x04, /// (R  ) LSB of Y sample
	AccReg_OUT_Z_MSB        = 0x05, /// (R  ) MSB of Z sample
	AccReg_OUT_Z_LSB        = 0x06, /// (R  ) LSB of Z sample
	/** 0x07 - 0x08 are reserved */
	AccReg_FIFO_SETUP       = 0x09, /// (R/W) FIFO setup
	AccReg_TRIG_CONFIG      = 0x0A, /// (R/W) Map of FIFO data capture events
	AccReg_SYSMOD           = 0x0B, /// (R  ) Current system mode
	AccReg_INT_SOURCE_STAT  = 0x0C, /// (R  ) Interrupt status
	AccReg_WHO_AM_I         = 0x0D, /// (R  ) Device ID (0x1A)
	AccReg_XYZ_DATA_CONFIG  = 0x0E, /// (R/W) Dynamic range settings
	AccReg_HP_FILTER_CUTOFF = 0x0F, /// (R/W) Cutoff frequency is 16Hz @ 800 Hz
	AccReg_PL_STATUS        = 0x10, /// (R  ) Landscape/Portrait orientation status
	AccReg_PL_CONFIG        = 0x11, /// (R/W) Landscape/Portrait configuration
	AccReg_PL_COUNT         = 0x12, /// (R/W) Landscape/Portrait debounce counter
	AccReg_PL_BF_ZCOMP      = 0x13, /// (R/W) Back/Front Z-Lock trip threshold
	AccReg_PL_THS_REG       = 0x14, /// (R/W) Portrait to Landscape trip angle is 29 deg
	AccReg_FF_MT_CONFIG     = 0x15, /// (R/W) Freefall/Motion functional block config
	AccReg_FF_MT_SOURCE     = 0x16, /// (R  ) Freefall/Motion event source
	AccReg_FF_MT_THS        = 0x17, /// (R/W) Freefall/Motion threshold
	AccReg_FF_MT_COUNT      = 0x18, /// (R/W) Freefall/Motion debounce counter
	/** 0x19 - 0x1C are reserved */
	AccReg_TRANSIENT_CONFIG = 0x1D, /// (R/W) Transient functional block configuration
	AccReg_TRANSIENT_SOURCE = 0x1E, /// (R  ) Transient event status register
	AccReg_TRANSIENT_THS    = 0x1F, /// (R/W) Transient event threshold
	AccReg_TRANSIENT_COUNT  = 0x20, /// (R/W) Transient debounce counter
	AccReg_PULSE_CONFIG     = 0x21, /// (R/W) ELE, Double_XYZ or Single_XYZ
	AccReg_PULSE_SOURCE     = 0x22, /// (R  ) EA, Double_XYZ or Single_XYZ
	AccReg_PULSE_THS_X      = 0x23, /// (R/W) X pulse threshold
	AccReg_PULSE_THS_Y      = 0x24, /// (R/W) Y pulse threshold
	AccReg_PULSE_THS_Z      = 0x25, /// (R/W) Z pulse threshold
	AccReg_PULSE_TMLT       = 0x26, /// (R/W) Time limit for pulse
	AccReg_PULSE_LTCY       = 0x27, /// (R/W) Latency time for 2nd pulse
	AccReg_PULSE_WIND       = 0x28, /// (R/W) Window time for 2nd pulse
	AccReg_ASLP_COUNT       = 0x29, /// (R/W) Counter setting for auto-sleep
	AccReg_CTRL_REG1        = 0x2A, /// (R/W) ODR = 800Hz, STANDBY Mode
	AccReg_CTRL_REG2        = 0x2B, /// (R/W) Sleep Enable, OS Modes, RST, ST
	AccReg_CTRL_REG3        = 0x2C, /// (R/W) Wake from Sleep, IPOL, PP_OD
	AccReg_CTRL_REG4        = 0x2D, /// (R/W) Interrupt enable register
	AccReg_CTRL_REG5        = 0x2E, /// (R/W) Interrupt pin (INT1/INT2) map
	AccReg_OFF_X            = 0x2F, /// (R/W) X-axis offset adjust
	AccReg_OFF_Y            = 0x30, /// (R/W) Y-axis offset adjust
	AccReg_OFF_Z            = 0x31  /// (R/W) Z-axis offset adjust
	/** 0x40 - 0x7F are reserved. Do not write to these!*/
} AccReg_t;

/** Defines the bitmasks for the bits in the AccReg_Status register of AccReg_t when
    not using the FIFO (F_MODE = 0 in AccReg_FIFO_SETUP).

    \note These bits are only correct when NOT USING THE FIFO! This register has
          a different meaning when using the FIFO.

    \see See FifoStatus.
 */
typedef enum Status
{
    Status_ZYXOW = 0x80, /// XYX-axis data overwrite occurred
    Status_ZOW   = 0x40, /// Z-axis data overwrite occurred
    Status_YOW   = 0x20, /// Y-axis data overwrite occurred
    Status_XOW   = 0x10, /// X-axis data overwrite occurred
    Status_ZYXDR = 0x08, /// ZYX-axis new data ready (cleared when ALL axis MSBs are read)
    Status_ZDR   = 0x04, /// Z-axis new data ready
    Status_YDR   = 0x02, /// Y-axis new data ready
    Status_XDR   = 0x01  /// X-axis new data ready
} Status_t;

/** Defines the bitmasks for the bits in the AccReg_Status register of AccReg_t when
    using the FIFO (F_MODE > 0 in AccReg_FIFO_SETUP).

    \note These bits are only correct when USING THE FIFO! This register has
          a different meaning when not using the FIFO.

    \see See Status.
 */
typedef enum FifoStatus
{
	FifoStatus_F_OVF       = 0x80, /// Set when the fifo overflows
	FifoStatus_F_WMRK_FLAG = 0x40, /// Set when the FIFO hits the watermark
	FifoStatus_F_CNT5      = 0x20, /// Bit 5 of the FIFO counter (how many samples are in the FIFO)
	FifoStatus_F_CNT4      = 0x10, /// Bit 4 of the FIFO counter
	FifoStatus_F_CNT3      = 0x08, /// Bit 3 of the FIFO counter
	FifoStatus_F_CNT2      = 0x04, /// Bit 2 of the FIFO counter
	FifoStatus_F_CNT1      = 0x02, /// Bit 1 of the FIFO counter
	FifoStatus_F_CNT0      = 0x01,  /// Bit 0 of the FIFO counter
	FifoStatus_F_CNT = FifoStatus_F_CNT5 | FifoStatus_F_CNT4 | FifoStatus_F_CNT3 |
	                   FifoStatus_F_CNT2 | FifoStatus_F_CNT1 | FifoStatus_F_CNT0
} FifoStatus_t;

/** Defines the bitmasks for the bits in the AccReg_FIFO_SETUP register of AccReg_t.

    F_MODE - 00: FIFO is disabled
             01: FIFO is circular buffer
             10: FIFO stops when full
             11: FIFO will take WMRK samples then stop
 */
typedef enum FifoSetup
{
    FifoSetup_F_MODE1 = 0x80, /// FIFO mode bit 1
    FifoSetup_F_MODE0 = 0x40, /// FIFO mode bit 0
    FifoSetup_F_MODE  = FifoSetup_F_MODE1 | FifoSetup_F_MODE0,
    FifoSetup_F_WMRK5 = 0x20, /// Bit 5 of the watermark setpoint
    FifoSetup_F_WMRK4 = 0x10, /// Bit 4 of the watermark setpoint
    FifoSetup_F_WMRK3 = 0x08, /// Bit 3 of the watermark setpoint
    FifoSetup_F_WMRK2 = 0x04, /// Bit 2 of the watermark setpoint
    FifoSetup_F_WMRK1 = 0x02, /// Bit 1 of the watermark setpoint
    FifoSetup_F_WMRK0 = 0x01, /// Bit 0 of the watermark setpoint
    FifoSetup_F_WMRK  = FifoSetup_F_WMRK5 | FifoSetup_F_WMRK4 | FifoSetup_F_WMRK3 |
                        FifoSetup_F_WMRK2 | FifoSetup_F_WMRK1 | FifoSetup_F_WMRK0
} FifoSetup_t;

/** Defines the bitmasks for the interrupt trigger configuration AccReg_TRIG_CONFIG
    register of the AccReg_t.
 */
typedef enum TriggerConfig
{
	TriggerConfig_TRANS  = 0x20, /// Transient interrupt trigger
	TriggerConfig_LNDPRT = 0x10, /// Landscape/Portrait orientation interrupt trigger
	TriggerConfig_PULSE  = 0x08, /// Pulse interrupt trigger
	TriggerConfig_FF_MT  = 0x04  /// Freefall/Motion interrupt trigger
} TriggerConfig_t;

/** Defines the bitmasks for the system mode AccReg_SYS_MOD of the AccReg_t.

    SYSMOD - 00: STANDBY mode
             01: WAKE mode
             10: SLEEP mode
 */
typedef enum SysMod
{
	SysMod_FGERR   = 0x80, /// FIFO gate error. Emptying the FIFO clears the error
	SysMod_FGT_4   = 0x40, /// Number of ODR time units since FGERR was asserted. Reset when FGERR is cleared
	SysMod_FGT_3   = 0x20, ///
	SysMod_FGT_2   = 0x10, ///
	SysMod_FGT_1   = 0x08, ///
	SysMod_FGT_0   = 0x04, ///
	SysMod_FGT     = SysMod_FGT_4 | SysMod_FGT_3 | SysMod_FGT_2 |
	                 SysMod_FGT_1 | SysMod_FGT_0,
	SysMod_SYSMOD1 = 0x02, /// Bit 1 of the system mode
	SysMod_SYSMOD0 = 0x01,  /// Bit 0 of the system mode
	SysMod_SYSMOD  = SysMod_SYSMOD1 | SysMod_SYSMOD0,
	SysMod_STNDBY  = 0x00, /// Value of the SYSMOD bits when system is in standby
	SysMod_WAKE    = 0x01, /// Value of the SYSMOD bits when the system is active
	SysMod_SLEEP   = 0x02  /// Value of the SYSMOD bits when the system is asleep
} SysMod_t;

/** Defines the bitmasks for the interrupt source status register AccReg_INT_SOURCE_STAT
    of the AccReg_t.
 */
typedef enum IntSourceStatus
{
	IntSourceStatus_ASLP   = 0x80, /// WAKE to SLEEP or SLEEP to WAKE interrupt occurred
	IntSourceStatus_FIFO   = 0x40, /// FIFO overflow or watermark occurred
	IntSourceStatus_TRANS  = 0x20, /// Acceleration transient value > THRESH occurred
	IntSourceStatus_LNDPRT = 0x10, /// Device orientation change occurred
	IntSourceStatus_PULSE  = 0x08, /// A single or double pulse occurred
	IntSourceStatus_FF_MT  = 0x04, /// A freefall or motion event occurred
	IntSourceStatus_DRDY   = 0x01  /// New data is ready or data overrun
} IntSourceStatus_t;

/** Define the expected value of the WHO_AM_I query */
#define WHO_AM_I ((uint8_t)0x1A)

/** Defines the bitmasks for the XYZ configuration register that sets the dynamic range
    and the high pass filter for the output data.

    Full scale range:
    FS - 00: 2g
         01: 4g
         10: 8g
         11: reserved

    \note 8g mode will not work with low noise or high resolution modes.
 */
typedef enum XyzDataConfig
{
    XyzDataConfig_HPF_OUT = 0x10, /// Enable high pass output data
    XyzDataConfig_FS1     = 0x02, /// Full scale range bit 1
    XyzDataConfig_FS0     = 0x01, /// Full scale range bit 0
    XyzDataConfig_FS      = XyzDataConfig_FS1 | XyzDataConfig_FS0,
    XyzDataConfig_2g      = 0x00, /// FS value when range is 2g
    XyzDataConfig_4g      = 0x01, /// FS value when range is 4g
    XyzDataConfig_8g      = 0x02  /// FS value when range is 8g
} XyzDataConfig_t;

/** Defines the bitmasks for the high pass filter cutoff settings. */
typedef enum HpFilterCutoff
{
	HpFilterCutoff_HPF_BYP = 0x20, /// Bypass the high pass filter for pulse processing
	HpFilterCutoff_LPF_EN  = 0x10, /// Enable the low pass filter for pulse processing
	HpFilterCutoff_SEL1    = 0x02, /// High Pass cutoff frequency selection. See datasheet Table 22.
	HpFilterCutoff_SEL0    = 0x01,
	HpFilterCutoff_SEL     = HpFilterCutoff_SEL1 | HpFilterCutoff_SEL0
} HpFilterCutoff_t;

/** Defines the bitmasks for the portrait landscape status register AccReg_PL_STATUS of
    the AccReg_t.

    LAPO - 00: Portrait up
           01: Portrait down
           10: Landscape right
           11: Landscape left
 */
typedef enum PlStatus
{
    PlStatus_NEWLP = 0x80, /// Landscape/Portrait status has changed
    PlStatus_LO    = 0x40, /// Z-tilt lockout angle has been exceeded
    PlStatus_LAPO1 = 0x04, /// Landscape/Portrait orientation bit 1
    PlStatus_LAPO0 = 0x02, /// Landscape/Portrait orientation bit 0
    PlStatus_LAPO  = PlStatus_LAPO1 | PlStatus_LAPO0,
    PlStatus_BAFRO = 0x01  /// Back or Front orientation. 0 = Front, 1 = Back
} PlStatus_t;

/** Defines the bitmasks for the portrait landscape configuration register
   AccReg_PL_CONFIG of the AccReg_t.
 */
typedef enum PlConfig
{
	PlConfig_DBCNTM = 0x80, /// 0 - decrement counter, 1 - clear counter on event
	PlConfig_PL_EN  = 0x40  /// Enable portrait/landscape detection
} PlConfig_t;

/** Defines the bitmasks for the back to front z-lockout compensation register 
    AccReg_PL_BF_COMP of the AccReg_t. 
    
               Back/Front Transistion(deg)   Front/Back Transistion(deg)
    BKFR = 00:    Z < 80 or Z > 280        Z > 100 and Z < 260
           01:    Z < 75 or Z > 285        Z > 105 and Z < 255
           10:    Z < 70 or Z > 290        Z > 110 and Z < 250
           11     Z < 65 or Z > 295        Z > 115 and Z < 245
                    
                 Threshold Angle (deg)
    ZLOCK = 0x00: 14 
            0x01: 18
            0x02: 21
            0x03: 25
            0x04: 29
            0x05: 33
            0x06: 37
            0x07: 42
            
*/
typedef enum PlBfComp
{
    PlBfComp_BKFR  = 0xC0, /// Back to front transition setpoint
    PlBfComp_ZLOCK = 0x07  /// Z-lock threshold angle
} PlBfComp_t; 

/** Defines the bit masks for the portrait to landscape threshold and hysteresis settings
    for the AccReg_PL_THS_REG of the AccReg_t. The default trip angle is 45 deg (0x10) and the 
    default hysteresis is +/-14 deg. 
    
    5-bit register  Threshold Angle (degrees)
    THS - 0x07:      15
          0x09:      20
          0x0C:      30
          0x0D:      35
          0x0F:      40
          0x10:      45
          0x13:      55
          0x14:      60
          0x17:      70
          0x19:      75
    
    3-bit register  Hysteresis  L->P   P->L
    HYST - 0x00:      0          45     45
           0x01:      4          49     41
           0x02:      7          52     38
           0x03:     11          56     34
           0x04:     14          59     31
           0x05:     17          62     28
           0x06:     21          66     24
           0x07:     24          69     21
    
    \note THS+HYST > 0 and THS + HYST < 32 for Landscape/Portrait detection to work correctly. 
          All angles accurate to +/- 2 degrees. 
*/
typedef enum PlThreshold
{
    PlThreshold_THS  = 0xF8, /// Portrait/Landscape trip threshold
    PlThreshold_HYST = 0x07  /// Portrait/Landscape trip hysteresis
} PlThreshold_t; 

// TODO: spg - populate AccReg_FF_MT_CONFIG
// TODO: spg - populate AccReg_FF_MT_SOURCE
// TODO: spg - populate AccReg_FF_MT_THS
// TODO: spg - populate AccReg_FF_MT_COUNT
// TODO: spg - populate AccReg_TRANSIENT_CONFIG
// TODO: spg - populate AccReg_TRANSIENT_SOURCE
// TODO: spg - populate AccReg_TRANSIENT_THS
// TODO: spg - populate AccReg_TRANSIENT_COUNT
// TODO: spg - populate AccReg_PULSE_CONFIG
// TODO: spg - populate AccReg_PULSE_SOURCE
// TODO: spg - populate AccReg_PULSE_THS_X
// TODO: spg - populate AccReg_PULSE_THS_Y
// TODO: spg - populate AccReg_PULSE_THS_Z
// TODO: spg - populate AccReg_PULSE_TMLT
// TODO: spg - populate AccReg_PULSE_LTCY
// TODO: spg - populate AccReg_PULSE_WIND
// TODO: spg - populate AccReg_ASLP_COUNT

/** Defines the bitmask for the control register one AccReg_CTRL_REG_1 of the AccReg_t. 

    ASLP_RATE - 00: 50    Hz
                01: 12.5  Hz
                10:  6.25 Hz
                11:  1.56 Hz

               Frequency (Hz) Period (ms)
    ODR - 000: 800             1.25
          001: 400             2.5
          010: 200             5
          011: 100            10
          100:  50            20
          101:  12.5          80
          110:   6.25         160
          111:   1.56         640
          
    \note except for active/standby, the device must be in standby mode to change any fields
          within this register. 
*/
typedef enum CtrlRegOne
{
	CtrlRegOne_ASLP_RATE1 = 0x80, /// bit 1 of the autowake sample frequency
	CtrlRegOne_ASLP_RATE0 = 0x40, /// bit 0 of the autowake sample frequency
    CtrlRegOne_ASLP_RATE  = CtrlRegOne_ASLP_RATE1 | CtrlRegOne_ASLP_RATE0, /// Configures the autowake sample frequency
    CtrlRegOne_ODR2       = 0x20,
    CtrlRegOne_ODR1       = 0x10,
    CtrlRegOne_ODR0       = 0x08,
    CtrlRegOne_ODR        = CtrlRegOne_ODR2 | CtrlRegOne_ODR1 | CtrlRegOne_ODR0, /// output data rate selection
    CtrlRegOne_LNOISE     = 0x04, /// reduced noise enabled
    CtrlRegOne_F_READ     = 0x02, /// fast read mode enable
    CtrlRegOne_ACTIVE     = 0x01, /// active mode enable
    CtrlRegOne_ODR_800    = 0x00,
    CtrlRegOne_ODR_400    = CtrlRegOne_ODR0,
    CtrlRegOne_ODR_200    = CtrlRegOne_ODR1,
    CtrlRegOne_ODR_100    = CtrlRegOne_ODR1 | CtrlRegOne_ODR0,
    CtrlRegOne_ODR_50     = CtrlRegOne_ODR2,
    CtrlRegOne_ODR_12_5   = CtrlRegOne_ODR2 | CtrlRegOne_ODR0,
    CtrlRegOne_ODR_6_25   = CtrlRegOne_ODR2 | CtrlRegOne_ODR1,
    CtrlRegOne_ODR_1_56   = CtrlRegOne_ODR2 | CtrlRegOne_ODR1 | CtrlRegOne_ODR0
} CtrlRegOne_t; 

/** Defines the bitmask for the Control register 2 AccReg_CTRL_REG2 of AccReg_t.

    The software reset can be activated in standby or active mode. Resetting the device
    causes all registers to immediately be cleared.

                  Mode
    (S)MODS - 00: Normal
              01: Low Noise, Low Power
              10: High Resolution
              11: Low Power

	\note Low noise and high resolution modes only work for 2g and 4g range, not 8g.
*/
typedef enum CtrlRegTwo
{
    CtrlRegTwo_ST_EN = 0x80, /// Self test enable
    CtrlRegTwo_RST   = 0x40, /// Software Reset enable
    CtrlRegTwo_SMODS1 = 0x10, /// Sleep mode power scheme bit 1
    CtrlRegTwo_SMODS0 = 0x08, /// Sleep mode power scheme bit 0
    CtrlRegTwo_SMODS = CtrlRegTwo_SMODS1 | CtrlRegTwo_SMODS0, /// Sleep mode power scheme selection
    CtrlRegTwo_SMODS_NRML = 0x00,
    CtrlRegTwo_SMODS_LNLP = CtrlRegTwo_SMODS0,
    CtrlRegTwo_SMODS_HRES = CtrlRegTwo_SMODS1,
    CtrlRegTwo_SMODS_LPWR = CtrlRegTwo_SMODS1 | CtrlRegTwo_SMODS0,
    CtrlRegTwo_SLPE  = 0x04, /// Auto sleep mode enable
    CtrlRegTwo_MODS1 = 0x02, /// Active mode power scheme bit 1
    CtrlRegTwo_MODS0 = 0x01, /// Active mode power scheme bit 0
    CtrlRegTwo_MODS  = CtrlRegTwo_MODS1 | CtrlRegTwo_MODS0, /// Active mode power scheme selection
    CtrlRegTwo_MODS_NRML = 0x00,
    CtrlRegTwo_MODS_LNLP = CtrlRegTwo_MODS0,
    CtrlRegTwo_MODS_HRES = CtrlRegTwo_MODS1,
    CtrlRegTwo_MODS_LPWR = CtrlRegTwo_MODS1 | CtrlRegTwo_MODS0
} CtrlRegTwo_t; 

/** Defines the bitmask for the Control register 3 AccReg_CTRL_REG3 of AccReg_t.

*/
typedef enum CtrlRegThree
{
	CtrlRegThree_FIFO_GATE   = 0x80, /// Enable the gate to block the FIFO once full
	CtrlRegThree_WAKE_TRANS  = 0x40, /// Transient function interrupt can wake up system
	CtrlRegThree_WAKE_LNDPRT = 0x20, /// Orientation interrupt can wake system
	CtrlRegThree_WAKE_PULSE  = 0x10, /// Pulse interrupt can wake system
	CtrlRegThree_WAKE_FF_MT  = 0x08, /// Freefall/motion interrupt can wake system
	CtrlRegThree_IPOL        = 0x02, /// Interrupt polarity active high enable
	CtrlRegThree_PP_OD       = 0x01  /// 0: Push-pull on interrupt pin, 1: open drain interrupt pin
} CtrlRegThree_t;

/** Defines the bitmask for the Control register 4 AccReg_CTRL_REG4 of AccReg_t.
    This register controls the interrupt enables.
*/
typedef enum CtrlRegFour
{
	CtrlRegFour_INT_EN_ASLP   = 0x80, /// auto sleep/wake interrupt enable
	CtrlRegFour_INT_EN_FIFO   = 0x40, /// FIFO interrupt enable
	CtrlRegFour_INT_EN_TRANS  = 0x20, /// Transient interrupt enable
	CtrlRegFour_INT_EN_LNDPRT = 0x10, /// Landscape/portrait interrupt enable
	CtrlRegFour_INT_EN_PULSE  = 0x80, /// Pulse detection interrupt enable
	CtrlRegFour_INT_EN_FF_MT  = 0x40, /// Freefall/motion interrupt enable
	CtrlRegFour_INT_EN_DRDY   = 0x01  /// Data ready interrupt enable
} CtrlRegFour_t;

/** Defines the bitmask for hte Control register 5 AccReg_CTRL_REG5 of AccReg_t.
    This register control which pin the interrupt are routed to. A value of '0' routes
    to INT2 pin and a value of '1' routes to INT1 pin.
*/
typedef enum CtrlRegFive
{
	CtrlRegFive_INT_CFG_ASLP   = 0x80, /// auto sleep/wake interrupt routing
	CtrlRegFive_INT_CFG_FIFO   = 0x40, /// FIFO interrupt routing
	CtrlRegFive_INT_CFG_TRANS  = 0x20, /// Transient interrupt routing
	CtrlRegFive_INT_CFG_LNDPRT = 0x10, /// Landscape/portrait interrupt routing
	CtrlRegFive_INT_CFG_PULSE  = 0x08, /// Pulse interrupt routing
	CtrlRegFive_INT_CFG_FF_MT  = 0x04, /// Freefall/motion interrupt routing
	CtrlRegFive_INT_CFG_DRDY   = 0x01  /// Data ready interrupt routing
} CtrlRegFive_t;

/** Defines the port the accelerometer is attached to */
#define ACC_PORT I2cPort_0

/** Defines the accelerometer's I2C address. The primary address is 0x1C and the alternate address is 0x1D */
#if 1	/* primary address */
	#define ACC_ADDRESS ((uint8_t)0x1C)
#else	/* alternate address */
	#define ACC_ADDRESS ((uint8_t)0x1D)
#endif

/** Defines the number of bytes per sample reading in normal 14-bit mode */
#define BYTES_PER_SAMPLE 6

/*-----------------------LOCAL VARIABLES-------------------------------------*/
/** Tracks if the accelerometer has been initialized. */
static bool _initialized = false;

/** Tracks if the accelerometer has ever been configured before */
static bool _configured = false;

/** Stores the ISR pointer */
static AccIsrPointer_t _accIsrPointer = NULL;

/** Stores the configuration parameters for the initialized accelerometer */
static AccConfig_t _parameters = {AccOdr_100, AccResolution_14bit, AccFullScaleRange_4g,
		                          AccOverSample_HighRes};

/** Store the values written to the control registers so they don't have to be
    read back.
 */
static volatile uint8_t _ctrlRegOne = 0u;
static volatile uint8_t _ctrlRegTwo = 0u;
static volatile uint8_t _ctrlRegThree = 0u;
static volatile uint8_t _ctrlRegFour = 0u;
static volatile uint8_t _ctrlRegFive = 0u;
static volatile uint8_t _xyzDataConfig = 0u;

/** Tracks if the accelerometer is actively collecting data */
static bool _active = false;

/** Tracks if data is ready to be read from the accelerometer */
static volatile bool _dataReady = false;

/*-----------------------LOCAL FUNCTION PROTOTYPES---------------------------*/
static void _HandleDataReadyInterrupt(void);
static void _HandleOtherInterrupt(void);
static uint8_t _AccReadSingleRegister(AccReg_t);
static void _AccReadMultipleRegisters(AccReg_t startReg,
                                   uint8_t numRegisters,
                                   uint8_t registers[]);
static I2cError_t _AccWriteSingleRegister(AccReg_t reg,
                                       uint8_t value);

/***************************************************************************//**
    Initializes the accelerometer, but does not configure or start sampling
*//****************************************************************************/
void Accelerometer_Init(void)
{
    /* See if the accelerometer has already been initialized */
	ASSERT(_initialized == false, ErrorLedMode_SOFTWARE);

	/* check to make sure we have the expected accelerometer using the
	   WHO_AM_I id.
	 */
	volatile uint8_t receivedByte = 0u;
	receivedByte = _AccReadSingleRegister(AccReg_WHO_AM_I);
	ASSERT(receivedByte == WHO_AM_I, ErrorLedMode_SOFTWARE);

	/* register the interrupt for the data ready pin */
    Gpio_RegisterIsr(ACCELEROMETER_INTERRUPT_0_PORT, ACCELEROMETER_INTERRUPT_0_PIN,
                     &_HandleDataReadyInterrupt, GpioEdge_RISING);
    Gpio_RegisterIsr(ACCELEROMETER_INTERRUPT_1_PORT, ACCELEROMETER_INTERRUPT_1_PIN,
                     &_HandleOtherInterrupt, GpioEdge_RISING);
    _initialized = true;
}

/***************************************************************************//**
    Configures the accelerometer, but does not start the sampling.

    \param[in] config - Structure of selected accelerometer settings.

    \return True if the configuration was successful, false otherwise. The
            configuration will fail if the accelerometer is not in standby when
            configured.
*//****************************************************************************/
bool Accelerometer_Config(const AccConfig_t* config)
{
	bool success = false;
	uint8_t systemStatus = 0u;
	I2cError_t i2cResult = I2cError_OK;

	ASSERT(config != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	/* store the initialization data */
	_parameters.odr = config->odr;
	_parameters.overSampleMode = config->overSampleMode;
	_parameters.range = config->range;
	_parameters.resolution = config->resolution;

	/* setup the configuration registers */
	_ctrlRegOne = 0u;
	_ctrlRegTwo = 0u;
	_ctrlRegThree = 0u;
	_ctrlRegFour = 0u;
	_ctrlRegFive = 0u;
	_xyzDataConfig = 0u;

	switch(config->range)
	{
		case AccFullScaleRange_8g:
			_xyzDataConfig |= XyzDataConfig_8g;
			break;

		case AccFullScaleRange_4g:
			_xyzDataConfig |= XyzDataConfig_4g;
			break;

		case AccFullScaleRange_2g:
			_xyzDataConfig |= XyzDataConfig_2g;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->odr)
	{
		case AccOdr_800:
			_ctrlRegOne |= CtrlRegOne_ODR_800;
			break;

		case AccOdr_400:
			_ctrlRegOne |= CtrlRegOne_ODR_400;
			break;

		case AccOdr_200:
			_ctrlRegOne |= CtrlRegOne_ODR_200;
			break;

		case AccOdr_100:
			_ctrlRegOne |= CtrlRegOne_ODR_100;
			break;

		case AccOdr_50:
			_ctrlRegOne |= CtrlRegOne_ODR_50;
			break;

		case AccOdr_12_5:
			_ctrlRegOne |= CtrlRegOne_ODR_12_5;
			break;

		case AccOdr_6_25:
			_ctrlRegOne |= CtrlRegOne_ODR_6_25;
			break;

		case AccOdr_1_56:
			_ctrlRegOne |= CtrlRegOne_ODR_1_56;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
			break;
	}

	switch(config->resolution)
	{
		case AccResolution_14bit:
			/* register should already be clear for this bit */
			break;

		case AccResolution_8bit:
			_ctrlRegOne |= CtrlRegOne_F_READ;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
	}

	switch(config->overSampleMode)
	{
		case AccOverSample_Normal:
			_ctrlRegTwo |= CtrlRegTwo_MODS_NRML;
			break;

		case AccOverSample_HighRes:
			_ctrlRegTwo |= CtrlRegTwo_MODS_HRES;
			break;

		case AccOverSample_LNLP:
			_ctrlRegTwo |= CtrlRegTwo_MODS_LNLP;
			break;

		case AccOverSample_LowPower:
			_ctrlRegTwo |= CtrlRegTwo_MODS_LPWR;
			break;

		default:
			ASSERT(false, ErrorLedMode_SOFTWARE);
	}

	/* set the interrupt polarity as active high */
	_ctrlRegThree |= CtrlRegThree_IPOL;

	/* enable the data ready interrupt */
	_ctrlRegFour |= CtrlRegFour_INT_EN_DRDY;

	/* route the data ready interrupt to INT0 */
	_ctrlRegFive |= CtrlRegFive_INT_CFG_DRDY;

	/* make sure the accelerometer is in idle mode or the settings won't take */
	systemStatus = _AccReadSingleRegister(AccReg_SYSMOD);

	if ((systemStatus & SysMod_SYSMOD) == SysMod_STNDBY)
	{
		/* now send the configuration settings */
		i2cResult = _AccWriteSingleRegister(AccReg_XYZ_DATA_CONFIG, _xyzDataConfig);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		i2cResult = _AccWriteSingleRegister(AccReg_CTRL_REG1, _ctrlRegOne);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		i2cResult = _AccWriteSingleRegister(AccReg_CTRL_REG2, _ctrlRegTwo);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		i2cResult = _AccWriteSingleRegister(AccReg_CTRL_REG3, _ctrlRegThree);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		i2cResult = _AccWriteSingleRegister(AccReg_CTRL_REG4, _ctrlRegFour);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		i2cResult = _AccWriteSingleRegister(AccReg_CTRL_REG5, _ctrlRegFive);
		ASSERT(i2cResult == I2cError_OK, ErrorLedMode_SOFTWARE);

		success = true;
	}

	_configured = true;

	return success;
}

/***************************************************************************//**
    Starts the accelerometer sampling at its configured data rate.
*//****************************************************************************/
void Accelerometer_Start(void)
{
	if (_configured == false)
	{
		DEBUG("acc not conf");
	}

	I2cError_t i2cError = I2cError_OK;
	static uint8_t dummyData[BYTES_PER_SAMPLE] = {0};

	/* make sure the accelerometer is initialized */
	ASSERT(_initialized, ErrorLedMode_SOFTWARE);

	/* put the accelerometer in standby mode to make sure the settings take */
	i2cError = _AccWriteSingleRegister(AccReg_CTRL_REG1, _ctrlRegOne);
	ASSERT(i2cError == I2cError_OK, ErrorLedMode_SOFTWARE);

	/* read out the current data registers to clear the data ready */
	_AccReadMultipleRegisters(AccReg_OUT_X_MSB, BYTES_PER_SAMPLE, dummyData);

	/* set the interrupt to trigger when data is ready */
	_active = true;
	GPIO_INT_ENABLE(ACCELEROMETER_INTERRUPT_0_PORT, ACCELEROMETER_INTERRUPT_0_PIN);

	/* set the accelerometer to active mode */
	_ctrlRegOne |= CtrlRegOne_ACTIVE;

	/* write the active settings to the magnetometer */
	i2cError = _AccWriteSingleRegister(AccReg_CTRL_REG1, _ctrlRegOne);
	ASSERT(i2cError == I2cError_OK, ErrorLedMode_SOFTWARE);
}

/***************************************************************************//**
    Stops the data collecting of the accelerometer.
*//****************************************************************************/
void Accelerometer_Stop(void)
{
	_active = false;

	/* clear the data ready interrupt trigger */
	GPIO_INT_DISABLE(ACCELEROMETER_INTERRUPT_0_PORT, ACCELEROMETER_INTERRUPT_0_PIN);

	/* put the accelerometer into standby */
	_ctrlRegOne &= ~CtrlRegOne_ACTIVE;

	/* write the settings to the accelerometer */
	_AccWriteSingleRegister(AccReg_CTRL_REG1, _ctrlRegOne);
}

/***************************************************************************//**
    Triggers the accelerometer to take a sample and block until the sample
    is returned.

    \param[out] sample - the result of the accelerometer sample.
*//****************************************************************************/
void Accelerometer_TriggerSampleBlocking(AccSample_t* sample)
{
	bool restart = false;

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	/* if the accelerometer is active, stop it and re-trigger it so that the
	   sample will be fresh.
	*/
	if (_active)
	{
		Accelerometer_Stop();
		restart = true;
	}

	if (_configured == false)
	{
		Accelerometer_Config(&_parameters);
		DEBUG("configure");
	}

	Accelerometer_Start();
	while(Accelerometer_ReadSample(sample) == false)
	{
		/* do nothing */
	}
	Accelerometer_Stop();

	if (restart)
	{
		Accelerometer_Start();
	}
}

void Accelerometer_SetCalibration(const AccSample_t* calibration)
{
	// TODO: spg - do something here
}

/***************************************************************************//**
    Reads a sample from the accelerometer if a sample was ready.

	\param[out] sample - structure to hold the sample data once it is collected

	\return True if the sample was ready, false otherwise
*//****************************************************************************/
bool Accelerometer_ReadSample(AccSample_t* sample)
{
	bool sampleObtained = false;
	uint8_t sampleArray[BYTES_PER_SAMPLE] = {0u};

	ASSERT(sample != NULL, ErrorLedMode_SOFTWARE);

	/* fast read mode is not supported, so only 14 bit samples are currently supported */
	ASSERT(_parameters.resolution == AccResolution_14bit, ErrorLedMode_SOFTWARE);

	if (_dataReady)
	{
		/* clear the data ready flag before reading the data because the chip
		   interrupt will be cleared when the first byte is read. If the data
		   ready flag is cleared after the first byte is read, it could be a new
		   data ready that is being cleared.
		 */
		_dataReady = false;

		_AccReadMultipleRegisters(AccReg_OUT_X_MSB, BYTES_PER_SAMPLE, sampleArray);
		sample->xData = (int16_t)((((uint16_t)sampleArray[0u])<<8u) + sampleArray[1u]);
		sample->yData = (int16_t)((((uint16_t)sampleArray[2u])<<8u) + sampleArray[3u]);
		sample->zData = (int16_t)((((uint16_t)sampleArray[4u])<<8u) + sampleArray[5u]);
		sampleObtained = true;
	}

	return sampleObtained;
}

/***************************************************************************//**
    Reads a sample from the accelerometer if a sample was ready.

	\param[in]  maxLength - the maximum length of bytes data[] can hold
	\param[out] actualLength - the actual number of bytes written to data[]
	\param[in]  data - an array to hold the data from the accelerometer reading

*//****************************************************************************/
void Accelerometer_ReadSampleArray(
	uint16_t  maxLength,
	uint16_t* actualLength,
	uint8_t   data[])
{
	ASSERT(data != NULL, ErrorLedMode_SOFTWARE);
	ASSERT(maxLength >= BYTES_PER_SAMPLE, ErrorLedMode_SOFTWARE);

	/* fast read mode is not supported, so only 14 bit samples are currently supported */
	ASSERT(_parameters.resolution == AccResolution_14bit, ErrorLedMode_SOFTWARE);

	if (_dataReady)
	{
		/* clear the data ready flag before reading the data because the chip
		   interrupt will be cleared when the first byte is read. If the data
		   ready flag is cleared after the first byte is read, it could be a new
		   data ready that is being cleared.
		 */
		_dataReady = false;

		_AccReadMultipleRegisters(AccReg_OUT_X_MSB, BYTES_PER_SAMPLE, data);
		*actualLength = BYTES_PER_SAMPLE;
	}
	else
	{
		*actualLength = 0u;
	}
}

/***************************************************************************//**
    Sets a function that will be ran when the data ready interrupt occurs.

	\param[in] isr - pointer to the accelerometer data ready ISR

*//****************************************************************************/
void Accelerometer_RegisterIsr(AccIsrPointer_t isr)
{
	ASSERT(_accIsrPointer == NULL, ErrorLedMode_SOFTWARE);

	_accIsrPointer = isr;
}


/***************************************************************************//**
    Clears the custom ISR for the accelerometer.
*//****************************************************************************/
void Accelerometer_UnregisterIsr(void)
{
	_accIsrPointer = NULL;
}


/***************************************************************************//**
    Handles the GPIO event that indicates that data is ready on the accelerometer.
*//****************************************************************************/
static void _HandleDataReadyInterrupt(void)
{
	_dataReady = true;
	/* call the accelerometer data ready interrupt handler */
	if (_accIsrPointer != NULL )
	{
		(*_accIsrPointer)();
	}
}

/***************************************************************************//**
    Handles the GPIO event that indicates that a special event on the
    accelerometer.
*//****************************************************************************/
void _HandleOtherInterrupt(void)
{
	// TODO: spg - handle other interrupt.
}

/***************************************************************************//**
    Reads a single register from the accelerometer.

	\param[in] reg- the register to read

	\return The value read from the accelerometer register
*//****************************************************************************/
static uint8_t _AccReadSingleRegister(AccReg_t reg)
{
	uint8_t registerValue = 0;
	I2cError_t i2cSuccess = I2cError_OK;

	/* read in the register */
	I2c_WaitUntilBusIdle(ACC_PORT);
	i2cSuccess = I2c_TransmitStart(ACC_PORT, ACC_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(ACC_PORT, reg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitStart(ACC_PORT, ACC_ADDRESS, I2cOperation_READ);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	I2c_ReceiveByteEnd(ACC_PORT, &registerValue);
	return registerValue;
}

/***************************************************************************//**
    Reads a multiple consecutive registers from the accelerometer.

	\param[in] startReg - the first register to read of the consecutive registers
	\param[in] numRegisters - The number of consecutive registers to read
	\param[in] registers[] - the array in which the register values will be stored.

	\note Make sure that registers[] has at least numRegisters elements to hold
	      the register data.
*//****************************************************************************/
static void _AccReadMultipleRegisters(AccReg_t startReg,
                                   uint8_t numRegisters,
                                   uint8_t registers[])
{
	I2cError_t i2cSuccess = I2cError_OK;
	volatile int16_t regNum;

	ASSERT((registers != NULL) && (numRegisters > 0u), ErrorLedMode_SOFTWARE);

	I2c_WaitUntilBusIdle(ACC_PORT);
	i2cSuccess = I2c_TransmitStart(ACC_PORT, ACC_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(ACC_PORT, startReg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitStart(ACC_PORT, ACC_ADDRESS, I2cOperation_READ);

	for(regNum = 0; regNum < numRegisters; regNum++)
	{
		if (regNum < numRegisters - 1)
		{
			I2c_ReceiveByte(ACC_PORT, &registers[regNum]);
		}
		else
		{
			I2c_ReceiveByteEnd(ACC_PORT, &registers[regNum]);
		}
	}
}

/***************************************************************************//**
    Writes a single register to the accelerometer.

	\param[in] reg - the register to write to
	\param[in] value - The value to write to the register

	\return I2cError_OK if the register could be written successfully. Otherwise
	        return an I2cError.
*//****************************************************************************/
static I2cError_t _AccWriteSingleRegister(AccReg_t reg,
                                       uint8_t value)
{
	I2cError_t i2cSuccess = I2cError_OK;

	/* write the magnetometer register */
	I2c_WaitUntilBusIdle(ACC_PORT);
	i2cSuccess = I2c_TransmitStart(ACC_PORT, ACC_ADDRESS, I2cOperation_WRITE);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByte(ACC_PORT, reg);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	i2cSuccess = I2c_TransmitByteEnd(ACC_PORT, value);
	ASSERT(i2cSuccess == I2cError_OK, ErrorLedMode_SOFTWARE);

	return i2cSuccess;
}
