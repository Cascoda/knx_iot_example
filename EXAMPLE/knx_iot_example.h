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

// URL defines
#define URL_LED_1 "/p/o_1_1" // define URL LED_1 for /p/o_1_1
#define URL_PB_1 "/p/o_2_2" // define URL PB_1 for /p/o_2_2

/**
 * Callback invoked by the stack when a successfull put is done
 *
 * @param[in] url the url of the put
 *
 */
typedef void (*oc_put_cb_t)(char* url);

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
int app_set_serial_number(char* serial_number);

/**
 * @brief checks if the url depicts an boolean
 * 
 * @param url the url to check
 * @return true: url conveys a boolean
 */
bool app_is_bool_url(char* url);

/**
 * @brief retrieve the boolean variable of the url/data point
 * the caller needs to know if the resource/data point is conveying a boolean
 * "/p/o_1_1" of LED_1 
 * "/p/o_2_2" of PB_1 
 * 
 * @param url the url of the resource/data point
 * @return true value is true
 * @return false value is false or error.
 */
bool app_retrieve_bool_variable(char* url);

/**
 * @brief set the boolean variable that belongs to the url 
 * The caller needs to know if the resource/data point is conveying a boolean
 * "/p/o_1_1" of LED_1
 * 
 * @param url the url of the resource/data point
 * @param value the boolean value to be set
 */
void app_set_bool_variable(char* url, bool value);

/**
 * @brief checks if the url depicts an int
 * 
 * @param url the url to check
 * @return true: url conveys a int
 */
bool app_is_int_url(char* url);

/**
 * @brief retrieve the int variable of the url/data point
 * the caller needs to know if the resource/data point is conveying a integer
 * 
 * @param url the url of the resource/data point
 * @return the integer value of the global variable
 */
int app_retrieve_int_variable(char* url);

/**
 * @brief set the integer variable that belongs to the url 
 * The caller needs to know if the resource/data point is conveying a integer
 * 
 * @param url the url of the resource/data point
 * @param value the integer value to be set
 */
 
void app_set_int_variable(char* url, int value);

void app_set_integer_variable(char* url, int value);

/**
 * @brief checks if the url depicts an double
 * 
 * @param url the url to check
 * @return true: url conveys a double
 */
bool app_is_double_url(char* url);

/**
 * @brief retrieve the double variable of the url/data point
 * the caller needs to know if the resource/data point is conveying a integer
 * 
 * @param url the url of the resource/data point
 * @return the double value of the global variable
 */
double app_retrieve_double_variable(char* url);

/**
 * @brief set the double variable that belongs to the url 
 * The caller needs to know if the resource/data point is conveying a double
 * 
 * @param url the url of the resource/data point
 * @param value the double value to be set
 */
void app_set_double_variable(char* url, double value);

/**
 * @brief function to check if the url is represented by a string
 *
 * @param url the url value is a string
 * @return true = url returns a string, false = url is not a string
 */
bool app_is_string_url(char* url);

/**
 * @brief sets the global string variable at the url
 *
 * @param url the url indicating the global variable
 * @param value the string value to be set
 */
void app_set_string_variable(char* url, char* value);

/**
 * @brief retrieve the global string variable at the url
 *
 * @param url the url indicating the global variable
 * @return the value of the variable
 */
char* app_retrieve_string_variable(char* url);

/**
 * @brief checks if the url represents a parameter
 *
 * @param url the url
 * @return true the url represents a parameter
 */
bool app_is_url_parameter(char* url);

/**
 * @brief retrieves the url of a parameter
 * index starts at 1
 * @param index the index to retrieve the url from
 * @return the url or NULL
 */
char* app_get_parameter_url(int index);

/**
 * @brief retrieves the name of a parameter
 * index starts at 1
 * @param index the index to retrieve the parameter name from
 * @return the name or NULL
 */
char* app_get_parameter_name(int index);

/**
 * @brief sets the fault state of the url/data point 
 * the caller needs to know if the resource/data point implements a fault situation
 * 
 * @param url the url of the resource/data point
 * @param value the boolean fault value to be set
 */
void app_set_fault_variable(char* url, bool value);

/**
 * @brief checks if the url is in use (e.g. used in the Group Object Table)
 * 
 * @param url the url of the resource/data point
 * @return true: entry in Group Object Table has the URL
 * @return false: No entry in Group Object Table has the URL
 */
bool app_is_url_in_use(char* url);

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
char* app_get_password();

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
void dev_btn_toggle_cb(char *url);

#ifdef __cplusplus
}
#endif
