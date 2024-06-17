/*
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 Copyright (c) 2022-2024 Cascoda Ltd
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

#define THIS_DEVICE 0 //!< single device instance, e.g. number 0

// URL defines
#define URL_SENDSHOT "/p/o_1_1" //!< URL "SendShot"  desc:""
#define URL_RECEIVESHOT "/p/o_1_2" //!< URL "ReceiveShot"  desc:""
#define URL_SENDSHOTSTATUS "/p/o_1_3" //!< URL "SendShotStatus"  desc:""
#define URL_RECEIVESHOTSTATUS "/p/o_1_4" //!< URL "ReceiveShotStatus"  desc:""
#define URL_SENDREADY "/p/o_1_5" //!< URL "SendReady"  desc:""
#define URL_RECEIVEREADY "/p/o_1_6" //!< URL "ReceiveReady"  desc:""
#define URL_STARTING_PLAYER "/p/p_1_1" //!< URL "Starting_Player"  desc:""

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

/* all data points */
extern const datapoint_t g_datapoints[];
extern const size_t num_datapoints; 

/* all parameters */
extern const datapoint_t g_parameters[];
extern const size_t num_parameters; 
/* ENUM defines, for each module instance */ 


/**
 * @brief Returns the datapoint for the given URL
 *
 * @param url URL of the datapoint
 */
const datapoint_t *get_datapoint_by_url(const char *url);

/**
 * @brief Returns the url of the module by instance
 *
 * @param out_url URL of the datapoint given back (e.g. with the corrected post fix)
 * @param in_url URL of the datapoint of the module
 * @param module_index URL the module index
 * @return false if operation successful, true if something went wrong 
 */
bool get_module_url(char* out_url, const char* in_url, int module_index); 

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

///////////////////////////////////////////////////////////////////////////////
//               EINK Typedefs/handler structs                               //
///////////////////////////////////////////////////////////////////////////////

/**
 * @name EINK declarations
 * EINK functions, macros and defines.
 */
/**@{*/

#define LINE_HEIGHT 10 // 8pixels for character + 1 pixel padding above and below

// Line mapping: l1 -> 15, l2 -> 25, l3 -> 35 etc.
#define EINK_LINE_NO(x) (((x) * LINE_HEIGHT) + 5)

// Defines used for scrolling mechanism
#define TOP_MOST_LINE_Y_COORD EINK_LINE_NO(1)
#define BOTTOM_MOST_LINE_Y_COORD EINK_LINE_NO(8)
// This is the number of lines that will keep scrolling before looping back,
// after the last line has been displayed.
#define CONVENIENCE_PADDING_FOR_SCROLLING 0
#ifndef INITIAL_SCREEN_TIMEOUT
#define INITIAL_SCREEN_TIMEOUT 0
#endif

extern bool g_eink_clean_redraw;

/**
 * @brief Screen load callback
 * called to draw to the screenbuffer and update the display
 * 
 * @return false if write to display already handled <br/>
 * true otherwise (caller will update display)
 */
typedef bool(*load_screen_cb_t)(void);

/**
 * @brief Button event callback
 * Called when a button event occurs for a screen
 */
typedef void(*button_cb_t)(void);

struct EinkScreenHandler
{
  load_screen_cb_t load_screen_cb;
  const char *title;
  button_cb_t screen_button_1_ShortPress_cb;
  button_cb_t screen_button_1_LongPress_cb;
  button_cb_t screen_button_2_ShortPress_cb;
  button_cb_t screen_button_2_LongPress_cb;
  button_cb_t screen_button_3_ShortPress_cb;
  button_cb_t screen_button_3_LongPress_cb;
  button_cb_t screen_button_3_Hold_cb; 
};enum Screen
{
  SPLASH_SCREEN = 0,
  MENU_SCREEN = 1,
  QR_SCREEN = 2,
  DEV_SCREEN = 3,
  RESET_SCREEN = 4,
  ROLE_SCREEN = 5,
  TABLES_SCREEN = 6,
  HELP_SCREEN = 7, 
  GAME_INTRO,
  CONTROLS_1,
  PLACE_SHIPS,
  READY_WAIT,
  FIRE_SHOT,
  SHOT_INFO,
  GAME_OVER, 
  NUM_SCREENS
};

extern enum Screen g_screen_nr;

const char* screen_get_title(enum Screen);

struct menu_t{
  const char *text;
  void (*onclick)(void);
};

enum BattDisplaySymbol
{
  BATT_DISPLAY_NONE,
  BATT_DISPLAY_ICON,
  BATT_DISPLAY_PERCENT,
  BATT_DISPLAY_END,
};

extern enum BattDisplaySymbol g_batt_display_symbol;

uint8_t get_batt_percent(void);

/**
 * @brief Draw the nice screen header and frame.
 *
 * @param screen Screen number for the header
 */
void app_header_draw(enum Screen nr);

/**
 * @brief Draw a scrollable menu and highlight
 * the selected item.
 */
void draw_scrollable_menu(struct menu_t *menu, int numentries, enum Screen nr);
void menu_next();
void menu_prev();
void menu_select();
void go_CONNECTIVITY_SCREEN();

/**
 * @brief Set the screen to be loaded and redraw.
 *
 * @param screen_no Screen to be loaded
 */
void set_screen(enum Screen screen_no);

/**
 * @brief Load the next or previous sequential screen immediately
 * 
 * @param increase whether to go to the next (increase) or previous (decrease) screen.
 */
void next_screen(bool increase);
void refresh_screen(bool clean_redraw);
/**
 * @brief Load the next sequential screen immediately
 * Can be used as an eink screen event handler.
 */
void go_next_screen();
/**
 * @brief Load the previous sequential screen immediately
 * Can be used as an eink screen event handler.
 */
