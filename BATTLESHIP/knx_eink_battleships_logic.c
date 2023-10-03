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

#include "knx_eink_battleships.h"
#include "cascoda-util/cascoda_tasklet.h"
#include "oc_core_res.h"
#include "api/oc_knx_dev.h"
#include "api/oc_knx_sec.h"
#include "api/oc_knx_fp.h"
#include "oc_knx.h"
///////////////////////////////////////////////////////////////////////////////
//               EINK code                                                   //
///////////////////////////////////////////////////////////////////////////////

#include "gfx_library.h"
#include "gfx_driver.h"
#include "knx_iot_image_1_54.h" // splash screen
#include "sif_ssd1681.h"


enum Screen g_screen_nr;
uint32_t g_qr;
bool g_eink_clean_redraw;
ca_tasklet screen_Tasklet; 

#define SCHEDULE_NOW 0



#include "cascoda-util/cascoda_tasklet.h"
#include "cascoda-util/cascoda_time.h"

// application specific
#include "api/oc_knx_dev.h"
#include "api/oc_knx_sec.h"
#include "gfx_driver.h"  // include  controller driver source code
#include "gfx_library.h" // include graphics library header

#include <math.h>
#include <stdbool.h>

#define SPI_NUM 1                    // SPI interface used for display
#define THIS_DEVICE 0
#define BTN_LONG_TIME_THRESHOLD 250   /* time threshold to trigger button long function */
#define BTN_HOLD_TIME_THRESHOLD 600   /* time threshold to trigger button hold function */
#define INITIAL_SCREEN_TIMEOUT 100     /* time (at startup) to poll when the device is active */
#define NEXT_SCREEN_TIMEOUT    0       /* time between next screens */

/**
 * @brief Ship orientation - 
 * Vertical/Horizontal
*/
enum ShipOrientation{
  VERTICAL = 0,
  HORIZONTAL = 1,
};

/**
 * @brief Player number - 
 * P1/P2/Observer
*/
enum PlayerNo{
  PLAYER1,
  PLAYER2,
  OBSERVER,
};

/**
 * @brief Hit state for cell
 * None: no shot on cell
 * Hit: cell shot and contained ship
 * Miss: cell shot but was empty
*/
enum HitState{
  HS_NONE,
  HS_HIT,
  HS_MISS,
};

/**
 * @brief Ship data struct
 * contains position x/y, length
 * boolean for if sunk, ship orientation.
*/
struct battleships_ship{
  uint8_t px, py;
  uint8_t length;
  bool sunk;
  enum ShipOrientation orientation;
};

/**
 * @brief board cell data struct
 * Hitstate for hit/miss/none,
 * occupy index (index into ships array
 * rather than using a large pointer)
*/
struct battleships_cell
{
  enum HitState hitstate:3;
  int occupy_index:5;
  // struct battleships_ship *occupy;
};

/** 
 * @brief battleships game board
 * contains 10x10 cells
*/
struct battleships_board
{
  struct battleships_cell cells[10][10];
};

/**
 * @brief battleships game state struct
 * contains boards, ships, current player,
 * my player, target x/y and current ship
*/
struct battleships_game
{
  struct battleships_board boards[2];
  struct battleships_ship ships[2][5];
  enum PlayerNo currentPlayerNo;
  enum PlayerNo myPlayerNo;
  int8_t tx, ty; //target x/y positions
  int8_t currentShip;
};

/**
 * @brief Global battleships game state
*/
struct battleships_game g_game;

/**
 * @brief settings used to change drawing behaviour
*/
enum GraphicsSettings
{
  GRID_COUNT = 10,
  GRID_SIZE = 9,
  CELL_SIZE = 8,
  GRID_OX = 3,
  GRID_OY = 5,
  CELL_OFF = 1,
};

// ===============================
// GLOBAL VARIABLE DEFINITIONS
// ===============================
// Tasklet used sending out the s-mode message
static ca_tasklet g_send_s_mode;

// Flags for sensor readings ready
static int g_wait_time = 0;

// application generic
// ============================
// FUNCTIONS
// ============================

/**
 * @brief length of each ship
*/
const uint8_t ship_lens[] = {5, 4, 3, 3, 2};

/**
 * @brief Get the ship* given an integer index
 * 
 * @param idx integer idx as stored in each cell
 * @return pointer to the occupying ship (if present)
*/
struct battleships_ship* get_occupy_from_idx(int idx) {
  if (idx%6 == 0)
    return NULL;
  return &g_game.ships[idx/6][(idx%6)-1];
}

/**
 * @brief Get the ship integer index given a ship*
 * 
 * @param ship Ship to get the index of
 * @return index of ship
 * 
 * @note WILL RETURN AN INVALID INDEX IF SHIP IS NOT
 * PART OF THE GAME!
*/
int get_occupy_idx_from_ship(struct battleships_ship *ship) {
  if (ship == NULL)
    return 0;
  return 1 + (ship - *g_game.ships);
}

/**
 * @brief Fire a shot at the opponent. 
 * Targeted square is the location of the shot.
 * Will fire the shot even if there is a shot already
 * there.
*/
static void game_fire_shot()
{
  if(g_game.currentPlayerNo != g_game.myPlayerNo)
    return;
  DPT_Uint_XY shot_pos;
  shot_pos._X = g_game.tx;
  shot_pos._Y = g_game.ty;
  app_set_DPT_Uint_XY_variable(URL_SENDSHOT, &shot_pos);
  oc_do_s_mode_with_scope(5, URL_SENDSHOT, "w");
}

/**
 * @brief Move the target square by {dx,dy}
 * Will wrap around the edge of the board
 * 
 * @param dx delta x to move by
 * @param dy delta y to move by
*/
static void game_move_target(int dx, int dy)
{
  g_game.tx = (g_game.tx + dx + 10) % 10;
  g_game.ty = (g_game.ty + dy + 10) % 10;
}

/**
 * @brief Move the current ship when placing
 * Ship is moved by {dx, dy} and will wrap around
 * the edge of the board. Wrapping takes into account
 * ship orientation AND length so the ship will never
 * be partially off the board
 * 
 * @param dx delta x to move ship by
 * @param dy delta y to move ship by 
 */
static void game_move_current_ship(int dx, int dy)
{
  struct battleships_ship *ship =
    &g_game.ships[g_game.myPlayerNo][g_game.currentShip];
  uint8_t px = ship->px;
  uint8_t py = ship->py;
  uint8_t mx = 10 - (ship->orientation == VERTICAL?0:ship->length-1);
  uint8_t my = 10 - (ship->orientation == HORIZONTAL?0:ship->length-1);
  px = (px + dx + mx) % mx;
  py = (py + dy + my) % my;
  ship->px = px;
  ship->py = py;
}

/**
 * @brief perform movement in the game for the current
 * context. i.e. moves the current ship if placing ships, 
 * moves the target if targeting opponent square.
 * 
 * @param dx delta x to move by
 * @param dy delta y to move by 
*/
static void game_move(int dx, int dy)
{
  if (g_screen_nr == FIRE_SHOT)
    game_move_target(dx, dy);
  if (g_screen_nr == PLACE_SHIPS)
    game_move_current_ship(dx, dy);
  refresh_screen(false);
}

/**
 * @brief Perform upwards movement in the game for
 * the current context. <br>
 * Can be used as a button handler.
*/
static void game_move_up()
{ game_move(0, -1); }

