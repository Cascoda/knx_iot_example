/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2023 Cascoda Ltd
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
 * KNX Switching example
 
 * ## Application Design
 *
 * support functions:
 *
 * - app_init
 *   initializes the stack values.
 * - register_resources
 *   function that registers all endpoints,
 *   e.g. sets the GET/PUT/POST/DELETE
 *      handlers for each end point
 *
 * - main
 *   starts the stack, with the registered resources.
 *   can be compiled out with NO_MAIN
 *
 *  handlers for the implemented methods (get/put):
 *   - get_[path]
 *     function that is being called when a GET is called on [path]
 *     set the global variables in the output
 *   - put_[path]
 *     function that is being called when a PUT is called on [path]
 *     if input data is of the correct type
 *       updates the global variables
 *
 * ## stack specific defines
 * - __linux__
 *   build for Linux
 * - WIN32
 *   build for Windows
 * - OC_OSCORE
 *   oscore is enabled as compile flag
 * ## File specific defines
 * - NO_MAIN
 *   compile out the function main()
 * - INCLUDE_EXTERNAL
 *   includes header file "external_header.h", so that other tools/dependencies
 *   can be included without changing this code
 * - KNX_GUI
 *   build the GUI with console option, so that all
 *   logging can be seen in the command window
 */
#include "oc_api.h"
#include "oc_core_res.h"
#include "oc_rep.h"
#include "oc_helpers.h"
#include "api/oc_knx_fp.h"
#include "port/oc_clock.h"
#include <signal.h>
/* test purpose only; commandline reset */
#include "api/oc_knx_dev.h"
#ifdef OC_SPAKE
#include "security/oc_spake2plus.h"
#endif
#ifdef INCLUDE_EXTERNAL
/* import external definitions from header file*/
/* this file should be externally supplied */
#include "external_header.h"
#endif
#include "knx_iot_example.h"

#include <stdlib.h>
#include <ctype.h>

#ifdef __linux__
/** linux specific code */
#include <pthread.h>
#ifndef NO_MAIN
static pthread_mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
#endif /* NO_MAIN */
#endif

#include <stdio.h> /* defines FILENAME_MAX */
#define STORE_PREFIX "app"

#define MY_NAME "KNX Switching example" /**< The name of the application */
#define APP_MAX_STRING 30

#ifdef WIN32
/** windows specific code */
#include <windows.h>
static CONDITION_VARIABLE cv; /**< event loop variable */
static CRITICAL_SECTION cs;   /**< event loop variable */
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define btoa(x) ((x) ? "true" : "false")
volatile int quit = 0;  /**< stop variable, used by handle_signal */
bool g_reset = false;   /**< reset variable, set by commandline arguments */
char g_serial_number[20] = "00FA10010710";





volatile DPT_Switch g_LED_1;   /**< global variable for LED_1 */
volatile DPT_Switch g_PB_1;   /**< global variable for PB_1 */

volatile bool g_fault_LED_1;   /**< global variable for fault LED_1 */




void
get_generic(oc_request_t *request, oc_interface_mask_t interfaces, void *user_data);
void
put_generic(oc_request_t *request, oc_interface_mask_t interfaces, void *user_data);

typedef enum DatapointType{
  DatapointType_bool,
  DatapointType_int,
  DatapointType_float,
  DatapointType_string,
  DatapointType_DPT_Switch, 
  DatapointType_MAX_NUM,
} DatapointType;

typedef struct datapoint_t {
  oc_resource_t resource;
  const char *const *metadata;
  const char *feedback_url;
  DatapointType type;
  volatile void *g_var;
  volatile void *g_fault;
  bool persistent;
  int num_elements;
} datapoint_t;


const char *const md_LED_1[3] = {
  "desc", "On/Off switch 1", 
  NULL
}; 
const char *const md_PB_1[3] = {
  "desc", "On/Off push button 1", 
  NULL
};  

extern const datapoint_t g_datapoints[];
const size_t num_datapoints = 2; 

 
oc_resource_dummy_t app_resource_end;
oc_resource_data_t runtime_data_LED_1;
oc_resource_data_t runtime_data_PB_1; 

