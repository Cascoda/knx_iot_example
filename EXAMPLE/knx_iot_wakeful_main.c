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
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*/
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include <openthread/diag.h>
#include <openthread/platform/settings.h>
#include <openthread/tasklet.h>
#include <openthread/thread.h>
#include <platform.h>
#include "cascoda-util/cascoda_tasklet.h"

#include "ca-ot-util/cascoda_dns.h"
#include "cascoda-bm/cascoda_evbme.h"
#include "cascoda-bm/cascoda_interface.h"
#include "cascoda-bm/cascoda_ota_upgrade.h"
#include "cascoda-bm/cascoda_serial.h"
#include "cascoda-bm/cascoda_types.h"
#include "cascoda-util/cascoda_time.h"
#include "ca821x_api.h"

#include "cascoda-bm/cascoda_wait.h"

#include "knx_iot_wakeful_main_extern.h"

#include "api/oc_knx_dev.h"
#include "api/oc_knx_fp.h"
#include "port/oc_assert.h"
#include "port/oc_clock.h"
#include "port/oc_log.h"
#include "port/dns-sd.h"
#include "security/oc_spake2plus.h"
#include "manufacturer_storage.h"
#include "oc_api.h"
#include "oc_buffer_settings.h"
#include "oc_core_res.h"
#include "oc_uuid.h"
#include "sntp_helper.h"
#include "oc_helpers.h"

// Used for knxctl (not yet implemented)
#define KNX_COMMAND (0xB0)
#define KNX_COMMAND_STORAGE_RESET (0x00)
#define KNX_COMMAND_POWER (0x01)
#define KNX_COMMAND_FACTORY (0x02)

#define THIS_DEVICE 0

void exit(int e)
{
	(void)e;
	while (1)
	{
		;
	}
}

otInstance *OT_INSTANCE;
// Defer publishing of the service until you have the correct IP address
// to advertise
void knx_srp_add_service(void);

// To be implemented by the application
void register_resources(void);
void factory_presets_cb(size_t device_index, void *data);
void hostname_cb(size_t device_index, oc_string_t host_name, void *data);
int app_set_serial_number(char *serial_number);
int app_init(void);

#if CASCODA_OTA_UPGRADE_ENABLED
// Implemented in the cascoda knx-iot port
void swu_cb(size_t device_index, oc_separate_response_t *response, size_t binary_size, size_t offset, uint8_t *payload, size_t len, void *data);
#endif

/**
 * @file
 *  Example of wakeful main
 */
/**
 * @ingroup ocf
 * @defgroup ocf-wakeful-main example ocf wakeful main
 * @brief  Example of wakeful main
 *
 * @{
 *
 * ## Application Main
 * This file contains the functionality and main function of the OCF application.
 * main is using the bare metal module to run on the chili.
 *
 * \include wakeful_main.c
 */

/**
 * Handle application specific commands.
 */
static int ot_serial_dispatch(uint8_t *buf, size_t len, struct ca821x_dev *pDeviceRef)
{
	int ret = 0;

	// KNX Commands, to be used with the "knxctl" POSIX application
	if (buf[0] == KNX_COMMAND)
	{
		switch (buf[2])
		{
		case KNX_COMMAND_STORAGE_RESET:
			oc_knx_device_storage_reset(0, 2);
			break;
		case KNX_COMMAND_POWER:
			BSP_SystemReset(SYSRESET_APROM);
			break;
		case KNX_COMMAND_FACTORY:
			otInstanceFactoryReset(OT_INSTANCE);
			break;
		default:
			break;
		}
	}

	// switch clock otherwise chip is locking up as it loses external clock
	if (((buf[0] == EVBME_SET_REQUEST) && (buf[2] == EVBME_RESETRF)) || (buf[0] == EVBME_HOST_CONNECTED))
	{
		EVBME_SwitchClock(pDeviceRef, 0);
	}
	return ret;
}

static void ot_state_changed(uint32_t flags, void *context)
{
	(void)context;
	if (flags & OT_CHANGED_THREAD_ROLE)
	{
		otDeviceRole role = otThreadGetDeviceRole(OT_INSTANCE);
		PRINT("Role: %s\n", otThreadDeviceRoleToString(role));
	}
	// publish the MDNS service on startup
	oc_device_info_t* device = oc_core_get_device_info(0);
	knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);

#ifdef USE_SNTP
	bool must_update_rtc = (SNTP_GetState() == NO_TIME);
	if (flags & (OT_CHANGED_IP6_ADDRESS_ADDED | OT_CHANGED_IP6_ADDRESS_REMOVED))
	{
		if (must_update_rtc)
			SNTP_Update();
	}
#endif /* USE_SNTP */
}

static void signal_event_loop(void)
{
}

static void prog_mode_cb(size_t device_index, bool programming_mode, void *data)
{
	(void)data;
	PRINT_APP("prog_mode_cb(), device: %d, programming_mode %d\n", device_index, programming_mode);
	programming_mode_embedded(device_index, programming_mode);
}

static void reset_cb(size_t device_index, int reset_value, void *data)
{
	(void)device_index;
	PRINT_APP("reset_cb()\n");

	reset_embedded(device_index, reset_value, data);
}

/**
 * main application.
 * intializes the global variables
 * registers and starts the handler
 * handles (in a loop) the next event.
 * shuts down the stack
 */