/**
 * @brief Perform downwards movement in the game
 * for the current context. <br>
 * Can be used as a button handler.
*/
static void game_move_down()
{ game_move(0, 1); }

/**
 * @brief Perform leftwards movement in the game 
 * for the current context. <br>
 * Can be used as a button handler.
*/
static void game_move_left()
{ game_move(-1, 0); }

/**
 * @brief Perform rightwards movement in the game
 * for the current context. <br>
 * Can be used as a button handler.
*/
static void game_move_right()
{ game_move(1, 0); }

/**
 * @brief Place the current ship in the game. <br>
 * Can be used as a button handler.
*/
static void game_place_current_ship()
{
  if(g_game.currentShip >= 0){
    struct battleships_ship *ship =
      &g_game.ships[g_game.myPlayerNo][g_game.currentShip];
    struct battleships_board *board = &g_game.boards[g_game.myPlayerNo];
    for(int i = 0; i < ship->length; i++){
      uint8_t sx = ship->px + i*(ship->orientation==HORIZONTAL?1:0);
      uint8_t sy = ship->py + i*(ship->orientation==VERTICAL?1:0);
      if (get_occupy_from_idx(board->cells[sx][sy].occupy_index) != NULL)
        return;
    }
    for(int i = 0; i < ship->length; i++){
      uint8_t sx = ship->px + i*(ship->orientation==HORIZONTAL?1:0);
      uint8_t sy = ship->py + i*(ship->orientation==VERTICAL?1:0);
      board->cells[sx][sy].occupy_index = get_occupy_idx_from_ship(ship);
    }

  }
  g_game.currentShip++;
  if(g_game.currentShip == 5){
    next_screen(true);
    return;
  }
  struct battleships_ship *ship =
    &g_game.ships[g_game.myPlayerNo][g_game.currentShip];
  ship->length = ship_lens[g_game.currentShip];
  refresh_screen(false);
  
}

/**
 * @brief Rotate the current ship in the game. If
 * the rotation would cause the ship to be off the 
 * edge of the board the ship is moved to the opposite
 * side of the board (wrapped around). <br>
 * Can be used as a button handler.
*/
static void game_rotate_current_ship()
{
  struct battleships_ship *ship =
    &g_game.ships[g_game.myPlayerNo][g_game.currentShip];
  if (ship->orientation == VERTICAL)
    ship->orientation = HORIZONTAL;
  else if (ship->orientation == HORIZONTAL)
    ship->orientation = VERTICAL;
  game_move_current_ship(0,0);
  refresh_screen(false);
}

/**
 * @brief Draw the game grid 
*/
static void game_draw_grid()
{
  int ox = GRID_OX;
  int oy = GRID_OY;
  // for(int i = 0; i < 10; i++){
  //   display_drawLine(i*CELL_SIZE + ox                , oy,
  //                    i*CELL_SIZE + ox                , oy + GRID_COUNT * CELL_SIZE - 1, BLACK);
  //   display_drawLine(i*CELL_SIZE + ox + CELL_SIZE - 1, oy,
  //                    i*CELL_SIZE + ox + CELL_SIZE - 1, oy + GRID_COUNT * CELL_SIZE - 1, BLACK);
  //   display_drawLine(ox                            , i*CELL_SIZE + oy                ,
  //                    ox + GRID_COUNT * CELL_SIZE - 2, i*CELL_SIZE + oy                , BLACK);
  //   display_drawLine(ox                            , i*CELL_SIZE + oy + CELL_SIZE - 1,
  //                    ox + GRID_COUNT * CELL_SIZE - 2, i*CELL_SIZE + oy + CELL_SIZE - 1, BLACK);
  // }
  for(int i = 0; i <= 10; i++){
    display_drawLine(i*GRID_SIZE + ox                , oy,
                     i*GRID_SIZE + ox                , oy + GRID_COUNT * GRID_SIZE - 1, BLACK);
    display_drawLine(ox                            , i*GRID_SIZE + oy                ,
                     ox + GRID_COUNT * GRID_SIZE - 2, i*GRID_SIZE + oy                , BLACK);
  }
}

/**
 * @brief Draw all fired shots to the board and
 * whether they were hits or misses.
 * 
 * @param idx Player number to draw shots for
*/
static void game_draw_shots(int idx)
{
  int ox = GRID_OX;
  int oy = GRID_OY;
  for(uint8_t x = 0; x < 10; x++){
    for(uint8_t y = 0; y < 10; y++){
      struct battleships_cell *cell =
        &g_game.boards[idx].cells[x][y];
      enum HitState hs = cell->hitstate;
      if (hs != HS_NONE){
        display_fillRoundRect(ox + CELL_OFF + 1 + x*GRID_SIZE,
                              oy + CELL_OFF + 1 + y*GRID_SIZE,
                              CELL_SIZE-2, CELL_SIZE-3,
                              2+CELL_SIZE%2, BLACK);
      }
      if (hs == HS_MISS) {
        display_fillRoundRect(ox + CELL_OFF + 2 + x*GRID_SIZE,
                              oy + CELL_OFF + 2 + y*GRID_SIZE,
                              CELL_SIZE-4, CELL_SIZE-5,
                              1+CELL_SIZE%2, WHITE);
      }
    }
  }
}

/** 
 * @brief Draw all (known) ships to the board
 * and fill in sunk ships.
 * 
 * @param idx Player number to draw ships for
*/
static void game_draw_ships(int idx)
{
  int ox = GRID_OX;
  int oy = GRID_OY;
  for(int i = 0; i < 5; i++){
    struct battleships_ship *ship = &g_game.ships[idx][i];
    if (ship->length == 0)
      continue;
    uint8_t sx = ship->px * GRID_SIZE + ox + CELL_OFF + 1;
    uint8_t sy = ship->py * GRID_SIZE + oy + CELL_OFF + 1;
    uint8_t w = 6 + ((ship->orientation == VERTICAL) ? 0 : (ship->length-1)*GRID_SIZE);
    uint8_t h = 5 + ((ship->orientation == HORIZONTAL) ? 0 : (ship->length-1)*GRID_SIZE);
    display_fillRoundRect(sx, sy, w, h, 2, BLACK);
    if (!ship->sunk)
      display_fillRoundRect(sx+1, sy+1, w-2, h-2, 1, WHITE);
  }
}

/**
 * @brief Perform game setup, including
 * initialisation of yours and opponents ships,
 * initialisation of each board cell for both players,
 * and setting of the game state in preparation 
 * for a new game.
*/
static void game_setup()
{
  
  for(int i = 0; i < 5; i++){
    g_game.ships[PLAYER1][i].length = 0;
    g_game.ships[PLAYER2][i].length = 0;
  }
  for(int x = 0; x < 10; x++) {
    for(int y = 0; y < 10; y++) {
      g_game.boards[PLAYER1].cells[x][y].hitstate = HS_NONE;
      g_game.boards[PLAYER1].cells[x][y].occupy_index = 0;
      g_game.boards[PLAYER2].cells[x][y].hitstate = HS_NONE;
      g_game.boards[PLAYER2].cells[x][y].occupy_index = 0;
    }
  }
  g_game.currentShip = -1;
  g_game.currentPlayerNo = g_game.myPlayerNo = PLAYER1;
  game_place_current_ship();
#if BATTLESHIPS_AUTOPLACE

#endif
}