const datapoint_t g_datapoints[2] = {
  /*[0] = */{
    /* .resource=*/ { 
      /*next*/ (oc_resource_t*)&g_datapoints[1].resource,
      /*device*/ 0,
      /*name*/ oc_string_create_const("LED_1"),
      /*uri*/ oc_string_create_const("/p/o_1_1"),
      /*types*/ oc_string_array_create_const(_ECHO, 1, "urn:knx:dpa.417.52" ),
      /*dpt*/ oc_string_create_const("urn:knx:dpt.switch"),
      /*interfaces*/ OC_IF_A,
      /*content_type*/ APPLICATION_CBOR,
      /*properties*/ OC_DISCOVERABLE,
      /*get_handler*/ {get_generic, (void*)&g_datapoints[0]},
      /*put_handler*/ {put_generic, (void*)&g_datapoints[0]},
      /*post_handler*/ {NULL, NULL},
      /*delete_handler*/ {NULL, NULL},
      /*get_properties*/ { NULL, NULL },
      /*set_properties*/ { NULL, NULL },
      /*observe_period_seconds*/ 0,
      /*fb_instance*/ 1,
      /*is_const*/ true,
      /*runtime_data*/ &runtime_data_LED_1
    },
    /*.metadata =*/ md_LED_1,
    /*.feedback_url =*/NULL, 
    /*.type =*/ DatapointType_DPT_Switch,
    /*.g_var =*/ (void*)&g_LED_1,
    /*.g_fault =*/ &g_fault_LED_1,
    /*.persistent =*/ false,
    /*.num_elements =*/ 0
  },
  /*[1] = */{
    /* .resource=*/ { 
      /*next*/(oc_resource_t*)&app_resource_end,
      /*device*/ 0,
      /*name*/ oc_string_create_const("PB_1"),
      /*uri*/ oc_string_create_const("/p/o_2_2"),
      /*types*/ oc_string_array_create_const(_ECHO, 1, "urn:knx:dpa.421.61" ),
      /*dpt*/ oc_string_create_const("urn:knx:dpt.switch"),
      /*interfaces*/ OC_IF_S,
      /*content_type*/ APPLICATION_CBOR,
      /*properties*/ OC_DISCOVERABLE,
      /*get_handler*/ {get_generic, (void*)&g_datapoints[1]},
      /*put_handler*/ {put_generic, (void*)&g_datapoints[1]},
      /*post_handler*/ {NULL, NULL},
      /*delete_handler*/ {NULL, NULL},
      /*get_properties*/ { NULL, NULL },
      /*set_properties*/ { NULL, NULL },
      /*observe_period_seconds*/ 0,
      /*fb_instance*/ 1,
      /*is_const*/ true,
      /*runtime_data*/ &runtime_data_PB_1
    },
    /*.metadata =*/ md_PB_1,
    /*.feedback_url =*/NULL, 
    /*.type =*/ DatapointType_DPT_Switch,
    /*.g_var =*/ (void*)&g_PB_1,
    /*.g_fault =*/ NULL,
    /*.persistent =*/ false,
    /*.num_elements =*/ 0
  }, 
}; 
 

typedef const volatile void* (*app_get_variable_fn)(const char*, void*);
typedef const volatile void* (*app_get_array_fn)(const char*, void*, int);
typedef const volatile void* (*app_get_array_elems_fn)(const char*, void*, int start, int n);
typedef void (*app_set_variable_fn)(const char*, const void*);
typedef void (*app_set_array_fn)(const char*, const void*, int n);
typedef void (*app_set_array_elems_fn)(const char*, const void*, int start, int n);
typedef void (*oc_encode_fn)(const void*);
typedef void (*oc_encode_array_fn)(const void*, int n);
typedef void (*oc_encode_array_fn)(const void*, int n);
typedef bool (*oc_parse_fn)(oc_rep_t *rep, void *out);
typedef bool (*oc_parse_array_fn)(oc_rep_t *rep, void *out, int n);
typedef bool (*persistent_load_fn)(const char *filename, void *out);
typedef bool (*persistent_load_array_fn)(const char *filename, void *out, int n);

struct datapoint_type_t {
  size_t size;
  app_get_variable_fn app_get_variable;
  app_get_array_fn app_get_array;
  app_get_array_elems_fn app_get_array_elems;
  app_set_variable_fn app_set_variable;
  app_set_array_fn app_set_array;
  app_set_array_elems_fn app_set_array_elems;
  oc_encode_fn oc_encode;
  oc_encode_array_fn oc_encode_array;
  oc_parse_fn oc_parse;
  oc_parse_array_fn oc_parse_array;
  persistent_load_fn persistent_load;
  persistent_load_array_fn persistent_load_array;
};

const struct datapoint_type_t g_datapoint_types[DatapointType_MAX_NUM] = {
  {sizeof(bool)},
  {sizeof(int)},
  {sizeof(float)},
  {sizeof(char*)},
  {
    sizeof(DPT_Switch),
    (app_get_variable_fn)app_get_DPT_Switch_variable,
    (app_get_array_fn)app_get_DPT_Switch_array,
    (app_get_array_elems_fn)app_get_DPT_Switch_array_elems,
    (app_set_variable_fn)app_set_DPT_Switch_variable,
    (app_set_array_fn)app_set_DPT_Switch_array,
    (app_set_array_elems_fn)app_set_DPT_Switch_array_elems,
    (oc_encode_fn)oc_encode_DPT_Switch,
    (oc_encode_array_fn)oc_encode_DPT_Switch_array,
    (oc_parse_fn)oc_parse_DPT_Switch,
    (oc_parse_array_fn)oc_parse_DPT_Switch_array,
    (persistent_load_fn)persistent_load_DPT_Switch,
    (persistent_load_array_fn)persistent_load_DPT_Switch_array,
  }, 
};

size_t get_dpt_size(DatapointType t) {
  return g_datapoint_types[t].size;
}

static const char *get_datapoint_name(const datapoint_t *dp) {
  return oc_string(dp->resource.name);
}

static const char *get_datapoint_url(const datapoint_t *dp) {
  return oc_string(dp->resource.uri);
}

static const char *get_datapoint_dpt(const datapoint_t *dp) {
  return oc_string(dp->resource.dpt);
}

static const char *get_datapoint_dpa(const datapoint_t *dp) {
  for (int i = 0; i < oc_string_array_size(dp->resource.types); i++) {
    const char *rt = oc_string_array(dp->resource.types)[i];
    if (strstr("dpa.", rt) != NULL)
      return rt;
  }
  return NULL;
}

static const char *const *get_datapoint_metadata(const datapoint_t *dp) {
  return dp->metadata;
}

