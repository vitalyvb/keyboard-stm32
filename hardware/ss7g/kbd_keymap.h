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

#ifndef KBD_KEYMAP
#define KBD_KEYMAP

#include "keycode.h"
#include "keymap.h"

/* http://deskthority.net/wiki/Controller_matrix_traces#Steelseries_7G */

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( \
    KB7, KG7, KG6, KE7, KE6, KI7, KI6, KH7, KH6, KJ7, KJ6, KF7, KF6,            KD7, KC7, KL7, \
    KB6, KB5, KG3, KN7, KO7, KO6, KP7, KP6, KN6, KH5, KJ5, KH4, KI5, KI1, KI4,  KE5, KF5, KG5,  KK6, KD5, KD4, KK7, \
    KB4, KB3, KG1, KN4, KO5, KO4, KP5, KP4, KN5, KH3, KJ0, KH1, KI3,            KE4, KF4, KG4,  KD3, KF3, KE3, KD2, \
    KA4, KB1, KG2, KN1, KO3, KO1, KP3, KP1, KN3, KH2, KJ3, KJ1, KI0,                            KD1, KF1, KE1,      \
    KL3,   KB0, KG0, KN2, KO2, KO0, KP2, KP0, KN0, KH0, KJ2,    KL0,                 KM5,       KD0, KF2, KE2, KC6, \
    KA3, KC5, KK1,           KM6,                     KK2, KM4, KD6, KA0,       KI2, KJ4, KM7,    KF0,    KE0       \
) { \
/*                 C0        C1        C2        C3        C4        C5        C6        C7    */ \
/* R0  A */   { KC_##KA0, KC_NO   , KC_NO   , KC_##KA3, KC_##KA4, KC_NO   , KC_NO   , KC_NO    }, \
/* R1  B */   { KC_##KB0, KC_##KB1, KC_NO   , KC_##KB3, KC_##KB4, KC_##KB5, KC_##KB6, KC_##KB7 }, \
/* R2  C */   { KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_NO   , KC_##KC5, KC_##KC6, KC_##KC7 }, \
/* R3  D */   { KC_##KD0, KC_##KD1, KC_##KD2, KC_##KD3, KC_##KD4, KC_##KD5, KC_##KD6, KC_##KD7 }, \
/* R4  E */   { KC_##KE0, KC_##KE1, KC_##KE2, KC_##KE3, KC_##KE4, KC_##KE5, KC_##KE6, KC_##KE7 }, \
/* R5  F */   { KC_##KF0, KC_##KF1, KC_##KF2, KC_##KF3, KC_##KF4, KC_##KF5, KC_##KF6, KC_##KF7 }, \
/* R6  G */   { KC_##KG0, KC_##KG1, KC_##KG2, KC_##KG3, KC_##KG4, KC_##KG5, KC_##KG6, KC_##KG7 }, \
/* R7  H */   { KC_##KH0, KC_##KH1, KC_##KH2, KC_##KH3, KC_##KH4, KC_##KH5, KC_##KH6, KC_##KH7 }, \
/* R8  I */   { KC_##KI0, KC_##KI1, KC_##KI2, KC_##KI3, KC_##KI4, KC_##KI5, KC_##KI6, KC_##KI7 }, \
/* R9  J */   { KC_##KJ0, KC_##KJ1, KC_##KJ2, KC_##KJ3, KC_##KJ4, KC_##KJ5, KC_##KJ6, KC_##KJ7 }, \
/* R10 K */   { KC_NO   , KC_##KK1, KC_##KK2, KC_NO   , KC_NO   , KC_NO   , KC_##KK6, KC_##KK7 }, \
/* R11 L */   { KC_##KL0, KC_NO   , KC_NO   , KC_##KL3, KC_NO   , KC_NO   , /*rNUBSKC_##KL6*/KC_NO, KC_##KL7 }, \
/* R12 M */   { KC_NO   , KC_NO   , KC_NO   , /*KC_##KM3 lNUBS*/KC_NO, KC_##KM4, KC_##KM5, KC_##KM6, KC_##KM7 }, \
/* R13 N */   { KC_##KN0, KC_##KN1, KC_##KN2, KC_##KN3, KC_##KN4, KC_##KN5, KC_##KN6, KC_##KN7 }, \
/* R14 O */   { KC_##KO0, KC_##KO1, KC_##KO2, KC_##KO3, KC_##KO4, KC_##KO5, KC_##KO6, KC_##KO7 }, \
/* R15 P */   { KC_##KP0, KC_##KP1, KC_##KP2, KC_##KP3, KC_##KP4, KC_##KP5, KC_##KP6, KC_##KP7 }, \
}

#endif /* KBD_KEYMAP */