/**
 * @brief Get the opposite player
 * 
 * @param p Player to get the opposite of
 * @returns Opposite player for p1/p2, 
 * or OBSERVER if p is OBSERVER
*/
enum PlayerNo swapPlayers(enum PlayerNo p)
{
  switch (p)
  {
  case PLAYER1: return PLAYER2;
  case PLAYER2: return PLAYER1;
  case OBSERVER: return OBSERVER;
  }
}

/**
 * @brief Loads the READY_WAIT Screen to be drawn
 * 
 * @returns true (display buffer filled, please draw me)
*/
static bool load_screen_ready_wait()
{
  char buf[32];
  snprintf(buf, 32, " opponent (%ds)", g_wait_time);
  g_wait_time+=10;
  display_setCursor(0,0);
  display_puts("  Waiting for   ");
  display_setCursor(0,9);
  display_puts(buf);
  display_setCursor(0, 40);
  display_puts("Hold (4) to exit");
  DPT_Start ready = false;
  app_set_DPT_Start_variable(URL_SENDREADY, &ready);
  oc_do_s_mode_with_scope(5, URL_SENDREADY, "w");
  TASKLET_ScheduleDelta(&screen_Tasklet, 10*1000, NULL);
  return true;
}

/**
 * @brief Stop waiting for opponent and reset game <br>
 * Is used as a button handler.
*/
void stop_waiting()
{
  if (g_screen_nr != READY_WAIT)
    return;
  TASKLET_Cancel(&screen_Tasklet);
  game_setup();
  go_first_screen();
}

/**
 * @brief Loads the PLACE_SHIPS screen <br>
 * This includes <br>
 * - draw the grid <br>
 * - draw ships <br>
 * 
 * @returns true (display buffer filled, please draw me)
*/
static bool load_place_ships()
{
  game_draw_grid();
  game_draw_ships(g_game.currentPlayerNo);
  return true;
}

/**
 * @brief Loads the FIRE_SHOT screen <br>
 * This includes <br>
 * - draw the grid <br>
 * - draw target (if current player is self) <br>
 * - draw ships <br>
 * - draw shots <br>
 * 
 * @returns true (display buffer filled, please draw me)
*/
static bool load_fire_shot()
{
  game_draw_grid();
  if (g_game.currentPlayerNo == g_game.myPlayerNo){
    display_fillRect(   GRID_OX + CELL_OFF + g_game.tx*GRID_SIZE,
                        GRID_OY + CELL_OFF + g_game.ty*GRID_SIZE,
                        CELL_SIZE, CELL_SIZE, BLACK);
  }
  game_draw_ships(swapPlayers(g_game.currentPlayerNo));
  game_draw_shots(swapPlayers(g_game.currentPlayerNo));
  return true;
}

/**
 * @brief Draw the devboard buttons on the
 * EINK display in the format <br>
 * (4) (3) (2) (x)
*/
static void draw_buttons()
{
  for (int i = 0; i < 4; i++) {
    display_drawCircle((1+2*i)*96/8, 7*96/8, 6, BLACK);
    display_setCursor((1+2*i)*96/8-2, 7*96/8-3);
    char s[2] = {};
    s[0] = '4'-i;
    display_puts(s);
  }
  display_drawLine((1+2*3)*96/8 - 7, 7*96/8 - 7,
                  (1+2*3)*96/8 + 7, 7*96/8 + 7, BLACK);
  display_drawLine((1+2*3)*96/8 + 7, 7*96/8 - 7,
                  (1+2*3)*96/8 - 7, 7*96/8 + 7, BLACK);
}


/**
 * @brief loads the GAME_INTRO screen
 * This includes a welcome message, as well
 * as buttons for controls help and start.
 * 
 * @returns true (display buffer filled, please redraw)
*/
static bool load_game_intro()
{
  display_clear();
  display_setTextSize(1);
  display_setCursor(16, 0);
  display_puts("Welcome to");
  display_setCursor(12, 10);
  display_puts("Battleships!");
  display_setCursor(12, 24);
  display_puts("(By Cascoda)");
  display_setCursor(0, 40);
  display_puts("Press 4 to start");
  display_setCursor(0, 50);
  display_puts("Press 3 for");
  display_setCursor(0, 60);
  display_puts("game controls");
  draw_buttons();
  return true;
}

/**
 * Start the game
*/
static void game_start()
{
  g_clean_redraw = true;
  set_screen(PLACE_SHIPS);
}

/**
 * @brief Loads the SHOT_INFO screen
 * Draw the shot info
 * This displays whether the shot was
 * a hit or a miss. If it was a hit
 * it also shows which ship was hit.
 * 
 * @returns true (display buffer filled, please redraw)
*/
static bool load_shot_info()
{
  display_clear();
  char *ship_names[] = {
    "Destroyer",
    "Submarine",
    "Cruiser",
    "Battleship",
    "Carrier",
  };

  DPT_Shot_Status *status = app_get_DPT_Shot_Status_variable(URL_RECEIVESHOTSTATUS, NULL);
  display_setCursor(0, 10);
  if (!status->_F1 /*hit*/) {
    display_puts("MISS");
  } else {
    display_puts(status->_F2?"SUNK":"HIT");
    display_setCursor(0, 20);
    display_puts(ship_names[status->_F3-1]);
  }
  display_setCursor(0, 40);
  display_puts("Press 4");
  display_setCursor(0, 50);
  display_puts("to continue");
  draw_buttons();
  return true;
}

static int g_controls_page = 0;
/**
 * @brief Draw the controls page
 * This page explains how to interact
 * with the game in order to play it
 * If the page is out of bounds then
 * this function will switch to the 
 * next or previous screens.
 * 
 * @returns true (display buffer filled, please redraw)
*/
static bool load_controls_1()
{
  if (g_controls_page == -1) {
    g_controls_page = 0;
    go_prev_screen();
    return false;
  }
  display_setCursor(0, 0);
  // display_setTextColor(WHITE)
  display_puts("-   Controls   -");
  display_setCursor(0, 60);
  display_puts("back - SP 4");
  if (g_controls_page < 2) {
    display_setCursor(0, 70);
    display_puts("next - SP 3");
  }
  draw_buttons();
  display_setCursor(0,10);
  switch (g_controls_page) {
  case 0: {
    display_puts("Definitions:    ");
    display_puts("LP long press   ");
    display_puts("SP short press  ");
    display_puts("HLD hold        ");
    break;
  }
  case 1: {
    display_puts("Movement:       ");
    display_puts("UP     - LP 3   ");
    display_puts("DOWN   - SP 3   ");
    display_puts("LEFT   - LP 2   ");
    display_puts("RIGHT  - SP 2   ");
    break;
  }
  case 2: {
    display_puts("Actions:        ");
    display_puts("PLACE  - LP 4   ");
    display_puts("ROTATE - SP 4   ");
    display_puts("FIRE   - SP 4   ");
    break;
  }
  }
  return true;
}

/**
 * @brief Go to the next controls page <br>
 * Is used as a button handler
*/
static void next_controls_page()
{
  g_controls_page++;
  refresh_screen(false);
}

/**
 * @brief Go to the previous controls page <br>
 * Is used as a button handler
*/
static void prev_controls_page()
{
  g_controls_page--;
  refresh_screen(false);
}

