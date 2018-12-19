/***************************************************************************//**
    \file SelfTest.h
    \brief Manages the self test mode.

    \b Description Handles the sequencing, configuration, and data management
                   for the self test mode.
*//****************************************************************************/
/*******************************************************************************
    Surround SCM Information
    $Archive: Macmillan_1814_IOLab/Code/RemoteSensorProcessor/priv/Applications/IOLabRemoteSensor/src/Application/SelfTest.c$
    $Revision: 6$
    $Date: Friday, July 17, 2015 2:21:18 AM$
    $Author: spgreene$

    Copyright (C) 2014 Indesign LLC  (Indesign Coding Standard Revision 3)

*******************************************************************************/

#include <ProjectTypes.h>
#include <SelfTest.h>
#include <EventQueue.h>
#include <Board.h>
#include <Gpio.h>
#include <AdcManager.h>
#include <LedControl.h>
#include <BatteryControl.h>
#include <Barometer.h>
#include <Debug.h>
#include <Serial.h>
#include <Assert.h>
#include <TimerTick.h>
#include <FixedConfigSettings.h>
#include <CommProtocol.h>
#include <DataBuffer.h>
#include <FixedConfigList.h>
#include <ConfigurationManager.h>
#include <Conversion.h>
#include <UiManager.h>
#include <Clock.h>

/*-----------------------LOCAL DEFINITIONS------------------------------------*/

/** Define an enum that contains the possible states for a self-test to be in */
typedef enum SelfTestStates
{
    SelfTestState_NOT_STARTED = 0u,
    SelfTestState_IN_PROGRESS,
    SelfTestState_PASSED,
    SelfTestState_FAILED
} SelfTestState_t;

/** Defines the sensors that the self test must complete to be finished. */
typedef enum SelfTestType
{
    SelfTestType_INVALID        = -1,
    SelfTestType_FIRST          = 0u,
    SelfTestType_DIGITAL  = SelfTestType_FIRST,
    SelfTestType_BUZZER,
    SelfTestType_ANALOG,
    SelfTestType_BATTERY,
    SelfTestType_HIGH_GAIN,
    SelfTestType_THERMOMETER,
    SelfTestType_BAROMETER,
    SelfTestType_MICROPHONE,
    SelfTestType_AMBIENT_LIGHT,
    SelfTestType_FORCE_GAUGE,
    SelfTestType_ACCELEROMETER,
    SelfTestType_GYROSCOPE,
    SelfTestType_MAGNETOMETER,
    SelfTestType_WHEEL,
    SelfTestType_LAST
} SelfTestType_t;

/** Defines the pass criteria for the ADC tests. */
typedef enum AdcTestTypes
{
    AdcTestType_IN_RANGE,
    AdcTestType_NOT_IN_RANGE
} AdcTestType_t;

/** Defines a structure to access the contents of a self-test data packet.  This structure
    must always be aligned with the self-test buffer structure or errors will occur. */
#if BUFFER_SIZE_SELFTEST != 16u
    /* This won't cover all errors, but should catch some.  */
    #error "Self-test buffer changed and the SelfTest_t struct must also be updated."
#endif
typedef struct SelfTest
{
    uint8_t numberOfBytes;
    uint8_t numberOfSensors;
    uint8_t magnetometerId;
    uint8_t magnetometerLength;
    uint8_t magnetometerData[12u];
    uint8_t accelerometerId;
    uint8_t accelerometerLength;
    uint8_t accelerometerData[12u];
    uint8_t encoderId;
    uint8_t encoderLength;
    uint8_t encoderData[8u];
    uint8_t forceGaugeId;
    uint8_t forceGaugeLength;
    uint8_t forceGaugeData[4u];
    uint8_t gyroscopeId;
    uint8_t gyroscopeLength;
    uint8_t gyroscopeData[12u];
    uint8_t ambientLightId;
    uint8_t ambientLightLength;
    uint8_t ambientLightData[4u];
    uint8_t microphoneId;
    uint8_t microphoneLength;
    uint8_t microphoneData[4u];
} SelfTest_t;

/** Define a structure that can contain/point to the data in a three-axis sensor (accel, mag, gyro) */
typedef struct ThreeAxisSensorData
{
    int16_t xData;
    int16_t yData;
    int16_t zData;
} ThreeAxisSensorData_t;

/** Defines the minimum threshold for the ADC high */
#define ADC_HIGH_MINIMUM    ( 0x0FF0 )
/** Defines the maximum threshold for the ADC low */
#define ADC_LOW_MAXIMUM     ( 0x000F )
/** Defines how many samples the ADC should take to determine a pass */
#define ADC_NUM_SAMPLES     ( 4u )

/** Defines the range that the data must span for the ambient light sensor to pass */
#define AMBIENT_LIGHT_RANGE ( 16u )
/** Defines the range that the data must span for the force gauge sensor to pass */
#define FORCE_GAUGE_RANGE   ( 32u )
/** Defines the range that the data must span for the encoder to pass */
#define ENCODER_RANGE       ( 16u )
/** Defines the range that the data must span for the magnetometer sensor to pass */
#define MAGNETOMETER_RANGE  ( 128 )
/** Defines the range that the data must span for the gyroscope sensor to pass */
#define GYROSCOPE_RANGE     ( 1024 )
/** Defines the range that the data must span for the acceleromter sensor to pass */
#define ACCELEROMETER_RANGE ( 512 )
/** Defines the range that the data must span for hte microphone sensor to pass */
#define MICROPHONE_RANGE    ( 0x0040u )
/** Defines the value that the microphone settles around */
#define MICROPHONE_SETTLING_VALUE     ( 0x0FFFu / 2u )
/** Defines the amplitude tolerance that the microphone must settle within. */
#define MICROPHONE_SETTLING_TOLERANCE ( 0x00F0u )
/** Defines the number of samples the settling must remain within tolerance */
#define MICROPHONE_SETTLING_SAMPLES   ( 4u )

