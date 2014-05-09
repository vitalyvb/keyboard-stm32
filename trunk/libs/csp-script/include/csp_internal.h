/* Copyright (c) 2014, Vitaly Bursov <vitaly<AT>bursov.com>
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

#ifndef CSP_INTERNAL_H
#define CSP_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include "csp_defs.h"

#define MAPPING_SIZE (256)
struct csp_mapping {
    uint8_t data[MAPPING_SIZE]; /* MUST be first */
    uint16_t count;
    uint8_t min;
    uint8_t max;
};


void _csp_set_error(int err, int line, const char *msg, ...);

#if CSP_STRING_ERRORS
#define csp_set_error _csp_set_error
#else
#define csp_set_error(e,l,x...) _csp_set_error((e),(l),NULL)
#endif

/* internal aliases */
#ifndef CSP_TEXT_INPUT_CALLBACK
#define CSP_TEXT_INPUT_CALLBACK csp_text_input_callback
#endif

#ifndef CSP_GET_CONST_VALUE
#define CSP_GET_CONST_VALUE csp_get_const_value
#endif

#ifndef CSP_VM_API_CALL_CALLBACK
#define CSP_VM_API_CALL_CALLBACK csp_vm_api_call_callback
#endif

#endif /* CSP_INTERNAL_H */

