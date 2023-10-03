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
#define URL_SENDSHOT "/p/o_1_1" //!< URL define for SendShot
#define URL_RECEIVESHOT "/p/o_1_2" //!< URL define for ReceiveShot
#define URL_SENDSHOTSTATUS "/p/o_1_3" //!< URL define for SendShotStatus
#define URL_RECEIVESHOTSTATUS "/p/o_1_4" //!< URL define for ReceiveShotStatus
#define URL_SENDREADY "/p/o_1_5" //!< URL define for SendReady
#define URL_RECEIVEREADY "/p/o_1_6" //!< URL define for ReceiveReady
#define URL_STARTING_PLAYER "/p/p_1_1" //!< URL define for Starting_Player 

///@defgroup DPT_Param_Bool
///@ingroup DPT_Param_Bool
typedef 
enum DPT_Param_Bool_ValueBool {
  _DPST600013_F11 = 0,
  _ValueBool_False = 0,
  DPT_Param_Bool_ValueBool_False = 0,
  _DPST600013_F12 = 1,
  _ValueBool_True = 1,
  DPT_Param_Bool_ValueBool_True = 1,
} DPT_Param_Bool;

///@defgroup DPT_Shot_Status

enum DPT_Shot_Status_ShipType {
  _DPST600041_F31 = 0,
  _ShipType_NoHit = 0,
  DPT_Shot_Status_ShipType_NoHit = 0,
  _DPST600041_F32 = 1,
  _ShipType_Destroyer = 1,
  DPT_Shot_Status_ShipType_Destroyer = 1,
  _DPST600041_F33 = 2,
  _ShipType_Submarine = 2,
  DPT_Shot_Status_ShipType_Submarine = 2,
  _DPST600041_F34 = 3,
  _ShipType_Cruiser = 3,
  DPT_Shot_Status_ShipType_Cruiser = 3,
  _DPST600041_F35 = 4,
  _ShipType_Battleship = 4,
  DPT_Shot_Status_ShipType_Battleship = 4,
  _DPST600041_F36 = 5,
  _ShipType_Carrier = 5,
  DPT_Shot_Status_ShipType_Carrier = 5,
}; 

/**
 * @ingroup DPT_Shot_Status
 * DPT_Shot_Status struct with member fields
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
    bool _Hit; //!< DPST-60004-1_F-1 member alias Hit for DPT_Shot_Status
    bool _DPST600041_F1; //!< DPST-60004-1_F-1 member alias DPST-60004-1_F-1 for DPT_Shot_Status
    bool _F1; //!< DPST-60004-1_F-1 member alias F-1 for DPT_Shot_Status
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
    bool _Sunk; //!< DPST-60004-1_F-2 member alias Sunk for DPT_Shot_Status
    bool _DPST600041_F2; //!< DPST-60004-1_F-2 member alias DPST-60004-1_F-2 for DPT_Shot_Status
    bool _F2; //!< DPST-60004-1_F-2 member alias F-2 for DPT_Shot_Status
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
    enum DPT_Shot_Status_ShipType _ShipType:3; //!< DPST-60004-1_F-3 member alias ShipType for DPT_Shot_Status
    enum DPT_Shot_Status_ShipType _DPST600041_F3:3; //!< DPST-60004-1_F-3 member alias DPST-60004-1_F-3 for DPT_Shot_Status
    enum DPT_Shot_Status_ShipType _F3:3; //!< DPST-60004-1_F-3 member alias F-3 for DPT_Shot_Status
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
 * DPT_Uint_XY struct with member fields
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
    unsigned int _X:8; //!< DPST-60009-1_F-1 member alias X for DPT_Uint_XY
    unsigned int _DPST600091_F1:8; //!< DPST-60009-1_F-1 member alias DPST-60009-1_F-1 for DPT_Uint_XY
    unsigned int _F1:8; //!< DPST-60009-1_F-1 member alias F-1 for DPT_Uint_XY
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
    unsigned int _Y:8; //!< DPST-60009-1_F-2 member alias Y for DPT_Uint_XY
    unsigned int _DPST600091_F2:8; //!< DPST-60009-1_F-2 member alias DPST-60009-1_F-2 for DPT_Uint_XY
    unsigned int _F2:8; //!< DPST-60009-1_F-2 member alias F-2 for DPT_Uint_XY
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
#define CONVENIENCE_PADDING_FOR_SCROLLING 3
#define INITIAL_SCREEN_TIMEOUT 0

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
  GOT_SCREEN = 3,
  GRT_SCREEN = 4,
  GPT_SCREEN = 5,
  AUTH_SCREEN = 6,
  DEV_SCREEN = 7,
  RESET_SCREEN = 8,
  ROLE_SCREEN = 9,
  TABLES_SCREEN = 10, 
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

/**
 * @brief Draw the nice screen header and frame.
 *
 * @param screen Screen number for the header
 */
