/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022 Cascoda Ltd
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Cascoda Limited.
 *    integrated circuit in a product or a software update for such product, must
 *    reproduce the above copyright notice, this list of  conditions and the following
 *    disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Cascoda Limited nor the names of its contributors may be used to
 *    endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * 4. This software, whether provided in binary or any other form must not be decompiled,
 *    disassembled, reverse engineered or otherwise modified.
 *
 *  5. This software, in whole or in part, must only be used with a Cascoda Limited circuit.
 *
 * THIS SOFTWARE IS PROVIDED BY CASCODA LIMITED "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CASCODA LIMITED OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/
/**
 * @file
 *
 * This file is used to hook up the resource to the actual device driver.
 * e.g. it makes the connection between the knx data points and the hardware.
 *
 * it uses knx_iot_wakeful_main as main function
 * e.g. it needs to implement the 3 external functions:
 * - put_callback
 * - hardware_init
 * - hardware_poll
 
 *
 * define
 * - ACTUATOR_TEST_MODE
 *   calls at the end of the hardware init a test sequence for the actuators
 * - SLEEPY
 *   additional code to enter sleep modes
 * - SLEEPY_USE_LED
 *   use LED to indicate if the device is awake
 */

#include "oc_api.h"
#include "oc_core_res.h"
#include "api/oc_knx_dev.h"
#include "api/oc_knx_fp.h"
#include "port/oc_clock.h"
#include "port/dns-sd.h"
#ifndef EXCLUDE_CASCODA_BAREMETAL

#include "cascoda-bm/cascoda_sensorif.h"
#include "cascoda-bm/cascoda_interface.h"
#include "cascoda-util/cascoda_tasklet.h"
#include "cascoda-util/cascoda_time.h"
#include "cascoda-bm/cascoda_wait.h"
#include "ca821x_error.h"
#include "sed_poll.h"
#include <openthread/thread.h> 
#include <platform.h>

//#include "sif_btn_ext_pi4ioe5v6408.h"

#include "devboard_btn.h"
#include "devboard_btn_ext.h"
 

#ifdef SLEEPY
#include "knx_iot_sleepy_main_extern.h"
#include "knx_iot_sleepy_main.h"
#else
#include "knx_iot_wakeful_main_extern.h"
#endif

#endif /* EXCLUDE_CASCODA_BAREMETAL */
#include "knx_iot_example.h"



// generic defines
#define SCHEDULE_NOW 0
#define S_MODE_INTERVAL 30
#define BTN_LONGPRESS_TIME_DEFAULT 250
#define BTN_HOLD_TIME_DEFAULT 600
#define THIS_DEVICE 0

#ifdef SLEEPY
#include "sed_poll.h"
#endif

// forward declaration.
void dev_put_callback(const char* url);
//
// code for programming mode and reset
//
// forward declarations

enum prog_and_reset_constants
{
  RESET_HOLD_AND_LONG_PRESS_THRESHOLD_MS = 3000,
  PROGRAMMING_MODE_INDICATOR_FLASHING_PERIOD_MS = 1000,
  RESET_VALUE = 2,
  RESET_INDICATOR_FLICKER_COUNT = 5,
  RESET_KNX_INDICATOR_FLICKER_PERIOD_MS = 300,
  RESET_THREAD_INDICATOR_FLICKER_PERIOD_MS = 600,
};

enum reset_button_state
{
  KNX_RESET = 0,
  THREAD_RESET = 1,
  IGNORE_FURTHER_ACTION = 2,
};

enum reset_button_state g_reset_state;
static ca_error reset_done_feedback(void *context);
static ca_error programming_mode_handler(void *context);
static void exit_programming_mode(size_t device_index);
static void prog_mode_short_press_cb(void *context);

// Tasklet used for flashing LED when in programming mode
static ca_tasklet g_programming_mode_handler;
// Tasklet used for flickering LED when reset is done
static ca_tasklet g_reset_done_indicator;

/**
 * @brief retrieve the fault state of the url/data point
 * the caller needs to know if the resource/data point implements a fault situation
 * 
 * @param url the url of the resource/data point
 * @return true value is true
 * @return false value is false or error.
 */
bool app_retrieve_fault_variable(const char* url);




#include "api/oc_knx_dev.h"
#include "devboard_btn.h"
#include "cascoda-util/cascoda_tasklet.h"
#include "ca821x_error.h"
#include "cascoda-bm/cascoda_wait.h"

// ================================
// DEFINES
// ================================

#define THIS_DEVICE 0