void go_prev_screen();
/**
 * @brief Load the builtin SPLASH_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_SPLASH_SCREEN() { set_screen(SPLASH_SCREEN); }
/**
 * @brief Load the builtin MENU_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_MENU_SCREEN() { set_screen(MENU_SCREEN); }
/**
 * @brief Load the builtin QR_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_QR_SCREEN() { set_screen(QR_SCREEN); }
/**
 * @brief Load the builtin DEV_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_DEV_SCREEN() { set_screen(DEV_SCREEN); }
/**
 * @brief Load the builtin RESET_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_RESET_SCREEN() { set_screen(RESET_SCREEN); }
/**
 * @brief Load the builtin ROLE_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_ROLE_SCREEN() { set_screen(ROLE_SCREEN); }
/**
 * @brief Load the builtin TABLES_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_TABLES_SCREEN() { set_screen(TABLES_SCREEN); }
/**
 * @brief Load the builtin HELP_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_HELP_SCREEN() { set_screen(HELP_SCREEN); } 
/**
 * @brief Load the user defined GAME_INTRO screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_GAME_INTRO() { set_screen(GAME_INTRO); }
/**
 * @brief Load the user defined CONTROLS_1 screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_CONTROLS_1() { set_screen(CONTROLS_1); }
/**
 * @brief Load the user defined PLACE_SHIPS screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_PLACE_SHIPS() { set_screen(PLACE_SHIPS); }
/**
 * @brief Load the user defined READY_WAIT screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_READY_WAIT() { set_screen(READY_WAIT); }
/**
 * @brief Load the user defined FIRE_SHOT screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_FIRE_SHOT() { set_screen(FIRE_SHOT); }
/**
 * @brief Load the user defined SHOT_INFO screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_SHOT_INFO() { set_screen(SHOT_INFO); }
/**
 * @brief Load the user defined GAME_OVER screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_GAME_OVER() { set_screen(GAME_OVER); } 
/**
 * @brief Load the first user defined screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_first_screen() { set_screen(GAME_INTRO); }

/**@}*/ 

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
void button_1_ShortPress_cb(void *ctx);
/**
 * 
 */
void button_1_LongPress_cb(void *ctx);  
// Devboard  callbacks
/**
 * 
 */
void button_2_ShortPress_cb(void *ctx);
/**
 * 
 */
void button_2_LongPress_cb(void *ctx);  
// Devboard  callbacks
/**
 * 
 */
void button_3_ShortPress_cb(void *ctx);
/**
 * 
 */
void button_3_LongPress_cb(void *ctx);
/**
 * 
 */
void button_3_Hold_cb(void *ctx);    
///////////////////////////////////////////////////////////////////////////////
//               EINK                                                        //
///////////////////////////////////////////////////////////////////////////////   

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
 * @name DPT_Param_Bool functions
 * getters/setters and other functions for DPT_Param_Bool
 */
/**@{*/
/**
 * @ingroup DPT_Param_Bool
 * @brief Checks if the url depicts a DPT_Param_Bool
 * 
 * @param[in] url the url 
 * @return true: url conveys a DPT_Param_Bool
 */
bool app_is_DPT_Param_Bool_url(const char* url);

/**
 * @ingroup DPT_Param_Bool
 * @brief Set a DPT_Param_Bool to the default value
 * 
 * @param[in] url the url for the DPT_Param_Bool to set
 * 
 * ~~~{.c}
 * app_set_DPT_Param_Bool_default_value("/some/url");
 * ~~~
 */
void app_set_DPT_Param_Bool_default_value(const char* url);

/**
 * @ingroup DPT_Param_Bool
 * @brief Set a DPT_Param_Bool
 * 
 * @param[in] url the url for the DPT_Param_Bool to set
 * @param[in] in a pointer to the DPT_Param_Bool to copy
 * Can be the global variable itself
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * // ...set my_var somehow
 * app_set_DPT_Param_Bool_variable("/some/url", &my_var);
 * ~~~
 */
void app_set_DPT_Param_Bool_variable(const char* url, const DPT_Param_Bool* in);

/**
 * @ingroup DPT_Param_Bool
 * @brief Set a DPT_Param_Bool array
 * If n would overflow the array then it is limited to the size
 * of the array variable
 * 
 * @param[in] url the url for the DPT_Param_Bool to set
 * @param[in] in a pointer to the DPT_Param_Bool to copy
 * Can be the global variable itself
 * @param[in] n number of elements in the array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Param_Bool_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Param_Bool_array(const char* url, const DPT_Param_Bool* in, int n, bool store_persistently);

/**
 * @ingroup DPT_Param_Bool
 * @brief Set part of a DPT_Param_Bool array
 * If n would overflow the array then it is limited to the size
 * of the array variable. If start > array size then nothing happens.
 * 
 * @param[in] url the url for the DPT_Param_Bool to set
 * @param[in] in a pointer to the DPT_Param_Bool to copy
 * Can be the global variable itself
 * @param[in] start starting index to write to array
 * @param[in] n number of elements to write to array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Param_Bool_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Param_Bool_array_elems(const char* url, const DPT_Param_Bool* in, int start, int n, bool store_persistently);

/**
 * @ingroup DPT_Param_Bool
 * @brief Get a DPT_Param_Bool array
 * 
 * @param[in] url the url for the DPT_Param_Bool to get
 * @param[out] out a pointer to the DPT_Param_Bool to copy out to
 * If NULL, won't copy but return the global variable if available
 * @return pointer to the (copied) DPT_Param_Bool <br>
 * If out is NULL then returns the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 *
 * Example of how to use:
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * if (app_get_DPT_Param_Bool_variable("/some/url", &my_var) == NULL) {
 *   //Something went wrong, maybe print an error message.
 *   return; 
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Param_Bool* app_get_DPT_Param_Bool_variable(const char *url, DPT_Param_Bool* out);

/**
 * @ingroup DPT_Param_Bool
 * @brief Get a DPT_Param_Bool array
 * 
 * @param[in] url the url for the DPT_Param_Bool to get
 * @param[out] out a pointer to the DPT_Param_Bool to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] n number of DPT_Param_Bool elements to copy
 * @return pointer to the (copied) DPT_Param_Bool <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * if (app_get_DPT_Param_Bool_variable("/some/url", my_arr, 5) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
const DPT_Param_Bool* app_get_DPT_Param_Bool_array(const char *url, DPT_Param_Bool* out, int n);


/**
 * @ingroup DPT_Param_Bool
 * @brief Get a DPT_Param_Bool as string
 * 
 * @param[in] in the data type
 * @param[in] text, reserved space to copy the generated text too
 * @param[in] size size of the allocated text
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_type;
 * char my_text[100];
 * if (app_sprintf_DPT_Param_Bool(my_type, my_text, 100) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // printf(my_text);
 * ~~~
 */
