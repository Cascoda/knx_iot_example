/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2025 Cascoda Ltd
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
 * Stubs for the application
 * This is generated code. please do not modify but run ChiliCuisine again.
 *
 * Note: this is stubbing a subset of the KNX IoT Stack API, additional functions might be required
 *
 * Typical usage of this file:
 * - create input file
 * - create test folder
 * - create test application (in test folder)
 *   - include this file in the test application (via an include directive)
 *     - copy file over from the generated data to the test folder.
 *   - include the application file in the test application (via an include directive)
 *   - compile the test application (via CMAKE)
 *
 *
 *
 
 */
 #ifdef __cplusplus
extern "C" {
#endif

// flag in the code...
#define UNIT_TEST


// c/c++ includes
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef PACKED
#define PACKED
#endif

#define btoa(x) ((x) ? "true" : "false")
#define param_time int
#define color DPT_Colour_RGB







//-----------------------------------------------
// Open thread Functions
//-----------------------------------------------

#define otError int
#define otInstance int
#define OT_INSTANCE 0
#define OT_DEVICE_ROLE_DETACHED 0

/**
 * This structure represents an MLE Link Mode configuration.
 */
typedef struct otLinkModeConfig
{
    bool mRxOnWhenIdle : 1; ///< 1, if the sender has its receiver on when not transmitting. 0, otherwise.
    bool mDeviceType : 1;   ///< 1, if the sender is an FTD. 0, otherwise.
    bool mNetworkData : 1;  ///< 1, if the sender requires the full Network Data. 0, otherwise.
} otLinkModeConfig;

otError  otThreadSetLinkMode(otInstance *aInstance, otLinkModeConfig aConfig)
{
  return 0;
}

otError otThreadSleepyChildResynchronize(otInstance *aInstance)
{
    return 0;
}

#define otDeviceRole int
otDeviceRole otThreadGetDeviceRole(otInstance *aInstance)
{
    return 0;
}

struct ca821x_dev *PlatformGetDeviceRef(void)
{
    return NULL;
}

ca_error EVBME_PowerUpRadio(struct ca821x_dev *pDeviceRef)
{
    return 0;
}



//-----------------------------------------------
// BSP Functions
//-----------------------------------------------

unsigned int BSP_ReadAbsoluteTime(void)
{
  return 0;
}

struct ca821x_dev {
/**
	 * Context pointer for free use by the application. This context pointer can be set
	 * by the application as required, such as to track the application
	 * state belonging to a certain device.
	 */
	void *context;
	/**
	 * Context for free use by the exchange (internal to the cascoda stack)
	 */
	void *exchange_context;

	/** Callback routines registered by the user, to be called by the api for upstream commands */
	//struct ca821x_api_callbacks callbacks;

#if CASCODA_CA_VER == 8210
	uint8_t  extaddr[8]; /**< Mirrors nsIEEEAddress in the PIB */
	uint16_t shortaddr;  /**< Mirrors macShortAddress in the PIB */
	uint8_t  lqi_mode;   /**< Mirrors lqi_mode on the CA8210 */
#endif                   // CASCODA_CA_VER == 8210

	//MAC Workarounds for V1.1 and MPW silicon (V0.x)
	uint8_t MAC_MPW; /**< Flag to enable workarounds for ca8210 v0.x */

#if CASCODA_MAC_BLACKLIST != 0
	/** An array of addresses. Messages from any of these addresses
     * will be filtered out. Works with short and extended addresses,
     * and addresses with AddressMode == MAC_MODE_NO_ADDR are not in
     * use. The PanID field is ignored.
     */
	//struct MacAddr blacklist[CASCODA_MAC_BLACKLIST];

#endif
};

// used in some functions....
struct ca821x_dev dev;

void cascoda_io_handler(struct ca821x_dev *pDeviceRef)
{
}

//-----------------------------------------------
// SDK Functions
//-----------------------------------------------

#define ca_tasklet int


// values define in ca821x-api/include/ca821x_error.h
#define ca_error int
	/*General Errors*/
#define CA_ERROR_SUCCESS          0x00 /*  Success */
#define CA_ERROR_FAIL             0x01 /*  Failed for miscellaneous reason*/
#define CA_ERROR_UNKNOWN          0x02 /*  Request for unknown data*/
#define CA_ERROR_INVALID          0x03 /*  Invalid request*/
#define CA_ERROR_INVALID_STATE    0x05 /*  Request cannot be processed in current state*/
#define CA_ERROR_BUSY             0x06 /*  Busy, try again*/
#define CA_ERROR_INVALID_ARGS     0x07 /*  Invalid arguments*/
#define CA_ERROR_NOT_HANDLED      0x08 /*  Request has not been handled*/
#define CA_ERROR_NOT_FOUND        0x09 /*  Requested resource not found*/
#define CA_ERROR_NO_BUFFER        0x0A /*  No buffers currently available*/
#define CA_ERROR_TIMEOUT          0x0B /*  Operation timed out*/
#define CA_ERROR_ALREADY          0x0C /*  Operation already executed*/
#define CA_ERROR_NOT_IMPLEMENTED  0x0D /*  Functionality not implemented*/
/* SPI Errors*/
#define CA_ERROR_SPI_WAIT_TIMEOUT        0xA0 /*  SPI Wait timed out*/
#define CA_ERROR_SPI_NACK_TIMEOUT        0xA1 /*  SPI NACKing for too long*/
#define CA_ERROR_SPI_SCAN_IN_PROGRESS    0xA2 /*  CA-821x is scanning and cannot be used*/
#define CA_ERROR_SPI_SEND_EXCHANGE_FAIL  0xA3 /*  SPI Message failed to send*/


void TASKLET_Cancel(ca_tasklet *task)
{

}

int TASKLET_ScheduleDelta(ca_tasklet* task, int period, void* data)
{
  return 0;
}

int TASKLET_Init(ca_tasklet* task, void* data)
{
  return 0;
}

void SED_InitPolling(int a, int b, int c)
{
  return;
}
int SED_PollSoon(void)
{
  return 0;
}
void SED_DeinitPolling(void)
{
}


int CO2DEV_PollButtons()
{
  return 0;
}


void PlatformSleep(uint32_t taskletTimeLeft)
{
}


void knx_service_sleep_period(uint32_t sleep_period)
{
}


//-----------------------------------------------
// STACK Functions - Memory
//-----------------------------------------------

void *
knx_malloc(size_t size)
{
  void *ptr = malloc(size);
  return ptr;
}

void *
knx_calloc(size_t amount, size_t size)
{
  void *ptr = calloc(amount, size);
  return ptr;
}

void
knx_free(void *ptr)
{
  free(ptr);
}

void *
knx_realloc(void *ptr, size_t size)
{
  void *new_ptr = realloc(ptr, size);
  return new_ptr;
}


//-----------------------------------------------
// STACK Functions
//-----------------------------------------------
#define PRINT_APP printf
#define PRINT printf
#define OC_ERR printf
#define OC_WRN printf
#define OC_DBG printf
#define ca_log_note printf

#define MAX_STRING 1024
#define MAX_PAYLOAD_STRING 1024

#define oc_rep_t int
#define CborEncoder int
#define oc_event_callback_retval_t int

void oc_do_s_mode_with_scope(int scope, char* url, char* something)
{

}
void oc_do_s_mode_with_scope_and_groupaddress(int scope, const char *resource_url,
                                         char *rp, bool check,
                                         int group_address_to_send)
{
}

#define oc_knx_version_info_t int
#define oc_string_t char*
#define oc_core_add_device_cb_t int
#define oc_request_handler_t int
#define oc_string_array_t char**
#define oc_properties_cb_t int
#define oc_resource_properties_t int
#define oc_content_format_t int
#define oc_resource_data_t int



/**
 * @brief LSM state machine values
 *
 */
typedef enum oc_lsm_state {
  LSM_S_UNLOADED = 0,      /**< (0) state is unloaded, e.g. ready for loading */
  LSM_S_LOADED = 1,        /**< (1) state is LOADED, e.g. normal operation */
  LSM_S_LOADING = 2,       /**< (2) state loading. */
  LSM_S_UNLOADING = 4,     /**< (4) state unloading loading. */
  LSM_S_LOADCOMPLETING = 5 /**< (5) cmd unload: state will be UNLOADED */
} oc_lsm_state_t;


typedef enum {
  OC_IF_NONE = 0,        /**< no interface defined */
  OC_IF_I = (1 << 1),    /**< if.i  (2)*/
  OC_IF_O = (1 << 2),    /**< if.o (4)*/
  OC_IF_G = (1 << 3),    /**< if.g.s.[ga] (8) */
  OC_IF_C = (1 << 4),    /**< if.c (16) */
  OC_IF_P = (1 << 5),    /**< if.p (32)*/
  OC_IF_D = (1 << 6),    /**< if.d (64)*/
  OC_IF_A = (1 << 7),    /**< if.a (128)*/
  OC_IF_S = (1 << 8),    /**< if.s (256)*/
  OC_IF_LI = (1 << 9),   /**< if.ll (512)*/
  OC_IF_B = (1 << 10),   /**< if.b (1024) */
  OC_IF_SEC = (1 << 11), /**< if.sec (2048)*/
  OC_IF_SWU = (1 << 12), /**< if.swu (4096)*/
  OC_IF_PM = (1 << 13),  /**< if.pm (8192)*/
  OC_IF_M = (1 << 14)    /**< if.m (manufacturer) (16384) */
} oc_interface_mask_t;

typedef struct oc_device_info_t
 {
   oc_string_t serialnumber;  
   oc_knx_version_info_t hwv; 
   oc_knx_version_info_t fwv; 
   oc_knx_version_info_t ap;  
   oc_string_t hwt; 
   oc_string_t model;    
   oc_string_t hostname; 
   uint32_t mid;         
   uint64_t fid;         
   uint32_t ia;          
   uint64_t iid;         
   uint32_t port;        
   uint32_t mport;       
   bool pm;              
   oc_lsm_state_t lsm_s; 
   oc_core_add_device_cb_t add_device_cb; 
   void *data;                            
 } oc_device_info_t;


// global variable
oc_device_info_t g_device_info;

oc_device_info_t* oc_core_get_device_info(size_t device)
{
  memset(&g_device_info, 0, sizeof(oc_device_info_t));
  return &g_device_info;
}


/**
 * @brief resource structure
 *
 */
struct oc_resource_s
{
  struct oc_resource_s *next;          /**< next resource */
  size_t device;                       /**< device index */
  oc_string_t name;                    /**< name of the resource (e.g. "n") */
  oc_string_t uri;                     /**< uri of the resource */
  oc_string_array_t types;             /**< "rt" types of the resource */
  oc_string_t dpt;                     /**< dpt of the resource */
  oc_interface_mask_t interfaces;      /**< supported interfaces */
  oc_content_format_t content_type;    /**< the content format that the resource
                                            supports, e.g. only 1 at the moment */
  oc_resource_properties_t properties; /**< properties (as bit mask) */
  oc_request_handler_t get_handler;    /**< callback for GET */
  oc_request_handler_t put_handler;    /**< callback for PUT */
  oc_request_handler_t post_handler;   /**< callback for POST */
  oc_request_handler_t delete_handler; /**< callback for DELETE */
  oc_properties_cb_t get_properties;   /**< callback for get properties */
  oc_properties_cb_t set_properties;   /**< callback for set properties */
  uint16_t observe_period_seconds;     /**< observe period in seconds */
  uint8_t fb_instance; /**< function block instance, default = 0 */
  const bool is_const; /**< Whether the associated resource data is readonly */
  oc_resource_data_t *runtime_data; /**< Runtime modifiable data*/
};

typedef struct oc_resource_s oc_resource_t;


typedef struct
{
  uint8_t id[16];
} oc_uuid_t;
#define OC_UUID_LEN  (37)

void
oc_gen_uuid(oc_uuid_t *uuid)
{
  
}

void
oc_uuid_to_str(const oc_uuid_t *uuid, char *buffer, int buflen)
{
  memset(buffer,0,buflen);
}


//----------------------------------------------
//----------------------------------------------

char* oc_string(oc_string_t in)
{
return in;
}


void oc_new_string( oc_string_t *ocstring, const char *str, size_t str_len)
{
  //oc_malloc(
  //  ocstring, str_len + 1, BYTE_POOL);
  //memcpy(oc_string(*ocstring), (const uint8_t *)str, str_len);
  //memcpy(oc_string(*ocstring) + str_len, (const uint8_t *)"", 1);
}

void oc_free_string(
  oc_string_t *ocstring)
{
  
}

//----------------------------------------------
//----------------------------------------------

typedef struct oc_group_object_notification_t
{
  oc_string_t value; /**< generic value received. */
  uint32_t sia;      /**< (source id) sender individual address */
  oc_string_t st;    /**< Service type code (write=w, read=r, response=rp) */
  uint32_t ga;       /**< group address */
} oc_group_object_notification_t;


typedef oc_event_callback_retval_t (*oc_trigger_t)(void *);

void
oc_set_delayed_callback(void *cb_data, oc_trigger_t callback, uint16_t seconds)
{
  
}

void
oc_remove_delayed_callback(void *cb_data, oc_trigger_t callback)
{
}


//----------------------------------------------
// GOT table
//----------------------------------------------

#define GOT_MAX_ENTRIES 100

typedef enum {
  OC_CFLAG_NONE = 0, /**< Communication */
  OC_CFLAG_COMMUNICATION =
  1 << 2, /**< false = Group Object value cannot read or written.*/
  OC_CFLAG_READ = 1 << 3, /**< 8 false = Group Object value cannot be read.*/
  OC_CFLAG_WRITE =
  1 << 4, /**< 16 false = Group Object value cannot be written.*/
  OC_CFLAG_INIT = 1 << 5, /**< 32 false = Disable read after initialization.*/
  OC_CFLAG_TRANSMISSION =
  1 << 6, /**< 64 false = Group Object value is not transmitted.*/
  OC_CFLAG_UPDATE =
  1 << 7, /**< 128 false = Group Object value is not updated.*/
} oc_cflag_mask_t;

typedef struct oc_group_object_table_t
{
  int id;                 /**< contents of id*/
  oc_string_t href;       /**< contents of href*/
  oc_cflag_mask_t cflags; /**< contents of cflags as bitmap*/
  int ga_len;             /**< length of the array of ga identifiers*/
  uint32_t *ga;           /**< array of group addresses (unsigned integers) */
} oc_group_object_table_t;

int g_stubs_go_table_index = 1;
int
oc_core_find_group_object_table_index(uint32_t group_address)
{
return g_stubs_go_table_index;
}

oc_string_t
oc_core_find_group_object_table_url_from_index(int index)
{
  return "";
}

int g_stubs_oc_core_find_group_object_table_url = -1;
int
oc_core_find_group_object_table_url(const char *url)
{
  return g_stubs_oc_core_find_group_object_table_url;
}

int
oc_core_get_group_object_table_total_size(void)
{
  return GOT_MAX_ENTRIES;
}


oc_group_object_table_t *
oc_core_get_group_object_table_entry(int index)
{
  return NULL;
}

void
oc_print_group_object_table_entry(int entry)
{
}


//----------------------------------------------
// GM table
//----------------------------------------------

#define G_GM_MAX_ENTRIES 100
typedef struct oc_group_mapping_table_t
{
  int id;       /**< (0) contents of id*/
  int ga_len;   /**< length of the array of ga identifiers*/
  uint64_t *ga; /**< (7) array of group addresses (unsigned 64 bit integers) */
  uint32_t dataType;    /**< (116) dataType */
  oc_string_t groupKey; /**< (s:107) groupKey */
  bool authentication;  /**< (115:28:97) a authentication applied (default true,
                           if  groupKey exists)*/
  bool confidentiality; /**< (115:28:99) c confidentiality applied (default
                           true, if groupKey exists) */
} oc_group_mapping_table_t;



void
oc_print_group_mapping_table_entry(int entry)
{
}

int
oc_core_find_nr_used_in_group_mapping_table()
{
  return 0;
}

int
oc_core_get_group_mapping_table_size(void)
{

  return G_GM_MAX_ENTRIES;

}


oc_group_mapping_table_t *
oc_get_group_mapping_entry(size_t device_index, int index)
{
  (void)device_index;

  if (index < 0) {
    return NULL;
  }
  if (index >= oc_core_get_group_mapping_table_size()) {
    return NULL;
  }
  return NULL;

}

int oc_get_f_netip_tol(size_t device_index)
{
  return 0;
}

oc_string_t oc_get_f_netip_key(size_t device_index)
{
    return NULL;
}

int oc_get_f_netip_ttl(size_t device_index)
{
   return 64;
}

uint32_t oc_get_f_netip_mcast(size_t device_index)
{
    return 5;
}

//----------------------------------------------
// base64
//----------------------------------------------
int
oc_base64_encode(const uint8_t *input, size_t input_len, uint8_t *output_buffer,
                 size_t output_buffer_len)
{
  /* The Base64 alphabet. This table provides a mapping from 6-bit binary
   * values to Base64 characters.
   */
  uint8_t alphabet[65] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                           'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                           'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
                           'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                           'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                           'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', '+', '/', '=' };
  uint8_t val = 0;
  size_t i;
  int j = 0;

  /* Calculate the length of the Base64 encoded output.
   * Every sequence of 3 bytes (with padding, if necessary)
   * is represented as 4 bytes (characters) in Base64.
   */
  size_t output_len = (input_len / 3) * 4;
  if (input_len % 3 != 0) {
    output_len += 4;
  }

  /* If the output buffer provided was not large enough, return an error. */
  if (output_buffer_len < output_len)
    return -1;

  /* handle the case that an empty input is provided */
  if (input_len == 0) {
    output_buffer[0] = '\0';
  }
  /* Process every byte of input by keeping state across 3 byte blocks
   * to capture 4 6-bit binary blocks that each map to a Base64 character.
   */
  for (i = 0; i < input_len; i++) {
    /* This is the first byte of a 3 byte block of input. Its first
     * 6 bits would be encoded into a character from the Base64 alphabet.
     * Its last 2 bits would be the first 2 bits of the following 6-bit binary
     * block.
     * Explicitly zero out the remaining bits.
     */
    if (i % 3 == 0) {
      val = (input[i] >> 2);
      output_buffer[j++] = alphabet[val];
      val = input[i] << 4;
      val &= 0x30;
    }
    /* This is the second byte of a 3 byte block of input. Combine
     * the last 2 bits of the previous byte of input with the first 4 bits
     * of the current byte of input to encode the next Base64 character.
     * Its last 4 bits would be the first 4 bits of the following 6-bit binary
     * block.
     * Explicitly zero out the remaining bits.
     */
    else if (i % 3 == 1) {
      val |= (input[i] >> 4);
      output_buffer[j++] = alphabet[val];
      val = input[i] << 2;
      val &= 0x3D;
    }
    /* This is the last byte of a 3 byte block of input. Combine
     * the last 4 bits of the previous byte of input with the first 2 bits
     * of the current byte of input to encode the next Base64 character.
     * Its last 6 bits directly map to the following Base64 character
     * thereby completing a 4 byte encoding of the preceeding 3 byte
     * block of input.
     */
    else {
      val |= (input[i] >> 6);
      output_buffer[j++] = alphabet[val];
      val = input[i] & 0x3F;
      output_buffer[j++] = alphabet[val];
    }
  }

  /* If the input size wasn't a multiple of 3, we would
   * have leftover bits to encode into the next Base64 character.
   */
  if (i % 3 != 0) {
    output_buffer[j++] = alphabet[val];
  }

  /* Any leftover space in the encoded string is padded with the
   * = character.
   */
  while (j < (int)output_len) {
    output_buffer[j++] = '=';
  }

  return j;
}