/** Defines the time between ADC samples */
#define TIME_BETWEEN_ADC_SAMPLES_MS ( 10u )

/** Defines the maximum time that the kitchsen sink mode of the self-test should take */
#define SELF_TEST_CONTINUOUS_TIMEOUT_MS ( 10000ul )

/** Defines the maximum number of samples to collect to verify if a sensor is good or not.
    Set the duration to be 4 seconds maximum */
#define MAXIMUM_NUM_SAMPLES (SELF_TEST_CONTINUOUS_TIMEOUT_MS / TIME_BETWEEN_ADC_SAMPLES_MS)

/*-----------------------LOCAL CONSTANTS--------------------------------------*/

/*-----------------------LOCAL VARIABLES--------------------------------------*/

/** Stores the status of each of the self tests */
static SelfTestState_t _testStatus[SelfTestType_LAST] = { SelfTestState_NOT_STARTED };

/** Keeps track of the current type that is running */
static SelfTestType_t _currentTestType = SelfTestType_INVALID;

/** Tracks if the kitchsen sink data streaming is active */
static bool _selfTestContinuousActive = false;

/** Holds the timer handle for a timer tick */
TickTimeoutHandle_t _tickHandle;

/*-----------------------LOCAL FUNCTION PROTOTYPES----------------------------*/
static void _SelfTest_ProcessContinuous(void);
static void _SelfTestComplete(void);
static bool _AllTestsInState(
        SelfTestState_t state,
        const SelfTestState_t status[],
        uint16_t statusLength);
static bool _AnyTestsInState(
        SelfTestState_t state,
        const SelfTestState_t status[],
        uint16_t statusLength);
static SelfTestState_t _TotalTestState(
        const SelfTestState_t status[],
        uint16_t statusLength);

static SelfTestState_t _TestDigitalInputs(void);
static bool _TestDigitalInput(
        int16_t port,
        uint8_t pinMask);
static SelfTestState_t _TestAnalogInputs(void);
static bool _TestAnalogInput(
        AdcManChannel_t channel,
        uint16_t maxValue,
        uint16_t minValue,
        AdcTestType_t type,
        uint16_t *value);
static SelfTestState_t _TestBattery(void);
static SelfTestState_t _TestHighGain(void);
static SelfTestState_t _TestThermometer(void);
static SelfTestState_t _TestBarometer(void);
static SelfTestState_t _TestAmbientLight(
        const uint16_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestMicrophone(
        const uint16_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestForceGauge(
        const uint16_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestMagnetometer(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestAccelerometer(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestGyroscope(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength);
static SelfTestState_t _TestEncoder(
        const int16_t data[],
        uint16_t dataLength);

/***************************************************************************//**
    Initializes the self-test module.
*//****************************************************************************/
void SelfTest_Init(void)
{
    UiManager_Init();
}

/***************************************************************************//**
    Starts the self-test.  Sets all of the necessary configurations for the test
    to begin.
*//****************************************************************************/
void SelfTest_Start(void)
{
    FixedConfigList_t config = FixedConfigList_SELFTEST;
    ConMan_SetFixedConfig(sizeof(config), (uint8_t*)&config);
    /* start the tests that can be performed without needing to run the
       configuration manager     */
    LedControl_SetLed(Led_0_GREEN, LedState_BLINK_SLOW_EVEN);
    LedControl_SetLed(Led_0_RED, LedState_BLINK_SLOW_ODD);
    LedControl_SetLed(Led_1_GREEN, LedState_BLINK_SLOW_ODD);
    LedControl_SetLed(Led_1_RED, LedState_BLINK_SLOW_EVEN);

    /* start the UI */
    UiManager_PlaySequence(UiSequence_IN_PROGRESS, PlayMode_REPEAT);
    /* Start the first test */
    _currentTestType = SelfTestType_FIRST;
    EventQueue_Add(Event_SELF_TEST, EventPriority_NORMAL);
}

/***************************************************************************//**
    Processes the data received during the self test.
*//****************************************************************************/
void SelfTest_Process(void)
{
    SelfTestState_t testState = SelfTestState_NOT_STARTED;
    bool selfTestComplete = false;

    if (_selfTestContinuousActive == false)
    {
        switch(_currentTestType)
        {
            case SelfTestType_ANALOG:
                testState = _TestAnalogInputs();
                break;

            case SelfTestType_BAROMETER:
                testState = _TestBarometer();
                break;

            case SelfTestType_BATTERY:
                testState = _TestBattery();
                break;

            case SelfTestType_BUZZER:
                /* TODO: turn on buzzer */
                /* The buzzer cannot be programmatically tested, so assume it passed */
                testState = SelfTestState_PASSED;
                break;

            case SelfTestType_DIGITAL:
                testState = _TestDigitalInputs();
                break;

            case SelfTestType_HIGH_GAIN:
                testState = _TestHighGain();
                break;

            case SelfTestType_THERMOMETER:
                testState = _TestThermometer();
                break;

            case SelfTestType_LAST:
                CommProtocol_HandleStopDataPacket(NULL, 0);
                selfTestComplete = true;
                _SelfTestComplete();
                break;

            default:
                /* enter continuous test state */
                ASSERT(_selfTestContinuousActive == false, ErrorLedMode_SOFTWARE);
                _testStatus[SelfTestType_MAGNETOMETER] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_ACCELEROMETER] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_WHEEL] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_FORCE_GAUGE] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_GYROSCOPE] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_AMBIENT_LIGHT] = SelfTestState_IN_PROGRESS;
                _testStatus[SelfTestType_MICROPHONE] = SelfTestState_IN_PROGRESS;
                AdcManager_PoweredChannelControl(AdcManChannel_MICROPHONE, true);
                _selfTestContinuousActive = true;
                /* Handle start data packet will kick off the data acquision for continuous test */
                CommProtocol_HandleStartDataPacket(NULL, 0);
                _tickHandle = TimerTick_RequestTimeout(TickTimeoutType_REPEAT, 10u, _SelfTest_ProcessContinuous);
                break;
        }

        if (_selfTestContinuousActive == false)
        {
            _testStatus[_currentTestType] = testState;
            /* if the test is complete, go to the next test. */
            if (testState == SelfTestState_PASSED)
            {
                Serial_SendStringNumber("Test passed ", _currentTestType);
                ++_currentTestType;
            }
            else if (testState == SelfTestState_FAILED)
            {
                Serial_SendStringNumber("Test failed ", _currentTestType);
                /* if the test failed, skip to the last test */
                _currentTestType = SelfTestType_LAST;
            }
            else
            {
                /* do nothing */
            }

            /* Some tests need to manage their own timing when they are in progress.
               Only enqueue an event to process the self-test when the current test
               is not in progress.      */
            if ((testState != SelfTestState_IN_PROGRESS) &&
                (selfTestComplete == false))
            {
                /* Enqueue the event to handle the next test */
                EventQueue_Add(Event_SELF_TEST, EventPriority_LOWEST);
            }
        }
        else
        {
            DEBUG("continuous true");
        }
    }
    else
    {
        DEBUG("shouldn't get here in continuous, oops");
    }
}