int app_sprintf_DPT_Param_Bool(const DPT_Param_Bool *in, char* text, int size);


/**
 * @ingroup DPT_Param_Bool
 * @brief Get a DPT_Param_Bool from a string
 * 
 * @param[in] in the data type
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_type;
 * char my_text[100];
 * if (app_sscanf_DPT_Param_Bool(&my_type, my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_type
 * ~~~
 */
int app_sscanf_DPT_Param_Bool(DPT_Param_Bool *in, char* text);

/**
 * @ingroup DPT_Param_Bool
 * @brief Get an example of DPT_Param_Bool in a string
 * 
 * @param[in] select 1 = format, 2 = example
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_type;
 * char my_text[100];
 * if (app_str_expected_DPT_Param_Bool(1 , my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * printf(my_text);
 * ~~~
 */
int app_str_expected_DPT_Param_Bool(int select, char* text);

/**
 * @ingroup DPT_Param_Bool
 * @brief Get a DPT_Param_Bool array
 * 
 * @param[in] url the url for the DPT_Param_Bool to get
 * @param[out] out a pointer to the DPT_Param_Bool to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] start starting index in global array to copy
 * @param[in] n number of DPT_Param_Bool elements to copy
 * @return pointer to the (copied) DPT_Param_Bool <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * // Gets 1 element at index 5 in the array for "/some/url"
 * if (app_get_DPT_Param_Bool_variable("/some/url", &my_var, 5, 1) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Param_Bool* app_get_DPT_Param_Bool_array_elems(const char *url, DPT_Param_Bool* out, int start, int n);

/**
 * @ingroup DPT_Param_Bool
 * @brief Parse a DPT_Param_Bool from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Param_Bool to store into
 * @return true: successfully parsed DPT_Param_Bool
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Param_Bool my_var;
 * if (oc_parse_DPT_Param_Bool(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
bool oc_parse_DPT_Param_Bool(oc_rep_t *rep, DPT_Param_Bool *out);

/**
 * @ingroup DPT_Param_Bool
 * @brief Parse a DPT_Param_Bool[] from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Param_Bool[] to store into
 * @param[in] n The number of elements
 * @return true: successfully parsed DPT_Param_Bool
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Param_Bool my_arr[5];
 * if (oc_parse_DPT_Param_Bool_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
bool oc_parse_DPT_Param_Bool_array(oc_rep_t *rep, DPT_Param_Bool *out, int n);

/**
 * @ingroup DPT_Param_Bool
 * @brief Encode a DPT_Param_Bool using oc_rep
 * 
 * @param[in] in The DPT_Param_Bool to encode
 * @param[is_metadata] is_metadata Whether function is called during metadata query handling
 *
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * app_get_DPT_Param_Bool_variable("/some/url", &my_var);
 * if (oc_encode_DPT_Param_Bool(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Param_Bool(const DPT_Param_Bool *in, bool is_metadata);

/**
 * @ingroup DPT_Param_Bool
 * @brief Encode a DPT_Param_Bool[] using oc_rep
 * 
 * @param[in] in The DPT_Param_Bool[] to encode
 * @param[in] n The number of elements
 *
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * app_get_DPT_Param_Bool_array("/some/url", my_arr, 5);
 * if (oc_encode_DPT_Param_Bool_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Param_Bool_array(const DPT_Param_Bool *in, int n);

/**
 * @ingroup DPT_Param_Bool
 * @brief Store a DPT_Param_Bool into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Param_Bool to store
 *
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * app_get_DPT_Param_Bool_variable("/some/url", &my_var);
 * persistent_storeDPT_Param_Bool("/some/other/url", &my_var);
 * ~~~
 */
void persistent_store_DPT_Param_Bool(const char *name, const DPT_Param_Bool *in);

/**
 * @ingroup DPT_Param_Bool
 * @brief Store a DPT_Param_Bool[] into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Param_Bool[] to store
 * @param[in] n number of elements to store
 *
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * app_get_DPT_Param_Bool_array("/some/url", my_var, 5);
 * persistent_storeDPT_Param_Bool_array("/some/other/url", my_arr, 5);
 * ~~~
 */
void persistent_store_DPT_Param_Bool_array(const char *name, const DPT_Param_Bool *in, int n);