int
oc_base64_decode(uint8_t *str, size_t len)
{
  /* All valid Base64 encoded strings will be multiples of 4 */
  if (0 != (len % 4)) {
    return -1;
  }
  /* The Base64 input string is decoded in-place. */
  size_t i = 0;
  int j = 0;
  unsigned char val_c = 0, val_s = 0;

  /* Process every input character */
  for (i = 0; i < len; i++) {
    val_s = str[i];

    /* Perform a reverse-mapping from Base64 character to
     * a 6-bit binary sequence.
     */
    if (val_s >= 'A' && val_s <= 'Z')
      val_s -= 65;
    else if (val_s >= 'a' && val_s <= 'z')
      val_s -= 71;
    else if (val_s >= '0' && val_s <= '9')
      val_s += 4;
    else if (val_s == '+')
      val_s = 62;
    else if (val_s == '/')
      val_s = 63;
    /* Break if we encounter the padding character.
     * The input buffer str now contains the fully decoded string.
     */
    else if (val_s == '=') {
      /* Padding character "=" can only show up as last 2 characters */
      if (i < len - 2) {
        return -1;
      }
      if (i == len - 2 && '=' != str[i + 1]) {
        return -1;
      }
      break;
    }
    /* Return an error if we encounter a character that is outside
     * of the Base64 alphabet.
     */
    else
      return -1;

    /* Decode all 4 byte blocks to 3 bytes of binary output by
     * laying out their 6-bit blocks into a sequence of 3 bytes.
     */
    if (i % 4 == 0) {
      /* 1st 6 bits of output byte 1 */
      val_c = val_s << 2;
      val_c &= 0xFD;
    } else if (i % 4 == 1) {
      /* Last 2 bits of output byte 1 */
      val_c |= (val_s >> 4);
      str[j++] = val_c;
      /* 1st 4 bits of output byte 2 */
      val_c = val_s << 4;
      val_c &= 0xF0;
    } else if (i % 4 == 2) {
      /* Last 4 bits of output byte 2 */
      val_c |= (val_s >> 2);
      str[j++] = val_c;
      /* 1st 2 bits of output byte 3 */
      val_c = val_s << 6;
      val_c &= 0xD0;
    } else {
      /* Last 6 bits of output byte 3 */
      val_c |= val_s;
      str[j++] = val_c;
    }
  }

  /* zero out the remaining bytes */
  for (i = j; i < len; i++) {
    str[i] = 0;
  }

  return j;
}