static bool oc_encode_datapoint(const datapoint_t *dp, int pn, int ps) {
  size_t count = dp->num_elements;
  const struct datapoint_type_t *dpt = &g_datapoint_types[dp->type];
  int n = ps;
  count = count?count:1;
  if (pn*ps+n >= count)
    n = count-pn*ps;
  if (ps > 1 && dpt->oc_encode_array == NULL)
    return false;
  if (ps == 1 && dpt->oc_encode == NULL)
    return false;
  if (dpt->app_get_array_elems == NULL)
    return false;
  void *var = malloc(get_dpt_size(dp->type) * (ps));
  if(g_datapoint_types[dp->type].app_get_array_elems(get_datapoint_url(dp), var, pn*ps, n) == NULL) {
    free(var);
    return false;
  }
  
  if (ps > 1) 
    g_datapoint_types[dp->type].oc_encode_array(var, n);
  else 
    g_datapoint_types[dp->type].oc_encode(var);
  
  free(var);
  return true;
}

static bool oc_parse_datapoint(const datapoint_t *dp, oc_rep_t *rep, void *out, int n) {
  const struct datapoint_type_t *dpt = &g_datapoint_types[dp->type];
  if (out == NULL || rep == NULL)
    return false;
  if (n > 1){
    if (dpt->oc_parse_array == NULL)
      return false;
    return dpt->oc_parse_array(rep, out, n);
  }
  else{
    if (dpt->oc_parse == NULL)
      return false;
    return dpt->oc_parse(rep, out);
  } 
}

static void datapoint_set(const datapoint_t *dp, void *in, int start, int n) 
{
  if (in == NULL)
    return;
  if (start > 0 || n > 1){
    if (g_datapoint_types[dp->type].app_set_array)
      g_datapoint_types[dp->type].app_set_array_elems(get_datapoint_url(dp), in, start, n);
  }
  else
    if (g_datapoint_types[dp->type].app_set_variable)
      g_datapoint_types[dp->type].app_set_variable(get_datapoint_url(dp), in);
}

static const datapoint_t *get_datapoint_by_url(const char *url) {
  //this can likely be optimised in the future for speed
  //with a binary search or similar
for(int i = 0; i < num_datapoints; i++) {
    const datapoint_t *it = &g_datapoints[i];
    const char *_url = get_datapoint_url(it);
    if (strcmp(_url, url) == 0) {
      return it;
    }
  } 
 
  return NULL;
}

int app_get_url_array_size(const char *url)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL)
    return -1;
  return dp->num_elements;
}

// Getters/Setters for DPT_Switch

bool app_is_DPT_Switch_url(const char* url)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return false;
  }
  
  
  if (dp->type == DatapointType_DPT_Switch) {
    return true;
  }
  return false;
}

void app_set_DPT_Switch_array_elems(const char* url, const DPT_Switch* in, int start, int n)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return;
  }
  int max_elem = dp->num_elements;
  if(max_elem == 0) {
    max_elem = 1;
  }
  if (start >= max_elem) {
    return;
  }
  if (start+n > max_elem) {
    n = max_elem-start;
  }
  if (dp->g_var) {
    memcpy(&((DPT_Switch*)dp->g_var)[start], in, sizeof(DPT_Switch)*n);
    if (dp->persistent) {
      persistent_store_DPT_Switch_array(get_datapoint_url(dp), (DPT_Switch*)dp->g_var, max_elem);
    }
  }else if (dp->persistent) {
    int count = dp->num_elements;
    count = count?count:1;
    DPT_Switch *data = calloc(sizeof(DPT_Switch), count);
    memcpy(&data[start], in, n*sizeof(DPT_Switch));
    persistent_store_DPT_Switch_array(get_datapoint_url(dp), data, count);
    free(data);
  }
  return;
}

void app_set_DPT_Switch_array(const char* url, const DPT_Switch* in, int n)
{
  app_set_DPT_Switch_array_elems(url, in, 0, n);
}

void app_set_DPT_Switch_variable(const char* url, const DPT_Switch* in)
{
  app_set_DPT_Switch_array(url, in, 1);
}

const volatile DPT_Switch* app_get_DPT_Switch_array_elems(const char *url, DPT_Switch* out, int start, int n)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return NULL;
  }
  int max_elem = dp->num_elements;
  if(max_elem == 0) {
    max_elem = 1;
  }
  if (start >= max_elem) {
    return NULL;
  }
  if (start+n > max_elem){
    n = max_elem - start;
  }
  if (dp->g_var) {
    if (out == NULL) {
      return dp->g_var;
    }
    memcpy(out, &((DPT_Switch*)dp->g_var)[start], sizeof(DPT_Switch)*n);
    return out;
  }else if (dp->persistent && out) {
    int count = dp->num_elements;
    count = count?count:1;
    DPT_Switch *data = calloc(sizeof(DPT_Switch), count);
    memset(out, 0, n*sizeof(DPT_Switch));
    if (persistent_load_DPT_Switch_array(get_datapoint_url(dp), data, count)) {
      memcpy(out, &data[start], n*sizeof(DPT_Switch));
    }
    free(data);
    return out;
  }
  return NULL;
}

const volatile DPT_Switch* app_get_DPT_Switch_array(const char *url, DPT_Switch* out, int n)
{
  return app_get_DPT_Switch_array_elems(url, out, 0, n);
}

const volatile DPT_Switch* app_get_DPT_Switch_variable(const char *url, DPT_Switch* out)
{
  return app_get_DPT_Switch_array(url, out, 1);
}

bool oc_parse_DPT_Switch_single(oc_rep_t *rep, DPT_Switch *out)
{
  oc_array_t arr;
  // this really shouldn't be void*
  // we need to find a better way.
  if (rep->type != OC_REP_BOOL) return false;
  return oc_rep_i_get_bool(rep, 1, (void*)out);
}

bool oc_parse_DPT_Switch(oc_rep_t *rep, DPT_Switch *out)
{
  if (rep == NULL || out == NULL) {
    return false;
  }
  while (rep != NULL && rep->iname != 1) {
    rep = rep->next;
  }
  if (rep == NULL) {
    return false;
  }
  return oc_parse_DPT_Switch_single(rep, out);
}

