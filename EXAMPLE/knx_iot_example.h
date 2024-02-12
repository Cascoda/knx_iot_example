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
 * header file for the generated application.
 * header file contains functions to use the generated application with an external main.
 * e.g. if the c code is compiled without main then 
 * these functions can be used to call all generated code
 *
 
 */

#include "oc_api.h"
#include "oc_core_res.h"
#include "port/oc_clock.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM) || defined(__TI_ARM__)
#define PACKED __attribute__((__packed__)) //!< Helper macro to create smaller packed structs
#else
#define PACKED //!< Helper macro to create smaller packed structs
#endif

#define THIS_DEVICE 0

// URL defines
#define URL_LED_1 "/p/o_1_1" //!< URL define for LED_1
#define URL_PB_1 "/p/o_2_2" //!< URL define for PB_1
#define URL_INFOONOFF_1 "/p/o_3_3" //!< URL define for InfoOnOff_1

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
  void *g_var;
  volatile void *g_fault;
  bool persistent;
  bool default_present;
  int num_elements;
} datapoint_t;

extern const datapoint_t g_datapoints[];
extern const size_t num_datapoints; 

 

/**
 * @brief Returns the datapoint for the given URL
 *
 * @param url URL of the datapoint
 */
const datapoint_t *get_datapoint_by_url(const char *url); 

///@defgroup DPT_Switch
///@ingroup DPT_Switch
typedef bool DPT_Switch;

 

///////////////////////////////////////////////////////////////////////////////
//               Functions to be implemented by the user!                    //
//               NOTE: Some of these may get implemented by other            //
//               modules depending on the input file!                        //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//               Buttons                                                     //
///////////////////////////////////////////////////////////////////////////////
// Devboard  callbacks
/**
 * 
 */
void lssb_ShortPress_cb(void *ctx);    
 

///////////////////////////////////////////////////////////////////////////////
//               LEDs                                                        //
///////////////////////////////////////////////////////////////////////////////

typedef enum {
  LED_lsab, 
} led_t;
void setLED(led_t led, bool value);
inline static void setLED_lsab(bool value) { setLED(LED_lsab, value); }
inline static void setLED_lsab_ON()  { setLED(LED_lsab, true); }
inline static void setLED_lsab_OFF()  { setLED(LED_lsab, false); }   

///////////////////////////////////////////////////////////////////////////////
//           END Functions to be implemented by the user!                    //
///////////////////////////////////////////////////////////////////////////////

/**
 * Callback invoked by the stack when a successful put is done
 *
 * @param[in] url the url of the put
 *
 */
typedef void (*oc_put_cb_t)(const char* url);

/**
 * @brief The put callback
 *
 */
typedef struct oc_put_struct_t
{
  oc_put_cb_t cb; /**< the put callback, e.g. when something has changed */
} oc_put_struct_t;

/**
 * @brief set the put callback (on application level)
 * 
 * @param cb the callback
 */

void app_set_put_cb(oc_put_cb_t cb);
/**
 * Callback invoked by the stack when a successful put is done
 *
 * @param[in] url the url of the put
 *
 */
typedef void (*oc_get_cb_t)(const char* url);

/**
 * @brief The get callback
 *
 */
typedef struct oc_get_struct_t
{
  oc_get_cb_t cb; /**< the get callback, e.g. when something has changed */
} oc_get_struct_t;

/**
 * @brief set the get callback (on application level)
 * 
 * @param cb the callback
 */
void app_set_get_cb(oc_get_cb_t cb);

/**
 * @brief initialize the stack
 * 
 * @return int 0 == success
 */
int app_initialize_stack();

/**
 * @brief sets the serial number
 * should be called before app_initialize_stack()
 * 
 * @param serial_number the serial number as string
 * @return int 0 == success
 */
int app_set_serial_number(const char* serial_number);

/**
 * @brief Gets the number of elements in dp/param array
 * 
 * @param url the url to check
 * @return int 0 if not an array endpoint, else length of array
 */
int app_get_url_array_size(const char *url);

/**
 * @name DPT_Switch functions
 * getters/setters and other functions for DPT_Switch
 */
/**@{*/
/**
 * @ingroup DPT_Switch
 * @brief Checks if the url depicts a DPT_Switch
 * 
 * @param[in] url the url 
 * @return true: url conveys a DPT_Switch
 */
bool app_is_DPT_Switch_url(const char* url);

/**
 * @ingroup DPT_Switch
 * @brief Set a DPT_Switch to the default value
 * 
 * @param[in] url the url for the DPT_Switch to set
 * 
 * ~~~{.c}
 * app_set_DPT_Switch_default_value("/some/url");
 * ~~~
 */
void app_set_DPT_Switch_default_value(const char* url);

/**
 * @ingroup DPT_Switch
 * @brief Set a DPT_Switch
 * 
 * @param[in] url the url for the DPT_Switch to set
 * @param[in] in a pointer to the DPT_Switch to copy
 * Can be the global variable itself
 * 
 * ~~~{.c}
 * DPT_Switch my_var;
 * // ...set my_var somehow
 * app_set_DPT_Switch_variable("/some/url", &my_var);
 * ~~~
 */
