/***************************************************************************//**
    \file Board.h
    \brief Board initialization module header file

    \b Project 1814 IOLab Remote Sensor Application

    \b Description This header file contains definitions of the MSP430 pins
       being used.  These pins are initialized in Gpio.c.

*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Drivers/Micro/Board.h$
    $Revision: 6$
    $Date: Thursday, September 13, 2012 11:50:50 AM$
    $Author: spgreene$

    Copyright (C) 2012 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/
#ifndef BOARD_H_
#define BOARD_H_


/*-----------------------PUBLIC DEFINITIONS-----------------------------------*/
/** \name Button/switch pins
    \{
*/
#define BUTTON_0_PORT						8
#define BUTTON_0_PIN_NUM					0
#define BUTTON_0_PIN						((uint8_t)(1u << BUTTON_0_PIN_NUM))

#define BUTTON_1_PORT						8
#define BUTTON_1_PIN_NUM					2
#define BUTTON_1_PIN						((uint8_t)(1u << BUTTON_1_PIN_NUM))
/** \} */

/** \name I2C bus pins
    \{
*/
#define I2C_SDA_PORT                        3
#define I2C_SDA_PIN_NUM                     0
#define I2C_SDA_PIN                         ((uint8_t)(1u << I2C_SDA_PIN_NUM))

#define I2C_SCL_PORT                        3
#define I2C_SCL_PIN_NUM                     1
#define I2C_SCL_PIN                         ((uint8_t)(1u << I2C_SCL_PIN_NUM))
/** \} */

/** \name Power control pins
    \{
*/
#define POWER_ADC_V1_ENABLE_PORT               1
#define POWER_ADC_V1_ENABLE_PIN_NUM            4
#define POWER_ADC_V1_ENABLE_PIN                ((uint8_t)(1u << POWER_ADC_V1_ENABLE_PIN_NUM))

#define POWER_ULTRASONIC_ENABLE_PORT        2
#define POWER_ULTRASONIC_ENABLE_PIN_NUM     4
#define POWER_ULTRASONIC_ENABLE_PIN         ((uint8_t)(1u << POWER_ULTRASONIC_ENABLE_PIN_NUM))

#define POWER_ADC_V2_ENABLE_PORT            3
#define POWER_ADC_V2_ENABLE_PIN_NUM         7
#define POWER_ADC_V2_ENABLE_PIN             ((uint8_t)(1u << POWER_ADC_V2_ENABLE_PIN_NUM))

#define POWER_LATCH_PORT                    U
#define POWER_LATCH_PIN_NUM                 0
#define POWER_LATCH_PIN                     ((uint8_t)(1u << POWER_LATCH_PIN_NUM))

#define POWER_FORCE_GAUGE_ENABLE_PORT       6
#define POWER_FORCE_GAUGE_ENABLE_PIN_NUM    3
#define POWER_FORCE_GAUGE_ENABLE_PIN        ((uint8_t)(1u << POWER_FORCE_GAUGE_ENABLE_PIN_NUM))

#define POWER_ENCODER_ENABLE_PORT           U
#define POWER_ENCODER_ENABLE_PIN_NUM        1
#define POWER_ENCODER_ENABLE_PIN            ((uint8_t)(1u << POWER_ENCODER_ENABLE_PIN_NUM))
/** \} */

/** \name Oscillator pins
    \{
*/
#define OSCILLATOR_IN_PORT                  5
#define OSCILLATOR_IN_PIN_NUM               2
#define OSCILLATOR_IN_PIN                   ((uint8_t)(1u << OSCILLATOR_IN_PIN_NUM))

#define OSCILLATOR_OUT_PORT                 5
#define OSCILLATOR_OUT_PIN_NUM              3
#define OSCILLATOR_OUT_PIN                  ((uint8_t)(1u << OSCILLATOR_OUT_PIN_NUM))
/** \} */

/** \name Debug UART interface pins
    \{
*/
#define DEBUG_UART_TX_PORT                  4
#define DEBUG_UART_TX_PIN_NUM               4
#define DEBUG_UART_TX_PIN                   ((uint8_t)(1u << DEBUG_UART_TX_PIN_NUM))