bool oc_parse_DPT_Switch_array(oc_rep_t *rep, DPT_Switch *out, int n)
{
  oc_rep_t *rep_arr;
  int i = 0;
  if (rep == NULL || out == NULL) {
    return false;
  }
  while (rep != NULL && i < n){
    if (rep->type != OC_REP_OBJECT) {
      return false;
    }
    oc_parse_DPT_Switch(rep->value.object, &out[i]);
    rep = rep->next;
    i++;
  }
  return true;
}

void oc_encode_DPT_Switch_single(CborEncoder *parent, const DPT_Switch *in)
{
  if (in == NULL) {
    return;
  }
  oc_rep_i_set_key(parent, 1);
  cbor_encode_boolean(parent, *in);
}

void oc_encode_DPT_Switch(const DPT_Switch *in)
{
  if (in == NULL) {
    return;
  }
  oc_rep_begin_root_object();
  oc_encode_DPT_Switch_single(oc_rep_object(root), in);
  oc_rep_end_root_object();
}

void oc_encode_DPT_Switch_array(const DPT_Switch *in, int n)
{
  if (in == NULL) {
    return;
  }
  oc_rep_begin_links_array();
  for (int i = 0; i < n; i++) {
    oc_rep_begin_object(oc_rep_array(links), obj);
    oc_encode_DPT_Switch_single(oc_rep_object(obj), &in[i]);
    oc_rep_end_object(oc_rep_array(links), obj);
  }
  oc_rep_end_links_array();
}

void persistent_store_DPT_Switch(const char *name, const DPT_Switch *in)
{
  persistent_store_DPT_Switch_array(name, in, 1);
}

void persistent_store_DPT_Switch_array(const char *name, const DPT_Switch *in, int n)
{
  uint8_t *rep_buf;
  const size_t max_size = 4 * n + 2;
  rep_buf = malloc(max_size);
  char store_name[32] = STORE_PREFIX;
  strcat(store_name, name);
  char *pos = strchr(store_name, '/');
  while(pos) {
    *pos = '_';
    pos = strchr(store_name, '/');
  }

  oc_rep_new(rep_buf, max_size);
  oc_encode_DPT_Switch_array(in, n);
  int size = oc_rep_get_encoded_payload_size();
  if (size > 0) {
    PRINT_APP("storing '%s', size: %d\n", name, size);
    for(int i = 0; i < size; i++) {
      PRINT_APP("%02X ", rep_buf[i]);
    }
    PRINT_APP("\n");
    oc_storage_write(store_name, rep_buf, size);
  }else{
    PRINT_APP("Error encoding DPT_Switch %s for storage\n", name);
  }
  free(rep_buf);
}

bool persistent_load_DPT_Switch(const char *name, DPT_Switch *out)
{
  return persistent_load_DPT_Switch_array(name, out, 1);
}

bool persistent_load_DPT_Switch_array(const char *name, DPT_Switch *out, int n)
{
  oc_rep_t *rep = NULL;
  int max_size = 4 * n + 2;
  uint8_t *oc_storage_buf;
  char store_name[32] = STORE_PREFIX;
  strcat(store_name, name);
  char *pos = strchr(store_name, '/'); 
  while(pos) {
    *pos = '_';
    pos = strchr(store_name, '/');
  }
  long ret;
  bool error = true;
  struct oc_memb rep_objects = { sizeof(oc_rep_t), 0, 0, 0, 0 };
  oc_storage_buf = malloc(max_size);
  oc_rep_set_pool(&rep_objects);

  ret = oc_storage_read(store_name, oc_storage_buf, max_size);
  PRINT_APP("oc_storage_read() returned %d\n", ret);
  for(int i = 0; i < ret; i++) {
    PRINT_APP("%02X ", oc_storage_buf[i]);
  }
  PRINT_APP("\n");
  if (ret <= 0) {
    goto err;
  }
  if (oc_parse_rep(oc_storage_buf, ret, &rep) != CborNoError) {
    goto err;
  }
  if (oc_parse_DPT_Switch_array(rep, out, n) != true) {
    goto err;
  }
  error = false;
err:
  free(oc_storage_buf);
  oc_free_rep(rep);
  if(error) {
    oc_storage_erase(name);
  }
  return !error;
}
// BOOLEAN code

/**
 * @brief function to check if the url is represented by a boolean
 *
 * @param true = url value is a boolean
 * @param false = url is not a boolean
 */
bool app_is_bool_url(const char* url)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return false;
  }
  return dp->type == DatapointType_bool;
}

/**
 * @brief sets the global boolean variable at the url
 *
 * Note: if the url is ot found then the value is not changed
 *
 * @param url the url indicating the global variable
 * @param value the value to be set
 */
void app_set_bool_variable(const char* url, bool value)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return;
  }
  if (dp->type != DatapointType_bool) {
    return;
  }
  if (dp->g_var) {
    *((bool*)dp->g_var) = value;
  }
}

/**
 * @brief retrieve the global boolean variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
bool app_retrieve_bool_variable(const char* url)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return false;
  }
  if (dp->type != DatapointType_bool) {
    return false;
  }
  if (dp->g_var) {
    return *((bool*)dp->g_var);
  }
  return false;
}

// FAULT code

/**
 * @brief set the fault (boolean) variable at the url
 *
 * @param url the url indicating the fault variable
 * @param value the value of the fault variable
 */