/**
 * @brief Loads the GAME_OVER screen
 * Draw the GAME OVER or YOU WIN
 * messages as the game is now 
 * finished.
 * 
 * @returns true (display buffer filled, please redraw)
*/
static bool load_game_over()
{
  draw_buttons();
  display_setCursor(0, 40);
  display_puts("Press 4 for");
  display_setCursor(0, 50);
  display_puts("new game!");
  display_setCursor(0,0);

  int i;
  struct battleships_ship *ships;
  //check if all our opponent's ships are sunk
  ships = g_game.ships[swapPlayers(g_game.myPlayerNo)];
  for (i = 0; i < 5; i++) {
    if (!ships[i].sunk)
      break;
  }

  if (i == 5) {
    display_puts("You win!");
    return true;
  }
  //check if all our opponent's ships are sunk
  ships = g_game.ships[g_game.myPlayerNo];
  for (i = 0; i < 5; i++) {
    if (!ships[i].sunk)
      break;
  }

  if (i == 5) {
    display_puts("You lose!");
    return true;
  }
  display_puts("ERROR!");
  return true;
}

// application initialisation
void app_initialize(void)
{

  game_setup();

}
static void init_globals()
{
  // Empty for now
}

/**
 * @brief Called when ShotStatus url is PUT
 * This handles updating self player with
 * information provided by the opponent about
 * the shot just taken.
*/
void onReceivedShotStatus(const char *url)
{
  if (strcmp(url, URL_RECEIVESHOTSTATUS) != 0)
    return;
  //can only recieve shotstatus when it is our turn
  if (g_game.currentPlayerNo != g_game.myPlayerNo)
    return;
  // current player is ourselves
  // put shot on opponent board
  //our turn is complete!
  enum PlayerNo player = swapPlayers(g_game.currentPlayerNo);
  DPT_Shot_Status *status = app_get_DPT_Shot_Status_variable(URL_RECEIVESHOTSTATUS, NULL);
  DPT_Uint_XY *shot = app_get_DPT_Uint_XY_variable(URL_SENDSHOT, NULL);
  struct battleships_board *board = &g_game.boards[player];
  struct battleships_cell *cell = &(board->cells[shot->_X][shot->_Y]);
  struct battleships_ship *ship = &g_game.ships[player][status->_F3-1];
  int ship_idx = get_occupy_idx_from_ship(ship);
  if (!status->_F1 /*hit*/) {
    cell->hitstate = HS_MISS;
  }else{
    cell->hitstate = HS_HIT;
    cell->occupy_index = ship_idx;
    if (status->_F2 /*sunk*/) {
      ship->sunk = true;
      ship->length = ship_lens[5-status->_F3];
      //locate the ship
      // it could be horizontal or vertical
      // we could be at the edge of it
      // if theres match at x-1, y then horiz
      int px;
      int py;
      px = shot->_X;
      py = shot->_Y;
      // check up/down
      if (   (py > 0 && board->cells[px][py-1].occupy_index == ship_idx)
          || (py < 9 && board->cells[px][py+1].occupy_index == ship_idx)) {
        ship->orientation = VERTICAL;
        while(py > 0 && board->cells[px][py-1].occupy_index == ship_idx)
          py--;
        ship->px = px;
        ship->py = py;
      } else
      // check left/right
      if (   (px > 0 && board->cells[px-1][py].occupy_index == ship_idx)
          || (px < 9 && board->cells[px+1][py].occupy_index == ship_idx)) {
        ship->orientation = HORIZONTAL;
        while(px > 0 && board->cells[px-1][py].occupy_index == ship_idx)
          px--;
        ship->px = px;
        ship->py = py;
      } else {
        //something went wrong!!
        PRINT_APP("BAD SHIP CALCULATION!\n");
      }
    }
  }

  //check if all our opponent's ships are sunk
  int i;
  struct battleships_ship *theirships = g_game.ships[swapPlayers(g_game.myPlayerNo)];
  for (i = 0; i < 5; i++) {
    if (!theirships[i].sunk)
      break;
  }
  
  if (i != 5) {
    //redraw
    g_clean_redraw = false;
    set_screen(SHOT_INFO);

    g_game.currentPlayerNo = player;
  } else {
    set_screen(GAME_OVER);
  }

}

/**
 * @brief Called when Shot url is PUT
 * This handles much of the game logic regarding
 * ship hits and notifying the opponent of what 
 * happened.
*/
void onReceivedShot(const char *url)
{
  if (strcmp(url, URL_RECEIVESHOT) != 0)
    return;
  //can only recieve shot when its not our turn
  if (g_game.currentPlayerNo == g_game.myPlayerNo)
    return;
  DPT_Uint_XY *shot = app_get_DPT_Uint_XY_variable(URL_RECEIVESHOT, NULL);
  // current player is opponent, 
  // place shot on our board
  enum PlayerNo player = swapPlayers(g_game.currentPlayerNo);
  struct battleships_board *board = &g_game.boards[player];
  struct battleships_cell *cell = &(board->cells[shot->_X][shot->_Y]);
  struct battleships_ship *ship = get_occupy_from_idx(cell->occupy_index);
  struct battleships_ship *ships = g_game.ships[player];

  //it's now our turn!

  DPT_Shot_Status *status = app_get_DPT_Shot_Status_variable(URL_SENDSHOTSTATUS, NULL);
  status->_F1 = false;
  status->_F2 = false;
  status->_F3 = _ShipType_NoHit;

  if(cell->hitstate == HS_HIT)
    return;

  if (ship) {
    cell->hitstate = HS_HIT;
    status->_F1 = true;
    status->_F3 = 5-(ship-ships);
    int i;
    for(i = 0; i < ship->length; i++){
      uint8_t sx = ship->px + i*((ship->orientation==HORIZONTAL)?1:0);
      uint8_t sy = ship->py + i*((ship->orientation==VERTICAL)?1:0);
      if (board->cells[sx][sy].hitstate != HS_HIT)
        break;
    }
    if (i == ship->length){
      status->_F2 = true; /*sunk*/
      ship->sunk = true;
    }
  }else{
    cell->hitstate = HS_MISS;
  }

  oc_do_s_mode_with_scope(5, URL_SENDSHOTSTATUS, "w");

  //check if all our ships are sunk
  int i;
  struct battleships_ship *myships = g_game.ships[g_game.myPlayerNo];
  for (i = 0; i < 5; i++) {
    if (!myships[i].sunk)
      break;
  }
  if (i != 5) {
    //redraw
    g_clean_redraw = false;
    eink_load_screen(FIRE_SHOT);

    g_game.currentPlayerNo = player;

    g_clean_redraw = false;
    TASKLET_ScheduleDelta(&screen_Tasklet, 2*1000, NULL);
  } else {
    set_screen(GAME_OVER);
  }
}

/**
 * @brief Called when a Ready packet is recieved
 * This is used to synchronize the two players and
 * start them at the same time once boards have
 * been setup.
*/
void onReceivedReady(const char *url)
{
  if (strcmp(url, URL_RECEIVEREADY) != 0)
    return;
  if (g_screen_nr < READY_WAIT){
    DPT_Start ready = false;
    app_set_DPT_Start_variable(URL_SENDREADY, &ready);
    return;
  }

  DPT_Start ready = *app_get_DPT_Start_variable(URL_RECEIVEREADY, NULL);
  if (ready == false){
    ready = true;
    app_set_DPT_Start_variable(URL_SENDREADY, &ready);
    oc_do_s_mode_with_scope(5, URL_SENDREADY, "w");
  }else if (g_screen_nr == READY_WAIT) {
    DPT_Param_Bool *start = app_get_DPT_Param_Bool_variable(URL_STARTING_PLAYER, NULL);
    if (*start)
      g_game.currentPlayerNo = PLAYER1;
    else
      g_game.currentPlayerNo = PLAYER2;
    go_next_screen();
  }
}