#define DEBUG_UART_RX_PORT                  4
#define DEBUG_UART_RX_PIN_NUM               5
#define DEBUG_UART_RX_PIN                   ((uint8_t)(1u << DEBUG_UART_RX_PIN_NUM))
/** \} */

/** \name LED Output Pins
    \{
*/
#define LED0_RED_PORT                       7
#define LED0_RED_PIN_NUM                    4
#define LED0_RED_PIN                        ((uint8_t)(1u << LED0_RED_PIN_NUM))

#define LED0_GREEN_PORT                     7
#define LED0_GREEN_PIN_NUM                  5
#define LED0_GREEN_PIN                      ((uint8_t)(1u << LED0_GREEN_PIN_NUM))

#define LED1_RED_PORT                       7
#define LED1_RED_PIN_NUM                    6
#define LED1_RED_PIN                        ((uint8_t)(1u << LED1_RED_PIN_NUM))

#define LED1_GREEN_PORT                     7
#define LED1_GREEN_PIN_NUM                  7
#define LED1_GREEN_PIN                      ((uint8_t)(1u << LED1_GREEN_PIN_NUM))
/** \} */

/** \name External Header I/O Pins
    \{
*/
#define EXT_HEADER_0_PORT                   5
#define EXT_HEADER_0_PIN_NUM                4
#define EXT_HEADER_0_PIN                    ((uint8_t)(1u << EXT_HEADER_0_PIN_NUM))

#define EXT_HEADER_1_PORT                   5
#define EXT_HEADER_1_PIN_NUM                5
#define EXT_HEADER_1_PIN                    ((uint8_t)(1u << EXT_HEADER_1_PIN_NUM))

#define EXT_HEADER_2_PORT                   1
#define EXT_HEADER_2_PIN_NUM                6
#define EXT_HEADER_2_PIN                    ((uint8_t)(1u << EXT_HEADER_2_PIN_NUM))

#define EXT_HEADER_3_PORT                   3
#define EXT_HEADER_3_PIN_NUM                5
#define EXT_HEADER_3_PIN                    ((uint8_t)(1u << EXT_HEADER_3_PIN_NUM))

#define EXT_HEADER_4_PORT                   3
#define EXT_HEADER_4_PIN_NUM                6
#define EXT_HEADER_4_PIN                    ((uint8_t)(1u << EXT_HEADER_4_PIN_NUM))

#define EXT_HEADER_5_PORT                   4
#define EXT_HEADER_5_PIN_NUM                6
#define EXT_HEADER_5_PIN                    ((uint8_t)(1u << EXT_HEADER_5_PIN_NUM))

#define EXT_HEADER_6_PORT                   6
#define EXT_HEADER_6_PIN_NUM                0
#define EXT_HEADER_6_PIN                    ((uint8_t)(1u << EXT_HEADER_6_PIN_NUM))

#define EXT_HEADER_7_PORT                   6
#define EXT_HEADER_7_PIN_NUM                1
#define EXT_HEADER_7_PIN                    ((uint8_t)(1u << EXT_HEADER_7_PIN_NUM))

#define EXT_HEADER_8_PORT                   6
#define EXT_HEADER_8_PIN_NUM                2
#define EXT_HEADER_8_PIN                    ((uint8_t)(1u << EXT_HEADER_8_PIN_NUM))
/** \} */

/** \name SPI master pins (sensor interface)
    \{
*/
#define SPI_MASTER_MOSI_PORT                3
#define SPI_MASTER_MOSI_PIN_NUM             3
#define SPI_MASTER_MOSI_PIN                 ((uint8_t)(1u << SPI_MASTER_MOSI_PIN_NUM))

#define SPI_MASTER_MISO_PORT                3
#define SPI_MASTER_MISO_PIN_NUM             4
#define SPI_MASTER_MISO_PIN                 ((uint8_t)(1u << SPI_MASTER_MISO_PIN_NUM))

#define SPI_MASTER_CLK_PORT                 2
#define SPI_MASTER_CLK_PIN_NUM              7
#define SPI_MASTER_CLK_PIN                  ((uint8_t)(1u << SPI_MASTER_CLK_PIN_NUM))