void app_set_DPT_Switch_variable(const char* url, const DPT_Switch* in);

/**
 * @ingroup DPT_Switch
 * @brief Set a DPT_Switch array
 * If n would overflow the array then it is limited to the size
 * of the array variable
 * 
 * @param[in] url the url for the DPT_Switch to set
 * @param[in] in a pointer to the DPT_Switch to copy
 * Can be the global variable itself
 * @param[in] n number of elements in the array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Switch my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Switch_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Switch_array(const char* url, const DPT_Switch* in, int n, bool store_persistently);

/**
 * @ingroup DPT_Switch
 * @brief Set part of a DPT_Switch array
 * If n would overflow the array then it is limited to the size
 * of the array variable. If start > array size then nothing happens.
 * 
 * @param[in] url the url for the DPT_Switch to set
 * @param[in] in a pointer to the DPT_Switch to copy
 * Can be the global variable itself
 * @param[in] start starting index to write to array
 * @param[in] n number of elements to write to array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Switch my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Switch_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Switch_array_elems(const char* url, const DPT_Switch* in, int start, int n, bool store_persistently);

/**
 * @ingroup DPT_Switch
 * @brief Get a DPT_Switch array
 * 
 * @param[in] url the url for the DPT_Switch to get
 * @param[out] out a pointer to the DPT_Switch to copy out to
 * If NULL, won't copy but return the global variable if available
 * @return pointer to the (copied) DPT_Switch <br>
 * If out is NULL then returns the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 *
 * Example of how to use:
 * ~~~{.c}
 * DPT_Switch my_var;
 * if (app_get_DPT_Switch_variable("/some/url", &my_var) == NULL) {
 *   //Something went wrong, maybe print an error message.
 *   return; 
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Switch* app_get_DPT_Switch_variable(const char *url, DPT_Switch* out);

/**
 * @ingroup DPT_Switch
 * @brief Get a DPT_Switch array
 * 
 * @param[in] url the url for the DPT_Switch to get
 * @param[out] out a pointer to the DPT_Switch to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] n number of DPT_Switch elements to copy
 * @return pointer to the (copied) DPT_Switch <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Switch my_arr[5];
 * if (app_get_DPT_Switch_variable("/some/url", my_arr, 5) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
const DPT_Switch* app_get_DPT_Switch_array(const char *url, DPT_Switch* out, int n);


/**
 * @ingroup DPT_Switch
 * @brief Get a DPT_Switch as string
 * 
 * @param[in] in the data type
 * @param[in] text, reserved space to copy the generated text too
 * @param[in] size size of the allocated text
 * 
 * ~~~{.c}
 * DPT_Switch my_type;
 * char my_text[100];
 * if (app_sprintf_DPT_Switch(my_type, my_text, 100) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // printf(my_text);
 * ~~~
 */
int app_sprintf_DPT_Switch(const DPT_Switch *in, char* text, int size);


/**
 * @ingroup DPT_Switch
 * @brief Get a DPT_Switch from a string
 * 
 * @param[in] in the data type
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Switch my_type;
 * char my_text[100];
 * if (app_sscanf_DPT_Switch(&my_type, my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_type
 * ~~~
 */
int app_sscanf_DPT_Switch(DPT_Switch *in, char* text);

/**
 * @ingroup DPT_Switch
 * @brief Get an example of DPT_Switch in a string
 * 
 * @param[in] select 1 = format, 2 = example
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Switch my_type;
 * char my_text[100];
 * if (app_str_expected_DPT_Switch(1 , my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * printf(my_text);
 * ~~~
 */
int app_str_expected_DPT_Switch(int select, char* text);

/**
 * @ingroup DPT_Switch
 * @brief Get a DPT_Switch array
 * 
 * @param[in] url the url for the DPT_Switch to get
 * @param[out] out a pointer to the DPT_Switch to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] start starting index in global array to copy
 * @param[in] n number of DPT_Switch elements to copy
 * @return pointer to the (copied) DPT_Switch <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Switch my_var;
 * // Gets 1 element at index 5 in the array for "/some/url"
 * if (app_get_DPT_Switch_variable("/some/url", &my_var, 5, 1) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Switch* app_get_DPT_Switch_array_elems(const char *url, DPT_Switch* out, int start, int n);

/**
 * @ingroup DPT_Switch
 * @brief Parse a DPT_Switch from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Switch to store into
 * @return true: successfully parsed DPT_Switch
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Switch my_var;
 * if (oc_parse_DPT_Switch(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
bool oc_parse_DPT_Switch(oc_rep_t *rep, DPT_Switch *out);

/**
 * @ingroup DPT_Switch
 * @brief Parse a DPT_Switch[] from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Switch[] to store into
 * @param[in] n The number of elements
 * @return true: successfully parsed DPT_Switch
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Switch my_arr[5];
 * if (oc_parse_DPT_Switch_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
bool oc_parse_DPT_Switch_array(oc_rep_t *rep, DPT_Switch *out, int n);

/**
 * @ingroup DPT_Switch
 * @brief Encode a DPT_Switch using oc_rep
 * 
 * @param[in] in The DPT_Switch to encode
 *
 * ~~~{.c}
 * DPT_Switch my_var;
 * app_get_DPT_Switch_variable("/some/url", &my_var);
 * if (oc_encode_DPT_Switch(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Switch(const DPT_Switch *in);

/**
 * @ingroup DPT_Switch
 * @brief Encode a DPT_Switch[] using oc_rep
 * 
 * @param[in] in The DPT_Switch[] to encode
 * @param[in] n The number of elements
 *
 * ~~~{.c}
 * DPT_Switch my_arr[5];
 * app_get_DPT_Switch_array("/some/url", my_arr, 5);
 * if (oc_encode_DPT_Switch_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Switch_array(const DPT_Switch *in, int n);

/**
 * @ingroup DPT_Switch
 * @brief Store a DPT_Switch into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Switch to store
 *
 * ~~~{.c}
 * DPT_Switch my_var;
 * app_get_DPT_Switch_variable("/some/url", &my_var);
 * persistent_storeDPT_Switch("/some/other/url", &my_var);
 * ~~~
 */