//----------------------------------------------
//----------------------------------------------

typedef void (*oc_factory_presets_cb_t)(size_t device, void *data);
void
oc_set_factory_presets_cb(oc_factory_presets_cb_t cb, void *data)
{
}

typedef void (*oc_gateway_s_mode_cb_t)(
  size_t device_index, char *sender_ip_address,
  oc_group_object_notification_t *s_mode_message, void *data);
int
oc_set_gateway_cb(oc_gateway_s_mode_cb_t cb, void *data)
{
  return 0;
}

oc_lsm_state_t g_stub_lsm_state = LSM_S_LOADED;

oc_lsm_state_t oc_a_lsm_state(int device_index)
{
  return g_stub_lsm_state;
}


static const char *interface_strings[] = { "if.i",  "if.o",  "if.g.s", "if.c",
                                           "if.p",  "if.d",  "if.a",   "if.s",
                                           "if.ll", "if.b",  "if.sec", "if.swu",
                                           "if.pm", "if.m.x" };

const char *
get_interface_string(oc_interface_mask_t mask)
{
  if (mask & OC_IF_I)
    return interface_strings[0];
  if (mask & OC_IF_O)
    return interface_strings[1];
  if (mask & OC_IF_G)
    return interface_strings[2];
  if (mask & OC_IF_C)
    return interface_strings[3];
  if (mask & OC_IF_P)
    return interface_strings[4];
  if (mask & OC_IF_D)
    return interface_strings[5];
  if (mask & OC_IF_A)
    return interface_strings[6];
  if (mask & OC_IF_S)
    return interface_strings[7];
  if (mask & OC_IF_LI)
    return interface_strings[8];
  if (mask & OC_IF_B)
    return interface_strings[9];
  if (mask & OC_IF_SEC)
    return interface_strings[10];
  if (mask & OC_IF_SWU)
    return interface_strings[11];
  if (mask & OC_IF_PM)
    return interface_strings[12];
  if (mask & OC_IF_M)
    return interface_strings[13];
  return "";
}

//-----------------------------------------------
// URL defines
//-----------------------------------------------
#define URL_SENDSHOT "/p/o_1_1" /* URL 'SendShot'  desc:'' */
#define URL_RECEIVESHOT "/p/o_1_2" /* URL 'ReceiveShot'  desc:'' */
#define URL_SENDSHOTSTATUS "/p/o_1_3" /* URL 'SendShotStatus'  desc:'' */
#define URL_RECEIVESHOTSTATUS "/p/o_1_4" /* URL 'ReceiveShotStatus'  desc:'' */
#define URL_SENDREADY "/p/o_1_5" /* URL 'SendReady'  desc:'' */
#define URL_RECEIVEREADY "/p/o_1_6" /* URL 'ReceiveReady'  desc:'' */
#define URL_STARTING_PLAYER "/p/p_1_1" /* URL 'Starting_Player'  desc:'' */

/* all data points */
//extern const datapoint_t g_datapoints[];
//extern const size_t num_datapoints; 



//-----------------------------------------------
// MT ENUM defines
//-----------------------------------------------
/* all parameters */
//extern const datapoint_t g_parameters[];
//extern const size_t num_parameters; 
/* ENUM defines, for each module instance */ 

//-----------------------------------------------
// TYPES
//----------------------------------------------- 

///@defgroup DPT_Param_Bool
///@ingroup DPT_Param_Bool
typedef 
enum DPT_Param_BoolValueBool {
  DPST_60012_3_F_1_1 = 0,
  ValueBoolFalse = 0,
  DPT_Param_BoolValueBoolFalse = 0,
  DPST_60012_3_F_1_2 = 1,
  ValueBoolTrue = 1,
  DPT_Param_BoolValueBoolTrue = 1,
} DPT_Param_Bool;

