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
#include "sif_ssd1681.h"  // 1.5 inch display
#define SPI_NUM 1

#endif /* EXCLUDE_CASCODA_BAREMETAL */
#include "knx_eink_battleships.h"



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

/**
 * @brief retrieve the fault state of the url/data point
 * the caller needs to know if the resource/data point implements a fault situation
 * 
 * @param url the url of the resource/data point
 * @return true value is true
 * @return false value is false or error.
 */
bool app_retrieve_fault_variable(const char* url);






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
  if (strcmp(url, URL_RECEIVESHOT) == 0) {
      if (strcmp(url, URL_RECEIVESHOT) == 0)
      {
        onReceivedShot(url);
      }
  }
  if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0) {
      if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0)
      {
        onReceivedShotStatus(url);
      }
  }
  if (strcmp(url, URL_RECEIVEREADY) == 0) {
      if (strcmp(url, URL_RECEIVEREADY) == 0)
      {
        onReceivedReady(url);
      }
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
}

void programming_mode_embedded(size_t device_index, bool programming_mode)
{
  (void)device_index;
  (void)programming_mode;

  PRINT_APP("programming_mode_embedded()\n");

  // Nothing to do if the device is already in the programming mode that is
  // requested
  if (programming_mode == oc_knx_device_in_programming_mode(device_index))
    return;

  oc_knx_device_set_programming_mode(device_index, programming_mode);
  
  if (g_screen_nr == DEV_SCREEN)
    refresh_screen(false);
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
  /* Eink Initialisation */
  SENSORIF_SPI_Config(SPI_NUM);
  SIF_SSD1681_Initialise();
  DVBD_RegisterButtonIRQInput(DEV_SWITCH_2, JUMPER_POS_1); 
  DVBD_SetButtonShortPressCallback(DEV_SWITCH_2, &button_1_ShortPress_cb, NULL, BTN_SHORTPRESS_RELEASED);
  DVBD_SetButtonLongPressCallback(DEV_SWITCH_2, &button_1_LongPress_cb, NULL, BTN_LONGPRESS_TIME_DEFAULT);   
  DVBD_RegisterButtonIRQInput(DEV_SWITCH_3, JUMPER_POS_1); 
  DVBD_SetButtonShortPressCallback(DEV_SWITCH_3, &button_2_ShortPress_cb, NULL, BTN_SHORTPRESS_RELEASED);
  DVBD_SetButtonLongPressCallback(DEV_SWITCH_3, &button_2_LongPress_cb, NULL, BTN_LONGPRESS_TIME_DEFAULT);   
  DVBD_RegisterButtonIRQInput(DEV_SWITCH_4, JUMPER_POS_1); 
  DVBD_SetButtonShortPressCallback(DEV_SWITCH_4, &button_3_ShortPress_cb, NULL, BTN_SHORTPRESS_RELEASED);
  DVBD_SetButtonLongPressCallback(DEV_SWITCH_4, &button_3_LongPress_cb, NULL, BTN_LONGPRESS_TIME_DEFAULT);
  DVBD_SetButtonHoldCallback(DEV_SWITCH_4, &button_3_Hold_cb, NULL, BTN_HOLD_TIME_DEFAULT);     

 







#ifdef ACTUATOR_TEST_MODE
  /* run the tests after hardware initialiation */
  actuator_test_init();
#endif

#ifdef SLEEPY
    // default sleepy polling: poll every 10 minutes
    SED_InitPolling(1500, 10 * 60 * 1000, 0);
#endif
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