void app_set_fault_variable(const char* url, bool value)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return;
  }
  if (dp->resource.interfaces & OC_IF_A == 0) {
    return;
  }
  if (dp->g_fault) {
    *((bool*)dp->g_fault) = value;
  }
  //TODO: Add back support for changing the feedback variable
}

/**
 * @brief retrieve the fault (boolean) variable at the url
 *
 * @param url the url indicating the fault variable
 * @return the value of the fault variable
 */
bool app_retrieve_fault_variable(const char* url)
{
  const datapoint_t *dp = get_datapoint_by_url(url);
  if (dp == NULL) {
    return false;
  }
  if (dp->resource.interfaces & OC_IF_A == 0) {
    return false;
  }
  if (dp->g_fault) {
    return *((bool*)dp->g_fault);
  }
  return false;
}

// PARAMETER code

bool app_is_url_parameter(const char* url)
{
   
  return false;
}

const char* app_get_parameter_url(int index)
{
 
  return NULL;
}

const char* app_get_parameter_name(int index)
{
   
  return NULL;
}

bool app_is_secure()
{
#ifdef OC_OSCORE
  return true;
#else
  return false;
#endif /* OC_OSCORE */
}

static oc_put_struct_t app_put = { NULL };

void
app_set_put_cb(oc_put_cb_t cb)
{
  app_put.cb = cb;
}

oc_put_struct_t *
oc_get_put_cb(void)
{
  return &app_put;
}

void do_put_cb(const char* url)
{
  oc_put_struct_t *my_cb = oc_get_put_cb();
  if (my_cb && my_cb->cb) {
    my_cb->cb(url);
  }
}

static oc_get_struct_t app_get = { NULL };

void
app_set_get_cb(oc_get_cb_t cb)
{
  app_get.cb = cb;
}

oc_get_struct_t *
oc_get_get_cb(void)
{
  return &app_get;
}

void do_get_cb(const char* url)
{
  oc_get_struct_t *my_cb = oc_get_get_cb();
  if (my_cb && my_cb->cb) {
    my_cb->cb(url);
  }
}

#ifdef __cplusplus
extern "C" {
#endif
int app_initialize_stack();
void signal_event_loop(void);
void register_resources(void);
void initialize_variables();
void logic_initialize();
int app_init(void);
#ifdef __cplusplus
}
#endif

// DEVBOARD code

/**
 * @brief devboard button toggle callback
 *
 */
// we no longer have app_set_bool_variable
//void dev_btn_toggle_cb(char *url)
//{
//  PRINT_APP("Handling %s\n", url);
//  bool val = app_retrieve_bool_variable(url);
//  if (val == true)
//  {
//    val = false;
//  }
//  else
//  {
//    val = true;
//  }
//  app_set_bool_variable(url, val);
//  oc_do_s_mode_with_scope(5, url, "w");
//}

/**
 * @brief s-mode response callback
 * will be called when a response is received on an s-mode read request
 *
 * @param url the url
 * @param rep the full response
 * @param rep_value the parsed value of the response
 */
void
oc_add_s_mode_response_cb(char *url, oc_rep_t *rep, oc_rep_t *rep_value)
{
  (void)rep;
  (void)rep_value;

  PRINT("oc_add_s_mode_response_cb %s\n", url);
}



/**
 * @brief function to set the input string to upper case
 *
 * @param str the string to make upper case
 *
 */
void app_str_to_upper(char *str){
    while (*str != '\0')
    {
        *str = toupper(*str);
        str++;
    }
}

/**
 * @brief function to set up the device.
 *
 * sets the:
 * - manufacturer     : cascoda
 * - serial number    : 00FA10010710
 * - base path
 * - knx spec version
 * - hardware version : [0, 4, 0]
 * - firmware version : [0, 4, 0]
 * - hardware type    : devbrd
 * - device model     : dev board example
 *
 */
int
app_init(void)
{
  int ret = oc_init_platform("cascoda", NULL, NULL);
  char serial_number_uppercase[20];

  /* set the application name, version, base url, device serial number */
  ret |= oc_add_device(MY_NAME, "1.0.0", "//", g_serial_number, NULL, NULL);

  oc_device_info_t *device = oc_core_get_device_info(0);

  
  /* set the hardware version 0.4.0 */
  oc_core_set_device_hwv(0, 0, 4, 0);
  
  
  /* set the firmware version 0.4.0 */
  oc_core_set_device_fwv(0, 0, 4, 0);
  

  /* set the hardware type*/
  oc_core_set_device_hwt(0, "006576627264");

  /* set the model */
  oc_core_set_device_model(0, "dev board example");

  oc_set_s_mode_response_cb(oc_add_s_mode_response_cb);
#define PASSWORD "4N6AFK6T83YWDUTW23U2"
#ifdef OC_SPAKE
  if (strlen(oc_spake_get_password()) == 0)
    oc_spake_set_password(PASSWORD);


  strncpy(serial_number_uppercase, oc_string(device->serialnumber), 19);
  app_str_to_upper(serial_number_uppercase);
  printf("\n === QR Code: KNX:S:%s;P:%s ===\n", serial_number_uppercase, oc_spake_get_password());
#endif

  initialize_variables();  

  return ret;
}

/**
 * @brief returns the password
 */
const char* app_get_password()
{
#ifdef OC_SPAKE
  return oc_spake_get_password();
#else
  return PASSWORD;
#endif
}


bool request_query_get_int(oc_request_t *request, const char *query, int *out)
{
  char *query_str;
  
  int query_len = oc_get_query_value(request, query, &query_str);
  if(query_len <= 0)
    return false;
  char buf[16];
  strncpy(buf, query_str, query_len);
  *out = atoi(buf);
  return true;
}

