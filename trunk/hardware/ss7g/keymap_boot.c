/* Copyright (c) 2013, Vitaly Bursov <vitaly<AT>bursov.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef BOOTLOADER

#include "stm32conf.h"
#include "kbd_keymap.h"

/*
 * SteelSeries 7G layout
 *
 * ,---.   ,---------------. ,---------------. ,---------------. ,-----------.
 * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
 * `---'   `---------------' `---------------' `---------------' `-----------'
 * ,-----------------------------------------------------------. ,-----------. ,---------------.
 * |~  |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Bks| |Ins|Hom|PgU| |NuL|  /|  *|  -|
 * |-----------------------------------------------------------| |-----------| |---------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|     | |Del|End|PgD| |  7|  8|  9|   |
 * |-----------------------------------------------------+     | `-----------' |-----------| + |
 * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '| Return |               |  4|  5|  6|   |
 * |-----------------------------------------------------------|     ,---.     |---------------|
 * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |     |Up |     |  1|  2|  3| R |
 * |-----------------------------------------------------------| ,-----------. |-----------| e |
 * |Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig| |   0   |  .| t |
 * `-----------------------------------------------------------' `-----------' `---------------'
 */

const uint8_t keymaps[][MATRIX_COLS][MATRIX_ROWS] = {
#if 1
  KEYMAP(
    ESC,  F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10,  F11,  F12,             PSCR, SLCK,  BRK, 

    GRV,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0, MINS,  EQL, BSLS, BSPC,  INS, HOME, PGUP,  NLCK, PSLS, PAST, PMNS,
    TAB,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P, LBRC, RBRC,              DEL,  END, PGDN,    P7,   P8,   P9, PPLS,
    CAPS,  A,   S,   D,   F,   G,   H,   J,   K,   L, SCLN, QUOT,     ENT,                              P4,   P5,   P6, 
    LSFT,    Z,   X,   C,   V,   B,   N,   M, COMM, DOT, SLSH,      RSFT,                 UP,           P1,   P2,   P3, PENT,
    LCTL, LGUI, LALT,             SPC,                RALT, RGUI, APP, RCTL,       LEFT, DOWN, RGHT,       P0,    PDOT       ),
#else
  KEYMAP(
    ESC,  F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10,  F11,  F12,             PSCR, SLCK,  BRK, 

    GRV,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0, MINS,  EQL, BSLS, BSPC,  INS, HOME, PGUP,  NLCK, PSLS, PAST, PMNS,
    TAB,   Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P, LBRC, RBRC,              DEL,  END, PGDN,    P7,   P8,   P9, PPLS,
    FN1 ,  A,   S,   D,   F,   G,   H,   J,   K,   L, SCLN, QUOT,     ENT,                              P4,   P5,   P6, 
    LSFT,    Z,   X,   C,   V,   B,   N,   M, COMM, DOT, SLSH,      RSFT,                 UP,           P1,   P2,   P3, PENT,
    LCTL, LGUI, LALT,             SPC,                RALT, RGUI, APP, RCTL,       LEFT, DOWN, RGHT,       P0,    PDOT       ),

  KEYMAP(
    GRV,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  F11,  F12,             PSCR, SLCK,  BRK, 

    ESC,  F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10, MINS,  EQL, BSLS, BSPC,  INS, HOME, PGUP,  NLCK, PSLS, PAST, PMNS,
    CAPS,  Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P, LBRC, RBRC,              DEL,  END, PGDN,    P7,   P8,   P9, PPLS,
    FN1 ,  A,   S,   D,   F,   G,   H,   J,   K,   L, SCLN, QUOT,     ENT,                              P4,   P5,   P6, 
    LSFT,    Z,   X,   C,   V,   B,   N,   M, COMM, DOT, SLSH,      RSFT,                 UP,           P1,   P2,   P3, PENT,
    LCTL, LGUI, LALT,             SPC,                RALT, RGUI, APP, RCTL,       LEFT, DOWN, RGHT,       P0,    PDOT       ),
#endif
};

#endif /* BOOTLOADER */