// ================================
// TYPE DEFINITIONS
// ================================
enum ImplementationDefinedParameters
{
  PROGRAMMING_MODE_INDICATOR = DEV_SWITCH_3,
  TRIGGER_FOR_PROGRAMMING_MODE_AND_RESET = DEV_SWITCH_4,
};

// ===============================
// GLOBAL VARIABLE DEFINITIONS
// ===============================

// ===============================
// FORWARD DECLARATIONS
// ===============================

// short LSSB button callback
void lssb_ShortPress_cb(void *context)
{
  (void)context;
  PRINT_APP("LSSB button pressed\n");
  DPT_Switch sw;
  app_get_DPT_Switch_variable(URL_PB_1, &sw);
  sw = !sw; // invert
  app_set_DPT_Switch_variable(URL_PB_1, &sw);
  // send out the s-mode message
  oc_do_s_mode_with_scope(5, URL_PB_1, "w");
}



// Uncomment the line of code below if you want the test code to execute
//#define ACTUATOR_TEST_MODE

#ifdef ACTUATOR_TEST_MODE
// forward declaration
void actuator_test_init();
#endif

///////////////////////////////////////////////////////////////////////////////
//              App GET/PUT callbacks                                        //
///////////////////////////////////////////////////////////////////////////////


/**
 * @brief handle the callback on put for the url
 * the function should:
 * - determine what type the url is using
 * - receive the current data of the url
 *   the value was set in the PUT handler before this callback is called.
 * - use the data to actuate something
 *
 * @param url the url that received a PUT invocation.
 */
void dev_put_callback(const char* url){
  if (strcmp(url, URL_LED_1) == 0) {
        /* update led */ 
        setLED(LED_lsab, *app_get_DPT_Switch_variable(URL_LED_1, NULL));
        app_set_DPT_Switch_variable(URL_INFOONOFF_1, app_get_DPT_Switch_variable(URL_LED_1, NULL));
        oc_do_s_mode_with_scope(5, URL_INFOONOFF_1, "w");
  }
}

/**
 * @brief handle the callback on get for the url
 * the function should:
 * - determine what type the url is using
 * - read data from a sensor
 * - set the data of the url
 *   the value is read in the GET handler after this callback is called.
 *
 * @param url the url that received a GET invocation.
 */
void dev_get_callback(const char* url){
}

//
// Development Board
// Generic code for programming mode and reset
//
static enum reset_button_state state_snapshot;

/**
 * @brief button callback for the reset procedure
 * 
 * @param context the callback context
 */
static void reset_hold_cb(void *context)
{
  (void)context;
  PRINT_APP("=== reset_hold_cb()\n");

#ifdef DEMO_MODE
  PRINT_APP("Cannot reset when in DEMO_MODE\n");
  return;
#endif

  switch(g_reset_state)
  {
    case KNX_RESET:
      // Exit programming mode if in programming mode
      if (oc_knx_device_in_programming_mode(THIS_DEVICE))
        exit_programming_mode(THIS_DEVICE);

      // Do the actual reset
      oc_reset_device(THIS_DEVICE, RESET_VALUE);
      const char *url = NULL;
      for (int i = 0; url = app_get_parameter_url(i); i++) {
        oc_storage_erase(url);
      }

      // Give feedback to the user that reset is done (so they know when to release the button)
      state_snapshot = KNX_RESET;
      oc_device_info_t* device = oc_core_get_device_info(0);
      knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);

      

      break;

    case THREAD_RESET:
      // Erase the the Thread credentials
      PlatformEraseJoinerCredentials(OT_INSTANCE);

      // Give feedback to the user that reset is done (so they know when to release the button)
      state_snapshot = THREAD_RESET;

      break;

    case IGNORE_FURTHER_ACTION:
      return;
  }

  ++g_reset_state;
}

/**
 * @brief button callback for the reset procedure
 * 
 * @param context the callback context
 */
static void reset_long_press_cb(void *context)
{
  (void)context;
  PRINT_APP("=== reset_long_press_cb\n");
  TASKLET_ScheduleDelta(&g_reset_done_indicator, SCHEDULE_NOW, &state_snapshot);
  g_reset_state = KNX_RESET;
}

/**
 * @brief initialize the reset functiononality
 * 
 * @param reset_button the (long press) button callback to start the reset procedure
 */