/**
 * @ingroup DPT_Param_Bool
 * @brief Load a DPT_Param_Bool from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Param_Bool to load into
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * app_get_DPT_Param_Bool_variable("/some/url", &my_var);
 * if (persistent_loadDPT_Param_Bool("/some/other/url", &my_var) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Param_Bool(const char *name, DPT_Param_Bool *out);

/**
 * @ingroup DPT_Param_Bool
 * @brief Load a DPT_Param_Bool[] from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Param_Bool[] to load into
 * @param[in] n number of elements in array
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * app_get_DPT_Param_Bool_array("/some/url", my_arr, 5);
 * if (persistent_loadDPT_Param_Bool("/some/other/url", my_arr, 5) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Param_Bool_array(const char *name, DPT_Param_Bool *out, int n);
/**@}*/

/**
 * @name DPT_Shot_Status functions
 * getters/setters and other functions for DPT_Shot_Status
 */
/**@{*/
/**
 * @ingroup DPT_Shot_Status
 * @brief Checks if the url depicts a DPT_Shot_Status
 * 
 * @param[in] url the url 
 * @return true: url conveys a DPT_Shot_Status
 */
bool app_is_DPT_Shot_Status_url(const char* url);

/**
 * @ingroup DPT_Shot_Status
 * @brief Set a DPT_Shot_Status to the default value
 * 
 * @param[in] url the url for the DPT_Shot_Status to set
 * 
 * ~~~{.c}
 * app_set_DPT_Shot_Status_default_value("/some/url");
 * ~~~
 */
void app_set_DPT_Shot_Status_default_value(const char* url);

/**
 * @ingroup DPT_Shot_Status
 * @brief Set a DPT_Shot_Status
 * 
 * @param[in] url the url for the DPT_Shot_Status to set
 * @param[in] in a pointer to the DPT_Shot_Status to copy
 * Can be the global variable itself
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * // ...set my_var somehow
 * app_set_DPT_Shot_Status_variable("/some/url", &my_var);
 * ~~~
 */
void app_set_DPT_Shot_Status_variable(const char* url, const DPT_Shot_Status* in);

/**
 * @ingroup DPT_Shot_Status
 * @brief Set a DPT_Shot_Status array
 * If n would overflow the array then it is limited to the size
 * of the array variable
 * 
 * @param[in] url the url for the DPT_Shot_Status to set
 * @param[in] in a pointer to the DPT_Shot_Status to copy
 * Can be the global variable itself
 * @param[in] n number of elements in the array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Shot_Status_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Shot_Status_array(const char* url, const DPT_Shot_Status* in, int n, bool store_persistently);

/**
 * @ingroup DPT_Shot_Status
 * @brief Set part of a DPT_Shot_Status array
 * If n would overflow the array then it is limited to the size
 * of the array variable. If start > array size then nothing happens.
 * 
 * @param[in] url the url for the DPT_Shot_Status to set
 * @param[in] in a pointer to the DPT_Shot_Status to copy
 * Can be the global variable itself
 * @param[in] start starting index to write to array
 * @param[in] n number of elements to write to array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Shot_Status_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Shot_Status_array_elems(const char* url, const DPT_Shot_Status* in, int start, int n, bool store_persistently);

/**
 * @ingroup DPT_Shot_Status
 * @brief Get a DPT_Shot_Status array
 * 
 * @param[in] url the url for the DPT_Shot_Status to get
 * @param[out] out a pointer to the DPT_Shot_Status to copy out to
 * If NULL, won't copy but return the global variable if available
 * @return pointer to the (copied) DPT_Shot_Status <br>
 * If out is NULL then returns the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 *
 * Example of how to use:
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * if (app_get_DPT_Shot_Status_variable("/some/url", &my_var) == NULL) {
 *   //Something went wrong, maybe print an error message.
 *   return; 
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Shot_Status* app_get_DPT_Shot_Status_variable(const char *url, DPT_Shot_Status* out);

/**
 * @ingroup DPT_Shot_Status
 * @brief Get a DPT_Shot_Status array
 * 
 * @param[in] url the url for the DPT_Shot_Status to get
 * @param[out] out a pointer to the DPT_Shot_Status to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] n number of DPT_Shot_Status elements to copy
 * @return pointer to the (copied) DPT_Shot_Status <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * if (app_get_DPT_Shot_Status_variable("/some/url", my_arr, 5) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
const DPT_Shot_Status* app_get_DPT_Shot_Status_array(const char *url, DPT_Shot_Status* out, int n);


/**
 * @ingroup DPT_Shot_Status
 * @brief Get a DPT_Shot_Status as string
 * 
 * @param[in] in the data type
 * @param[in] text, reserved space to copy the generated text too
 * @param[in] size size of the allocated text
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_type;
 * char my_text[100];
 * if (app_sprintf_DPT_Shot_Status(my_type, my_text, 100) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // printf(my_text);
 * ~~~
 */
int app_sprintf_DPT_Shot_Status(const DPT_Shot_Status *in, char* text, int size);


/**
 * @ingroup DPT_Shot_Status
 * @brief Get a DPT_Shot_Status from a string
 * 
 * @param[in] in the data type
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_type;
 * char my_text[100];
 * if (app_sscanf_DPT_Shot_Status(&my_type, my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_type
 * ~~~
 */
int app_sscanf_DPT_Shot_Status(DPT_Shot_Status *in, char* text);

/**
 * @ingroup DPT_Shot_Status
 * @brief Get an example of DPT_Shot_Status in a string
 * 
 * @param[in] select 1 = format, 2 = example
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_type;
 * char my_text[100];
 * if (app_str_expected_DPT_Shot_Status(1 , my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * printf(my_text);
 * ~~~
 */
int app_str_expected_DPT_Shot_Status(int select, char* text);