///@defgroup DPT_Shot_Status

enum DPT_Shot_StatusShipType {
  DPST_60004_1_F_3_1 = 0,
  ShipTypeNo_Hit = 0,
  DPT_Shot_StatusShipTypeNo_Hit = 0,
  DPST_60004_1_F_3_2 = 1,
  ShipTypeDestroyer = 1,
  DPT_Shot_StatusShipTypeDestroyer = 1,
  DPST_60004_1_F_3_3 = 2,
  ShipTypeSubmarine = 2,
  DPT_Shot_StatusShipTypeSubmarine = 2,
  DPST_60004_1_F_3_4 = 3,
  ShipTypeCruiser = 3,
  DPT_Shot_StatusShipTypeCruiser = 3,
  DPST_60004_1_F_3_5 = 4,
  ShipTypeBattleship = 4,
  DPT_Shot_StatusShipTypeBattleship = 4,
  DPST_60004_1_F_3_6 = 5,
  ShipTypeCarrier = 5,
  DPT_Shot_StatusShipTypeCarrier = 5,
}; 

/**
 * @ingroup DPT_Shot_Status
 * DPT_Shot_Status struct with member fields for DPT_Shot_Status
 */
struct DPT_Shot_Status_s {

  /**
   * union to alias DPST-60004-1_F-1 name variations
   */
  ///@cond U_START
  union {
  ///@endcond
  ///@name Aliases for DPT_Shot_Status DPST-60004-1_F-1
  ///@{
    bool Hit; //!< DPST-60004-1_F-1 member alias Hit for DPT_Shot_Status
    bool DPST_60004_1_F_1; //!< DPST-60004-1_F-1 member alias DPST-60004-1_F-1 for DPT_Shot_Status
    bool F_1; //!< DPST-60004-1_F-1 member alias F-1 for DPT_Shot_Status
  ///@}
  ///@cond U_END
  } PACKED; //!< union to alias DPST-60004-1_F-1 name variations
  ///@endcond

  /**
   * union to alias DPST-60004-1_F-2 name variations
   */
  ///@cond U_START
  union {
  ///@endcond
  ///@name Aliases for DPT_Shot_Status DPST-60004-1_F-2
  ///@{
    bool Sunk; //!< DPST-60004-1_F-2 member alias Sunk for DPT_Shot_Status
    bool DPST_60004_1_F_2; //!< DPST-60004-1_F-2 member alias DPST-60004-1_F-2 for DPT_Shot_Status
    bool F_2; //!< DPST-60004-1_F-2 member alias F-2 for DPT_Shot_Status
  ///@}
  ///@cond U_END
  } PACKED; //!< union to alias DPST-60004-1_F-2 name variations
  ///@endcond

  /**
   * union to alias DPST-60004-1_F-3 name variations
   */
  ///@cond U_START
  union {
  ///@endcond
  ///@name Aliases for DPT_Shot_Status DPST-60004-1_F-3
  ///@{
    enum DPT_Shot_StatusShipType ShipType; //!< DPST-60004-1_F-3 member alias ShipType for DPT_Shot_Status
    enum DPT_Shot_StatusShipType DPST_60004_1_F_3; //!< DPST-60004-1_F-3 member alias DPST-60004-1_F-3 for DPT_Shot_Status
    enum DPT_Shot_StatusShipType F_3; //!< DPST-60004-1_F-3 member alias F-3 for DPT_Shot_Status
  ///@}
  ///@cond U_END
  } PACKED; //!< union to alias DPST-60004-1_F-3 name variations
  ///@endcond
} PACKED;
///@ingroup DPT_Shot_Status
typedef struct DPT_Shot_Status_s DPT_Shot_Status;

///@defgroup DPT_Start
///@ingroup DPT_Start
typedef bool DPT_Start;

///@defgroup DPT_Uint_XY 

/**
 * @ingroup DPT_Uint_XY
 * DPT_Uint_XY struct with member fields for DPT_Uint_XY
 */
struct DPT_Uint_XY_s {

  /**
   * union to alias DPST-60009-1_F-1 name variations
   */
  ///@cond U_START
  union {
  ///@endcond
  ///@name Aliases for DPT_Uint_XY DPST-60009-1_F-1
  ///@{
    unsigned int X; //!< DPST-60009-1_F-1 member alias X for DPT_Uint_XY
    unsigned int DPST_60009_1_F_1; //!< DPST-60009-1_F-1 member alias DPST-60009-1_F-1 for DPT_Uint_XY
    unsigned int F_1; //!< DPST-60009-1_F-1 member alias F-1 for DPT_Uint_XY
  ///@}
  ///@cond U_END
  } PACKED; //!< union to alias DPST-60009-1_F-1 name variations
  ///@endcond

  /**
   * union to alias DPST-60009-1_F-2 name variations
   */
  ///@cond U_START
  union {
  ///@endcond
  ///@name Aliases for DPT_Uint_XY DPST-60009-1_F-2
  ///@{
    unsigned int Y; //!< DPST-60009-1_F-2 member alias Y for DPT_Uint_XY
    unsigned int DPST_60009_1_F_2; //!< DPST-60009-1_F-2 member alias DPST-60009-1_F-2 for DPT_Uint_XY
    unsigned int F_2; //!< DPST-60009-1_F-2 member alias F-2 for DPT_Uint_XY
  ///@}
  ///@cond U_END
  } PACKED; //!< union to alias DPST-60009-1_F-2 name variations
  ///@endcond
} PACKED;
///@ingroup DPT_Uint_XY
typedef struct DPT_Uint_XY_s DPT_Uint_XY;



//-----------------------------------------------
// MT ENUM defines
//-----------------------------------------------

typedef enum DatapointType{
  DatapointType_bool,
  DatapointType_int,
  DatapointType_float,
  DatapointType_string,
  DatapointType_DPT_Param_Bool,
  DatapointType_DPT_Shot_Status,
  DatapointType_DPT_Start,
  DatapointType_DPT_Uint_XY,
  DatapointType_MAX_NUM,
} DatapointType;

//-----------------------------------------------
// Generated Types
//-----------------------------------------------

typedef struct datapoint_t {
  oc_resource_t resource;
  const char *const *metadata;
#ifndef OPTIMIZE_FLASH_SIZE
  const char *feedback_url;
#endif
  DatapointType type;
  void *g_var;
#ifndef OPTIMIZE_FLASH_SIZE
  volatile void *g_fault;
#endif
  uint16_t num_elements;
  uint8_t persistent : 1;
  uint8_t default_present : 1;
} datapoint_t;



//-----------------------------------------------
// Helper functions
//-----------------------------------------------

// copied
bool get_module_url(char* out_url, const char* in_url, int module_index)
{
  int digits_after_underscore = 0;
  size_t url_length = strlen(in_url);
  size_t keep_len;
  char temp[50];
      
  // Find out how many more digits we have after the underscore
  for (int i = url_length - 1; i >= 0; --i)
  {
      // We have reached the underscore, we can stop counting
      if (*(in_url + i) == '_')
          break; 
          
      // We have counted one more digit after the underscore
      if (isdigit(*(in_url + i)))
          ++digits_after_underscore;
      else // We have encountered a non-digit and non-underscore, which is unexpected.
          return true; 
  }
  
  // We can now calculate the number of characters that we need to keep from the original URL
  keep_len = url_length - digits_after_underscore;
  
  // Copy over those characters into a temp array
  strncpy(temp, in_url, keep_len);
  temp[keep_len] = '\0';
  
  // Add the module number at the end, and set the out_url pointer
  sprintf(temp + strlen(temp), "%d", module_index);
  strcpy(out_url, temp);
  
  return false;
}

// copied
uint16_t get_instance_num_from_url(const char *url)
{  
  enum
  {
      UNITS_DIGIT = 0,
      TENS_DIGIT = 1,
      HUNDREDS_DIGIT = 2,
      MAX_DIGIT_SUPPORT = 3,
  };
  
  uint8_t digits_after_underscore = 0;
  char digits_array[MAX_DIGIT_SUPPORT] = {0};
  size_t url_length = strlen(url);
    
  // Find out how many more digits we have after the underscore
  for (int i = url_length - 1; i >= 0; --i) {
    // We have reached the underscore, we can stop counting 
    if (url[i] == '_') {
      break;
    }
    
    // We have counted one more digit after the underscore
    if (isdigit(url[i])) {
      if (++digits_after_underscore > MAX_DIGIT_SUPPORT) {
        PRINT_APP("ERROR: Instance number is too big, > 999\n");
        return 0;
      } 
      digits_array[digits_after_underscore - 1] = url[i] - '0';
    } else {
      PRINT_APP("ERROR: URL Does not end with an underscore followed by a number. Are you sure this URL uses instances?\n");
      // We have encountered a non-digit and non-underscore, which is not expected.
      return 0;
    }
  }
  
  // Add all the digits up
  uint16_t instance_number = digits_array[UNITS_DIGIT]  
                             + 10 * digits_array[TENS_DIGIT]
                             + 100 * digits_array[HUNDREDS_DIGIT];
                             
  return instance_number;
}