/***************************************************************************//**
    Handles the processing of the data when in the continuous state of the
    test.
*//****************************************************************************/
static void _SelfTest_ProcessContinuous(void)
{
    SelfTestState_t totalState;
    uint16_t bufferLength;
    SelfTest_t *continousSelfTest;
    ThreeAxisSensorData_t *data;
    uint16_t lengthOfData;
    static uint16_t iterationCounter = 0u;

    ++iterationCounter;
    CommProtocol_TxHandler();
    DataBuffer_GetIdleBuffer(&bufferLength, (uint8_t**)&continousSelfTest);

    if (_testStatus[SelfTestType_MAGNETOMETER] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            data = (ThreeAxisSensorData_t*)&continousSelfTest->magnetometerData[0u];
            lengthOfData = continousSelfTest->magnetometerLength/sizeof(ThreeAxisSensorData_t);
            _testStatus[SelfTestType_MAGNETOMETER] = _TestMagnetometer(data, lengthOfData);
        }
        else
        {
            DEBUG("timeout mag");
            _testStatus[SelfTestType_MAGNETOMETER] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_ACCELEROMETER] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            data = (ThreeAxisSensorData_t*)&continousSelfTest->accelerometerData[0u];
            lengthOfData = continousSelfTest->accelerometerLength/sizeof(ThreeAxisSensorData_t);
            _testStatus[SelfTestType_ACCELEROMETER] = _TestAccelerometer(data, lengthOfData);
        }
        else
        {
            DEBUG("timeout acc");
            _testStatus[SelfTestType_ACCELEROMETER] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_WHEEL] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            _testStatus[SelfTestType_WHEEL] = _TestEncoder((int16_t*)&continousSelfTest->encoderData[0u],
                                                           continousSelfTest->encoderLength/sizeof(int16_t));
        }
        else
        {
            DEBUG("timeout wheel");
            _testStatus[SelfTestType_WHEEL] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_FORCE_GAUGE] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            _testStatus[SelfTestType_FORCE_GAUGE] = _TestForceGauge((uint16_t*)&continousSelfTest->forceGaugeData[0u],
                                                                    continousSelfTest->forceGaugeLength/sizeof(uint16_t));
        }
        else
        {
            DEBUG("timeout force");
            _testStatus[SelfTestType_FORCE_GAUGE] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_GYROSCOPE] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            data = (ThreeAxisSensorData_t*)&continousSelfTest->gyroscopeData[0u];
            lengthOfData = continousSelfTest->gyroscopeLength/sizeof(ThreeAxisSensorData_t);
            _testStatus[SelfTestType_GYROSCOPE] = _TestGyroscope(data, lengthOfData);
        }
        else
        {
            DEBUG("timeout gyro");
            _testStatus[SelfTestType_GYROSCOPE] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_AMBIENT_LIGHT] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            _testStatus[SelfTestType_AMBIENT_LIGHT] = _TestAmbientLight((uint16_t*)&continousSelfTest->ambientLightData[0u],
                                                                        continousSelfTest->ambientLightLength/sizeof(uint16_t));
        }
        else
        {
            DEBUG("timeout amb");
            _testStatus[SelfTestType_AMBIENT_LIGHT] = SelfTestState_FAILED;
        }
    }

    if (_testStatus[SelfTestType_MICROPHONE] == SelfTestState_IN_PROGRESS)
    {
        if (iterationCounter < MAXIMUM_NUM_SAMPLES)
        {
            _testStatus[SelfTestType_MICROPHONE] = _TestMicrophone((uint16_t*)&continousSelfTest->microphoneData[0u],
                                                                   continousSelfTest->microphoneLength/sizeof(uint16_t));
        }
        else
        {
            DEBUG("timeout mic");
            _testStatus[SelfTestType_MICROPHONE] = SelfTestState_FAILED;
        }
    }

    totalState = _TotalTestState(_testStatus, ARRAY_LENGTH(_testStatus));
    if (totalState == SelfTestState_PASSED ||
        totalState == SelfTestState_FAILED)
    {
        DEBUG_BLOCKING("continuous test done");
        AdcManager_PoweredChannelControl(AdcManChannel_MICROPHONE, false);
        TimerTick_CancelTimeoutFromCallback();
        _selfTestContinuousActive = false;
        _currentTestType = SelfTestType_LAST;
        EventQueue_Add(Event_SELF_TEST, EventPriority_LOWEST);
    }
    else if (totalState != SelfTestState_IN_PROGRESS)
    {
        Serial_SendStringNumber("oops wrong state ", totalState);
    }
    else
    {
        /* do nothing */
    }
}