static void reset_init(dvbd_led_btn reset_button)
{
  // Initializes the tasklet for reset done indicator
  TASKLET_Init(&g_reset_done_indicator, &reset_done_feedback);

  // Reset the device if it is held down for the duration of RESET_HOLD_AND_LONG_PRESS_THRESHOLD_MS.
  DVBD_SetButtonHoldCallback(reset_button, &reset_hold_cb, NULL, RESET_HOLD_AND_LONG_PRESS_THRESHOLD_MS);
  DVBD_SetButtonLongPressCallback(reset_button, &reset_long_press_cb, NULL, RESET_HOLD_AND_LONG_PRESS_THRESHOLD_MS);
}
 


/**
 * @brief reset done
 * e.g. stop flickering the LED for reset
 * 
 * @param context the callback context
 */
static ca_error reset_done_feedback(void *context)
{
  enum reset_button_state reset_type = *((enum reset_button_state *)context);
  static uint8_t count = 0;

  if (count++ < RESET_INDICATOR_FLICKER_COUNT)
  {
    uint8_t led_state = 0;

    
    DVBD_SenseOutput(PROGRAMMING_MODE_INDICATOR, &led_state);
    DVBD_SetLED(PROGRAMMING_MODE_INDICATOR, !(led_state));
     

    if (reset_type == KNX_RESET)
    {
      TASKLET_ScheduleDelta(&g_reset_done_indicator, RESET_KNX_INDICATOR_FLICKER_PERIOD_MS / 2, context);
    }
    else if (reset_type == THREAD_RESET) 
    {
      TASKLET_ScheduleDelta(&g_reset_done_indicator, RESET_THREAD_INDICATOR_FLICKER_PERIOD_MS / 2, context);
    }
  }
  else
  {
    // Reset counter for next time reset happens
    count = 0;
    // This is to make sure that the final iteration always results in the indicator being off
    
    DVBD_SetLED(PROGRAMMING_MODE_INDICATOR, LED_OFF);
     
    
    // After the feedback is shown for the Thread Reset, reboot the device.
    if (reset_type == THREAD_RESET)
      BSP_SystemReset(SYSRESET_APROM);
  }
}

/**
 * @brief programming mode task
 * e.g. task to do the flickering the LED
 * 
 * @param context the callback context
 */
static ca_error programming_mode_handler(void *context)
{
  (void)context;
  uint8_t led_state = 0;
  
  
    DVBD_SenseOutput(PROGRAMMING_MODE_INDICATOR, &led_state);
    DVBD_SetLED(PROGRAMMING_MODE_INDICATOR, !(led_state));
   



  TASKLET_ScheduleDelta(&g_programming_mode_handler, PROGRAMMING_MODE_INDICATOR_FLASHING_PERIOD_MS / 2, NULL);
}

static ca_error become_sleepy(void *ctx)
{
  otLinkModeConfig linkMode = {0};
  otThreadSetLinkMode(OT_INSTANCE, linkMode);
  // Poll after the child tells its parent it is a SED once more
  SED_PollSoon();
  return CA_ERROR_SUCCESS;
}

static ca_tasklet become_sleepy_tasklet;

/**
 * @brief exit the programming mode
 * e.g. stop flickering the LED
 * 
 * @param device_index the device index
 */
static void exit_programming_mode(size_t device_index)
{
  oc_knx_device_set_programming_mode(device_index, false);
  TASKLET_Cancel(&g_programming_mode_handler);

  
  DVBD_SetLED(PROGRAMMING_MODE_INDICATOR, LED_OFF);
   

  oc_device_info_t* device = oc_core_get_device_info(0);
  knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);
#ifdef SLEEPY
  // Devices remain awake for 20 seconds after PM mode is disabled, to ensure programming
  // is fast and reliable
  TASKLET_ScheduleDelta(&become_sleepy_tasklet, 20 * 1000, NULL);
#endif
}

/**
 * @brief enter the programming mode
 * e.g. start flickering the LED
 * 
 * @param device_index the device index
 */
static void enter_programming_mode(size_t device_index)
{
#ifdef SLEEPY
  // When in programming mode, devices must respond within 2 seconds
  otLinkModeConfig linkMode = {0};
  linkMode.mRxOnWhenIdle = 1;
  otThreadSetLinkMode(OT_INSTANCE, linkMode);
  TASKLET_Cancel(&become_sleepy_tasklet);
#endif
  oc_knx_device_set_programming_mode(device_index, true);
  TASKLET_ScheduleDelta(&g_programming_mode_handler, SCHEDULE_NOW, NULL);

  oc_device_info_t* device = oc_core_get_device_info(0);
  knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);
}