#define CS_BAROMETER_PORT                   8
#define CS_BAROMETER_PIN_NUM                1
#define CS_BAROMETER_PIN                    ((uint8_t)(1u << CS_BAROMETER_PIN_NUM))

#define CS_GYRO_PORT                        3
#define CS_GYRO_PIN_NUM                     2
#define CS_GYRO_PIN                         ((uint8_t)(1u << CS_GYRO_PIN_NUM))

#define CS_DAC_PORT                         4
#define CS_DAC_PIN_NUM                      7
#define CS_DAC_PIN                          ((uint8_t)(1u << CS_DAC_PIN_NUM))
/** \} */

/** \name SPI slave pins (RF micro interface)
    \{
*/
#define SPI_SLAVE_MOSI_PORT                 4
#define SPI_SLAVE_MOSI_PIN_NUM              1
#define SPI_SLAVE_MOSI_PIN                  ((uint8_t)(1u << SPI_SLAVE_MOSI_PIN_NUM))

#define SPI_SLAVE_MISO_PORT                 4
#define SPI_SLAVE_MISO_PIN_NUM              2
#define SPI_SLAVE_MISO_PIN                  ((uint8_t)(1u << SPI_SLAVE_MISO_PIN_NUM))

#define SPI_SLAVE_CLK_PORT                  4
#define SPI_SLAVE_CLK_PIN_NUM               3
#define SPI_SLAVE_CLK_PIN                   ((uint8_t)(1u << SPI_SLAVE_CLK_PIN_NUM))

#define CS_RADIO_PORT                       4
#define CS_RADIO_PIN_NUM                    0
#define CS_RADIO_PIN                        ((uint8_t)(1u << CS_RADIO_PIN_NUM))
/** \} */

/** \name Analog Input Pins
    \{
*/
#define ADC_FORCE_GAUGE_PORT                6
#define ADC_FORCE_GAUGE_PIN_NUM             4
#define ADC_FORCE_GAUGE_PIN                 ((uint8_t)(1u << ADC_FORCE_GAUGE_PIN_NUM))

#define ADC_ECG_1_PORT                      6
#define ADC_ECG_1_PIN_NUM                   5
#define ADC_ECG_1_PIN                       ((uint8_t)(1u << ADC_ECG_1_PIN_NUM))

#define ADC_ECG_2_PORT                      6
#define ADC_ECG_2_PIN_NUM                   6
#define ADC_ECG_2_PIN                       ((uint8_t)(1u << ADC_ECG_2_PIN_NUM))

#define ADC_ECG_3_PORT                      6
#define ADC_ECG_3_PIN_NUM                   7
#define ADC_ECG_3_PIN                       ((uint8_t)(1u << ADC_ECG_3_PIN_NUM))

#define ADC_LIGHT_SENSOR_PORT               7
#define ADC_LIGHT_SENSOR_PIN_NUM            0
#define ADC_LIGHT_SENSOR_PIN                ((uint8_t)(1u << ADC_LIGHT_SENSOR_PIN_NUM))

#define ADC_HIGH_GAIN_PORT                  7
#define ADC_HIGH_GAIN_PIN_NUM               1
#define ADC_HIGH_GAIN_PIN                   ((uint8_t)(1u << ADC_HIGH_GAIN_PIN_NUM))

#define ADC_MICROPHONE_PORT                 7
#define ADC_MICROPHONE_PIN_NUM              2
#define ADC_MICROPHONE_PIN                  ((uint8_t)(1u << ADC_MICROPHONE_PIN_NUM))

#define ADC_BATTERY_PORT                    7
#define ADC_BATTERY_PIN_NUM                 3
#define ADC_BATTERY_PIN                     ((uint8_t)(1u << ADC_BATTERY_PIN_NUM))
/** \} */

/** \name Ultrasonic transducer pins
    \{
*/
#define ULTRASONIC_DIRECTION_PORT           1
#define ULTRASONIC_DIRECTION_PIN_NUM        0
#define ULTRASONIC_DIRECTION_PIN            ((uint8_t)(1u << ULTRASONIC_DIRECTION_PIN_NUM))

