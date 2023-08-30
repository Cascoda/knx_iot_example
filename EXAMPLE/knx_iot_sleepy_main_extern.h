/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2023 Cascoda Limited
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
*/

#include "cascoda-bm/cascoda_interface.h"
#include "cascoda-bm/cascoda_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* for OpenThread: poll period for keep-alive */
#define SED_POLL_PERIOD 5000
/* devices will stay awake for at least this long before going back to sleep*/
#define SED_MIN_AWAKE_TIME 1000

/**
 * @brief define what happens when receiving a post mesage on a URL
 *
 * @param url the URL of the message as a string
 */
void post_callback(char *url);

/**
 * @brief any hardware specific setup e.g. registering board LEDs and buttons
 *
 */
void hardware_init();

/**
 * @brief any hardware specific actions to be continually run e.g. checking buttons for input
 *
 */
void hardware_poll();

/**
 * @brief Application-specific handling of programming mode command received from linker
 *
 */
void programming_mode_embedded(size_t device_index, bool programming_mode);

/**
 * @brief Application-specific handling of reset when received from linker
 *
 */
void reset_embedded(size_t device_index, int reset_value, void *data);

/**
 * @brief any hardware specific reinitialisation after wakeup from sleep
 *
 */
 void hardware_reinitialise();

/**
 * @brief hardware specific sleep (power-down) function
 *
 * @param pDeviceRef - Pointer to initialised ca821x_device_ref struct
 *
 * @param nextAppEvent - time to next application layer event
 *
 */
void hardware_sleep(struct ca821x_dev *pDeviceRef, uint32_t nextAppEvent);

/**
 * @brief hardware specific check if device can sleep
 *
 * @return true if device is allowed to sleep, false otherwise
 *
 */
bool hardware_can_sleep();

/**
 * @brief poll handler for openthread sleepy end device for keep-alive
 *
 * @param pDeviceRef - Pointer to tasklet context
 *
 * @return error status
 *
 */
ca_error sed_poll_handler(void *aContext);

#ifdef __cplusplus
}
#endif