const datapoint_t *get_datapoint_by_url(const char *url) {
  //this can likely be optimised in the future for speed
  //with a binary search or similar
  
  return NULL;
}

void datapoint_set(const datapoint_t *dp, void *in, int start, int n)
{
}

//-----------------------------------
// TYPES
//-----------------------------------
// type DPT_Param_Bool
// type DPT_Shot_Status
// type DPT_Start
// type DPT_Uint_XY

// define parameter type float as datapoint type
#define DPT_Param_Float float
#define DatapointType_DPT_Param_Float DatapointType_float

//-----------------------------------
// GLOBAL data points stub variables
//-----------------------------------
/*
 OrderedDict([('name', 'SendShot'), ('url', '/p/o_1_1'), ('interfaces', 'if.o'), ('resourcetypes', ['urn:knx:dpa.65500.101']), ('dpt', 'urn:knx:dpt.uint_XY'), ('instance', 1)])
*/
DPT_Uint_XY g_stub_SendShot; /* global "SendShot"  desc:"" */
/*
 OrderedDict([('name', 'ReceiveShot'), ('url', '/p/o_1_2'), ('interfaces', 'if.i'), ('resourcetypes', ['urn:knx:dpa.65501.111']), ('dpt', 'urn:knx:dpt.uint_XY'), ('instance', 1), ('device_receive_function_array', ['  if (strcmp(url, URL_RECEIVESHOT) == 0)', '  {', '    onReceivedShot(url);', '  }'])])
*/
DPT_Uint_XY g_stub_ReceiveShot; /* global "ReceiveShot"  desc:"" */
/*
 OrderedDict([('name', 'SendShotStatus'), ('url', '/p/o_1_3'), ('interfaces', 'if.o'), ('resourcetypes', ['urn:knx:dpa.65501.102']), ('dpt', 'urn:knx:dpt.shot_Status'), ('instance', 1)])
*/
DPT_Shot_Status g_stub_SendShotStatus; /* global "SendShotStatus"  desc:"" */
/*
 OrderedDict([('name', 'ReceiveShotStatus'), ('url', '/p/o_1_4'), ('interfaces', 'if.i'), ('resourcetypes', ['urn:knx:dpa.65500.112']), ('dpt', 'urn:knx:dpt.shot_Status'), ('instance', 1), ('device_receive_function_array', ['  if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0)', '  {', '    onReceivedShotStatus(url);', '  }'])])
*/
DPT_Shot_Status g_stub_ReceiveShotStatus; /* global "ReceiveShotStatus"  desc:"" */
/*
 OrderedDict([('name', 'SendReady'), ('url', '/p/o_1_5'), ('interfaces', 'if.o'), ('resourcetypes', ['urn:knx:dpa.65500.103']), ('dpt', 'urn:knx:dpt.start'), ('instance', 1)])
*/
DPT_Start g_stub_SendReady; /* global "SendReady"  desc:"" */
/*
 OrderedDict([('name', 'ReceiveReady'), ('url', '/p/o_1_6'), ('interfaces', 'if.i'), ('resourcetypes', ['urn:knx:dpa.65501.113']), ('dpt', 'urn:knx:dpt.start'), ('instance', 1), ('device_receive_function_array', ['  if (strcmp(url, URL_RECEIVEREADY) == 0)', '  {', '    onReceivedReady(url);', '  }'])])
*/
DPT_Start g_stub_ReceiveReady; /* global "ReceiveReady"  desc:"" */
/*
 parameter: OrderedDict([('name', 'Starting_Player'), ('url', '/p/p_1_1'), ('interfaces', 'if.p'), ('resourcetypes', ['urn:knx:dpa.65500.201']), ('dpt', 'urn:knx:dpt.param_Bool'), ('instance', 1)])
*/
DPT_Param_Bool g_stub_Starting_Player; /* global "Starting_Player"  */


// Getters/Setters for DPT_Param_Bool

bool app_is_DPT_Param_Bool_url(const char* url)
{
  if (strcmp(url, URL_STARTING_PLAYER) == 0){
    return true;
  }

  return false;
}

void app_set_DPT_Param_Bool_default_value(const char* url)
{
   if (strcmp(url, URL_STARTING_PLAYER) == 0){
      memset(&g_stub_Starting_Player, 0, sizeof(DPT_Param_Bool));
   }
  
}

void app_set_DPT_Param_Bool_default_value_persistent(const char* url)
{
// not really persistent
   if (strcmp(url, URL_STARTING_PLAYER) == 0){
      memset(&g_stub_Starting_Player, 0, sizeof(DPT_Param_Bool));
   }
}


void app_set_DPT_Param_Bool_array_elems(const char* url, const DPT_Param_Bool* in, int start, int n, bool store_persistently)
{
  printf("app_set_DPT_Param_Bool_array_elems: not implemented\n");
  return;
}

void app_set_DPT_Param_Bool_array(const char* url, const DPT_Param_Bool* in, int n, bool store_persistently)
{
  printf("app_set_DPT_Param_Bool_array : not implemented\n");
}

void app_set_DPT_Param_Bool_variable(const char* url, const DPT_Param_Bool* in)
{
// parameters exists
  // STARTING_PLAYER
   if (strcmp(url, URL_STARTING_PLAYER) == 0){
      memcpy(&g_stub_Starting_Player, in, sizeof(g_stub_Starting_Player));
   } 
}

const DPT_Param_Bool* app_get_DPT_Param_Bool_array_elems(const char *url, DPT_Param_Bool* out, int start, int n)
{
  printf("app_get_DPT_Param_Bool_array_elems: not implemented\n");
  return NULL;
}

const DPT_Param_Bool* app_get_DPT_Param_Bool_array(const char *url, DPT_Param_Bool* out, int n)
{
  return app_get_DPT_Param_Bool_array_elems(url, out, 0, n);
}



DPT_Param_Bool g_stub_DPT_Param_Bool;
const DPT_Param_Bool* app_get_DPT_Param_Bool_variable(const char *url, DPT_Param_Bool* out)
{
   // DPT_Param_Bool
   if (strcmp(url, URL_STARTING_PLAYER) == 0){
      memcpy(out, &g_stub_Starting_Player, sizeof(g_stub_Starting_Player));
   }
  if (out != NULL) {
    return out;
  } else {
    return &g_stub_DPT_Param_Bool;
  }
}

bool oc_parse_DPT_Param_Bool_single(oc_rep_t *rep, DPT_Param_Bool *out)
{
  printf("oc_parse_DPT_Param_Bool_single: not implemented\n");
  return false;
}

bool oc_parse_DPT_Param_Bool(oc_rep_t *rep, DPT_Param_Bool *out)
{
  printf("oc_parse_DPT_Param_Bool: not implemented\n");
  return false;
}

bool oc_parse_DPT_Param_Bool_array(oc_rep_t *rep, DPT_Param_Bool *out, int n)
{
  printf("oc_parse_DPT_Param_Bool_array: not implemented\n");
  return false;
}

void oc_encode_DPT_Param_Bool_single(CborEncoder *parent, const DPT_Param_Bool *in)
{
  printf("oc_encode_DPT_Param_Bool_single: not implemented\n");
}

void oc_encode_DPT_Param_Bool(const DPT_Param_Bool *in, bool is_metadata)
{
  printf("oc_encode_DPT_Param_Bool: not implemented\n");
}

void oc_encode_DPT_Param_Bool_array(const DPT_Param_Bool *in, int n)
{
  printf("oc_encode_DPT_Param_Bool_array: not implemented\n");
}

void persistent_store_DPT_Param_Bool(const char *name, const DPT_Param_Bool *in)
{
  printf("persistent_store_DPT_Param_Bool: not implemented\n");
}

void persistent_store_DPT_Param_Bool_array(const char *name, const DPT_Param_Bool *in, int n)
{

  printf("persistent_store_DPT_Param_Bool_array: not implemented\n");
}

bool persistent_load_DPT_Param_Bool(const char *name, DPT_Param_Bool *out)
{
  
  printf("persistent_load_DPT_Param_Bool: not implemented\n");
  return false;
}