/**
 * @brief short press callback for entering/leaving the programming mode
 * 
 * @param context the callback context
 */
static void prog_mode_short_press_cb(void *context)
{
  (void)context;
  PRINT_APP("=== prog_mode_short_press_cb()\n");

  // if the device is not initialized, the check for PM always returns false
  // and the PM indicator cannot be turned off, so we return early
  if (otThreadGetDeviceRole(OT_INSTANCE) <= OT_DEVICE_ROLE_DETACHED)
    return;

  // If in programming mode, exit. Otherwise enter.
  if (oc_knx_device_in_programming_mode(THIS_DEVICE))
    exit_programming_mode(THIS_DEVICE);
  else
    enter_programming_mode(THIS_DEVICE);
}

/**
 * @brief enable/disable the programming mode
 * e.g. start flickering the led
 * 
 * @param device_index the device to be reset
 * @param programming_mode the programming mode status
 */
void programming_mode_embedded(size_t device_index, bool programming_mode)
{
  PRINT_APP("=== programming_mode_embedded()\n");

  // Nothing to do if the device is already in the programming mode that is
  // requested
  if (programming_mode == oc_knx_device_in_programming_mode(device_index))
    return;

  if (programming_mode)
    enter_programming_mode(device_index);
  else
    exit_programming_mode(device_index);
}

/**
 * @brief reset the device (knx only)
 * 
 * @param device_index the device to be reset
 * @param reset_value the value of the reset
 * @param data the callback data
 */
void reset_embedded(size_t device_index, int reset_value, void *data)
{
  (void)device_index;
  (void)reset_value;
  (void)data;

  PRINT_APP("reset_embedded()\n");

  // Flicker the LED
  reset_done_feedback(NULL);

  // Exit programming mode if in programming mode
  exit_programming_mode(THIS_DEVICE);
}


/**
 * @brief initalize the knx programming mode functionality
 * 
 * @param flashing_led the led for indication
 * @param program_mode_button the button for long/short press 
 */

static void programming_mode_init(dvbd_led_btn flashing_led, dvbd_led_btn program_mode_button)
{
#if !FAKE_SENSOR_DATA

  // Initializes the tasklet for programming mode
  TASKLET_Init(&g_programming_mode_handler, &programming_mode_handler);

  // Registers the button and LED
  if (flashing_led != program_mode_button){
#ifdef SLEEPY
    DVBD_RegisterButtonIRQInput(program_mode_button, JUMPER_POS_1);
#else
    DVBD_RegisterButtonInput(program_mode_button, JUMPER_POS_1);
#endif // SLEEPY
    DVBD_RegisterLEDOutput(flashing_led, JUMPER_POS_1);
  } else {
#ifdef SLEEPY
    DVBD_RegisterSharedIRQButtonLED(program_mode_button, JUMPER_POS_1);
#else
    DVBD_RegisterSharedButtonLED(program_mode_button, JUMPER_POS_1);
#endif // SLEEPY
  }

  // Set the device in programming mode when the program_mode_button is short-pressed
  DVBD_SetButtonShortPressCallback(program_mode_button, &prog_mode_short_press_cb, NULL, BTN_SHORTPRESS_RELEASED);
#endif
}


/**
 * @brief restart the device (application depended)
 *
 * @param device_index the device identifier of the list of devices
 * @param data the supplied data.
 */
void
restart_cb(size_t device_index, void *data)
{
  (void)device_index;
  (void)data;

  PRINT("-----restart_cb -------\n");
  // turn off the programming mode light
  exit_programming_mode(device_index);
  
  // NOTE: We do not want to do a device reset here (i.e. System reboot),
  // despite the suggestion to do so by the spec.
}


/**
 * @brief initalize the knx functionality
 * 
 * - programming mode
 * - reset
 */
static void knx_specific_init()
{
#if !FAKE_SENSOR_DATA
  // Allows device to enter programming mode when button is pressed
  programming_mode_init(PROGRAMMING_MODE_INDICATOR, TRIGGER_FOR_PROGRAMMING_MODE_AND_RESET);

  // Allows device to be reset when button is held down
  reset_init(TRIGGER_FOR_PROGRAMMING_MODE_AND_RESET);
#endif
  // Allow the device to be restarted 
  oc_set_restart_cb(restart_cb, NULL);
}


#ifdef SLEEPY
// Sleepy handler for programming mode button
bool hardware_can_sleep()
{
return DVBD_CanSleep();
}