void
get_generic(oc_request_t *request, oc_interface_mask_t interfaces, void *user_data)
{
  
  const datapoint_t *dp = user_data; 

  bool error_state = false; /* the error state, the generated code */
  PRINT("-- Begin get_generic  (%s) \n", get_datapoint_name(dp));

  if (dp == NULL) {
    PRINT("Error dp is NULL\n");
    oc_send_response(request, OC_STATUS_INTERNAL_SERVER_ERROR);
    return;
  }
  /* MANUFACTORER: SENSOR add here the code to talk to the HW if one implements a
     sensor. the call to the HW needs to fill in the global variable before it
     returns to this function here. alternative is to have a callback from the
     hardware that sets the global variables.
  */

  /* check if the accept header is CBOR */
  if (oc_check_accept_header(request, APPLICATION_CBOR) == false) {
    oc_send_response(request, OC_STATUS_BAD_OPTION);
    return;
  }

  // check the query parameter m with the various values
  char *m;
  char* m_key;
  size_t m_key_len;
  size_t m_len = (int)oc_get_query_value(request, "m", &m);
  int pn, ps;
  if(!request_query_get_int(request, "pn", &pn) || dp->num_elements == 0) pn = 0;
  if(!request_query_get_int(request, "ps", &ps) || dp->num_elements == 0) ps = 1;
  if (m_len != -1) {
    PRINT("  Query param: %.*s",(int)m_len, m);
    oc_init_query_iterator();
    size_t device_index = request->resource->device;
    oc_device_info_t *device = oc_core_get_device_info(device_index);
    if (device != NULL) {
      oc_rep_begin_root_object();
      while (oc_iterate_query(request, &m_key, &m_key_len, &m, &m_len) != -1) {
        // unique identifier
        if ((strncmp(m, "id", m_len) == 0) |
            (strncmp(m, "*", m_len) == 0) ) {
          char mystring[100];
          snprintf(mystring,99,"urn:knx:sn:%s%s",oc_string(device->serialnumber),
           oc_string(request->resource->uri));
          oc_rep_i_set_text_string(root, 9, mystring);
        }
        // resource types
        if ((strncmp(m, "rt", m_len) == 0) |
            (strncmp(m, "*", m_len) == 0) ) {
          const char *dpa = get_datapoint_dpa(dp);
          if (dpa)
            oc_rep_set_text_string(root, rt, dpa);
        }
        // interfaces
        if ((strncmp(m, "if", m_len) == 0) |
            (strncmp(m, "*", m_len) == 0) ) {
          oc_rep_set_key(oc_rep_object(root), "if");
          oc_rep_begin_array(oc_rep_object(root), if);
          for (int i = 1; i <= (1<<OC_MAX_IF_MASKS); i <<=1)
            if (dp->resource.interfaces&i)
              oc_rep_add_text_string(if, get_interface_string(dp->resource.interfaces&i));
          oc_rep_end_array(oc_rep_object(root), if);
        }
        if ((strncmp(m, "dpt", m_len) == 0) |
            (strncmp(m, "*", m_len) == 0) ) {
          oc_rep_set_text_string(root, dpt, oc_string(request->resource->dpt));
        }
        // ga
        if ((strncmp(m, "ga", m_len) == 0) |
            (strncmp(m, "*", m_len) == 0) ) {
          int index = oc_core_find_group_object_table_url(oc_string(request->resource->uri));
          if (index > -1) {
             oc_group_object_table_t* got_table_entry = oc_core_get_group_object_table_entry(index);
             if (got_table_entry) {
               oc_rep_set_int_array(root, ga, got_table_entry->ga, got_table_entry->ga_len);
             }
          }
        }
        for (const char *const *md = get_datapoint_metadata(dp); md && *md; md+=2) {
          if((strncmp(m, md[0], m_len) == 0) |
              (strncmp(m, "*", m_len) == 0) ) {
                oc_rep_set_text_string_no_tag(root, md[0]);
                oc_rep_set_text_string_no_tag(root, md[1]);
              }
        }
      } /* query iterator */
      oc_rep_end_root_object();
    } else {
      /* device is NULL */
      oc_send_cbor_response(request, OC_STATUS_BAD_OPTION);
    }
    oc_send_cbor_response(request, OC_STATUS_OK);
    return;
  }
  if (oc_encode_datapoint(dp, pn, ps) == false) {
    oc_send_response(request, OC_STATUS_INTERNAL_SERVER_ERROR);
    goto done;
  }
  if (g_err) {
    error_state = true;
  }
  PRINT("CBOR encoder size %d\n", oc_rep_get_encoded_payload_size());
  if (error_state == false) {
    oc_send_cbor_response(request, OC_STATUS_OK);
  } else {
    oc_send_response(request, OC_STATUS_BAD_OPTION);
  }
done:
  PRINT("-- End get_generic (%s)\n", get_datapoint_url(dp));
}