#define ULTRASONIC_CAPTURE_END_PORT         2
#define ULTRASONIC_CAPTURE_END_PIN_NUM      0
#define ULTRASONIC_CAPTURE_END_PIN          ((uint8_t)(1u << ULTRASONIC_CAPTURE_END_PIN_NUM))

#define ULTRASONIC_CAPTURE_BEGIN_PORT       2
#define ULTRASONIC_CAPTURE_BEGIN_PIN_NUM    1
#define ULTRASONIC_CAPTURE_BEGIN_PIN        ((uint8_t)(1u << ULTRASONIC_CAPTURE_BEGIN_PIN_NUM))

#define ULTRASONIC_OUTPUT_PORT              5
#define ULTRASONIC_OUTPUT_PIN_NUM           6
#define ULTRASONIC_OUTPUT_PIN               ((uint8_t)(1u << ULTRASONIC_OUTPUT_PIN_NUM))
/** \} */

/** \name Quadrature encoder input Pins
    \{
*/
#define ENCODER_INPUT_DIRECTION_PORT        2
#define ENCODER_INPUT_DIRECTION_PIN_NUM     3
#define ENCODER_INPUT_DIRECTION_PIN         ((uint8_t)(1u << ENCODER_INPUT_DIRECTION_PIN_NUM))

#define ENCODER_INPUT_COUNTER_PORT          2
#define ENCODER_INPUT_COUNTER_PIN_NUM       2
#define ENCODER_INPUT_COUNTER_PIN           ((uint8_t)(1u << ENCODER_INPUT_COUNTER_PIN_NUM))
/** \} */

/** \name Accelerometer Interrupt Pins
    \{
*/
#define ACCELEROMETER_INTERRUPT_0_PORT      1
#define ACCELEROMETER_INTERRUPT_0_PIN_NUM   5
#define ACCELEROMETER_INTERRUPT_0_PIN       ((uint8_t)(1u << ACCELEROMETER_INTERRUPT_0_PIN_NUM))

#define ACCELEROMETER_INTERRUPT_1_PORT      1
#define ACCELEROMETER_INTERRUPT_1_PIN_NUM   7
#define ACCELEROMETER_INTERRUPT_1_PIN       ((uint8_t)(1u << ACCELEROMETER_INTERRUPT_1_PIN_NUM))
/** \} */

/** \name Magnetometer Interrupt Pin
    \{
*/
#define MAGNETOMETER_INTERRUPT_PORT         1
#define MAGNETOMETER_INTERRUPT_PIN_NUM      3
#define MAGNETOMETER_INTERRUPT_PIN          ((uint8_t)(1u << MAGNETOMETER_INTERRUPT_PIN_NUM))
/** \} */

/** \name Gyroscope Interrupt Pins
    \{
*/
#define GYRO_INTERRUPT_0_PORT               2
#define GYRO_INTERRUPT_0_PIN_NUM            5
#define GYRO_INTERRUPT_0_PIN                ((uint8_t)(1u << GYRO_INTERRUPT_0_PIN_NUM))

#define GYRO_INTERRUPT_1_PORT               2
#define GYRO_INTERRUPT_1_PIN_NUM            6
#define GYRO_INTERRUPT_1_PIN                ((uint8_t)(1u << GYRO_INTERRUPT_1_PIN_NUM))
/** \} */

/** \name Buzzer Output Pin
    \{
*/
#define BUZZER_OUTPUT_PORT                  5
#define BUZZER_OUTPUT_PIN_NUM               7
#define BUZZER_OUTPUT_PIN                   ((uint8_t)(1u << BUZZER_OUTPUT_PIN_NUM))
/** \} */

/** \name Boot Strap Loader pins
    \{
*/
#define BSL_TX_PORT                         1
#define BSL_TX_PIN_NUM                      1
#define BSL_TX_PIN                          ((uint8_t)(1u << BSL_TX_PIN_NUM))

#define BSL_RX_PORT                         1
#define BSL_RX_PIN_NUM                      2
#define BSL_RX_PIN                          ((uint8_t)(1u << BSL_RX_PIN_NUM))
/** \} */
/*-----------------------PUBLIC VARIABLES-------------------------------------*/

/*-----------------------PUBLIC FUNCTION PROTOTYPES---------------------------*/
void Board_Init(void);

#endif /* BOARD_H_ */