///////////////////////////////////////////////////////////////////////////////
//               BUTTON Functions                                            //
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//               EINK code                                                   //
///////////////////////////////////////////////////////////////////////////////

bool g_clean_redraw = true;

struct qr_code_t *g_qr_codes = NULL;
struct reset_t *g_resets = NULL;
int g_max_y_coord;
int g_scroll_position;

oc_auth_at_t *get_auth_at_entry(int idx)
{
  return oc_get_auth_at_entry(0, idx);
}

//////////////////////////////////////////////////////////////
//  EINK Scroll code
//////////////////////////////////////////////////////////////


// Updates the coordinates based on the scrolling position
static uint16_t scroll_adjust_coord(uint16_t num)
{
  // NOTE: For num < (LINE_HEIGHT * g_scroll_position), the return value
  // will be a very large positive value because of underflow of the uint.
  // This is completely fine, because there is a check at a later stage
  // (in function scroll_display_wrapper), which filters out based on
  // coordinates.
  return (num - (LINE_HEIGHT * g_scroll_position));
}

// Display a str at a given x and y coordinates. This function will
// automatically adjust the coordinates of the string to display it
// at the correct location on the screen based on the current scrolling
// position.
static void scroll_display_wrapper(uint16_t x, uint16_t y, char *str)
{
  // Keep track of lowest line that will be displayed
  if (y > g_max_y_coord)
    g_max_y_coord = y;

  uint16_t adjusted_y_coord = scroll_adjust_coord(y);
  // Only display if the text will not overlap the "title" section
  if (adjusted_y_coord >= TOP_MOST_LINE_Y_COORD && adjusted_y_coord <= BOTTOM_MOST_LINE_Y_COORD)
  {
    display_setCursor(x, adjusted_y_coord);
    display_puts(str);
  }
}

// Calculates how many lines to scroll
static uint16_t calculate_lines_to_scroll(void)
{
  if (g_max_y_coord > EINK_LINE_NO(8))
  return (((g_max_y_coord - EINK_LINE_NO(8)) / LINE_HEIGHT) + CONVENIENCE_PADDING_FOR_SCROLLING);
  else
  return 0;
}

// Returns true if a screen is Scrollable, false otherwise
static bool is_scrollable_screen(void)
{
  return g_max_y_coord > EINK_LINE_NO(8);
}

// Creates the scroll bar. Note: this function should be called after
// all calls to scroll_display_wrapper()
static void show_scroll_bar(void)
{
// Put all the stuff below in a function
#define SCALING_FACTOR 2
#define BOTTOM_OFFSET 3 //bottom-most y coordinate above the bottom border
#define TOP_OFFSET 13 //top-most y coordinate under the lower portion of the title border

  int lower_y_coord = display_getHeight() - BOTTOM_OFFSET - SCALING_FACTOR * calculate_lines_to_scroll();//- SCALING_FACTOR * calculate_lines_to_scroll();
  display_drawLine(1, TOP_OFFSET + SCALING_FACTOR * g_scroll_position, 1, lower_y_coord + SCALING_FACTOR * g_scroll_position, BLACK);

#undef SCALING_FACTOR
#undef BOTTOM_OFFSET
#undef TOP_OFFSET
}

void scroll_up()
{
  if (g_scroll_position > 0)
    --g_scroll_position;
  else
    g_scroll_position = calculate_lines_to_scroll();
  refresh_screen(false);
}

void scroll_down()
{
  if (g_scroll_position < calculate_lines_to_scroll())
    ++g_scroll_position;
  else
    g_scroll_position = 0;
  refresh_screen(false);
}

void eink_display()
{

}
static int g_selected_row;


void screen_header_draw(enum Screen nr)
{
  int display_width  = display_getWidth();
  int display_height = display_getHeight();
  char screen_str[32];
  int first_screen = TABLES_SCREEN+1;
  int num_screens = NUM_SCREENS-first_screen;
  int screen_nr = nr-first_screen+1;
  const char *title = screen_get_title(nr);
  sprintf((char *)&screen_str, "%d", screen_nr);
  // generic stuff
  display_drawRect(0, 0, display_width, display_width, BLACK);
  display_drawLine(0, 12, display_width, 12, BLACK);
  int width = ((float)display_width/(float)num_screens)*(screen_nr);
  display_drawLine(0, 11, width, 11, BLACK);
  display_drawLine(0, 10, width, 10, BLACK);
  display_drawLine(0, 12, display_width, 12, BLACK);
  if (title){
    display_setCursor(3, 2);
    snprintf(screen_str, 19, "%s", title);
  } else {
    display_setCursor(35, 2);
    snprintf(screen_str, 19, "%d/%d", screen_nr, num_screens);
  }
  display_puts(screen_str);
}

static const struct menu_t g_system_menu[] = {  
  {
    "QR Codes",
    &go_QR_SCREEN,
  }, 
  {
    "Group Objects",
    &go_GOT_SCREEN,
  }, 
  {
    "Recipients",
    &go_GRT_SCREEN,
  }, 
  {
    "Publisher",
    &go_GPT_SCREEN,
  }, 
  {
    "Security",
    &go_AUTH_SCREEN,
  }, 
  {
    "Device info",
    &go_DEV_SCREEN,
  }, 
  {
    "Reset",
    &go_RESET_SCREEN,
  }, 
  {
    "Role",
    &go_ROLE_SCREEN,
  }, 
  {
    "Tables",
    &go_TABLES_SCREEN,
  },  
};
const int system_menu_entries = 9;

struct menu_t *g_cur_menu;
int g_cur_menu_entries;

void draw_scrollable_menu(const struct menu_t* menu, int numentries)
{
  g_cur_menu = menu;
  g_cur_menu_entries = numentries;
  for (int i = 0; i < numentries; i++) {
    if (g_selected_row == i)
      display_setTextColor(WHITE, BLACK);
    else
      display_setTextColor(BLACK, WHITE);
    scroll_display_wrapper(3, EINK_LINE_NO(i+1), menu[i].text);
  }
  display_setTextColor(WHITE, BLACK);
}

bool load_menu_screen()
{
  screen_header_draw(MENU_SCREEN);
  draw_scrollable_menu(g_system_menu, system_menu_entries);
}

void menu_scroll()
{
  if ((g_selected_row - 8) >= g_scroll_position)
    g_scroll_position = g_selected_row-7;
  if ((g_selected_row) < g_scroll_position)
    g_scroll_position = g_selected_row;
}

void menu_next()
{
  g_selected_row++;
  if (g_selected_row > (g_cur_menu_entries))
    g_selected_row = 0;
  menu_scroll();
  refresh_screen(false);
}

void menu_prev()
{
  g_selected_row--;
  if (g_selected_row < 0)
    g_selected_row = (g_cur_menu_entries-1);
  menu_scroll();
  refresh_screen(false);
}

void menu_select()
{
  if (g_cur_menu == NULL)
    return;
  if (g_cur_menu[g_selected_row].onclick)
    g_cur_menu[g_selected_row].onclick();
}