int main(void)
{
	int init;
	oc_clock_time_t next_event;
	u8_t StartupStatus;
	struct ca821x_dev dev;
	cascoda_serial_dispatch = ot_serial_dispatch;
	otError otErr = OT_ERROR_NONE;

	ca821x_api_init(&dev);

	// Initialisation of Chip and EVBME
	StartupStatus = EVBMEInitialise(CA_TARGET_NAME, &dev);
	BSP_RTCInitialise();

	PlatformRadioInitWithDev(&dev);

	// OpenThread Configuration
	OT_INSTANCE = otInstanceInitSingle();

	otIp6SetEnabled(OT_INSTANCE, true);

	oc_assert(OT_INSTANCE);

	// Hardware specific setup
	hardware_init();

#if CASCODA_OTA_UPGRADE_ENABLED
	/* Initialises handling of OTA Firmware Upgrade */
	ota_upgrade_init();
#endif

	// A backoff mechanism for joining the network
	u32_t joinCooldownTimer = 0;

	// Try to join network
	do
	{
		cascoda_io_handler(&dev);

		// If the timer has expired, try to join the network
		if (joinCooldownTimer == 60)
		{
			printf("Trying to join Thread network...\n");

			// Print the joiner credentials, delaying for up to 1 second
			PlatformPrintJoinerCredentials(&dev, OT_INSTANCE, 0);

			otErr = PlatformTryJoin(&dev, OT_INSTANCE);
			if (otErr == OT_ERROR_NONE || otErr == OT_ERROR_ALREADY)
				break;
			joinCooldownTimer = 0;
		}

		joinCooldownTimer += 1;

		WAIT_ms(200);
	} while (1);

	otThreadSetEnabled(OT_INSTANCE, true);

	DNS_Init(OT_INSTANCE);
#ifdef USE_SNTP
	SNTP_Init();
#endif /* USE_SNTP */

#ifdef OC_RETARGET
	oc_assert(otPlatUartEnable() == OT_ERROR_NONE);
#endif

	otSetStateChangedCallback(OT_INSTANCE, ot_state_changed, NULL);

	/* initializes the handlers structure */
	static const oc_handler_t handler = {.init = app_init,
										 .signal_event_loop = signal_event_loop,
										 .register_resources = register_resources
#ifdef OC_CLIENT
										 ,
										 .requests_entry = 0
#endif
	};

	oc_storage_config("./knx_iot_creds");

	/* configure the serial number */
	uint8_t sn[6];
	int error = knx_get_stored_serial_number(sn);
	if (error)
	{
		PRINT_APP("ERROR: Unique serial number not found! Using default value...\n");
		PRINT_APP(
			"Please create the data file using knx-gen-data and flash it with chilictl in order to fix this issue.\n");
	}
	else
	{
		// turn binary to hexadecimal
		char serial_number_str[13];
		snprintf(serial_number_str,
				 sizeof(serial_number_str),
				 "%02X%02X%02X%02X%02X%02X",
				 sn[0],
				 sn[1],
				 sn[2],
				 sn[3],
				 sn[4],
				 sn[5]);
		app_set_serial_number(serial_number_str);
	}
	
#ifdef OC_SPAKE
	char pwd[33];
	error = knx_get_stored_password(pwd);
	if (error)
	{
		PRINT_APP("Error: Stored password not found! Using default value...\n");
		PRINT_APP(
			"Please create the data file using knx-gen-data and flash it with chilictl in order to fix this issue.\n");
	}
	else
	{
		oc_spake_set_password(pwd);
	}
	
	uint8_t salt[32], rand[32];
	uint32_t it;
	mbedtls_mpi w0;
	mbedtls_ecp_point L;
	mbedtls_mpi_init(&w0);
	mbedtls_ecp_point_init(&L);
	error = knx_get_stored_spake(salt, rand, &it, &w0, &L);
	if (error)
	{
		PRINT_APP("Error: Stored spake record not found! Using runtime generated values\n");
		PRINT_APP(
			"Please create the data file using knx-gen-data and flash it with chilictl in order to fix this issue.\n");
	}
	else
	{
		oc_spake_set_parameters(rand, salt, it, w0, L);
		mbedtls_mpi_free(&w0);
		mbedtls_ecp_point_free(&L);
	}
#endif

	/* set the application callbacks */
	oc_set_hostname_cb(hostname_cb, NULL);
	oc_set_reset_cb(reset_cb, NULL);
	oc_set_factory_presets_cb(factory_presets_cb, NULL);
#if CASCODA_OTA_UPGRADE_ENABLED
	oc_set_swu_cb(swu_cb, (void *)"image_name");
#endif
	oc_set_programming_mode_cb(prog_mode_cb, NULL);

	/* start the stack */
	init = oc_main_init(&handler);

	oc_set_max_app_data_size(1024);
	oc_set_mtu_size(1232);

	if (init < 0)
	{
		PRINT_APP("oc_main_init failed %d.\n", init);
	}

	PRINT("KNX IoT device, waiting on incoming connections.\n");

	uint64_t iid = oc_core_get_device_iid(THIS_DEVICE);
	// Note: we are using printf here instead of PRINT so that
	// this information always gets printed.
	printf("Device iid: ");
	oc_print_uint64_t(iid, DEC_REPRESENTATION);
    printf("\n");

	printf("group publisher table:\n");
	oc_print_reduced_group_publisher_table();
	printf("group recipient table:\n");
	oc_print_reduced_group_recipient_table();

	while (1)
	{
		cascoda_io_handler(&dev);
		otTaskletsProcess(OT_INSTANCE);
		hardware_poll();
		oc_main_poll();
	}

	/* shut down the stack, should not get here */
	oc_main_shutdown();
	return 0;
}

/**
 * @}
 */