/***************************************************************************//**
    Handles the necessary clean up from the self-test and outputs the result
*//****************************************************************************/
static void _SelfTestComplete(void)
{
    SelfTestState_t totalState;

    totalState = _TotalTestState(_testStatus, ARRAY_LENGTH(_testStatus));

    UiManager_StopSequence();

    DEBUG("self-test complete");

    if (totalState == SelfTestState_PASSED)
    {
        UiManager_PlaySequence(UiSequence_SUCCESS, PlayMode_SINGLE);
        LedControl_SetLed(Led_0_GREEN, LedState_BLINK_1);
        LedControl_SetLed(Led_1_GREEN, LedState_WINK_1);
        LedControl_SetLed(Led_0_RED,   LedState_OFF   );
        LedControl_SetLed(Led_1_RED,   LedState_OFF   );
    }
    else if (totalState == SelfTestState_FAILED)
    {
        UiManager_PlaySequence(UiSequence_FAIL, PlayMode_SINGLE);
        LedControl_SetLed(Led_0_GREEN, LedState_OFF   );
        LedControl_SetLed(Led_1_GREEN, LedState_OFF   );
        LedControl_SetLed(Led_0_RED,   LedState_BLINK_1);
        LedControl_SetLed(Led_1_RED,   LedState_WINK_1);
    }
    else
    {
        /* should not get here, tests should be either passed or failed */
        ASSERT(false, ErrorLedMode_SOFTWARE);
    }
}

/***************************************************************************//**
    Determines if all of the tests are in the specified state.

    \param[in] state  - the specified state of interest
    \param[in] status - array of test states for all of the tests that could be
                        in the specified state
    \param[in] statusLength - the number of elements in the status array.

    \returns True if all of the tests are in the specified state.
*//****************************************************************************/
static bool _AllTestsInState(
        SelfTestState_t state,
        const SelfTestState_t status[],
        uint16_t statusLength)
{
    uint16_t index;
    bool passResult = true;

    for (index = 0u; index < statusLength; ++index)
    {
        if (status[index] != state)
        {
            passResult = false;
            break;
        }
    }

    return passResult;
}

/***************************************************************************//**
    Determines if any of the tests are in the specified state.

    \param[in] state  - the specified state of interest
    \param[in] status - array of test states for all of the tests that could be
                        in the state of interest.
    \param[in] statusLength - the number of elements in the status array.

    \returns True if any of the tests are in the specified state.
*//****************************************************************************/
static bool _AnyTestsInState(
        SelfTestState_t state,
        const SelfTestState_t status[],
        uint16_t statusLength)
{
    uint16_t index;
    bool failResult = false;

    for (index = 0u; index < statusLength; ++index)
    {
        if (status[index] == state)
        {
            failResult = true;
            break;
        }
    }

    return failResult;
}

/***************************************************************************//**
    Computes the state of the total tests.

    \param[in] status - the array of all the statuses
    \param[in] statusLength - the number of elements in teh status array

    \returns The total state for the total of all the tests.
             All tests passed = passed
             Any tests failed = failed
             All tests are not started = not started
             otherwise = in progress
*//****************************************************************************/
static SelfTestState_t _TotalTestState(
        const SelfTestState_t status[],
        uint16_t statusLength)
{
    SelfTestState_t state;

    if (_AllTestsInState(SelfTestState_PASSED, status, statusLength))
    {
        state = SelfTestState_PASSED;
        DEBUG("Self-test passed");
    }
    else if (_AnyTestsInState(SelfTestState_FAILED, status, statusLength))
    {
        state = SelfTestState_FAILED;
        DEBUG("Self-test failed");
    }
    else if (_AllTestsInState(SelfTestState_NOT_STARTED, status, statusLength))
    {
        state = SelfTestState_NOT_STARTED;
        DEBUG("Self-test not started");
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
        DEBUG("Self-test in progress");
    }

    return state;
}

/***************************************************************************//**
    Configures and tests each of the digital inputs.

    \returns The state of the test.  Digital input test will only return pass
             or fail, never in progress.
*//****************************************************************************/
static SelfTestState_t _TestDigitalInputs(void)
{
    SelfTestState_t state;
    bool passResult = true;

    passResult &= _TestDigitalInput(EXT_HEADER_0_PORT, EXT_HEADER_0_PIN);
    passResult &= _TestDigitalInput(EXT_HEADER_1_PORT, EXT_HEADER_1_PIN);
    passResult &= _TestDigitalInput(EXT_HEADER_2_PORT, EXT_HEADER_2_PIN);
    passResult &= _TestDigitalInput(EXT_HEADER_3_PORT, EXT_HEADER_3_PIN);
    passResult &= _TestDigitalInput(EXT_HEADER_4_PORT, EXT_HEADER_4_PIN);
    passResult &= _TestDigitalInput(EXT_HEADER_5_PORT, EXT_HEADER_5_PIN);

    state = passResult ? SelfTestState_PASSED : SelfTestState_FAILED;
    return state;
}