bool load_dev_screen()
{
  screen_header_draw(DEV_SCREEN);
  oc_device_info_t *device = oc_core_get_device_info(THIS_DEVICE);
  char screen_str[20];
  if (device == NULL) {
    display_puts("Not inited");
    return true;  
  }
  char* hwt = oc_string(device->hwt );
  display_setCursor(3, EINK_LINE_NO(1));
  display_puts("LP4:Back SP4:PM");
  display_setCursor(2, EINK_LINE_NO(2));
  display_puts("IA:");
  snprintf(screen_str, 19, "%d",
      device->ia);
  display_puts(screen_str);
  display_setCursor(2, EINK_LINE_NO(3));
  display_puts("PM:");
  if (device->pm) {
    display_setTextColor(WHITE, BLACK);
    display_puts(" true ");
    display_setTextColor(BLACK, WHITE);
  } else {
    display_puts(" false ");
  }
  display_setCursor(2, EINK_LINE_NO(4));
  display_puts("HW:");
  snprintf(screen_str, 19, "%d %d %d",
      device->hwv.major,
      device->hwv.minor,
      device->hwv.patch);
  display_puts(screen_str);
  display_setCursor(2, EINK_LINE_NO(5));
  display_puts("SW:");
  snprintf(screen_str, 19, "%d %d %d",
      device->fwv.major,
      device->fwv.minor,
      device->fwv.patch);
  display_puts(screen_str);
  display_setCursor(2, EINK_LINE_NO(6));
  display_puts("Model:");
  display_puts(oc_string(device->model));
  display_setCursor(2, EINK_LINE_NO(7));
  display_puts("HWT:");
  display_puts(hwt);
  return true;
}

bool load_got_screen()
{
  screen_header_draw(GOT_SCREEN);
  oc_group_object_table_t *entry;
  uint8_t cur_line = 1;
  char c_flags_buffer[6];
  char screen_str[20];

  int max_entries = oc_core_get_group_object_table_total_size();
  for (int i = 0; i < max_entries; ++i)
  {
    entry = oc_core_get_group_object_table_entry(i);
    if (entry->ga_len <= 0) // There is no entry at that index
      break;

    // Print entry to the screen
    /*
    Entry %d (i + 1):
    - id:
    - href:
    - cflags: --cd-d
    - ga:
      .
      .
      .
    */
    snprintf(screen_str, 19, "==> Entry %d ", i + 1);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    snprintf(screen_str, 19, "-id: %d", entry->id);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    snprintf(screen_str, 19, "-href:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    snprintf(screen_str, 19, "  %s", oc_string_checked(entry->href));
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);

    c_flags_buffer[0] = '\0';
    oc_cflags_as_string(c_flags_buffer, entry->cflags);
    snprintf(screen_str, 19, "-cflags: %s", c_flags_buffer);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);

    snprintf(screen_str, 19, "-ga:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    for (int j = 0; j < entry->ga_len; ++j)
    {
      snprintf(screen_str, 19, "  %d", entry->ga[j]);
      scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    }
  }
  return true;
}

void load_grt_gpt_screen(int(*size_fn)(), void*(*entry_fn)(int))
{

  oc_group_rp_table_t *entry;
  uint8_t cur_line = 1;
  char screen_str[20];

  int max_entries = (*size_fn)();
  for (int i = 0; i < max_entries; ++i)
  {
    entry = (*entry_fn)(i);
    if (entry->ga_len <= 0) // There is no entry at that index
      break;

    // Print entry to the screen
    /*
    Entry %d (i + 1):
    - id:
    - iid:
    - grpid:
    - ga:
      .
      .
      .
    */
    snprintf(screen_str, 19, "==> Entry %d", i + 1);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    snprintf(screen_str, 19, "-id: %d", entry->id);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);

    // uint64_t decimal number has max 20 numbers + 1 for terminator
    char str[21];
    oc_conv_uint64_to_dec_string(str, entry->iid);
    snprintf(screen_str, 19, "-iid: %s", str);

    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    snprintf(screen_str, 19, "-grpid: %d", entry->grpid);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);

    snprintf(screen_str, 19, "-ga:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    for (int j = 0; j < entry->ga_len; ++j)
    {
      snprintf(screen_str, 19, "  %d", entry->ga[j]);
      scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    }
  }
}

bool load_grt_screen()
{
  screen_header_draw(GRT_SCREEN);
  load_grt_gpt_screen(&oc_core_get_recipient_table_size, 
                      &oc_core_get_recipient_table_entry);
  return true;
}

bool load_gpt_screen()
{
  screen_header_draw(GPT_SCREEN);
  load_grt_gpt_screen(&oc_core_get_publisher_table_size, 
                      &oc_core_get_publisher_table_entry);
  return true;
}

bool load_auth_screen()
{
  
  screen_header_draw(AUTH_SCREEN);
  oc_auth_at_t *entry;
  uint8_t cur_line = 1;
  char screen_str[20];

  int max_entries = oc_core_get_at_table_size();
  for (int i = 0; i < max_entries; ++i)
  {
    entry = get_auth_at_entry(i);
    if (oc_string_len(entry->id) == 0) // There is no entry at that index
      break;

    // Print entry to the screen
    /*
    Entry %d (i + 1):
    - id
    ...
    - osc_id
    ...
    - osc_rid
    ...
    - aud
    ...
    - ga:
      .
      .
    typedef struct oc_auth_at_t
    {
      oc_string_t id;            
      oc_interface_mask_t scope;
      oc_at_profile_t
        profile; 
      oc_string_t aud;         
      oc_string_t sub;         
      oc_string_t kid;         
      oc_string_t osc_version; 
      oc_string_t osc_ms;      
      uint8_t osc_hkdf;        
      uint8_t osc_alg;         
      oc_string_t osc_salt; 
      oc_string_t osc_contextid;
      oc_string_t osc_id; 
      oc_string_t osc_rid;   
      int nbf;     
      int ga_len;  
      int64_t *ga; 
    } oc_auth_at_t;
    */
    snprintf(screen_str, 19, "==> Entry %d", i + 1);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), "-id:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), oc_string(entry->id));
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), "-osc_id:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), oc_string(entry->osc_id));
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), "-osc_rid:");
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), oc_string(entry->osc_rid));
    scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), "-ga:");
    for (int j = 0; j < entry->ga_len; j++) {
      int64_t ga = entry->ga[j];
      snprintf(screen_str, 19, "  %d", ga);
      scroll_display_wrapper(3, EINK_LINE_NO(cur_line++), screen_str);
    }

  }
  return true;
}

bool load_tables_screen()
{
  screen_header_draw(TABLES_SCREEN);
  char screen_str[20];
  uint8_t num_entries = 0;
  uint8_t cur_line = 1;

  bool check_got_entry(oc_group_object_table_t *ga) { return oc_string_len(ga->href) != 0; }
  bool check_grt_gpt_entry(oc_group_rp_table_t *grp) { return grp->ga_len != 0; }
  bool check_sec_entry(oc_auth_at_t *at) { return oc_string_len(at->id) != 0; }

  struct table_fns_s{
    const char *fmt;
    int (*tbl_size_fn)(void);
    void *(*tbl_entry_fn)(int);
    bool (*checker)(void*)
  } table_fns[] = {
    {
      "Grp Obj Tbl: %d",
      &oc_core_get_group_object_table_total_size,
      &oc_core_get_group_object_table_entry,
      &check_got_entry
    }, {
      "Rec Tbl: %d",
      &oc_core_get_recipient_table_size,
      &oc_core_get_recipient_table_entry,
      &check_grt_gpt_entry
    }, {
      "Pub Tbl: %d",
      &oc_core_get_publisher_table_size,
      &oc_core_get_publisher_table_entry,
      &check_grt_gpt_entry
    }, {
      "Sec Tbl: %d",
      &oc_core_get_at_table_size,
      &get_auth_at_entry,
      &check_sec_entry
    }
  };

  const int num_tables = (sizeof(table_fns)/sizeof(table_fns[0])); 

  for (int i = 0; i < 4; i++) {
    struct table_fns_s *fns = &table_fns[i];
    display_setCursor(3, EINK_LINE_NO(cur_line++));
    num_entries = 0;
    for (uint8_t j = 0; j < (*fns->tbl_size_fn)(); ++j)
    {
      if ((*fns->checker)((*fns->tbl_entry_fn)(j)))
        ++num_entries;
    }
    snprintf(screen_str, 20, fns->fmt, num_entries);
    display_puts(screen_str);
  }

  return true;
}