/**
 * @ingroup DPT_Shot_Status
 * @brief Get a DPT_Shot_Status array
 * 
 * @param[in] url the url for the DPT_Shot_Status to get
 * @param[out] out a pointer to the DPT_Shot_Status to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] start starting index in global array to copy
 * @param[in] n number of DPT_Shot_Status elements to copy
 * @return pointer to the (copied) DPT_Shot_Status <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * // Gets 1 element at index 5 in the array for "/some/url"
 * if (app_get_DPT_Shot_Status_variable("/some/url", &my_var, 5, 1) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Shot_Status* app_get_DPT_Shot_Status_array_elems(const char *url, DPT_Shot_Status* out, int start, int n);

/**
 * @ingroup DPT_Shot_Status
 * @brief Parse a DPT_Shot_Status from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Shot_Status to store into
 * @return true: successfully parsed DPT_Shot_Status
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Shot_Status my_var;
 * if (oc_parse_DPT_Shot_Status(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
bool oc_parse_DPT_Shot_Status(oc_rep_t *rep, DPT_Shot_Status *out);

/**
 * @ingroup DPT_Shot_Status
 * @brief Parse a DPT_Shot_Status[] from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Shot_Status[] to store into
 * @param[in] n The number of elements
 * @return true: successfully parsed DPT_Shot_Status
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Shot_Status my_arr[5];
 * if (oc_parse_DPT_Shot_Status_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
bool oc_parse_DPT_Shot_Status_array(oc_rep_t *rep, DPT_Shot_Status *out, int n);

/**
 * @ingroup DPT_Shot_Status
 * @brief Encode a DPT_Shot_Status using oc_rep
 * 
 * @param[in] in The DPT_Shot_Status to encode
 * @param[is_metadata] is_metadata Whether function is called during metadata query handling
 *
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * app_get_DPT_Shot_Status_variable("/some/url", &my_var);
 * if (oc_encode_DPT_Shot_Status(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Shot_Status(const DPT_Shot_Status *in, bool is_metadata);

/**
 * @ingroup DPT_Shot_Status
 * @brief Encode a DPT_Shot_Status[] using oc_rep
 * 
 * @param[in] in The DPT_Shot_Status[] to encode
 * @param[in] n The number of elements
 *
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * app_get_DPT_Shot_Status_array("/some/url", my_arr, 5);
 * if (oc_encode_DPT_Shot_Status_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Shot_Status_array(const DPT_Shot_Status *in, int n);

/**
 * @ingroup DPT_Shot_Status
 * @brief Store a DPT_Shot_Status into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Shot_Status to store
 *
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * app_get_DPT_Shot_Status_variable("/some/url", &my_var);
 * persistent_storeDPT_Shot_Status("/some/other/url", &my_var);
 * ~~~
 */
void persistent_store_DPT_Shot_Status(const char *name, const DPT_Shot_Status *in);

/**
 * @ingroup DPT_Shot_Status
 * @brief Store a DPT_Shot_Status[] into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Shot_Status[] to store
 * @param[in] n number of elements to store
 *
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * app_get_DPT_Shot_Status_array("/some/url", my_var, 5);
 * persistent_storeDPT_Shot_Status_array("/some/other/url", my_arr, 5);
 * ~~~
 */
void persistent_store_DPT_Shot_Status_array(const char *name, const DPT_Shot_Status *in, int n);

/**
 * @ingroup DPT_Shot_Status
 * @brief Load a DPT_Shot_Status from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Shot_Status to load into
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * app_get_DPT_Shot_Status_variable("/some/url", &my_var);
 * if (persistent_loadDPT_Shot_Status("/some/other/url", &my_var) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Shot_Status(const char *name, DPT_Shot_Status *out);

/**
 * @ingroup DPT_Shot_Status
 * @brief Load a DPT_Shot_Status[] from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Shot_Status[] to load into
 * @param[in] n number of elements in array
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * app_get_DPT_Shot_Status_array("/some/url", my_arr, 5);
 * if (persistent_loadDPT_Shot_Status("/some/other/url", my_arr, 5) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Shot_Status_array(const char *name, DPT_Shot_Status *out, int n);
/**@}*/

/**
 * @name DPT_Start functions
 * getters/setters and other functions for DPT_Start
 */
/**@{*/
/**
 * @ingroup DPT_Start
 * @brief Checks if the url depicts a DPT_Start
 * 
 * @param[in] url the url 
 * @return true: url conveys a DPT_Start
 */
bool app_is_DPT_Start_url(const char* url);

/**
 * @ingroup DPT_Start
 * @brief Set a DPT_Start to the default value
 * 
 * @param[in] url the url for the DPT_Start to set
 * 
 * ~~~{.c}
 * app_set_DPT_Start_default_value("/some/url");
 * ~~~
 */
void app_set_DPT_Start_default_value(const char* url);

/**
 * @ingroup DPT_Start
 * @brief Set a DPT_Start
 * 
 * @param[in] url the url for the DPT_Start to set
 * @param[in] in a pointer to the DPT_Start to copy
 * Can be the global variable itself
 * 
 * ~~~{.c}
 * DPT_Start my_var;
 * // ...set my_var somehow
 * app_set_DPT_Start_variable("/some/url", &my_var);
 * ~~~
 */
void app_set_DPT_Start_variable(const char* url, const DPT_Start* in);