void screen_header_draw(enum Screen);

/**
 * @brief Draw a scrollable menu and highlight
 * the selected item.
 */
void draw_scrollable_menu(const struct menu_t *menu, int numentries);
void menu_next();
void menu_prev();
void menu_select();

/**
 * @name QRcode 
 * QRcode struct and register/deregister functions
 */
/**@{*/

struct qr_code_t {
  struct qr_code_t *next;
  const char *str_data;
  const char *desc;
};

/**
 * @brief Register a QR code to be displayed on the
 * QR code page of the EINK interface.
 * @note This function retains the pointer to qrcode
 * beyond the end of the function so these MUST be malloc'd or
 * global scope/lifetime.
 *
 * @param qrcode QR code structure to be registered
 *
 * ~~~{.c}
 * void my_init() {
 *   // this function is called ONCE at startup
 *   static struct qr_code_t my_qrcode;
 *   my_qrcode.desc = "My QR code";
 *   my_qrcode.str_data = "Data to embed in QR code";
 *   register_qr_code(&my_qrcode);
 * }
 * ~~~
 */
void register_qr_code(struct qr_code_t *qrcode);
/**
 * @brief De-register a QR code from the eink interface
 *
 * @param qrcode QR code to deregister.
 */
void deregister_qr_code(struct qr_code_t *qrcode);
/**
 * @brief Check if a QR code is registered to the eink interface
 *
 * @param qrcode QR code to check if registered
 */
bool qr_code_is_registered(struct qr_code_t *qrcode);
/**
 * @brief Get the number of QR codes registered
 *
 * @return number of QR codes registered
 */
uint32_t num_qr_codes();
/**@}*/


/**
 * @name Reset 
 * reset struct and register/deregister functions
 */
/**@{*/
struct reset_t {
  struct reset_t *next;
  const char *desc;
  void (*do_reset)(void);
};

/**
 * @brief Register a reset to be displayed on the
 * reset page of the EINK interface.
 * @note This function retains the pointer to reset
 * beyond the end of the function so these MUST be malloc'd or
 * global scope/lifetime.
 *
 * @param reset QR code structure to be registered
 *
 * ~~~{.c}
 * void my_reset_cb() {
 *   //do some kind of platform specific reset?
 * }
 * void my_init() {
 *   // this function is called ONCE at startup
 *   static struct reset_t my_reset;
 *   my_reset.desc = "My Reset";
 *   my_reset.do_reset = &my_reset_cb;
 *   register_reset(&my_reset);
 * }
 * ~~~
 */
void register_reset(struct reset_t *reset);
/**
 * @brief De-register a reset from the eink interface
 *
 * @param reset reset to deregister.
 */
void deregister_reset(struct reset_t *reset);
/**
 * @brief Check if a reset is registered to the eink interface
 *
 * @param reset reset to check if registered
 */
bool reset_is_registered(struct reset_t *reset);
/**
 * @brief Get the number of resets registered
 *
 * @return number of resets registered
 */
uint32_t num_resets();
/**@}*/

extern bool g_clean_redraw;

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
 * @brief Load the builtin GOT_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_GOT_SCREEN() { set_screen(GOT_SCREEN); }
