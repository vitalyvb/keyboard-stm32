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

#ifndef VM_H
#define VM_H

#include "csp_incl.h"
#include "csp_defs.h"
#include "csp_vm.h"
#include "csp_internal.h"

struct cache_item {
    uint16_t offset;
    uint8_t idx;
    uint8_t _pad;
};

struct vm {
    int globals_cnt;
    int maps_cnt;
    int funcs_cnt;

    int api_func_cnt;

    uint8_t *vm_buffer;	/* stack and call data for running functions */
    uint8_t *vm_buffer_end;

    struct cache_item *cache_maps;	/* points to structure tail */
    struct cache_item *cache_funcs;	/* points to structure tail */

#if CSP_ARRAYS_ENABLE && !CSP_ARRAYS_USE_MALLOC
    uint8_t *arrays_ptr;		/* points to structure tail */
    uint8_t *tail_ptr;			/* points to structure tail */
#endif

    int vm_bufsize;

    uint8_t *func_names;
    int func_names_len;
    int func_call_result;

    int global_vars[0];		/* buffer for global variables */
/*    struct cache_item cache_maps[0];  */
/*    struct cache_item cache_funcs[0]; */
/*    uint8_t arrays[0]; */	/* buffer for arrays */
};

extern struct vm *vm;

#endif /* VM_H */