bool load_reset_screen()
{  
  screen_header_draw(RESET_SCREEN);
  struct reset_t *it = g_resets;
  int i = 0;
  while(it) {
    if (g_selected_row == i)
      display_setTextColor(WHITE, BLACK);
    else
      display_setTextColor(BLACK, WHITE);
    scroll_display_wrapper(0, EINK_LINE_NO(i+1), it->desc);
    i++;
    it = it->next;
  }
  display_setTextColor(WHITE, BLACK);
  return true;
}

void reset_next()
{
  g_selected_row++;
  if (g_selected_row >= num_resets())
    g_selected_row = 0;
  menu_scroll();
  refresh_screen(false);
}

void reset_prev()
{
  g_selected_row--;
  if (g_selected_row < 0)
    g_selected_row = num_resets()-1;
  menu_scroll();
  refresh_screen(false);
}

void reset_select()
{
  int i = g_selected_row;
  struct reset_t *it = g_resets;
  while (i && it) {
    i--;
    it = it->next;
  }
  if (!it)
    return;
  (it->do_reset)();
  set_screen(MENU_SCREEN);
}

bool load_role_screen()
{  
  screen_header_draw(ROLE_SCREEN);
  return true;
}

bool load_splash_screen()
{
  display_fixed_image(knx_iot_logo);
  return false;
}

bool generic_is_registered(void *list, void *elem) {
  while(list) {
    if (list == elem)
      return true;
    list = *(void**)list;
  }
  return false;
}

void register_generic(void **list, void *elem) {
  if (generic_is_registered(*list, elem))
    return;
  while(*list)
    list = *list;
  *list = elem;
}

void deregister_generic(void **list, void *elem) {
  if (!generic_is_registered(*list, elem))
    return;
  while(*list != elem)
    list = *list;
  *list = (*(void**)elem);
}

uint32_t num_generic(void *list) {
  uint32_t i = 0;
  while(list) {
    list = *(void**)list;
    i++;
  }
  return i;
}

void register_qr_code(struct qr_code_t *qrcode) {
  register_generic(&g_qr_codes, qrcode);
  if (g_screen_nr == QR_SCREEN)
    refresh_screen(false);
}

void deregister_qr_code(struct qr_code_t *qrcode) {
  return deregister_generic(&g_qr_codes, qrcode);
  if (g_screen_nr == QR_SCREEN)
    refresh_screen(false);
}

bool qr_code_is_registered(struct qr_code_t *qrcode) {
  return generic_is_registered(g_qr_codes, qrcode);
}

uint32_t num_qr_codes() {
  return num_generic(g_qr_codes);
}

void register_reset(struct reset_t *reset)
{
  register_generic(&g_resets, reset);
  if (g_screen_nr == RESET_SCREEN)
    refresh_screen(false);
}

void deregister_reset(struct reset_t *reset)
{
  deregister_generic(&g_resets, reset);
  if (g_screen_nr == RESET_SCREEN)
    refresh_screen(false);
}

bool reset_is_registered(struct reset_t *reset)
{
  return generic_is_registered(g_resets, reset);
}

uint32_t num_resets()
{
  return num_generic(g_resets);
}


bool load_qr_screen()
{
  char qr[100];
  int i = 0;
  struct qr_code_t *qrcode = g_qr_codes;
  while (i < g_qr && qrcode) {
    qrcode = qrcode->next;
    i++;
  }
  if (qrcode != NULL){
    strncpy(qr, qrcode->str_data, 99);
    SIF_SSD1681_overlay_qr_code(qr, get_framebuffer(), 2, 20, 2);
  } else {
    strcpy(qr, "No QR codes registered!");
  }
  display_setCursor(1, 70);
  display_setTextColor(BLACK, WHITE),
  display_puts(qr);
  return true;
}

static void qr_switch_screen()
{
  // QR code on THREAD
  g_qr += 1;
  if (g_qr >= num_qr_codes()) {
    g_qr = 0;
  }
  refresh_screen(false);
}

// go to the next screen, either up or down
void next_screen(bool increase)
{
  g_scroll_position = 0;
  if(increase) {
  // increase the current screen number
  if (g_screen_nr < (NUM_SCREENS - 1)) {
    ++g_screen_nr;
  } else {
    g_screen_nr = 0;
  }
  } else {
  // decrease the current screen number
  if (g_screen_nr <= 0) {
    g_screen_nr = NUM_SCREENS-1;
  } else {
    --g_screen_nr;
  }
  }
  refresh_screen(false);
}

void set_screen(enum Screen screen_no)
{
  g_screen_nr = screen_no;
  g_scroll_position = 0;
  g_selected_row = 0; 
  refresh_screen(false);
}

void go_next_screen()
{
  next_screen(true);
}

void go_prev_screen()
{
  next_screen(false);
}

static bool load_pm_screen()
{
  display_setCursor(0,0);
  display_puts("Programming mode");
  display_setCursor(0, 10);
  display_puts("SP 4 - toggle");
  display_setCursor(10, 20);
  
  bool pm = oc_knx_device_in_programming_mode(THIS_DEVICE);
  if (pm)
    display_puts("  ON");
  else
    display_puts("  OFF");
  draw_buttons();
  return true;
}


static void toggle_pm()
{
  bool pm = oc_knx_device_in_programming_mode(THIS_DEVICE);
  pm = !pm;
  oc_knx_device_set_programming_mode(THIS_DEVICE, pm);
  oc_device_info_t* device = oc_core_get_device_info(THIS_DEVICE);
  knx_publish_service(oc_string(device->serialnumber), device->iid, device->ia, device->pm);
  refresh_screen(false);
}