/**
 * @ingroup DPT_Start
 * @brief Set a DPT_Start array
 * If n would overflow the array then it is limited to the size
 * of the array variable
 * 
 * @param[in] url the url for the DPT_Start to set
 * @param[in] in a pointer to the DPT_Start to copy
 * Can be the global variable itself
 * @param[in] n number of elements in the array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Start_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Start_array(const char* url, const DPT_Start* in, int n, bool store_persistently);

/**
 * @ingroup DPT_Start
 * @brief Set part of a DPT_Start array
 * If n would overflow the array then it is limited to the size
 * of the array variable. If start > array size then nothing happens.
 * 
 * @param[in] url the url for the DPT_Start to set
 * @param[in] in a pointer to the DPT_Start to copy
 * Can be the global variable itself
 * @param[in] start starting index to write to array
 * @param[in] n number of elements to write to array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Start my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Start_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Start_array_elems(const char* url, const DPT_Start* in, int start, int n, bool store_persistently);

/**
 * @ingroup DPT_Start
 * @brief Get a DPT_Start array
 * 
 * @param[in] url the url for the DPT_Start to get
 * @param[out] out a pointer to the DPT_Start to copy out to
 * If NULL, won't copy but return the global variable if available
 * @return pointer to the (copied) DPT_Start <br>
 * If out is NULL then returns the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 *
 * Example of how to use:
 * ~~~{.c}
 * DPT_Start my_var;
 * if (app_get_DPT_Start_variable("/some/url", &my_var) == NULL) {
 *   //Something went wrong, maybe print an error message.
 *   return; 
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Start* app_get_DPT_Start_variable(const char *url, DPT_Start* out);

/**
 * @ingroup DPT_Start
 * @brief Get a DPT_Start array
 * 
 * @param[in] url the url for the DPT_Start to get
 * @param[out] out a pointer to the DPT_Start to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] n number of DPT_Start elements to copy
 * @return pointer to the (copied) DPT_Start <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * if (app_get_DPT_Start_variable("/some/url", my_arr, 5) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
const DPT_Start* app_get_DPT_Start_array(const char *url, DPT_Start* out, int n);


/**
 * @ingroup DPT_Start
 * @brief Get a DPT_Start as string
 * 
 * @param[in] in the data type
 * @param[in] text, reserved space to copy the generated text too
 * @param[in] size size of the allocated text
 * 
 * ~~~{.c}
 * DPT_Start my_type;
 * char my_text[100];
 * if (app_sprintf_DPT_Start(my_type, my_text, 100) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // printf(my_text);
 * ~~~
 */
int app_sprintf_DPT_Start(const DPT_Start *in, char* text, int size);


/**
 * @ingroup DPT_Start
 * @brief Get a DPT_Start from a string
 * 
 * @param[in] in the data type
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Start my_type;
 * char my_text[100];
 * if (app_sscanf_DPT_Start(&my_type, my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_type
 * ~~~
 */
int app_sscanf_DPT_Start(DPT_Start *in, char* text);

/**
 * @ingroup DPT_Start
 * @brief Get an example of DPT_Start in a string
 * 
 * @param[in] select 1 = format, 2 = example
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Start my_type;
 * char my_text[100];
 * if (app_str_expected_DPT_Start(1 , my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * printf(my_text);
 * ~~~
 */
int app_str_expected_DPT_Start(int select, char* text);

/**
 * @ingroup DPT_Start
 * @brief Get a DPT_Start array
 * 
 * @param[in] url the url for the DPT_Start to get
 * @param[out] out a pointer to the DPT_Start to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] start starting index in global array to copy
 * @param[in] n number of DPT_Start elements to copy
 * @return pointer to the (copied) DPT_Start <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Start my_var;
 * // Gets 1 element at index 5 in the array for "/some/url"
 * if (app_get_DPT_Start_variable("/some/url", &my_var, 5, 1) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Start* app_get_DPT_Start_array_elems(const char *url, DPT_Start* out, int start, int n);

/**
 * @ingroup DPT_Start
 * @brief Parse a DPT_Start from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Start to store into
 * @return true: successfully parsed DPT_Start
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Start my_var;
 * if (oc_parse_DPT_Start(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
bool oc_parse_DPT_Start(oc_rep_t *rep, DPT_Start *out);

/**
 * @ingroup DPT_Start
 * @brief Parse a DPT_Start[] from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Start[] to store into
 * @param[in] n The number of elements
 * @return true: successfully parsed DPT_Start
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Start my_arr[5];
 * if (oc_parse_DPT_Start_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
bool oc_parse_DPT_Start_array(oc_rep_t *rep, DPT_Start *out, int n);

/**
 * @ingroup DPT_Start
 * @brief Encode a DPT_Start using oc_rep
 * 
 * @param[in] in The DPT_Start to encode
 * @param[is_metadata] is_metadata Whether function is called during metadata query handling
 *
 * ~~~{.c}
 * DPT_Start my_var;
 * app_get_DPT_Start_variable("/some/url", &my_var);
 * if (oc_encode_DPT_Start(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Start(const DPT_Start *in, bool is_metadata);

/**
 * @ingroup DPT_Start
 * @brief Encode a DPT_Start[] using oc_rep
 * 
 * @param[in] in The DPT_Start[] to encode
 * @param[in] n The number of elements
 *
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * app_get_DPT_Start_array("/some/url", my_arr, 5);
 * if (oc_encode_DPT_Start_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Start_array(const DPT_Start *in, int n);

/**
 * @ingroup DPT_Start
 * @brief Store a DPT_Start into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Start to store
 *
 * ~~~{.c}
 * DPT_Start my_var;
 * app_get_DPT_Start_variable("/some/url", &my_var);
 * persistent_storeDPT_Start("/some/other/url", &my_var);
 * ~~~
 */
void persistent_store_DPT_Start(const char *name, const DPT_Start *in);

/**
 * @ingroup DPT_Start
 * @brief Store a DPT_Start[] into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Start[] to store
 * @param[in] n number of elements to store
 *
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * app_get_DPT_Start_array("/some/url", my_var, 5);
 * persistent_storeDPT_Start_array("/some/other/url", my_arr, 5);
 * ~~~
 */
void persistent_store_DPT_Start_array(const char *name, const DPT_Start *in, int n);