bool persistent_load_DPT_Param_Bool_array(const char *name, DPT_Param_Bool *out, int n)
{
  printf("persistent_load_DPT_Param_Bool_array: not implemented\n");
  return false;
}

int app_sprintf_DPT_Param_Bool(const DPT_Param_Bool *in, char* text, int size)
{
  printf("app_sprintf_DPT_Param_Bool: not implemented\n");
  return 0;
}

int app_sscanf_DPT_Param_Bool(DPT_Param_Bool *in, char* text)
{
  printf("app_sscanf_DPT_Param_Bool: not implemented\n");
  return 0;
}

int app_str_expected_DPT_Param_Bool(int select, char* text)
{
  printf("app_str_expected_DPT_Param_Bool: not implemented\n");
  return 0;
  if (text == NULL) {
    return 1;
  }
 
  
  return 1;
}




// Getters/Setters for DPT_Shot_Status

bool app_is_DPT_Shot_Status_url(const char* url)
{
  if (strcmp(url, URL_SENDSHOTSTATUS) == 0){
    return true;
  }
  if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0){
    return true;
  }

  return false;
}

void app_set_DPT_Shot_Status_default_value(const char* url)
{
   if (strcmp(url, URL_SENDSHOTSTATUS) == 0){
      memset(&g_stub_SendShotStatus, 0, sizeof(DPT_Shot_Status));
   }
   if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0){
      memset(&g_stub_ReceiveShotStatus, 0, sizeof(DPT_Shot_Status));
   }
  
}

void app_set_DPT_Shot_Status_default_value_persistent(const char* url)
{
// not really persistent
   if (strcmp(url, URL_SENDSHOTSTATUS) == 0){
      memset(&g_stub_SendShotStatus, 0, sizeof(DPT_Shot_Status));
   }
   if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0){
      memset(&g_stub_ReceiveShotStatus, 0, sizeof(DPT_Shot_Status));
   }
}


void app_set_DPT_Shot_Status_array_elems(const char* url, const DPT_Shot_Status* in, int start, int n, bool store_persistently)
{
  printf("app_set_DPT_Shot_Status_array_elems: not implemented\n");
  return;
}

void app_set_DPT_Shot_Status_array(const char* url, const DPT_Shot_Status* in, int n, bool store_persistently)
{
  printf("app_set_DPT_Shot_Status_array : not implemented\n");
}

void app_set_DPT_Shot_Status_variable(const char* url, const DPT_Shot_Status* in)
{
   if (strcmp(url, URL_SENDSHOTSTATUS) == 0){
      memcpy(&g_stub_SendShotStatus, in, sizeof(g_stub_SendShotStatus));
   }
   if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0){
      memcpy(&g_stub_ReceiveShotStatus, in, sizeof(g_stub_ReceiveShotStatus));
   }
// parameters exists
  // STARTING_PLAYER 
}

const DPT_Shot_Status* app_get_DPT_Shot_Status_array_elems(const char *url, DPT_Shot_Status* out, int start, int n)
{
  printf("app_get_DPT_Shot_Status_array_elems: not implemented\n");
  return NULL;
}

const DPT_Shot_Status* app_get_DPT_Shot_Status_array(const char *url, DPT_Shot_Status* out, int n)
{
  return app_get_DPT_Shot_Status_array_elems(url, out, 0, n);
}



DPT_Shot_Status g_stub_DPT_Shot_Status;
const DPT_Shot_Status* app_get_DPT_Shot_Status_variable(const char *url, DPT_Shot_Status* out)
{
   if (strcmp(url, URL_SENDSHOTSTATUS) == 0){
      memcpy(out, &g_stub_SendShotStatus, sizeof(g_stub_SendShotStatus));
   }
   if (strcmp(url, URL_RECEIVESHOTSTATUS) == 0){
      memcpy(out, &g_stub_ReceiveShotStatus, sizeof(g_stub_ReceiveShotStatus));
   }
  if (out != NULL) {
    return out;
  } else {
    return &g_stub_DPT_Shot_Status;
  }
}

bool oc_parse_DPT_Shot_Status_single(oc_rep_t *rep, DPT_Shot_Status *out)
{
  printf("oc_parse_DPT_Shot_Status_single: not implemented\n");
  return false;
}

bool oc_parse_DPT_Shot_Status(oc_rep_t *rep, DPT_Shot_Status *out)
{
  printf("oc_parse_DPT_Shot_Status: not implemented\n");
  return false;
}

bool oc_parse_DPT_Shot_Status_array(oc_rep_t *rep, DPT_Shot_Status *out, int n)
{
  printf("oc_parse_DPT_Shot_Status_array: not implemented\n");
  return false;
}

void oc_encode_DPT_Shot_Status_single(CborEncoder *parent, const DPT_Shot_Status *in)
{
  printf("oc_encode_DPT_Shot_Status_single: not implemented\n");
}

void oc_encode_DPT_Shot_Status(const DPT_Shot_Status *in, bool is_metadata)
{
  printf("oc_encode_DPT_Shot_Status: not implemented\n");
}

void oc_encode_DPT_Shot_Status_array(const DPT_Shot_Status *in, int n)
{
  printf("oc_encode_DPT_Shot_Status_array: not implemented\n");
}

void persistent_store_DPT_Shot_Status(const char *name, const DPT_Shot_Status *in)
{
  printf("persistent_store_DPT_Shot_Status: not implemented\n");
}

void persistent_store_DPT_Shot_Status_array(const char *name, const DPT_Shot_Status *in, int n)
{

  printf("persistent_store_DPT_Shot_Status_array: not implemented\n");
}

bool persistent_load_DPT_Shot_Status(const char *name, DPT_Shot_Status *out)
{
  
  printf("persistent_load_DPT_Shot_Status: not implemented\n");
  return false;
}

bool persistent_load_DPT_Shot_Status_array(const char *name, DPT_Shot_Status *out, int n)
{
  printf("persistent_load_DPT_Shot_Status_array: not implemented\n");
  return false;
}

int app_sprintf_DPT_Shot_Status(const DPT_Shot_Status *in, char* text, int size)
{
  printf("app_sprintf_DPT_Shot_Status: not implemented\n");
  return 0;
}

int app_sscanf_DPT_Shot_Status(DPT_Shot_Status *in, char* text)
{
  printf("app_sscanf_DPT_Shot_Status: not implemented\n");
  return 0;
}

int app_str_expected_DPT_Shot_Status(int select, char* text)
{
  printf("app_str_expected_DPT_Shot_Status: not implemented\n");
  return 0;
  if (text == NULL) {
    return 1;
  }
 
  
  return 1;
}




// Getters/Setters for DPT_Start

bool app_is_DPT_Start_url(const char* url)
{
  if (strcmp(url, URL_SENDREADY) == 0){
    return true;
  }
  if (strcmp(url, URL_RECEIVEREADY) == 0){
    return true;
  }

  return false;
}

void app_set_DPT_Start_default_value(const char* url)
{
   if (strcmp(url, URL_SENDREADY) == 0){
      memset(&g_stub_SendReady, 0, sizeof(DPT_Start));
   }
   if (strcmp(url, URL_RECEIVEREADY) == 0){
      memset(&g_stub_ReceiveReady, 0, sizeof(DPT_Start));
   }
  
}

void app_set_DPT_Start_default_value_persistent(const char* url)
{
// not really persistent
   if (strcmp(url, URL_SENDREADY) == 0){
      memset(&g_stub_SendReady, 0, sizeof(DPT_Start));
   }
   if (strcmp(url, URL_RECEIVEREADY) == 0){
      memset(&g_stub_ReceiveReady, 0, sizeof(DPT_Start));
   }
}


void app_set_DPT_Start_array_elems(const char* url, const DPT_Start* in, int start, int n, bool store_persistently)
{
  printf("app_set_DPT_Start_array_elems: not implemented\n");
  return;
}

void app_set_DPT_Start_array(const char* url, const DPT_Start* in, int n, bool store_persistently)
{
  printf("app_set_DPT_Start_array : not implemented\n");
}

void app_set_DPT_Start_variable(const char* url, const DPT_Start* in)
{
   if (strcmp(url, URL_SENDREADY) == 0){
      memcpy(&g_stub_SendReady, in, sizeof(g_stub_SendReady));
   }
   if (strcmp(url, URL_RECEIVEREADY) == 0){
      memcpy(&g_stub_ReceiveReady, in, sizeof(g_stub_ReceiveReady));
   }
// parameters exists
  // STARTING_PLAYER 
}

const DPT_Start* app_get_DPT_Start_array_elems(const char *url, DPT_Start* out, int start, int n)
{
  printf("app_get_DPT_Start_array_elems: not implemented\n");
  return NULL;
}