void refresh_screen(bool clean_redraw)
{
  g_clean_redraw |= clean_redraw;
  TASKLET_ScheduleDelta(&screen_Tasklet, SCHEDULE_NOW, NULL);
}
struct EinkScreenHandler g_screenHandlers[NUM_SCREENS] =
{
  [SPLASH_SCREEN] = {
    .load_screen_cb = &load_splash_screen,
    .title = "Splash Screen",
    .screen_button_3_LongPress_cb = &menu_select, 
    .screen_button_3_ShortPress_cb = &go_first_screen, 
  }, [MENU_SCREEN] = {
    .load_screen_cb = &load_menu_screen,
    .title = "Sys Menu",
    .screen_button_3_ShortPress_cb = &menu_select, 
    .screen_button_3_LongPress_cb = &go_first_screen, 
    .screen_button_1_ShortPress_cb = &menu_next, 
    .screen_button_2_ShortPress_cb = &menu_prev, 
  }, [QR_SCREEN] = {
    .load_screen_cb = &load_qr_screen,
    .title = "QR codes",
    .screen_button_3_ShortPress_cb = &qr_switch_screen, 
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    
  }, [GOT_SCREEN] = {
    .load_screen_cb = &load_got_screen,
    .title = "Group addrs",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_1_ShortPress_cb = &scroll_down, 
    .screen_button_2_ShortPress_cb = &scroll_up, 
  }, [GRT_SCREEN] = {
    .load_screen_cb = &load_grt_screen,
    .title = "Recipients",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_1_ShortPress_cb = &scroll_down, 
    .screen_button_2_ShortPress_cb = &scroll_up, 
  }, [GPT_SCREEN] = {
    .load_screen_cb = &load_gpt_screen,
    .title = "Publishers",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_1_ShortPress_cb = &scroll_down, 
    .screen_button_2_ShortPress_cb = &scroll_up, 
  }, [AUTH_SCREEN] = {
    .load_screen_cb = &load_auth_screen,
    .title = "Security",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_1_ShortPress_cb = &scroll_down, 
    .screen_button_2_ShortPress_cb = &scroll_up, 
  }, [DEV_SCREEN] = {
    .load_screen_cb = &load_dev_screen,
    .title = "Device",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_3_ShortPress_cb = &toggle_pm, 
  }, [RESET_SCREEN] = {
    .load_screen_cb = &load_reset_screen,
    .title = "Reset",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    .screen_button_3_ShortPress_cb = &reset_select, 
    .screen_button_1_ShortPress_cb = &reset_next, 
    .screen_button_2_ShortPress_cb = &reset_prev, 
  }, [ROLE_SCREEN] = {
    .load_screen_cb = &load_role_screen,
    .title = "Role",
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
  }, [TABLES_SCREEN] = {
    .load_screen_cb = &load_tables_screen,
    .title = "Tables",
    
    .screen_button_3_LongPress_cb = &go_MENU_SCREEN, 
    
  }, [GAME_INTRO] = {
    .load_screen_cb = &load_game_intro,
    .title = NULL,
    .screen_button_1_ShortPress_cb = &go_MENU_SCREEN,
    .screen_button_2_ShortPress_cb = &go_CONTROLS_1,
    .screen_button_3_ShortPress_cb = &game_start, 
  }, [CONTROLS_1] = {
    .load_screen_cb = &load_controls_1,
    .title = NULL,
    .screen_button_2_ShortPress_cb = &next_controls_page,
    .screen_button_3_ShortPress_cb = &prev_controls_page, 
  }, [PLACE_SHIPS] = {
    .load_screen_cb = &load_place_ships,
    .title = NULL,
    .screen_button_1_ShortPress_cb = &game_move_right,
    .screen_button_1_LongPress_cb = &game_move_left,
    .screen_button_2_ShortPress_cb = &game_move_down,
    .screen_button_2_LongPress_cb = &game_move_up,
    .screen_button_3_ShortPress_cb = &game_rotate_current_ship,
    .screen_button_3_LongPress_cb = &game_place_current_ship, 
  }, [READY_WAIT] = {
    .load_screen_cb = &load_screen_ready_wait,
    .title = NULL,
    .screen_button_3_Hold_cb = &stop_waiting, 
  }, [FIRE_SHOT] = {
    .load_screen_cb = &load_fire_shot,
    .title = NULL,
    .screen_button_1_ShortPress_cb = &game_move_right,
    .screen_button_1_LongPress_cb = &game_move_left,
    .screen_button_2_ShortPress_cb = &game_move_down,
    .screen_button_2_LongPress_cb = &game_move_up,
    .screen_button_3_ShortPress_cb = &game_fire_shot, 
  }, [SHOT_INFO] = {
    .load_screen_cb = &load_shot_info,
    .title = NULL,
    .screen_button_3_ShortPress_cb = &go_prev_screen, 
  }, [GAME_OVER] = {
    .load_screen_cb = &load_game_over,
    .title = NULL,
    .screen_button_3_ShortPress_cb = &game_setup, 
  },  
};

const char* screen_get_title(enum Screen screen)
{
  return g_screenHandlers[screen].title;
}
void button_1_ShortPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_1_ShortPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_1_ShortPress_cb();
}
void button_1_LongPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_1_LongPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_1_LongPress_cb();
}
void button_2_ShortPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_2_ShortPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_2_ShortPress_cb();
}
void button_2_LongPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_2_LongPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_2_LongPress_cb();
}
void button_3_ShortPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_3_ShortPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_3_ShortPress_cb();
}
void button_3_LongPress_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_3_LongPress_cb)
    g_screenHandlers[g_screen_nr].screen_button_3_LongPress_cb();
}
void button_3_Hold_cb(void *ctx)
{
  (void)ctx;
  if (g_screenHandlers[g_screen_nr].screen_button_3_Hold_cb)
    g_screenHandlers[g_screen_nr].screen_button_3_Hold_cb();
} 

void eink_load_screen(enum Screen screen_nr)
{
  g_max_y_coord = EINK_LINE_NO(8); 
  if (g_screen_nr < 0 || screen_nr >= NUM_SCREENS) {
    //soft reset eink screens
    screen_nr = SPLASH_SCREEN;
    g_eink_clean_redraw = true;
  }
  PRINT_APP("Loading screen %u ..\n", screen_nr);
  display_clear();
  display_setTextColor(BLACK, WHITE);
  bool ret = false;
  if (g_screenHandlers[screen_nr].load_screen_cb)
    ret = g_screenHandlers[screen_nr].load_screen_cb();
  if (ret == false)
    return;
  if (is_scrollable_screen())
    show_scroll_bar();
  display_setCursor(0,0);
  display_setTextSize(1);
  if (g_eink_clean_redraw) {
    display_render_full();
    g_eink_clean_redraw = false;
  } else {
    display_render_partial(false);
  }
}

void eink_redraw()
{
  eink_load_screen(g_screen_nr);
}

ca_error eink_initial_screen(void *args)
{
  static int g_start_counter = 0; // initial check to draw splash screen
  oc_device_info_t* device = oc_core_get_device_info(THIS_DEVICE);
  if (g_start_counter == 0) {
    eink_load_screen(SPLASH_SCREEN);
    g_start_counter = 1;
    TASKLET_ScheduleDelta(&screen_Tasklet, 5*1000, NULL);
    return CA_ERROR_SUCCESS;
  } else if (g_start_counter == 1) {
    g_start_counter = 2;
    if (device == NULL || oc_is_device_in_runtime(THIS_DEVICE) == false) {
      go_QR_SCREEN();
    } else {
      go_first_screen();
    }
    return CA_ERROR_SUCCESS;
  } else if (g_start_counter == 2) {
    if (device != NULL && oc_is_device_in_runtime(THIS_DEVICE) == true) {
      g_start_counter = 3;
      go_first_screen();
      return CA_ERROR_SUCCESS;
    }
  }
  eink_load_screen(g_screen_nr);
  return CA_ERROR_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//              App logic initialisation                                     //
///////////////////////////////////////////////////////////////////////////////

void logic_initialize()
{
  
// screen update, do it ASAP since it is the splash screen
  TASKLET_Init(&screen_Tasklet, &eink_initial_screen);
  TASKLET_ScheduleDelta(&screen_Tasklet, INITIAL_SCREEN_TIMEOUT, NULL); 
}