/**
 * @ingroup DPT_Start
 * @brief Load a DPT_Start from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Start to load into
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Start my_var;
 * app_get_DPT_Start_variable("/some/url", &my_var);
 * if (persistent_loadDPT_Start("/some/other/url", &my_var) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Start(const char *name, DPT_Start *out);

/**
 * @ingroup DPT_Start
 * @brief Load a DPT_Start[] from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Start[] to load into
 * @param[in] n number of elements in array
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * app_get_DPT_Start_array("/some/url", my_arr, 5);
 * if (persistent_loadDPT_Start("/some/other/url", my_arr, 5) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Start_array(const char *name, DPT_Start *out, int n);
/**@}*/

/**
 * @name DPT_Uint_XY functions
 * getters/setters and other functions for DPT_Uint_XY
 */
/**@{*/
/**
 * @ingroup DPT_Uint_XY
 * @brief Checks if the url depicts a DPT_Uint_XY
 * 
 * @param[in] url the url 
 * @return true: url conveys a DPT_Uint_XY
 */
bool app_is_DPT_Uint_XY_url(const char* url);

/**
 * @ingroup DPT_Uint_XY
 * @brief Set a DPT_Uint_XY to the default value
 * 
 * @param[in] url the url for the DPT_Uint_XY to set
 * 
 * ~~~{.c}
 * app_set_DPT_Uint_XY_default_value("/some/url");
 * ~~~
 */
void app_set_DPT_Uint_XY_default_value(const char* url);

/**
 * @ingroup DPT_Uint_XY
 * @brief Set a DPT_Uint_XY
 * 
 * @param[in] url the url for the DPT_Uint_XY to set
 * @param[in] in a pointer to the DPT_Uint_XY to copy
 * Can be the global variable itself
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * // ...set my_var somehow
 * app_set_DPT_Uint_XY_variable("/some/url", &my_var);
 * ~~~
 */
void app_set_DPT_Uint_XY_variable(const char* url, const DPT_Uint_XY* in);

/**
 * @ingroup DPT_Uint_XY
 * @brief Set a DPT_Uint_XY array
 * If n would overflow the array then it is limited to the size
 * of the array variable
 * 
 * @param[in] url the url for the DPT_Uint_XY to set
 * @param[in] in a pointer to the DPT_Uint_XY to copy
 * Can be the global variable itself
 * @param[in] n number of elements in the array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Uint_XY_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Uint_XY_array(const char* url, const DPT_Uint_XY* in, int n, bool store_persistently);

/**
 * @ingroup DPT_Uint_XY
 * @brief Set part of a DPT_Uint_XY array
 * If n would overflow the array then it is limited to the size
 * of the array variable. If start > array size then nothing happens.
 * 
 * @param[in] url the url for the DPT_Uint_XY to set
 * @param[in] in a pointer to the DPT_Uint_XY to copy
 * Can be the global variable itself
 * @param[in] start starting index to write to array
 * @param[in] n number of elements to write to array
 * @param[in] store_persistently Whether or not the value should also be stored in persistent store.
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Uint_XY_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Uint_XY_array_elems(const char* url, const DPT_Uint_XY* in, int start, int n, bool store_persistently);

/**
 * @ingroup DPT_Uint_XY
 * @brief Get a DPT_Uint_XY array
 * 
 * @param[in] url the url for the DPT_Uint_XY to get
 * @param[out] out a pointer to the DPT_Uint_XY to copy out to
 * If NULL, won't copy but return the global variable if available
 * @return pointer to the (copied) DPT_Uint_XY <br>
 * If out is NULL then returns the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 *
 * Example of how to use:
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * if (app_get_DPT_Uint_XY_variable("/some/url", &my_var) == NULL) {
 *   //Something went wrong, maybe print an error message.
 *   return; 
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Uint_XY* app_get_DPT_Uint_XY_variable(const char *url, DPT_Uint_XY* out);

/**
 * @ingroup DPT_Uint_XY
 * @brief Get a DPT_Uint_XY array
 * 
 * @param[in] url the url for the DPT_Uint_XY to get
 * @param[out] out a pointer to the DPT_Uint_XY to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] n number of DPT_Uint_XY elements to copy
 * @return pointer to the (copied) DPT_Uint_XY <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * if (app_get_DPT_Uint_XY_variable("/some/url", my_arr, 5) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
const DPT_Uint_XY* app_get_DPT_Uint_XY_array(const char *url, DPT_Uint_XY* out, int n);


/**
 * @ingroup DPT_Uint_XY
 * @brief Get a DPT_Uint_XY as string
 * 
 * @param[in] in the data type
 * @param[in] text, reserved space to copy the generated text too
 * @param[in] size size of the allocated text
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_type;
 * char my_text[100];
 * if (app_sprintf_DPT_Uint_XY(my_type, my_text, 100) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // printf(my_text);
 * ~~~
 */
int app_sprintf_DPT_Uint_XY(const DPT_Uint_XY *in, char* text, int size);


/**
 * @ingroup DPT_Uint_XY
 * @brief Get a DPT_Uint_XY from a string
 * 
 * @param[in] in the data type
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_type;
 * char my_text[100];
 * if (app_sscanf_DPT_Uint_XY(&my_type, my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_type
 * ~~~
 */
int app_sscanf_DPT_Uint_XY(DPT_Uint_XY *in, char* text);

/**
 * @ingroup DPT_Uint_XY
 * @brief Get an example of DPT_Uint_XY in a string
 * 
 * @param[in] select 1 = format, 2 = example
 * @param[in] text, the input string
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_type;
 * char my_text[100];
 * if (app_str_expected_DPT_Uint_XY(1 , my_text) != 0) {
 *   //Something went wrong
 *   return;
 * }
 * printf(my_text);
 * ~~~
 */
int app_str_expected_DPT_Uint_XY(int select, char* text);