/***************************************************************************//**
    Configures and tests the specified digital input

    \param[in] port - the GPIO port of the digital input to test
    \param[in] pinMask - the pin mask for the digital input to test

    \returns True if the digital input passed the test
*//****************************************************************************/
static bool _TestDigitalInput(
        int16_t port,
        uint8_t pinMask)
{
    bool passResult = true;

    Gpio_Configure(port, pinMask, GpioType_INPUT, GpioValue_DONT_CARE, GpioDriveStrength_DEFAULT);
    Gpio_EnableResistor(port, pinMask, GpioResistorType_PULLUP);
    CLOCK_DELAY_US(200u);

    if (Gpio_GetValue(port, pinMask) == false)
    {
        passResult &= false;
    }

    Gpio_EnableResistor(port, pinMask, GpioResistorType_PULLDOWN);
    CLOCK_DELAY_US(200u);

    if (Gpio_GetValue(port, pinMask) == true)
    {
        passResult &= false;
    }

    return passResult;
}

/***************************************************************************//**
    Configures and tests each of the analog inputs.

    \returns The state of the test.  Returns passed or failed when complete, or
             in progress while it is busy.
*//****************************************************************************/
static SelfTestState_t _TestAnalogInputs(void)
{
    SelfTestState_t state;
    bool passResult = true;
    static uint16_t iteration = 0u;

    Serial_SendString3Numbers("adc limits ", ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, iteration);

    passResult &= _TestAnalogInput(AdcManChannel_HEADER_06, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);
    passResult &= _TestAnalogInput(AdcManChannel_HEADER_07, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);
    passResult &= _TestAnalogInput(AdcManChannel_HEADER_08, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);
    passResult &= _TestAnalogInput(AdcManChannel_ECG_1, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);
    passResult &= _TestAnalogInput(AdcManChannel_ECG_2, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);
    passResult &= _TestAnalogInput(AdcManChannel_ECG_3, ADC_HIGH_MINIMUM, ADC_LOW_MAXIMUM, AdcTestType_IN_RANGE, NULL);

    state = passResult ? SelfTestState_PASSED : SelfTestState_FAILED;

    if ((state != SelfTestState_FAILED) &&
        (iteration < ADC_NUM_SAMPLES))
    {
        ++iteration;
        state = SelfTestState_IN_PROGRESS;
        _tickHandle = TimerTick_RequestTimeout(TickTimeoutType_SINGLE, TIME_BETWEEN_ADC_SAMPLES_MS, SelfTest_Process);
    }

    return state;
}

/***************************************************************************//**
    Configures and tests the specified analog input

    \param[in]  channel  - the ADC manager channel for the analog input
    \param[in]  maxValue - the maximum value that the ADC sample can be to be
                           considered acceptable.
    \param[in]  minValue - the minimum value that the ADC sample can be to be
                          considered acceptable.
    \param[in]  type     - The type of test to use for comparison
    \param[out] value    - the result of the ADC sample.  If a null value is
                           passed in, the result will not be passed out.

    \returns True if the analog input passed the test
*//****************************************************************************/
static bool _TestAnalogInput(
        AdcManChannel_t channel,
        uint16_t maxValue,
        uint16_t minValue,
        AdcTestType_t type,
        uint16_t *value)
{
    uint16_t adcResult;
    bool passResult;

    adcResult = AdcManager_SingleSampleBlocking(channel);

    if (type == AdcTestType_IN_RANGE)
    {
        if ((adcResult < maxValue) &&
            (adcResult > minValue))
        {
            passResult = true;
        }
        else
        {
            passResult = false;
        }
    }
    else
    {
        if ((adcResult > maxValue) ||
            (adcResult < minValue))
        {
            passResult = true;
        }
        else
        {
            passResult = false;
        }
    }

    Serial_SendString3Numbers("adc ", channel, adcResult, type);

    if (value != NULL)
    {
        *value = adcResult;
    }

    return passResult;
}

/***************************************************************************//**
    Configures and tests the battery monitor.

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestBattery(void)
{
    SelfTestState_t state;
    bool success;
    static uint16_t iteration = 0u;

    /* Test the battery near its range.  Maximum of 5.9V = 4027 * 3V * 2gain / 4095.
                                         Minimum of 1.8V = 1228 * 3V * 2gain / 4095     */
    Serial_SendString3Numbers("batt limits ", 4027ul, 1228ul, 0u);
    success = _TestAnalogInput(AdcManChannel_BATTERY, 4027ul, 1228ul, AdcTestType_IN_RANGE, NULL);
    state = success ? SelfTestState_PASSED : SelfTestState_FAILED;

    if ((state != SelfTestState_FAILED) &&
        (iteration < ADC_NUM_SAMPLES))
    {
        ++iteration;
        state = SelfTestState_IN_PROGRESS;
        _tickHandle = TimerTick_RequestTimeout(TickTimeoutType_SINGLE, TIME_BETWEEN_ADC_SAMPLES_MS, SelfTest_Process);
    }

    return state;
}