const DPT_Start* app_get_DPT_Start_array(const char *url, DPT_Start* out, int n)
{
  return app_get_DPT_Start_array_elems(url, out, 0, n);
}



DPT_Start g_stub_DPT_Start;
const DPT_Start* app_get_DPT_Start_variable(const char *url, DPT_Start* out)
{
   if (strcmp(url, URL_SENDREADY) == 0){
      memcpy(out, &g_stub_SendReady, sizeof(g_stub_SendReady));
   }
   if (strcmp(url, URL_RECEIVEREADY) == 0){
      memcpy(out, &g_stub_ReceiveReady, sizeof(g_stub_ReceiveReady));
   }
  if (out != NULL) {
    return out;
  } else {
    return &g_stub_DPT_Start;
  }
}

bool oc_parse_DPT_Start_single(oc_rep_t *rep, DPT_Start *out)
{
  printf("oc_parse_DPT_Start_single: not implemented\n");
  return false;
}

bool oc_parse_DPT_Start(oc_rep_t *rep, DPT_Start *out)
{
  printf("oc_parse_DPT_Start: not implemented\n");
  return false;
}

bool oc_parse_DPT_Start_array(oc_rep_t *rep, DPT_Start *out, int n)
{
  printf("oc_parse_DPT_Start_array: not implemented\n");
  return false;
}

void oc_encode_DPT_Start_single(CborEncoder *parent, const DPT_Start *in)
{
  printf("oc_encode_DPT_Start_single: not implemented\n");
}

void oc_encode_DPT_Start(const DPT_Start *in, bool is_metadata)
{
  printf("oc_encode_DPT_Start: not implemented\n");
}

void oc_encode_DPT_Start_array(const DPT_Start *in, int n)
{
  printf("oc_encode_DPT_Start_array: not implemented\n");
}

void persistent_store_DPT_Start(const char *name, const DPT_Start *in)
{
  printf("persistent_store_DPT_Start: not implemented\n");
}

void persistent_store_DPT_Start_array(const char *name, const DPT_Start *in, int n)
{

  printf("persistent_store_DPT_Start_array: not implemented\n");
}

bool persistent_load_DPT_Start(const char *name, DPT_Start *out)
{
  
  printf("persistent_load_DPT_Start: not implemented\n");
  return false;
}

bool persistent_load_DPT_Start_array(const char *name, DPT_Start *out, int n)
{
  printf("persistent_load_DPT_Start_array: not implemented\n");
  return false;
}

int app_sprintf_DPT_Start(const DPT_Start *in, char* text, int size)
{
  printf("app_sprintf_DPT_Start: not implemented\n");
  return 0;
}

int app_sscanf_DPT_Start(DPT_Start *in, char* text)
{
  printf("app_sscanf_DPT_Start: not implemented\n");
  return 0;
}

int app_str_expected_DPT_Start(int select, char* text)
{
  printf("app_str_expected_DPT_Start: not implemented\n");
  return 0;
  if (text == NULL) {
    return 1;
  }
 
  
  return 1;
}




// Getters/Setters for DPT_Uint_XY

bool app_is_DPT_Uint_XY_url(const char* url)
{
  if (strcmp(url, URL_SENDSHOT) == 0){
    return true;
  }
  if (strcmp(url, URL_RECEIVESHOT) == 0){
    return true;
  }

  return false;
}

void app_set_DPT_Uint_XY_default_value(const char* url)
{
   if (strcmp(url, URL_SENDSHOT) == 0){
      memset(&g_stub_SendShot, 0, sizeof(DPT_Uint_XY));
   }
   if (strcmp(url, URL_RECEIVESHOT) == 0){
      memset(&g_stub_ReceiveShot, 0, sizeof(DPT_Uint_XY));
   }
  
}

void app_set_DPT_Uint_XY_default_value_persistent(const char* url)
{
// not really persistent
   if (strcmp(url, URL_SENDSHOT) == 0){
      memset(&g_stub_SendShot, 0, sizeof(DPT_Uint_XY));
   }
   if (strcmp(url, URL_RECEIVESHOT) == 0){
      memset(&g_stub_ReceiveShot, 0, sizeof(DPT_Uint_XY));
   }
}


void app_set_DPT_Uint_XY_array_elems(const char* url, const DPT_Uint_XY* in, int start, int n, bool store_persistently)
{
  printf("app_set_DPT_Uint_XY_array_elems: not implemented\n");
  return;
}

void app_set_DPT_Uint_XY_array(const char* url, const DPT_Uint_XY* in, int n, bool store_persistently)
{
  printf("app_set_DPT_Uint_XY_array : not implemented\n");
}

void app_set_DPT_Uint_XY_variable(const char* url, const DPT_Uint_XY* in)
{
   if (strcmp(url, URL_SENDSHOT) == 0){
      memcpy(&g_stub_SendShot, in, sizeof(g_stub_SendShot));
   }
   if (strcmp(url, URL_RECEIVESHOT) == 0){
      memcpy(&g_stub_ReceiveShot, in, sizeof(g_stub_ReceiveShot));
   }
// parameters exists
  // STARTING_PLAYER 
}

const DPT_Uint_XY* app_get_DPT_Uint_XY_array_elems(const char *url, DPT_Uint_XY* out, int start, int n)
{
  printf("app_get_DPT_Uint_XY_array_elems: not implemented\n");
  return NULL;
}

const DPT_Uint_XY* app_get_DPT_Uint_XY_array(const char *url, DPT_Uint_XY* out, int n)
{
  return app_get_DPT_Uint_XY_array_elems(url, out, 0, n);
}



DPT_Uint_XY g_stub_DPT_Uint_XY;
const DPT_Uint_XY* app_get_DPT_Uint_XY_variable(const char *url, DPT_Uint_XY* out)
{
   if (strcmp(url, URL_SENDSHOT) == 0){
      memcpy(out, &g_stub_SendShot, sizeof(g_stub_SendShot));
   }
   if (strcmp(url, URL_RECEIVESHOT) == 0){
      memcpy(out, &g_stub_ReceiveShot, sizeof(g_stub_ReceiveShot));
   }
  if (out != NULL) {
    return out;
  } else {
    return &g_stub_DPT_Uint_XY;
  }
}

bool oc_parse_DPT_Uint_XY_single(oc_rep_t *rep, DPT_Uint_XY *out)
{
  printf("oc_parse_DPT_Uint_XY_single: not implemented\n");
  return false;
}

bool oc_parse_DPT_Uint_XY(oc_rep_t *rep, DPT_Uint_XY *out)
{
  printf("oc_parse_DPT_Uint_XY: not implemented\n");
  return false;
}

bool oc_parse_DPT_Uint_XY_array(oc_rep_t *rep, DPT_Uint_XY *out, int n)
{
  printf("oc_parse_DPT_Uint_XY_array: not implemented\n");
  return false;
}

void oc_encode_DPT_Uint_XY_single(CborEncoder *parent, const DPT_Uint_XY *in)
{
  printf("oc_encode_DPT_Uint_XY_single: not implemented\n");
}

void oc_encode_DPT_Uint_XY(const DPT_Uint_XY *in, bool is_metadata)
{
  printf("oc_encode_DPT_Uint_XY: not implemented\n");
}

void oc_encode_DPT_Uint_XY_array(const DPT_Uint_XY *in, int n)
{
  printf("oc_encode_DPT_Uint_XY_array: not implemented\n");
}

void persistent_store_DPT_Uint_XY(const char *name, const DPT_Uint_XY *in)
{
  printf("persistent_store_DPT_Uint_XY: not implemented\n");
}

void persistent_store_DPT_Uint_XY_array(const char *name, const DPT_Uint_XY *in, int n)
{

  printf("persistent_store_DPT_Uint_XY_array: not implemented\n");
}

bool persistent_load_DPT_Uint_XY(const char *name, DPT_Uint_XY *out)
{
  
  printf("persistent_load_DPT_Uint_XY: not implemented\n");
  return false;
}

bool persistent_load_DPT_Uint_XY_array(const char *name, DPT_Uint_XY *out, int n)
{
  printf("persistent_load_DPT_Uint_XY_array: not implemented\n");
  return false;
}

int app_sprintf_DPT_Uint_XY(const DPT_Uint_XY *in, char* text, int size)
{
  printf("app_sprintf_DPT_Uint_XY: not implemented\n");
  return 0;
}

int app_sscanf_DPT_Uint_XY(DPT_Uint_XY *in, char* text)
{
  printf("app_sscanf_DPT_Uint_XY: not implemented\n");
  return 0;
}

int app_str_expected_DPT_Uint_XY(int select, char* text)
{
  printf("app_str_expected_DPT_Uint_XY: not implemented\n");
  return 0;
  if (text == NULL) {
    return 1;
  }
 
  
  return 1;
}

