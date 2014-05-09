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

#include "csp_incl.h"
#include "csp_defs.h"
#include "csp_vm.h"
#include "csp_internal.h"

#include "gen.h"
#include "vm.h"

#if CSP_ARRAYS_ENABLE

#if !CSP_ARRAYS_USE_MALLOC
static void *vm_buffer_alloc(size_t size)
{
    int free_space;
    void *res;

    free_space = vm->vm_bufsize - (vm->tail_ptr - (uint8_t*)vm);

    if (free_space < size){
	return NULL;
    }

    res = vm->tail_ptr;
    vm->tail_ptr += size;

    return res;
}
#endif

int CSP_EXTERNAL csp_vm_api_new_array(int num, int argc, int *argv, int *res)
{
    uint8_t *array;
    struct array_descr *descr;
    size_t size;
    int count;
    int type;

    if (argc != 2){
	return -CSP_ERR_VM_GENERIC;
    }

    count = argv[0];
    type = argv[1];

    if (count <= 0 || count >= 0x10000)
	return -CSP_ERR_OUT_OF_BOUND;

    switch (type){
	case 0: /* bit field */
	    size = (count + 7) / 8;
	    type = MAPPING_TYPE_ARR_BIT;
	    break;
	case 1: /* int8_t array */
	    size = count;
	    type = MAPPING_TYPE_ARR_CHAR;
	    break;
	case 2: /* int16_t array */
	    size = count * 2;
	    type = MAPPING_TYPE_ARR_SHORT;
	    break;
	case 4: /* int32_t array */
	    size = count * 4;
	    type = MAPPING_TYPE_ARR_INT;
	    break;
	default:
	    return -CSP_ERR_ARRAY_TYPE;
    }

    size += sizeof(struct array_descr);

    if (sizeof(void*) != sizeof(int)){
	return -CSP_ERR_VM_GENERIC; /* failsafe check */
    }

#if CSP_ARRAYS_USE_MALLOC
    array = malloc(size);
#else
    array = vm_buffer_alloc(size);
#endif

    if (array == NULL)
	return -CSP_ERR_NO_MEMORY;
    memset(array, 0, size);

    descr = (struct array_descr*)array;
    descr->type = type;
    descr->count = count;

    *res = (int)array;

    return 0;
}

#else
int CSP_EXTERNAL csp_vm_api_new_array(int num, int argc, int *argv, int *res)
{
    return -CSP_ERR_VM_GENERIC;
}
#endif /* CSP_ARRAYS_ENABLE */