/***************************************************************************//**
    Configures and tests the high gain input.  The high gain input is referenced
    to Vmid so it should have a very tight margin of values it returns since it
    isn't floating.

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestHighGain(void)
{
    SelfTestState_t state;
    bool success;
    static uint16_t iteration = 0u;

    /* Referenced to Vmid and not floating, only allow for 64 counts variance.  */
    Serial_SendString3Numbers("high gain limits ", 0x0840u, 0x07C0u, 0u);
    success = _TestAnalogInput(AdcManChannel_HIGH_GAIN, 0x0840u, 0x07C0u, AdcTestType_IN_RANGE, NULL);
    state = success ? SelfTestState_PASSED : SelfTestState_FAILED;

    if ((state != SelfTestState_FAILED) &&
        (iteration < ADC_NUM_SAMPLES))
    {
        ++iteration;
        state = SelfTestState_IN_PROGRESS;
        _tickHandle = TimerTick_RequestTimeout(TickTimeoutType_SINGLE, TIME_BETWEEN_ADC_SAMPLES_MS, SelfTest_Process);
    }

    return state;
}

/***************************************************************************//**
    Configures and tests the thermometer.  Make sure it reads a temperature in
    the possible operating range.

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestThermometer(void)
{
    SelfTestState_t state;
    uint16_t adcValue;
    int16_t celsiusTemp;
    static uint16_t iteration = 0u;

    adcValue = AdcManager_SingleSampleBlocking(AdcManChannel_TEMPERATURE);
//    Serial_SendStringNumber("temp count ", adcValue);
    Adc_ApplyCorrection(AdcCorrection_REF_GAIN_OFFSET, 1u, &adcValue, &adcValue);
//    Serial_SendStringNumber("temp corr ", adcValue);
    celsiusTemp = Adc_ConvertValueToTemp(adcValue);

    /* Make sure that the temperature is within the operating spec of the micro
       +85°C to -40°C */
    Serial_SendString3Numbers("temp ", celsiusTemp, 85u, (uint16_t)(-40));

    if ((celsiusTemp > -40) &&
        (celsiusTemp < 85))
    {
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_FAILED;
    }

    if ((state != SelfTestState_FAILED) &&
        (iteration < ADC_NUM_SAMPLES))
    {
        ++iteration;
        state = SelfTestState_IN_PROGRESS;
        _tickHandle = TimerTick_RequestTimeout(TickTimeoutType_SINGLE, TIME_BETWEEN_ADC_SAMPLES_MS, SelfTest_Process);
    }

    return state;
}

/***************************************************************************//**
    Configures and tests the Barometer.  Checks to make sure the data from the
    barometer exists and is not all FF's or 00's.

    \returns The state of the test.  The test returns passed or failed when
             complete.
*//****************************************************************************/
static SelfTestState_t _TestBarometer(void)
{
    SelfTestState_t state;
    static BarometerSample_t sample;

    Barometer_TriggerSampleBlocking(&sample);

    Serial_SendString3Numbers("bar:", sample.pressure, sample.temperature, 0);

    /* make sure the value isn't railed */
    if ((sample.pressure != UINT16_T_MIN) &&
        (sample.pressure != UINT16_T_MAX) &&
        (sample.temperature != UINT16_T_MIN) &&
        (sample.temperature != UINT16_T_MAX))
    {
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_FAILED;
    }

    return state;
}