void persistent_store_DPT_Switch(const char *name, const DPT_Switch *in);

/**
 * @ingroup DPT_Switch
 * @brief Store a DPT_Switch[] into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Switch[] to store
 * @param[in] n number of elements to store
 *
 * ~~~{.c}
 * DPT_Switch my_arr[5];
 * app_get_DPT_Switch_array("/some/url", my_var, 5);
 * persistent_storeDPT_Switch_array("/some/other/url", my_arr, 5);
 * ~~~
 */
void persistent_store_DPT_Switch_array(const char *name, const DPT_Switch *in, int n);

/**
 * @ingroup DPT_Switch
 * @brief Load a DPT_Switch from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Switch to load into
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Switch my_var;
 * app_get_DPT_Switch_variable("/some/url", &my_var);
 * if (persistent_loadDPT_Switch("/some/other/url", &my_var) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Switch(const char *name, DPT_Switch *out);

/**
 * @ingroup DPT_Switch
 * @brief Load a DPT_Switch[] from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Switch[] to load into
 * @param[in] n number of elements in array
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Switch my_arr[5];
 * app_get_DPT_Switch_array("/some/url", my_arr, 5);
 * if (persistent_loadDPT_Switch("/some/other/url", my_arr, 5) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Switch_array(const char *name, DPT_Switch *out, int n);
/**@}*/


// Getters/Setters for bool
/**
 * @brief Checks if the url depicts a bool
 * 
 * @param url the url 
 * @return true: url conveys a bool
 */
bool app_is_bool_url(const char* url);

/**
 * @brief Set a bool
 * 
 * @param url the url for the bool to set
 * @param value value to set
 */
void app_set_bool_variable(const char* url, bool value);

/**
 * @brief Get a bool
 * 
 * @param url the url for the bool to get
 * @return boolean variable
 */
bool app_retrieve_bool_variable(const char *url);
 
/**
 * @brief Get a int
 * 
 * @param url the url for the int to get
 * @param value the value to be used for the return value
 * @return bool, true, value is set
 */
bool app_retrieve_int_variable(const char* url, int* value);
 
 
/**
 * @brief checks if the url represents a parameter
 *
 * @param url the url
 * @return true the url represents a parameter
 */
bool app_is_url_parameter(const char* url);

/**
 * @brief retrieves the url of a parameter
 * index starts at 1
 * @param index the index to retrieve the url from
 * @return the url or NULL
 */
const char* app_get_parameter_url(int index);

/**
 * @brief retrieves the name of a parameter
 * index starts at 1
 * @param index the index to retrieve the parameter name from
 * @return the name or NULL
 */
const char* app_get_parameter_name(int index);

/**
 * @brief sets the fault state of the url/data point 
 * the caller needs to know if the resource/data point implements a fault situation
 * 
 * @param url the url of the resource/data point
 * @param value the boolean fault value to be set
 */
void app_set_fault_variable(const char* url, bool value);

/**
 * @brief checks if the url is in use (e.g. used in the Group Object Table)
 * 
 * @param url the url of the resource/data point
 * @return true: entry in Group Object Table has the URL
 * @return false: No entry in Group Object Table has the URL
 */
bool app_is_url_in_use(const char* url);

/**
 * @brief function to report if the (oscore) security is turn on for this instance
 * 
 * @return true is secure
 * @return false is not secure
 */
bool app_is_secure();

/**
 * @brief retrieves the password for showing on screen
 * 
 * @return password (as string)
 */
const char* app_get_password();

/**
 * @brief function to set the input string to upper case
 *
 * @param str the string to make upper case
 *
 */
void app_str_to_upper(char *str);

/**
 * @brief Function to be used as a callback for when a button is pressed.
 *        This function will toggle the value of the url, and send a message.
 * 
 * @param url the url of the resource/data point
 */
void dev_btn_toggle_cb(const char *url);
 

#ifdef __cplusplus
}
#endif