/**
 * @brief Load the builtin GRT_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_GRT_SCREEN() { set_screen(GRT_SCREEN); }
/**
 * @brief Load the builtin GPT_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_GPT_SCREEN() { set_screen(GPT_SCREEN); }
/**
 * @brief Load the builtin AUTH_SCREEN screen immediately
 * Can be used as an eink screen event handler.
 */
inline static void go_AUTH_SCREEN() { set_screen(AUTH_SCREEN); }
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
 * Callback invoked by the stack when a successfull put is done
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
 * Callback invoked by the stack when a successfull put is done
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
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Param_Bool_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Param_Bool_array(const char* url, const DPT_Param_Bool* in, int n);

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
 * 
 * ~~~{.c}
 * DPT_Param_Bool my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Param_Bool_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Param_Bool_array_elems(const char* url, const DPT_Param_Bool* in, int start, int n);

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
const volatile DPT_Param_Bool* app_get_DPT_Param_Bool_variable(const char *url, DPT_Param_Bool* out);

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
const volatile DPT_Param_Bool* app_get_DPT_Param_Bool_array(const char *url, DPT_Param_Bool* out, int n);

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
const volatile DPT_Param_Bool* app_get_DPT_Param_Bool_array_elems(const char *url, DPT_Param_Bool* out, int start, int n);

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
void oc_encode_DPT_Param_Bool(const DPT_Param_Bool *in);

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
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Shot_Status_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Shot_Status_array(const char* url, const DPT_Shot_Status* in, int n);

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
 * 
 * ~~~{.c}
 * DPT_Shot_Status my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Shot_Status_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Shot_Status_array_elems(const char* url, const DPT_Shot_Status* in, int start, int n);

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
const volatile DPT_Shot_Status* app_get_DPT_Shot_Status_variable(const char *url, DPT_Shot_Status* out);

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
const volatile DPT_Shot_Status* app_get_DPT_Shot_Status_array(const char *url, DPT_Shot_Status* out, int n);

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
const volatile DPT_Shot_Status* app_get_DPT_Shot_Status_array_elems(const char *url, DPT_Shot_Status* out, int start, int n);

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
void oc_encode_DPT_Shot_Status(const DPT_Shot_Status *in);

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
 * 
 * ~~~{.c}
 * DPT_Start my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Start_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Start_array(const char* url, const DPT_Start* in, int n);

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
 * 
 * ~~~{.c}
 * DPT_Start my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Start_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Start_array_elems(const char* url, const DPT_Start* in, int start, int n);

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
const volatile DPT_Start* app_get_DPT_Start_variable(const char *url, DPT_Start* out);

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
const volatile DPT_Start* app_get_DPT_Start_array(const char *url, DPT_Start* out, int n);

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
const volatile DPT_Start* app_get_DPT_Start_array_elems(const char *url, DPT_Start* out, int start, int n);

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
void oc_encode_DPT_Start(const DPT_Start *in);

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
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_arr[5];
 * // ...set my_arr somehow
 * app_set_DPT_Uint_XY_array("/some/url", my_arr, 5);
 * ~~~
 */
void app_set_DPT_Uint_XY_array(const char* url, const DPT_Uint_XY* in, int n);

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
 * 
 * ~~~{.c}
 * DPT_Uint_XY my_var;
 * // ...set my_var somehow
 * // sets 1 element at index 5 in the array for "/some/url"
 * app_set_DPT_Uint_XY_array("/some/url", &my_var, 5, 1);
 * ~~~
 */
void app_set_DPT_Uint_XY_array_elems(const char* url, const DPT_Uint_XY* in, int start, int n);

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
const volatile DPT_Uint_XY* app_get_DPT_Uint_XY_variable(const char *url, DPT_Uint_XY* out);

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
const volatile DPT_Uint_XY* app_get_DPT_Uint_XY_array(const char *url, DPT_Uint_XY* out, int n);

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
const volatile DPT_Uint_XY* app_get_DPT_Uint_XY_array_elems(const char *url, DPT_Uint_XY* out, int start, int n);

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
void oc_encode_DPT_Uint_XY(const DPT_Uint_XY *in);

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

#ifdef __cplusplus
}
#endif