/***************************************************************************//**
    Configures and tests the ambient light sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestAmbientLight(
        const uint16_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    uint16_t brightness;
    static bool railed = true;
    static bool rangeSufficient = false;
    static uint16_t maxValue = UINT16_T_MIN;
    static uint16_t minValue = UINT16_T_MAX;

    for (counter = 0u; counter < dataLength; ++counter)
    {
        brightness = Conversion_SwapBytes(data[counter]);
        Serial_SendStringNumber("amb: ", brightness);
        /* First make sure the value is not railed.  If it is not railed,
           then compare to see if it is unique.  To be unique, it only has to
           be different than the last value */
        if ((brightness < ADC_HIGH_MINIMUM) &&
            (brightness > ADC_LOW_MAXIMUM))
        {
            railed &= false;

            if (brightness > maxValue)
            {
                maxValue = brightness;
            }

            if (brightness < minValue)
            {
                minValue = brightness;
            }

            if ((maxValue != UINT16_T_MIN) &&
                (minValue != UINT16_T_MAX) &&
                ((maxValue - minValue) > AMBIENT_LIGHT_RANGE))
            {
                rangeSufficient |= true;
            }
        }
    }

    if ((!railed) && rangeSufficient)
    {
        DEBUG("amb passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the microphone.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestMicrophone(
        const uint16_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    uint16_t amplitude;
    static bool railed = true;
    static bool rangeSufficient = false;
    static bool settled = false;
    static uint16_t settledSamples = 0;
    static uint16_t maxValue = UINT16_T_MIN;
    static uint16_t minValue = UINT16_T_MAX;

    for (counter = 0u; counter < dataLength; ++counter)
    {
        amplitude = Conversion_SwapBytes(data[counter]);
        Serial_SendStringNumber("mic: ", amplitude);
        /* First make sure the value is not railed.  If it is not railed,
           then compare to see if it is unique.  To be unique, it only has to
           be different than the last value */
        if ((amplitude < ADC_HIGH_MINIMUM) &&
            (amplitude > ADC_LOW_MAXIMUM))
        {
            railed &= false;

            if ((amplitude > (MICROPHONE_SETTLING_VALUE - MICROPHONE_SETTLING_TOLERANCE)) &&
                (amplitude < (MICROPHONE_SETTLING_VALUE + MICROPHONE_SETTLING_TOLERANCE)) &&
                (settled == false))
            {
                ++settledSamples;
                if (settledSamples >= MICROPHONE_SETTLING_SAMPLES)
                {
                    DEBUG("mic settled");
                    settled |= true;
                }
            }
            else
            {
                settledSamples = 0u;
            }

            if (settled)
            {
                if (amplitude > maxValue)
                {
                    maxValue = amplitude;
                }

                if (amplitude < minValue)
                {
                    minValue = amplitude;
                }

                if ((maxValue != UINT16_T_MIN) &&
                    (minValue != UINT16_T_MAX) &&
                    ((maxValue - minValue) > MICROPHONE_RANGE))
                {
                    Serial_SendString3Numbers("mic range:", maxValue, minValue, amplitude);
                    rangeSufficient |= true;
                }
            }
        }
    }

    if ((!railed) && rangeSufficient)
    {
        DEBUG("mic passed");
        state = SelfTestState_PASSED;
        AdcManager_PoweredChannelControl(AdcManChannel_MICROPHONE, false);
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the force gauge sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestForceGauge(
        const uint16_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    uint16_t value;
    static bool notRailed = false;
    static bool rangeSufficient = false;
    static uint16_t maxValue = UINT16_T_MIN;
    static uint16_t minValue = UINT16_T_MAX;

    for (counter = 0u; counter < dataLength; ++counter)
    {
        value = (uint16_t)Conversion_SwapBytes(data[counter]);
        Serial_SendStringNumber("force ", value);

        if ((value > ADC_LOW_MAXIMUM) && (value < ADC_HIGH_MINIMUM))
        {
            notRailed |= true;
        }

        if (value > maxValue)
        {
            maxValue = value;
        }
        if (value < minValue)
        {
            minValue = value;
        }

        if ((maxValue != UINT16_T_MIN) &&
            (minValue != UINT16_T_MAX) &&
            ((maxValue - minValue) > FORCE_GAUGE_RANGE))
        {
            rangeSufficient |= true;
        }
    }

    if (notRailed && rangeSufficient)
    {
        DEBUG("force passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the magnetometer sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestMagnetometer(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    ThreeAxisSensorData_t magValue;
    static bool rangeSufficient = false;
    static ThreeAxisSensorData_t positive =
        {
                .xData = INT16_T_MIN,
                .yData = INT16_T_MIN,
                .zData = INT16_T_MIN
        };
    static ThreeAxisSensorData_t negative =
        {
                .xData = INT16_T_MAX,
                .yData = INT16_T_MAX,
                .zData = INT16_T_MAX
        };

    for (counter = 0u; counter < dataLength; ++counter)
    {
        magValue.xData = Conversion_SwapBytes(data[counter].xData);
        magValue.yData = Conversion_SwapBytes(data[counter].yData);
        magValue.zData = Conversion_SwapBytes(data[counter].zData);
        Serial_SendString3Numbers("mag: ", magValue.xData,  magValue.yData,  magValue.zData);

        /* make sure at least one value is not zero to know that the structure is not the default value */
        if ((magValue.xData != 0) ||
            (magValue.yData != 0) ||
            (magValue.zData != 0))
        {
            positive.xData = magValue.xData > positive.xData ? magValue.xData : positive.xData;
            positive.yData = magValue.yData > positive.yData ? magValue.yData : positive.yData;
            positive.zData = magValue.zData > positive.zData ? magValue.zData : positive.zData;
            negative.xData = magValue.xData < negative.xData ? magValue.xData : negative.xData;
            negative.yData = magValue.yData < negative.yData ? magValue.yData : negative.yData;
            negative.zData = magValue.zData < negative.zData ? magValue.zData : negative.zData;

            if (((positive.xData - negative.xData) > MAGNETOMETER_RANGE) &&
                ((positive.yData - negative.yData) > MAGNETOMETER_RANGE) &&
                ((positive.zData - negative.zData) > MAGNETOMETER_RANGE))
            {
                rangeSufficient |= true;
            }
        }
    }

    if (rangeSufficient)
    {
        DEBUG("mag passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the accelerometer sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestAccelerometer(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    ThreeAxisSensorData_t accValue;
    static bool rangeSufficient = false;
    static bool positiveX = false;
    static bool positiveY = false;
    static bool positiveZ = false;
    static bool negativeX = false;
    static bool negativeY = false;
    static bool negativeZ = false;
    static ThreeAxisSensorData_t positive =
        {
                .xData = INT16_T_MIN,
                .yData = INT16_T_MIN,
                .zData = INT16_T_MIN
        };
    static ThreeAxisSensorData_t negative =
        {
                .xData = INT16_T_MAX,
                .yData = INT16_T_MAX,
                .zData = INT16_T_MAX
        };


    for (counter = 0u; counter < dataLength; ++counter)
    {
        accValue.xData = (int16_t)Conversion_SwapBytes(data[counter].xData);
        accValue.yData = (int16_t)Conversion_SwapBytes(data[counter].yData);
        accValue.zData = (int16_t)Conversion_SwapBytes(data[counter].zData);
        Serial_SendString3Numbers("acc: ", accValue.xData,  accValue.yData,  accValue.zData);

        /* make sure at least one value is not zero to know that the structure is not the default value */
        if ((accValue.xData != 0) ||
            (accValue.yData != 0) ||
            (accValue.zData != 0))
        {
            /* Compare to see if it has sufficient range.  Also check
               that there are both positive and negative readings for each axis */
            positive.xData = accValue.xData > positive.xData ? accValue.xData : positive.xData;
            positive.yData = accValue.yData > positive.yData ? accValue.yData : positive.yData;
            positive.zData = accValue.zData > positive.zData ? accValue.zData : positive.zData;
            negative.xData = accValue.xData < negative.xData ? accValue.xData : negative.xData;
            negative.yData = accValue.yData < negative.yData ? accValue.yData : negative.yData;
            negative.zData = accValue.zData < negative.zData ? accValue.zData : negative.zData;

            if (((positive.xData - negative.xData) > ACCELEROMETER_RANGE) &&
                ((positive.yData - negative.yData) > ACCELEROMETER_RANGE) &&
                ((positive.zData - negative.zData) > ACCELEROMETER_RANGE))
            {
                rangeSufficient |= true;
            }

            positiveX |= accValue.xData > 0 ? true : false;
            negativeX |= accValue.xData < 0 ? true : false;
            positiveY |= accValue.yData > 0 ? true : false;
            negativeY |= accValue.yData < 0 ? true : false;
            positiveZ |= accValue.zData > 0 ? true : false;
            negativeZ |= accValue.zData < 0 ? true : false;
        }
    }

    if (rangeSufficient && positiveX && positiveY && positiveZ && negativeX && negativeY && negativeZ)
    {
        DEBUG("acc passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the gyroscope sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestGyroscope(
        const ThreeAxisSensorData_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    ThreeAxisSensorData_t gyroValue;
    static bool rangeSufficient = false;
    static bool positiveX = false;
    static bool positiveY = false;
    static bool positiveZ = false;
    static bool negativeX = false;
    static bool negativeY = false;
    static bool negativeZ = false;
    static ThreeAxisSensorData_t positive =
        {
                .xData = INT16_T_MIN,
                .yData = INT16_T_MIN,
                .zData = INT16_T_MIN
        };
    static ThreeAxisSensorData_t negative =
        {
                .xData = INT16_T_MAX,
                .yData = INT16_T_MAX,
                .zData = INT16_T_MAX
        };

    for (counter = 0u; counter < dataLength; ++counter)
    {
        gyroValue.xData = (int16_t)Conversion_SwapBytes(data[counter].xData);
        gyroValue.yData = (int16_t)Conversion_SwapBytes(data[counter].yData);
        gyroValue.zData = (int16_t)Conversion_SwapBytes(data[counter].zData);
        Serial_SendString3Numbers("gyro: ", gyroValue.xData,  gyroValue.yData,  gyroValue.zData);

        /* make sure at least one value is not zero to know that the structure is not the default value */
        if ((gyroValue.xData != 0) ||
            (gyroValue.yData != 0) ||
            (gyroValue.zData != 0))
        {
            /* Compare to see if it has sufficient range.  Also check
               that there are both positive and negative readings for each axis */
            positive.xData = gyroValue.xData > positive.xData ? gyroValue.xData : positive.xData;
            positive.yData = gyroValue.yData > positive.yData ? gyroValue.yData : positive.yData;
            positive.zData = gyroValue.zData > positive.zData ? gyroValue.zData : positive.zData;
            negative.xData = gyroValue.xData < negative.xData ? gyroValue.xData : negative.xData;
            negative.yData = gyroValue.yData < negative.yData ? gyroValue.yData : negative.yData;
            negative.zData = gyroValue.zData < negative.zData ? gyroValue.zData : negative.zData;

            if (((positive.xData - negative.xData) > GYROSCOPE_RANGE) &&
                ((positive.yData - negative.yData) > GYROSCOPE_RANGE) &&
                ((positive.zData - negative.zData) > GYROSCOPE_RANGE))
            {
                rangeSufficient |= true;
            }

            positiveX |= gyroValue.xData > 0 ? true : false;
            negativeX |= gyroValue.xData < 0 ? true : false;
            positiveY |= gyroValue.yData > 0 ? true : false;
            negativeY |= gyroValue.yData < 0 ? true : false;
            positiveZ |= gyroValue.zData > 0 ? true : false;
            negativeZ |= gyroValue.zData < 0 ? true : false;
        }
    }

    if (rangeSufficient && positiveX && positiveY && positiveZ && negativeX && negativeY && negativeZ)
    {
        DEBUG("gyro passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}

/***************************************************************************//**
    Configures and tests the encoder (wheel) sensor.  Checks to make sure it takes
    readings that aren't railed.  And that there is more than one unique value.

    \param[in] data - the data from the sensor stream
    \param[in] dataLength - the number of elements in the data array

    \returns The state of the test.  The test returns passed or failed when
             complete and returns in progress when busy.
*//****************************************************************************/
static SelfTestState_t _TestEncoder(
        const int16_t data[],
        uint16_t dataLength)
{
#if 1
    SelfTestState_t state;
    uint16_t counter;
    int16_t value;
    static uint16_t positive = 0u;
    static uint16_t negative = 0u;

    for (counter = 0u; counter < dataLength; ++counter)
    {
        value = (int16_t)Conversion_SwapBytes(data[counter]);
        Serial_SendStringNumber("enc ", value);
        positive += value > 0 ? value : 0u;
        negative += value < 0 ? -value: 0u;
    }

    if ((positive > ENCODER_RANGE) &&
        (negative > ENCODER_RANGE))
    {
        DEBUG("enc passed");
        state = SelfTestState_PASSED;
    }
    else
    {
        state = SelfTestState_IN_PROGRESS;
    }

    return state;
#else
    return SelfTestState_PASSED;
#endif
}