void
put_generic(oc_request_t *request, oc_interface_mask_t interfaces,
                void *user_data)
{
  (void)interfaces;
  const datapoint_t *dp = user_data;
  bool error_state = true;
  
  PRINT("-- Begin put_generic (%s):\n", get_datapoint_url(dp));

  if (dp == NULL) {
    PRINT("Error dp is NULL\n");
    oc_send_response(request, OC_STATUS_INTERNAL_SERVER_ERROR);
    return;
  }

  oc_rep_t *rep = NULL;
  int pn, ps;
  if(!request_query_get_int(request, "pn", &pn) || dp->num_elements == 0) pn = 0;
  if(!request_query_get_int(request, "ps", &ps) || dp->num_elements == 0) ps = 1;
  /* handle the different requests e.g. via s-mode or normal CoAP call*/
  if (oc_is_redirected_request(request)) {
    PRINT("  redirected request..\n");
  }
  rep = request->request_payload;
  /* loop over all the entries in the request */
  /* handle the type of payload correctly. */
  void *new_value = malloc(get_dpt_size(dp->type) * ps);
  error_state = !oc_parse_datapoint(dp, rep, new_value, ps);

  if (error_state == false){
      oc_send_cbor_response(request, OC_STATUS_CHANGED);
      datapoint_set(dp, new_value, pn*ps, ps);
      if (dp->feedback_url) {
        //check types match
        const datapoint_t *feedback = get_datapoint_by_url(dp->feedback_url);
        if (feedback->type == dp->type && feedback->num_elements == dp->num_elements) {
          datapoint_set(feedback, new_value, pn*ps, ps);
          PRINT("  Send status to '%s' with flag: 'w'\n", get_datapoint_url(feedback));
          oc_do_s_mode_with_scope(5, dp->feedback_url, "w");
        }
      }

      do_put_cb(get_datapoint_url(dp));
  } else {
    /* request data was not recognized, so it was a bad request */
    oc_send_response(request, OC_STATUS_BAD_REQUEST);
  }
  free(new_value);
  PRINT("-- End put_generic (%s)\n", get_datapoint_url(dp));
}


/**
 * @brief register all the data point resources to the stack
 * this function registers all data point level resources:
 * - each resource path is bind to a specific function for the supported methods
 *  (GET, PUT)
 * - each resource is
 *   - secure
 *   - observable
 *   - discoverable through well-known/core
 *   - used interfaces as: dpa.xxx.yyy
 *      - xxx : function block number
 *      - yyy : data point function number
 */
void
register_resources(void)
{
  const datapoint_t *it;
  const char *_t[2] = {
    "Datapoint",
    "Parameter"
  };
  const char **t = _t;
  PRINT_APP("Adding const resource block\n");
  const datapoint_t *dp = &g_datapoints[0];

  oc_ri_add_resource_block(&g_datapoints[0].resource); 
}

/**
 * @brief initiate preset for device
 * current implementation: device reset as command line argument
 * @param device_index the device identifier of the list of devices
 * @param data the supplied data.
 */
void
factory_presets_cb(size_t device_index, void *data)
{
  (void)device_index;
  (void)data;

  if (g_reset) {
    PRINT("factory_presets_cb: resetting device\n");
    oc_knx_device_storage_reset(device_index, 2);
  }
}

/**
 * @brief set the host name on the device (application depended)
 *
 * @param device_index the device identifier of the list of devices
 * @param host_name the host name to be set on the device
 * @param data the supplied data.
 */
void
hostname_cb(size_t device_index, oc_string_t host_name, void *data)
{
  (void)device_index;
  (void)data;

  PRINT("-----host name ------- %s\n", oc_string(host_name));
}

static oc_event_callback_retval_t send_delayed_response(void *context)
{
  oc_separate_response_t *response = (oc_separate_response_t *)context;

  if (response->active)
  {
    oc_set_separate_response_buffer(response);
    oc_send_separate_response(response, OC_STATUS_CHANGED);
    PRINT_APP("Delayed response sent\n");
  }
  else
  {
    PRINT_APP("Delayed response NOT active\n");
  }

  return OC_EVENT_DONE;
}

#if defined WIN32 || defined __linux__
/**
 * @brief software update callback
 *
 * @param device the device index
 * @param response the instance of an internal struct that is used to track
 *       		   the state of the separate response
 * @param binary_size the full size of the binary
 * @param offset the offset of the image
 * @param payload the image data
 * @param len the length of the image data
 * @param data the user data
 */
void swu_cb(size_t device,
            oc_separate_response_t *response,
            size_t binary_size,
            size_t offset,
            uint8_t *payload,
            size_t len,
            void *data)
{
  (void)device;
  (void)binary_size;
  char filename[] = "./downloaded.bin";
  PRINT(" swu_cb %s block=%d size=%d \n", filename, (int)offset, (int)len);

  FILE *write_ptr = fopen("downloaded_bin", "ab");
  size_t r = fwrite(payload, sizeof(*payload), len, write_ptr);
  fclose(write_ptr);

  oc_set_delayed_callback(response, &send_delayed_response, 0);
}
#endif

/**
 * @brief initializes the global variables
 * for the resources
 * for the parameters
 */
void
initialize_variables(void)
{
  /* initialize global variables for resources */
  /* if wanted read them from persistent storage */
  /* parameter variables */
  uint8_t oc_storage_buf[32];
  long ret;
  bool err;

  PRINT_APP("Initializing persitent data\n");
for(int i = 0; i < num_datapoints; i++) {
    const datapoint_t *it = &g_datapoints[i];
    if (!it->persistent) continue;
    if (it->g_var == NULL) continue;
    if (it->num_elements){
      if (g_datapoint_types[it->type].persistent_load_array == NULL){
        PRINT_APP("ERR: persistent load array missing for %d\n", it->type);
      } else {
        g_datapoint_types[it->type].persistent_load_array(get_datapoint_url(it), (void*)it->g_var, it->num_elements);
      }
    } else {
      if (g_datapoint_types[it->type].persistent_load == NULL){
        PRINT_APP("ERR: persistent load array missing for %d\n", it->type);
      } else {
        g_datapoint_types[it->type].persistent_load(get_datapoint_url(it), (void*)it->g_var);
      }
    }
  } 
 
}

int app_set_serial_number(const char* serial_number)
{
  strncpy(g_serial_number, serial_number, 20);
  return 0;
}