// Sleepy Device
void hardware_sleep(struct ca821x_dev *pDeviceRef, uint32_t nextAppEvent)
{
  // 20 min wakeup if no tasklet is scheduled (should not happen)
  uint32_t taskletTimeLeft = 20 * 60 * 1000;


  /* schedule wakeup */
  TASKLET_GetTimeToNext(&taskletTimeLeft);

  if (taskletTimeLeft > nextAppEvent)
    taskletTimeLeft = nextAppEvent;

  bool sleep_after_joining = otThreadGetDeviceRole(OT_INSTANCE) != OT_DEVICE_ROLE_DETACHED;

  /* check that it's worth going to sleep */
  if (taskletTimeLeft > 100 && sleep_after_joining)
  {
    /* and sleep */
    DVBD_DevboardSleep(taskletTimeLeft, pDeviceRef); 
  }
}

void hardware_reinitialise(void)
{
#ifdef SLEEPY_USE_LED
  // hardcoded pin # of sensor board pm mode led
  BSP_ModuleSetGPIOPin(36, LED_ON);
#endif
} 
#endif // SLEEPY


/**
 * @brief do the hardware installation
 *
 * This function needs to initialize all hardware that is being used
 */
void hardware_init()
{
  /* set the put callback on the underlaying code */
  app_set_put_cb(dev_put_callback);
  //app_set_get_cb(dev_get_callback);
#ifdef SLEEPY_USE_LED
  // Debug: blink programming mode indicator on wakeup
  // hardcoded pin # of sensor board pm mode led
	BSP_ModuleRegisterGPIOOutputOD(36, MODULE_PIN_TYPE_LED);
  BSP_ModuleSetGPIOPin(36, LED_ON);
#endif
  /* Initialize knx-specific development board functionality */
  knx_specific_init();
  DVBD_RegisterButtonInput(DEV_SWITCH_1, JUMPER_POS_1); 
  DVBD_SetButtonShortPressCallback(DEV_SWITCH_1, &lssb_ShortPress_cb, NULL, BTN_SHORTPRESS_RELEASED);     


DVBD_RegisterLEDOutput(DEV_SWITCH_2, JUMPER_POS_1);   




#ifdef ACTUATOR_TEST_MODE
  /* run the tests after hardware initialiation */
  actuator_test_init();
#endif

#ifdef SLEEPY
    // default sleepy polling: poll every 10 minutes
    SED_InitPolling(1500, 10 * 60 * 1000, 0);
#endif
}

void setLED(led_t led, bool value)
{
  switch(led){
    case LED_lsab: DVBD_SetLED(DEV_SWITCH_2, value?LED_ON:LED_OFF); break; 
    default: break;
  }
} 

/**
 * @brief do polling of the hardware
 *
 * same frequency as oc_poll.
 */
void hardware_poll()
{
#ifndef EXCLUDE_CASCODA_BAREMETAL

  
  DVBD_PollButtons();
  

#endif /* EXCLUDE_CASCODA_BAREMETAL */
}

bool app_is_url_in_use(const char* url)
{
   int index;
   oc_string_t oc_str;
   int table_size = oc_core_get_group_object_table_total_size();

  // check if the URL is in the Group Object Table
  for (index = 0; index++; index < table_size) {
    oc_str = oc_core_find_group_object_table_url_from_index( index);
    if (oc_string_len(oc_str) > 0) {
        if (strcmp(url, oc_string(oc_str)) == 0) {
            return true;
        }
    }
  }
  return false;
}

#ifdef ACTUATOR_TEST_MODE
/* code for actuator testing */

/* tasklet for testing */
static ca_tasklet g_test_tasklet;

/**
 * @brief test function run in a tasklet
 * tests each 3 seconds the actuators
 * with type
 * - boolean
 * - integer
 * with different inputs
 */
ca_error actuator_test(void *context)
{
  (void)context;
  static bool bvalue = true;
  static int ivalue = 0;
  TASKLET_ScheduleDelta(&g_test_tasklet, 3000, NULL);
}

/**
 * @brief initialisation of actuator tests
 */
void actuator_test_init()
{
  TASKLET_Init(&g_test_tasklet, &actuator_test);
  TASKLET_ScheduleDelta(&g_test_tasklet, 3000, NULL);
}

#endif // ACTUATOR_TEST_MODE

#ifndef EXCLUDE_CASCODA_BAREMETAL
//embedded is always built with 
__attribute__((__weak__)) 
extern void app_initialize(){}
#endif /* EXCLUDE_CASCODA_BAREMETAL */