//-----------------------------------------------
// SELF TEST
//-----------------------------------------------

void self_stub_test(void)
{
  char buffer[1024];
  
  // openthread
  otLinkModeConfig my_mode = {0};
  otThreadSetLinkMode(0,my_mode);
  
  // sdk
  TASKLET_Cancel(0);
  TASKLET_ScheduleDelta(0,0, NULL);
  TASKLET_Init(0, NULL);
  
  SED_InitPolling(0,0,0);
  SED_DeinitPolling();
  SED_PollSoon();
  
  CO2DEV_PollButtons();
  PlatformSleep(0);
  knx_service_sleep_period(0);
  
  
  // stack
  oc_do_s_mode_with_scope(5,"",NULL);
  oc_do_s_mode_with_scope_and_groupaddress(5,"","w",false,1);
  oc_device_info_t* mydevice = oc_core_get_device_info(0);
  oc_uuid_t my_uuid;
  oc_gen_uuid(&my_uuid);
  oc_uuid_to_str(&my_uuid, buffer, 1024);
  oc_string(NULL);
  oc_new_string(NULL,"",1);
  oc_free_string(NULL);
  
  oc_set_delayed_callback(NULL,NULL,1);
  oc_remove_delayed_callback(NULL,NULL);
  
  oc_core_find_group_object_table_index(1);
  oc_core_find_group_object_table_url_from_index(1);
  oc_core_find_group_object_table_url("");
  oc_core_get_group_object_table_total_size();
  oc_core_get_group_object_table_entry(1);
  oc_print_group_object_table_entry(0);
  
  oc_set_factory_presets_cb(NULL, NULL);
  oc_set_gateway_cb(NULL,NULL);
  
  oc_a_lsm_state(0);
  
  get_interface_string(OC_IF_I);
  get_interface_string(OC_IF_O);
  get_interface_string(OC_IF_G);
  get_interface_string(OC_IF_C);
  get_interface_string(OC_IF_P);
  get_interface_string(OC_IF_D);
  get_interface_string(OC_IF_A);
  get_interface_string(OC_IF_S);
  get_interface_string(OC_IF_LI);
  get_interface_string(OC_IF_B);
  get_interface_string(OC_IF_SEC);
  get_interface_string(OC_IF_SWU);
  get_interface_string(OC_IF_PM);
  get_interface_string(OC_IF_M);
  
  get_module_url((char*)buffer,"xxx_1",1);
  get_instance_num_from_url("");
  get_datapoint_by_url("");
  datapoint_set(NULL, NULL, 0, 0);

  // Getters/Setters for DPT_Param_Bool
  app_is_DPT_Param_Bool_url("");
  app_set_DPT_Param_Bool_default_value("");
  app_set_DPT_Param_Bool_default_value_persistent("");
  
  app_set_DPT_Param_Bool_array_elems("", NULL, 0, 1, true);
  app_set_DPT_Param_Bool_array("", NULL, 0, true);
  app_set_DPT_Param_Bool_variable("", NULL);
  
  app_get_DPT_Param_Bool_array_elems("", NULL, 0, 1);
  app_get_DPT_Param_Bool_array("", NULL, 0);
  app_get_DPT_Param_Bool_variable("", NULL);
  
  oc_parse_DPT_Param_Bool_single(NULL, NULL);
  oc_parse_DPT_Param_Bool(NULL, NULL);
  oc_parse_DPT_Param_Bool_array(NULL, NULL, 0);
  
  oc_encode_DPT_Param_Bool_single(NULL, NULL);
  oc_encode_DPT_Param_Bool(NULL, false);
  oc_encode_DPT_Param_Bool_array(NULL, 0);
  
  persistent_store_DPT_Param_Bool("", NULL);
  persistent_store_DPT_Param_Bool_array("", NULL, 0);
  
  persistent_load_DPT_Param_Bool("", NULL);
  persistent_load_DPT_Param_Bool_array("", NULL, 0);
   
  app_sprintf_DPT_Param_Bool(NULL, "", 0);
  app_sscanf_DPT_Param_Bool(NULL, "");
  app_str_expected_DPT_Param_Bool(0, "");
   


  // Getters/Setters for DPT_Shot_Status
  app_is_DPT_Shot_Status_url("");
  app_set_DPT_Shot_Status_default_value("");
  app_set_DPT_Shot_Status_default_value_persistent("");
  
  app_set_DPT_Shot_Status_array_elems("", NULL, 0, 1, true);
  app_set_DPT_Shot_Status_array("", NULL, 0, true);
  app_set_DPT_Shot_Status_variable("", NULL);
  
  app_get_DPT_Shot_Status_array_elems("", NULL, 0, 1);
  app_get_DPT_Shot_Status_array("", NULL, 0);
  app_get_DPT_Shot_Status_variable("", NULL);
  
  oc_parse_DPT_Shot_Status_single(NULL, NULL);
  oc_parse_DPT_Shot_Status(NULL, NULL);
  oc_parse_DPT_Shot_Status_array(NULL, NULL, 0);
  
  oc_encode_DPT_Shot_Status_single(NULL, NULL);
  oc_encode_DPT_Shot_Status(NULL, false);
  oc_encode_DPT_Shot_Status_array(NULL, 0);
  
  persistent_store_DPT_Shot_Status("", NULL);
  persistent_store_DPT_Shot_Status_array("", NULL, 0);
  
  persistent_load_DPT_Shot_Status("", NULL);
  persistent_load_DPT_Shot_Status_array("", NULL, 0);
   
  app_sprintf_DPT_Shot_Status(NULL, "", 0);
  app_sscanf_DPT_Shot_Status(NULL, "");
  app_str_expected_DPT_Shot_Status(0, "");
   


  // Getters/Setters for DPT_Start
  app_is_DPT_Start_url("");
  app_set_DPT_Start_default_value("");
  app_set_DPT_Start_default_value_persistent("");
  
  app_set_DPT_Start_array_elems("", NULL, 0, 1, true);
  app_set_DPT_Start_array("", NULL, 0, true);
  app_set_DPT_Start_variable("", NULL);
  
  app_get_DPT_Start_array_elems("", NULL, 0, 1);
  app_get_DPT_Start_array("", NULL, 0);
  app_get_DPT_Start_variable("", NULL);
  
  oc_parse_DPT_Start_single(NULL, NULL);
  oc_parse_DPT_Start(NULL, NULL);
  oc_parse_DPT_Start_array(NULL, NULL, 0);
  
  oc_encode_DPT_Start_single(NULL, NULL);
  oc_encode_DPT_Start(NULL, false);
  oc_encode_DPT_Start_array(NULL, 0);
  
  persistent_store_DPT_Start("", NULL);
  persistent_store_DPT_Start_array("", NULL, 0);
  
  persistent_load_DPT_Start("", NULL);
  persistent_load_DPT_Start_array("", NULL, 0);
   
  app_sprintf_DPT_Start(NULL, "", 0);
  app_sscanf_DPT_Start(NULL, "");
  app_str_expected_DPT_Start(0, "");
   


  // Getters/Setters for DPT_Uint_XY
  app_is_DPT_Uint_XY_url("");
  app_set_DPT_Uint_XY_default_value("");
  app_set_DPT_Uint_XY_default_value_persistent("");
  
  app_set_DPT_Uint_XY_array_elems("", NULL, 0, 1, true);
  app_set_DPT_Uint_XY_array("", NULL, 0, true);
  app_set_DPT_Uint_XY_variable("", NULL);
  
  app_get_DPT_Uint_XY_array_elems("", NULL, 0, 1);
  app_get_DPT_Uint_XY_array("", NULL, 0);
  app_get_DPT_Uint_XY_variable("", NULL);
  
  oc_parse_DPT_Uint_XY_single(NULL, NULL);
  oc_parse_DPT_Uint_XY(NULL, NULL);
  oc_parse_DPT_Uint_XY_array(NULL, NULL, 0);
  
  oc_encode_DPT_Uint_XY_single(NULL, NULL);
  oc_encode_DPT_Uint_XY(NULL, false);
  oc_encode_DPT_Uint_XY_array(NULL, 0);
  
  persistent_store_DPT_Uint_XY("", NULL);
  persistent_store_DPT_Uint_XY_array("", NULL, 0);
  
  persistent_load_DPT_Uint_XY("", NULL);
  persistent_load_DPT_Uint_XY_array("", NULL, 0);
   
  app_sprintf_DPT_Uint_XY(NULL, "", 0);
  app_sscanf_DPT_Uint_XY(NULL, "");
  app_str_expected_DPT_Uint_XY(0, "");
   
}




#ifdef __cplusplus
}
#endif