int app_initialize_stack()
{
  int init;
  char *fname = "my_software_image";

  PRINT("KNX-IOT Server name : \"%s\"\n", MY_NAME);

  /* show the current working folder */
  char buff[FILENAME_MAX];
  char *retbuf = NULL;
  retbuf = GetCurrentDir(buff, FILENAME_MAX);
  if (retbuf != NULL) {
    PRINT("Current working dir: %s\n", buff);
  }

  /*
   The storage folder depends on the build system
   the folder is created in the makefile, with $target as name with _cred as
   post fix.
  */
#ifdef WIN32
  char storage[400];
  sprintf(storage,"./knx_iot_example_%s",g_serial_number);
  PRINT("\tstorage at '%s' \n",storage);
  oc_storage_config(storage);
#else
  PRINT("\tstorage at 'knx_iot_example_creds' \n");
  oc_storage_config("./knx_iot_example_creds");
#endif


  /* initializes the handlers structure */
  static oc_handler_t handler = { .init = app_init,
                                  .signal_event_loop = signal_event_loop,
                                  .register_resources = register_resources,
                                  .requests_entry = NULL };

  /* set the application callbacks */
  oc_set_hostname_cb(hostname_cb, NULL);
  oc_set_factory_presets_cb(factory_presets_cb, NULL);

#if defined WIN32 || defined __linux__
  oc_set_swu_cb(swu_cb, (void *)fname);
#endif

  /* start the stack */
  init = oc_main_init(&handler);

  if (init < 0) {
    PRINT("oc_main_init failed %d, exiting.\n", init);
    return init;
  }

#ifdef OC_OSCORE
  PRINT("OSCORE - Enabled\n");
#else
  PRINT("OSCORE - Disabled\n");
#endif /* OC_OSCORE */

  oc_device_info_t *device = oc_core_get_device_info(0);
  PRINT("serial number: %s\n", oc_string(device->serialnumber));
  oc_endpoint_t *my_ep = oc_connectivity_get_endpoints(0);
  if (my_ep != NULL) {
    PRINTipaddr(*my_ep);
    PRINT("\n");
  }
  PRINT("Server \"%s\" running, waiting on incoming "
        "connections.\n",
        MY_NAME);
  return 0;
}

#ifdef WIN32
/**
 * @brief signal the event loop (windows version)
 * wakes up the main function to handle the next callback
 */
void
signal_event_loop(void)
{

#ifndef NO_MAIN
  WakeConditionVariable(&cv);
#endif /* NO_MAIN */
}
#endif /* WIN32 */

#ifdef __linux__
/**
 * @brief signal the event loop (Linux)
 * wakes up the main function to handle the next callback
 */
void
signal_event_loop(void)
{
#ifndef NO_MAIN
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
#endif /* NO_MAIN */
}
#endif /* __linux__ */


#ifndef NO_MAIN

/**
 * @brief handle Ctrl-C
 * @param signal the captured signal
 */
static void
handle_signal(int signal)
{
  (void)signal;
  signal_event_loop();
  quit = 1;
}

/**
 * @brief print usage and quits
 *
 */
static void
print_usage()
{
  PRINT("Usage:\n");
  PRINT("no arguments : starts the server\n");
  PRINT("-help  : this message\n");
  PRINT("reset  : does an full reset of the device\n");
  PRINT("-s <serial number> : sets the serial number of the device\n");
  exit(0);
}
/**
 * @brief main application.
 * initializes the global variables
 * registers and starts the handler
 * handles (in a loop) the next event.
 * shuts down the stack
 */
int
main(int argc, char *argv[])
{
  oc_clock_time_t next_event;
  bool do_send_s_mode = false;

#ifdef KNX_GUI
  WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
#endif

#ifdef WIN32
  /* windows specific */
  InitializeCriticalSection(&cs);
  InitializeConditionVariable(&cv);
  /* install Ctrl-C */
  signal(SIGINT, handle_signal);
#endif
#ifdef __linux__
  /* Linux specific */
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  /* install Ctrl-C */
  sigaction(SIGINT, &sa, NULL);
#endif

  for (int i = 0; i < argc; i++) {
    PRINT_APP("argv[%d] = %s\n", i, argv[i]);
  }
  if (argc > 1) {
    if (strcmp(argv[1], "reset") == 0) {
      PRINT(" internal reset\n");
      g_reset = true;
    }
    if (strcmp(argv[1], "-help") == 0) {
      print_usage();
    }
  }
  if (argc > 2) {
     if (strcmp(argv[1], "-s") == 0) {
        // serial number
        PRINT("serial number %s\n", argv[2]);
        app_set_serial_number(argv[2]);
     }
  }

  /* do all initialization */
  app_initialize_stack();

#ifdef WIN32
  /* windows specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    if (next_event == 0) {
      SleepConditionVariableCS(&cv, &cs, INFINITE);
    } else {
      oc_clock_time_t now = oc_clock_time();
      if (now < next_event) {
        SleepConditionVariableCS(
          &cv, &cs, (DWORD)((next_event - now) * 1000 / OC_CLOCK_SECOND));
      }
    }
  }
#endif

#ifdef __linux__
  /* Linux specific loop */
  while (quit != 1) {
    next_event = oc_main_poll();
    pthread_mutex_lock(&mutex);
    if (next_event == 0) {
      pthread_cond_wait(&cv, &mutex);
    } else {
      ts.tv_sec = (next_event / OC_CLOCK_SECOND);
      ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
      pthread_cond_timedwait(&cv, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
  }
#endif

  /* shut down the stack */
  oc_main_shutdown();
  return 0;
}
#endif /* NO_MAIN */