/**
 * @ingroup DPT_Uint_XY
 * @brief Get a DPT_Uint_XY array
 * 
 * @param[in] url the url for the DPT_Uint_XY to get
 * @param[out] out a pointer to the DPT_Uint_XY to copy out to
 * If NULL, won't copy but return the global variable
 * @param[in] start starting index in global array to copy
 * @param[in] n number of DPT_Uint_XY elements to copy
 * @return pointer to the (copied) DPT_Uint_XY <br>
 * If out is NULL then the global variable for url if not 
 * flash only for this datapoint/param <br>
 * If n is larger than the number of elements in the datatype, returns NULL <br>
 * If the variable is flash only then it will return out <br>
 * If out is provided then it will return it UNLESS an error occurred while 
 * retrieving the variable from flash, in which case it will return NULL. <br>
 * If an error occurs while getting the variable, returns NULL
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * // Gets 1 element at index 5 in the array for "/some/url"
 * if (app_get_DPT_Uint_XY_variable("/some/url", &my_var, 5, 1) == NULL) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
const DPT_Uint_XY* app_get_DPT_Uint_XY_array_elems(const char *url, DPT_Uint_XY* out, int start, int n);

/**
 * @ingroup DPT_Uint_XY
 * @brief Parse a DPT_Uint_XY from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Uint_XY to store into
 * @return true: successfully parsed DPT_Uint_XY
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Uint_XY my_var;
 * if (oc_parse_DPT_Uint_XY(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_var
 * ~~~
 */
bool oc_parse_DPT_Uint_XY(oc_rep_t *rep, DPT_Uint_XY *out);

/**
 * @ingroup DPT_Uint_XY
 * @brief Parse a DPT_Uint_XY[] from oc_rep_t
 * 
 * @param[in] rep The oc_rep_t* from which to parse
 * @param[out] out The DPT_Uint_XY[] to store into
 * @param[in] n The number of elements
 * @return true: successfully parsed DPT_Uint_XY
 *
 * ~~~{.c}
 * //get an oc_rep_t to parse
 * //oc_rep_t *rep;
 * DPT_Uint_XY my_arr[5];
 * if (oc_parse_DPT_Uint_XY_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * // do something with my_arr
 * ~~~
 */
bool oc_parse_DPT_Uint_XY_array(oc_rep_t *rep, DPT_Uint_XY *out, int n);

/**
 * @ingroup DPT_Uint_XY
 * @brief Encode a DPT_Uint_XY using oc_rep
 * 
 * @param[in] in The DPT_Uint_XY to encode
 * @param[is_metadata] is_metadata Whether function is called during metadata query handling
 *
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * app_get_DPT_Uint_XY_variable("/some/url", &my_var);
 * if (oc_encode_DPT_Uint_XY(rep, &my_var) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Uint_XY(const DPT_Uint_XY *in, bool is_metadata);

/**
 * @ingroup DPT_Uint_XY
 * @brief Encode a DPT_Uint_XY[] using oc_rep
 * 
 * @param[in] in The DPT_Uint_XY[] to encode
 * @param[in] n The number of elements
 *
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * app_get_DPT_Uint_XY_array("/some/url", my_arr, 5);
 * if (oc_encode_DPT_Uint_XY_array(rep, my_arr, 5) == false) {
 *   //Something went wrong
 *   return;
 * }
 * size_t size = oc_rep_get_encoded_payload_size();
 * uint8_t buf = oc_rep_get_encoder_buf();
 * ~~~
 */
void oc_encode_DPT_Uint_XY_array(const DPT_Uint_XY *in, int n);

/**
 * @ingroup DPT_Uint_XY
 * @brief Store a DPT_Uint_XY into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Uint_XY to store
 *
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * app_get_DPT_Uint_XY_variable("/some/url", &my_var);
 * persistent_storeDPT_Uint_XY("/some/other/url", &my_var);
 * ~~~
 */
void persistent_store_DPT_Uint_XY(const char *name, const DPT_Uint_XY *in);

/**
 * @ingroup DPT_Uint_XY
 * @brief Store a DPT_Uint_XY[] into persistent memory
 * 
 * @param[in] name name of entry to store
 * @param[in] in DPT_Uint_XY[] to store
 * @param[in] n number of elements to store
 *
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * app_get_DPT_Uint_XY_array("/some/url", my_var, 5);
 * persistent_storeDPT_Uint_XY_array("/some/other/url", my_arr, 5);
 * ~~~
 */
void persistent_store_DPT_Uint_XY_array(const char *name, const DPT_Uint_XY *in, int n);

/**
 * @ingroup DPT_Uint_XY
 * @brief Load a DPT_Uint_XY from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Uint_XY to load into
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * app_get_DPT_Uint_XY_variable("/some/url", &my_var);
 * if (persistent_loadDPT_Uint_XY("/some/other/url", &my_var) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Uint_XY(const char *name, DPT_Uint_XY *out);

/**
 * @ingroup DPT_Uint_XY
 * @brief Load a DPT_Uint_XY[] from persistent memory
 * 
 * @param[in] name name of entry to load
 * @param[out] out DPT_Uint_XY[] to load into
 * @param[in] n number of elements in array
 * @return true on success, false otherwise
 *
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * app_get_DPT_Uint_XY_array("/some/url", my_arr, 5);
 * if (persistent_loadDPT_Uint_XY("/some/other/url", my_arr, 5) == false) {
 *   //something went wrong
 *   return;
 * }
 * ~~~
 */
bool persistent_load_DPT_Uint_XY_array(const char *name, DPT_Uint_XY *out, int n);
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

void onReceivedShotStatus(const char *url);
void onReceivedShot(const char *url);
void onReceivedReady(const char *url);
void app_initialize();
void eink_load_screen(enum Screen screen_nr); 

#ifdef __cplusplus
}
#endif
