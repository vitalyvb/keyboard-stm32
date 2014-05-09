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


#if 0
#define TR(s, p...) \
		fprintf(stderr, "@%03x  op:%02x %4d " s "\n",(int)(code-program), opcode, (int)(((uint8_t*)_frame->stack)-vm->vm_buffer), ## p)
#else
#define TR(s, p...)
#endif

#define OPTIMIZE_FOR_CODE_SIZE 1	/* 0 - less RAM usage, 1 - compact code */

/********************************************************/

#define CACHE_ID_MAP (0)
#define CACHE_ID_FUNC (1)

#define UCODE_OP_MASK		(0x0f)
#define UCODE_IF_EXP		(1)
#define UCODE_IF_THEN		(2)

#define UCODE_LOOP_EXP		(3)
#define UCODE_LOOP_BODY		(4)
#define UCODE_LOOP_POSTBODY	(5)

#define UCODE_FLAG_MASK		(~UCODE_OP_MASK)
#define UCODE_BREAK_BIT		(0x80)
#define UCODE_CONT_BIT		(0x40)

/********************************************************/

struct builtin_func {
    int (*handler)(int *args, int *res);
};

struct u_frame {
    uint8_t ucode;
    const uint8_t *ptr;
#if OPTIMIZE_FOR_CODE_SIZE
    struct u_frame *prev;
#else
    uint16_t prev;
#endif
};

#if OPTIMIZE_FOR_CODE_SIZE
/* struct field is a pointer
 * pros: minimal code size, faster
 * cons: slightly higher memory usage - few bytes per funciton call
 */
# define UFRAME(f) ((f)->micro_frame)
# define UFRAME_VAL(f,u) (u)
# define PREV_UFRAME(f,u) (u->prev)
#else
/* struct field is a offset, data pointer is calculated
 */
# define UFRAME(f) (((f)->micro_frame == 0) ? NULL : ((struct u_frame*)((uint8_t*)f + (f)->micro_frame)))
# define UFRAME_VAL(f,u) (((u) == NULL) ? 0 : ((int)((uint8_t*)u - (uint8_t*)f)))
# define PREV_UFRAME(f,u) (((u)->prev == 0) ? NULL : ((struct u_frame*)((uint8_t*)f + (u)->prev)))
#endif


struct vm_frame {
    int *stack;		/* current stack position */

#if OPTIMIZE_FOR_CODE_SIZE
    struct vm_frame *prev_frame;
    struct u_frame *micro_frame;
#else
    uint16_t prev_frame;
    uint16_t micro_frame;
#endif

    const uint8_t *ret_code;
    uint32_t ret_size;

    int *argv;
    int argc;

    int local_vars[0];	/* local variables */
    /* stack here */
};

#if OPTIMIZE_FOR_CODE_SIZE
/* same as above */
# define NO_PREV_FRAME_VAL (NULL)
# define FRAME_VAL(f) (f)
# define PREV_FRAME(f) ((f)->prev_frame)
# define HAVE_PREV_FRAME(f) ((f)->prev_frame != 0)
#else
# define NO_PREV_FRAME_VAL (1) /* can't use 0 here, it's a first frame, but 1 is still pretty impossible */
# define FRAME_VAL(f) ((uint8_t*)(f) - (uint8_t*)vm->vm_buffer)
# define PREV_FRAME(f) ((struct vm_frame*)((uint8_t*)vm->vm_buffer + (f)->prev_frame))
# define HAVE_PREV_FRAME(f) ((f)->prev_frame != NO_PREV_FRAME_VAL)
#endif

static uint8_t *program;
struct vm *vm;

/********************************************************/

#define LD_UINT16(ptr, offset) (*(uint16_t*)((ptr) + (offset)))

#define find_function(i) find_cache_item(CACHE_ID_FUNC, (i))
#define find_mapping(i) find_cache_item(CACHE_ID_MAP, (i))
static int find_cache_item(int cachetype, int idx);

static int func_builtin_getidx(int *args, int *res);
static int func_builtin_setidx(int *args, int *res);

/********************************************************/

const struct builtin_func builtins[MACRO_OP_BUILTIN_COUNT] = {
    [MACRO_OP_FUNC_GETIDX] = {
	.handler = func_builtin_getidx,
    },
    [MACRO_OP_FUNC_SETIDX] = {
	.handler = func_builtin_setidx,
    },
};

/********************************************************/

static int get_loop_exp_size(const uint8_t *instr)
{
    return instr[1];
}
#define get_if_exp_size get_loop_exp_size

static int get_loop_body_size(const uint8_t *instr)
{
    return LD_UINT16(instr, 2);
}
#define get_if_then_size get_loop_body_size
#define get_if_else_size(x) get_loop_body_size(x+2)

static int get_loop_post_size(const uint8_t *instr)
{
    if (instr[0] == OP_FOR)
	return instr[4];

    return 0;
}

const uint8_t op_sizes[] = {
	SIZE_OP_IF,
	SIZE_OP_WHILE,
	SIZE_OP_DOWHILE,
	SIZE_OP_FOR,
};

static const uint8_t *get_loop_exp_code(const uint8_t *instr)
{
    uint8_t c = instr[0]-OP_IF;

    if (c == OP_DOWHILE-OP_IF)
	instr += get_loop_body_size(instr);

    return instr + op_sizes[c];
}
#define get_if_exp_code get_loop_exp_code

static const uint8_t *get_loop_exp_code_end(const uint8_t *instr)
{
    return get_loop_exp_code(instr) + get_loop_exp_size(instr);
}
#define get_if_exp_code_end get_loop_exp_code_end

static const uint8_t *get_loop_body_code(const uint8_t *instr)
{
    uint8_t c = instr[0]-OP_IF;

    if (c == OP_DOWHILE-OP_IF)
	return instr + op_sizes[c];

    return get_loop_exp_code_end(instr) + get_loop_post_size(instr);
}


static const uint8_t *get_loop_body_code_end(const uint8_t *instr)
{
    return get_loop_body_code(instr) + get_loop_body_size(instr);
}
#define get_if_then_code_end get_loop_body_code_end

static const uint8_t *get_loop_post_code(const uint8_t *instr)
{
    if (instr[0] == OP_FOR)
	return get_loop_exp_code_end(instr);
    return NULL;
}

static const uint8_t *get_loop_post_code_end(const uint8_t *instr)
{
    return get_loop_post_code(instr) + get_loop_post_size(instr);
}


static const uint8_t *get_loop_code_end(const uint8_t *instr)
{
    if (instr[0] == OP_DOWHILE)
	return get_loop_exp_code_end(instr);
    return get_loop_body_code_end(instr);
}

/********************************************************/

static int mapping_map_value(int mapid, int from, uint8_t **ptr)
{
    int offset = find_mapping(mapid);
    uint8_t *map, *pres;
    int count, min, max;

    if (offset < 0)
	return -CSP_ERR_MAP_NOT_FOUND;

    map = &program[offset];

    count = map[1];
    min = map[2];
    max = map[3];

    if (map[0] == MAPPING_TYPE_ARRAY){

	/* range check */
	if (from < min || from > max){
	    if (ptr)
		*ptr = NULL;
	    return 0;
	}

	pres = &map[SIZE_STR_MAPPING + from - min];

	if (ptr)
	    *ptr = pres;

	return *pres;

    } else if (map[0] == MAPPING_TYPE_PAIRS){
	int i;

	/* range check */
	if (from < min || from > max){
	    if (ptr)
		*ptr = NULL;
	    return 0;
	}

	map += SIZE_STR_MAPPING;

	count = count * 2;
	for (i=0;i<count;i+=2){
	    if (map[i] == from){
		if (ptr)
		    *ptr = &map[i+1];
		return map[i+1];
	    }
	}

	/* pair not found */
	if (ptr)
	    *ptr = NULL;
	return 0;
    }

    return -CSP_ERR_MAP_INVALID;
}

#if CSP_ARRAYS_ENABLE
static int array_ptr_xchg(int array, int index, int *dst, int *newval)
{
    struct array_descr *descr;
    void *data;

    descr = (struct array_descr*) array;
    data = ((char*)array) + sizeof(struct array_descr);

#if !CSP_ARRAYS_USE_MALLOC
    /* simple check that we're inside of allowed region */
    if (((uint8_t*)descr < vm->arrays_ptr) || ((uint8_t*)data >= vm->tail_ptr)){
	return -CSP_ERR_OUT_OF_BOUND;
    }
#endif

    if (index < 0 || index >= descr->count)
	return -CSP_ERR_OUT_OF_BOUND;

    if (dst){
	switch (descr->type){
	    case MAPPING_TYPE_ARR_BIT: /* bit field */
		// XXX add support for STM32 bit fields
		*dst = !!(((uint8_t*)data)[index/8] & (1 << (index & 7)));
		break;
	    case MAPPING_TYPE_ARR_CHAR: /* int8_t array */
		*dst = ((int8_t*)data)[index];
		break;
	    case MAPPING_TYPE_ARR_SHORT: /* int16_t array */
		*dst = ((int16_t*)data)[index];
		break;
	    case MAPPING_TYPE_ARR_INT: /* int32_t array */
		*dst = ((int32_t*)data)[index];
		break;
	    default:
		return -CSP_ERR_VM_GENERIC;
	}
    }

    if (newval){
	switch (descr->type){
	    case MAPPING_TYPE_ARR_BIT: /* bit field */
		if (*newval)
		    (((uint8_t*)data)[index/8] |= (1 << (index & 7)));
		else
		    (((uint8_t*)data)[index/8] &= ~(1 << (index & 7)));
		break;
	    case MAPPING_TYPE_ARR_CHAR: /* int8_t array */
		((int8_t*)data)[index] = *newval;
		break;
	    case MAPPING_TYPE_ARR_SHORT: /* int16_t array */
		((int16_t*)data)[index] = *newval;
		break;
	    case MAPPING_TYPE_ARR_INT: /* int32_t array */
		((int32_t*)data)[index] = *newval;
		break;
	    default:
		return -CSP_ERR_VM_GENERIC;
	}
    }

    return 0;
}
#endif

static int var_ptr_xchg(struct vm_frame *frame, int num, int *dst, int *newval)
{
    int tmp;
    int *val;

    tmp = vm->globals_cnt;
    if (num < tmp){
	val = &vm->global_vars[num];
	goto getset;
    }
    num -= tmp;

    tmp = vm->maps_cnt;
    if (num < tmp){
	if (dst)
	    *dst = num + vm->globals_cnt;
	else
	    return -CSP_ERR_MAP_CANT_SET;
	return 0;
    }
    num -= tmp;

    tmp = frame->argc;
    if (num < tmp){
	val = &frame->argv[num];
	goto getset;
    }
    num -= tmp;

    val = &frame->local_vars[num];

getset:
    if (dst)
	*dst = *val;

    if (newval)
	*val = *newval;

    return 0;
}


static int func_builtin_getidx(int *args, int *res)
{
    int array = args[-2];
    int idx = args[-1];
    int opres;

    if (array >= vm->globals_cnt && array < vm->maps_cnt + vm->globals_cnt){
	/* mapping */
	*res = mapping_map_value(array-vm->globals_cnt, idx, NULL);
	return 2;
    }
    /* array */
#if CSP_ARRAYS_ENABLE
    //printf("     xxxxxx %d[%d]\n", array, idx);

    opres = array_ptr_xchg(array, idx, res, NULL);
    if (opres)
	return opres;

    return 2;
#else
    return -CSP_ERR_NOARRAY_GET;
#endif
}

static int func_builtin_setidx(int *args, int *res)
{
    int array = args[-3];
    int idx = args[-2];
    int value = args[-1];
    int opres;

    /* XXX mapping can be in ROM */

    if (array >= vm->globals_cnt && array < vm->maps_cnt + vm->globals_cnt){
	/* mapping */
	uint8_t *mapv;

	mapping_map_value(array-vm->globals_cnt, idx, &mapv);
	if (mapv == NULL)
	    return -CSP_ERR_MAP_CANT_SET_VALUE;

	*mapv = value;
	*res = value;

	return 3;
    }
    /* array */

#if CSP_ARRAYS_ENABLE
    //printf("     xxxxxx %d[%d]=%d\n", array, idx, value);

    opres = array_ptr_xchg(array, idx, NULL, &value);
    if (opres)
	return opres;

    *res = value;

    return 3;
#else
    return -CSP_ERR_NOARRAY_SET;
#endif
}

/********************************************************/

static struct vm_frame *create_first_frame(uint8_t *buf, int bufsize, int vars)
{
    struct vm_frame *new;

    vm->vm_buffer = buf;
    vm->vm_buffer_end = buf + bufsize;

    new = (struct vm_frame *)buf;
    new->prev_frame = NO_PREV_FRAME_VAL;

    new->stack = (int*)((char*)new + sizeof(struct vm_frame) + sizeof(int)*vars);
    new->micro_frame = 0;

    return new;
}

static struct vm_frame *create_new_frame(struct vm_frame *cur_frame, int vars)
{
    struct vm_frame *new;

    new = (struct vm_frame *)cur_frame->stack;
    new->prev_frame = FRAME_VAL(cur_frame);
    new->stack = (int*)((char*)new + sizeof(struct vm_frame) + sizeof(int)*vars);
    new->micro_frame = 0;

    return new;
}

static struct u_frame *create_new_uframe(struct vm_frame *frame, int ucode)
{
    struct u_frame *new = (struct u_frame *)frame->stack;
    new->prev = UFRAME_VAL(frame, UFRAME(frame));
    frame->micro_frame = UFRAME_VAL(frame, new);

    frame->stack = (int*)(((uint8_t*)frame->stack) + sizeof(struct u_frame));
    new->ucode = ucode;
    return new;
}

static void remove_current_uframe(struct vm_frame *frame)
{
    struct u_frame *uf = UFRAME(frame);

    frame->stack = (int*)uf;
    frame->micro_frame = UFRAME_VAL(frame, PREV_UFRAME(frame, uf));
}

static void reset_uframe_stack(struct vm_frame *frame)
{
    struct u_frame *uf = UFRAME(frame);
    frame->stack = (int*)(((uint8_t*)uf) + sizeof(struct u_frame));
}

static struct u_frame *find_loop_uframe(struct vm_frame *frame)
{
    struct u_frame *uf = UFRAME(frame);

    while (uf){
	if ((uf->ucode & UCODE_OP_MASK) == UCODE_LOOP_BODY)
	    break;
	uf = PREV_UFRAME(frame, uf);
    }
    return uf;
}

/********************************************************/

static int vm_execute(struct vm_frame *_frame, const uint8_t *code_start, int size)
{
    const uint8_t *code;
    uint8_t opcode;
    uint32_t tmp;

    code = code_start;

#define STACK_GUARD_SIZE (sizeof(struct vm_frame))

#define STACK_VAL(_pos_) (_frame->stack[(_pos_)])
#define STACK_MOVE(_cnt_) do { _frame->stack += (int)(_cnt_); } while (0)


    while (code < (code_start+size)) {

	if ((uint8_t*)&STACK_VAL(0) - vm->vm_buffer_end < STACK_GUARD_SIZE)
	    return -CSP_ERR_STACK_OVERFLOW;

	if (UFRAME(_frame)){
	    opcode = 0;

	    if (UFRAME(_frame)->ucode & UCODE_FLAG_MASK){
		uint8_t flags = UFRAME(_frame)->ucode & UCODE_FLAG_MASK;
		struct u_frame *uf = find_loop_uframe(_frame);

		if (uf == NULL){
		    TR("unexpected break");
		    return -CSP_ERR_UNEXP_BRK_CONT;
		}

		TR("    loop ctl");

		/* rewind stack */
		_frame->micro_frame = UFRAME_VAL(_frame, uf);
		reset_uframe_stack(_frame);

		/* copy flag */
		UFRAME(_frame)->ucode |= flags;

	    }


	    switch (UFRAME(_frame)->ucode & UCODE_OP_MASK){

		case UCODE_IF_EXP:{
		    const uint8_t *ustart = UFRAME(_frame)->ptr;

		    if (code >= get_if_exp_code_end(ustart)){
			/* done evaluating expression */

			tmp = STACK_VAL(-1);
			STACK_MOVE(-1);
			if (tmp){
			    if (get_if_else_size(ustart) == 0){
				/* remove frame and continue */
				remove_current_uframe(_frame);
				TR(" IF exp %d then preout", tmp);
			    } else {
				TR(" IF exp %d then", tmp);
				UFRAME(_frame)->ucode = UCODE_IF_THEN;
			    }
			} else {
			    /* remove frame jump at else block
			     * if there's any */
			    remove_current_uframe(_frame);
			    TR(" IF exp %d else out", tmp);
			    code = get_if_then_code_end(ustart);
			}
		    }

		    break;
		}

		case UCODE_IF_THEN:{
		    const uint8_t *ustart = UFRAME(_frame)->ptr;

		    if (code >= get_if_then_code_end(ustart)){
			/* done executing then block */
			remove_current_uframe(_frame);
			code = code + get_if_else_size(ustart);
			TR(" IF exp then out");
		    }

		    break;
		}




		case UCODE_LOOP_EXP:{
		    const uint8_t *ustart = UFRAME(_frame)->ptr;

		    if (code >= get_loop_exp_code_end(ustart)){
			/* done evaluating expression */

			tmp = STACK_VAL(-1);
			STACK_MOVE(-1);
			if (tmp){
			    /* continue to loop body */
			    UFRAME(_frame)->ucode = UCODE_LOOP_BODY;
			    code = get_loop_body_code(ustart);
			} else {
			    /* remove frame and skip loop body */
			    remove_current_uframe(_frame);
			    TR(" LOOP exp %d out", tmp);
			    code = get_loop_code_end(ustart);
			}

		    }

		    break;
		}

		case UCODE_LOOP_BODY: {
		    const uint8_t *ustart = UFRAME(_frame)->ptr;
		    int ucode = UFRAME(_frame)->ucode;

		    if (ucode & UCODE_BREAK_BIT){
			/* break */
			TR(" LOOP body iter break");

			code = get_loop_code_end(ustart);
			remove_current_uframe(_frame);

		    } else if ((ucode & UCODE_CONT_BIT) /* continue */ ||
				(code >= get_loop_body_code_end(ustart))){
			/* done executing loop body */
			TR(" LOOP body iter done");
			if (get_loop_post_size(ustart) == 0){
			    /* jump to expression evaluation */
			    ucode = UCODE_LOOP_EXP;
			    code = get_loop_exp_code(ustart);
			} else {
			    /* jump to post-iteration code */
			    ucode = UCODE_LOOP_POSTBODY;
			    code = get_loop_post_code(ustart);
			}
			UFRAME(_frame)->ucode = ucode;
			reset_uframe_stack(_frame);
		    }

		    break;
		}
		case UCODE_LOOP_POSTBODY:{
		    const uint8_t *ustart = UFRAME(_frame)->ptr;

		    if (code >= get_loop_post_code_end(ustart)){
			UFRAME(_frame)->ucode = UCODE_LOOP_EXP;
			code = get_loop_exp_code(ustart);
			reset_uframe_stack(_frame);
			TR(" LOOP body post done");
		    }

		    break;
		}

		default:
		    TR("unknown uF code: %x\n", UFRAME(_frame)->ucode);
		    return -CSP_ERR_IE_UNK_UF;
	    }
	}

	opcode = *code++;

	switch (opcode >> OPCODE_XSHIFT){
	case 0: /* 0x00..0x3f */
	    tmp = opcode & OPCODE_VALMASK1;
	    if (tmp & 0x20)
		tmp |= 0xffffffe0; /* restore sign */
	    TR("ldi %d", tmp);
	    STACK_VAL(0) = tmp;
	    STACK_MOVE(1);
	    break;
	case 1:{ /* 0x40..0x7f */
	    int res;

	    tmp = opcode & OPCODE_VALMASK1;
	    TR("pushvi %d", tmp);

	    if ((res = var_ptr_xchg(_frame, tmp, &STACK_VAL(0),  NULL)))
		return res;

	    STACK_MOVE(1);
	    break;
	    }
	case 2: /* 0x80..0xbf */
	    switch ((opcode >> 4) - 0x8){
	    case 0: /* 0x80..0x8f */
		switch (opcode){
		case OPCODE_IMMED_8:
		    tmp = *(int8_t*)code++;
		    TR("ld1 %d", tmp);
		    goto op_ld_tmp;
		case OPCODE_IMMED_16:
		    tmp = *(int16_t*)code;
		    code += 2;
		    TR("ld2 %d", tmp);
		    goto op_ld_tmp;
		case OPCODE_IMMED_32:
		    tmp = *(uint32_t*)code;
		    code += 4;
		    TR("ld4 %d", tmp);
	    op_ld_tmp:
		    STACK_VAL(0) = tmp;
		    STACK_MOVE(1);
		    break;
		case SET_VARNUM:{
		    int res;

		    tmp = *code++;
		    TR("setv %d", tmp);

		    if ((res = var_ptr_xchg(_frame, tmp, NULL, &STACK_VAL(-1))))
			return res;

		    break;
		    }
		case PUSH_VARNUM:{
		    int res;

		    tmp = *code++;
		    TR("pushv %d", tmp);

		    if ((res = var_ptr_xchg(_frame, tmp, &STACK_VAL(0), NULL)))
			return res;

		    STACK_MOVE(1);
		    break;
		    }
		}
		break;
	    case 1:{ /* 0x90..0x9f */
		int a, b, res=0;
		a = STACK_VAL(-2);
		b = STACK_VAL(-1);
		switch (opcode){
		    case OP_BIN_ADD:
			TR("add %d %d", a, b);
			res = a + b;
			break;
		    case OP_BIN_SUB:
			TR("sub %d %d", a, b);
			res = a - b;
			break;
		    case OP_BIN_MUL:
			TR("mul %d %d", a, b);
			res = a * b;
			break;
		    case OP_BIN_DIV:
			TR("div %d %d", a, b);
			if (b == 0)
			    return -CSP_ERR_DIV_BY_ZERO;
			res = a / b;
			break;
		    case OP_BIN_GR:
			TR(" > %d %d", a, b);
			res = a > b;
			break;
		    case OP_BIN_LS:
			TR(" < %d %d", a, b);
			res = a < b;
			break;
		    case OP_BIN_GE:
			TR(" >= %d %d", a, b);
			res = a >= b;
			break;
		    case OP_BIN_LE:
			TR(" <= %d %d", a, b);
			res = a <= b;
			break;
		    case OP_BIN_EQ:
			TR(" == %d %d", a, b);
			res = a == b;
			break;
		    case OP_BIN_NE:
			TR(" != %d %d", a, b);
			res = a != b;
			break;
		    case OP_BIN_OROR:
			TR(" || %d %d", a, b);
			res = a || b;
			break;
		    case OP_BIN_ANDAND:
			TR(" && %d %d", a, b);
			res = a && b;
			break;
		    case OP_BIN_OR:
			TR(" | %d %d", a, b);
			res = a | b;
			break;
		    case OP_BIN_AND:
			TR(" & %d %d", a, b);
			res = a & b;
			break;
		    case OP_BIN_XOR:
			TR(" ^ %d %d", a, b);
			res = a ^ b;
			break;
		    case OP_BIN_MOD:
			TR(" %% %d %d", a, b);
			if (b == 0)
			    return -CSP_ERR_DIV_BY_ZERO;
			res = a % b;
			break;
		}
		STACK_MOVE(-1);
		STACK_VAL(-1) = res;
		break;
	    }
	    case 2:{ /* 0xa0..0xaf */
		int a, res=0;
		a = STACK_VAL(-1);
		switch (opcode){
		case OP_UN_NEG:
		    TR(" - %d", STACK_VAL(-1));
		    res = -a;
		    break;
		case OP_UN_NOT:
		    TR(" ! %d", STACK_VAL(-1));
		    res = !a;
		    break;
		case OP_UN_BITNEG:
		    TR(" ~ %d", STACK_VAL(-1));
		    res = ~a;
		    break;
		}
		STACK_VAL(-1) = res;
		break;
	    }
	    case 3:{ /* 0xb0..0xbf */
		/* OP_CALL only here */
		uint8_t *newcode;
		int args, vars, newsize, o;

		tmp = *code++;


		if (tmp < vm->api_func_cnt) {
		    int argc;
		    int op_res;
		    int f_res;


		    if (tmp < MACRO_OP_BUILTIN_COUNT) {
			/* parameters in stack:
			 *  @-N  [ arg1, arg2, ... argN ]  @0
			 * function must to know and return parameter count
			 */
			TR("CALLint %d", tmp);
			f_res = builtins[tmp].handler(&STACK_VAL(0), &op_res);
			argc = f_res-1;
		    } else {
			/* parameters in stack:
			 *  @-N-1  [ arg1, arg2, ... argN, argCNT ]  @0
			 * last argument is generated by a parser, it's a actual
			 * arguments count.
			 *
			 * If function has zero arguments argCNTs cell is used
			 * to store a return value.
			 */
			TR("CALLapi %d", tmp);
			argc = STACK_VAL(-1);
			f_res = CSP_VM_API_CALL_CALLBACK(tmp - MACRO_OP_BUILTIN_COUNT, argc, &STACK_VAL(-(argc+1)), &op_res);
		    }

		    if (f_res<0)
			return f_res;
		
		    STACK_MOVE(-argc);
		    STACK_VAL(-1) = op_res;
		} else {
		    struct vm_frame *fr;

		    tmp -= vm->api_func_cnt;

		    TR("CALL %d", tmp);
		    o = find_function(tmp);
		    if (o < 0)
			return -CSP_ERR_CALL_UNK_FUNC;

		    newcode = &program[o];
		    args = newcode[0];
		    vars = newcode[1];
		    newsize = LD_UINT16(newcode, 2);

		    STACK_MOVE(-1);	/* pop argc */

		    if (args != STACK_VAL(0))
			return -CSP_ERR_CALL_ARGC_MISMATCH;

		    fr = create_new_frame(_frame, vars);

		    fr->argv = &STACK_VAL(-args);
		    fr->argc = args;
		    fr->ret_code = code;
		    fr->ret_size = size;

		    _frame = fr;
		    code = newcode + SIZE_OP_FUNC;
		    size = newsize;
		}

		break;
		}
	    }
	    break;
	case 3: /* 0xc0..0xff */
	    switch ((opcode>>4) - 0xc){
	    case 0: /* 0xc0..0xcf */
		break;
	    case 1: /* 0xd0..0xdf */
		break;
	    case 2:{ /* 0xe0..0xef */
		const uint32_t ucodes[] = { UCODE_IF_EXP, UCODE_LOOP_EXP, UCODE_LOOP_BODY, UCODE_LOOP_EXP };
		struct u_frame *uf;
		int tc;

		switch (opcode){
		case OP_IF:
		    TR("IF");
		    break;
		case OP_WHILE:
		    TR("WHILE");
		    break;
		case OP_DOWHILE:
		    TR("DOWHILE");
		    break;
		case OP_FOR:
		    TR("FOR");
		    break;
		}

		tc = opcode-0xe0;

		uf = create_new_uframe(_frame, ucodes[tc]);
		uf->ptr = code-1;
		code += op_sizes[tc]-1;

		break;
	    }
	    case 3: /* 0xf0..0xff */
		switch (opcode){
		case OP_CONTINUE:
		case OP_BREAK:
		    TR("   break/cont");
		    UFRAME(_frame)->ucode |= opcode << 6;
		    break;
		case OP_RETURN:{
		    struct vm_frame *f = _frame;
		    int val = STACK_VAL(-1);
		    TR("RET %d", val);

		    if (!HAVE_PREV_FRAME(_frame)){
			vm->func_call_result = val;
			return CSP_ERR_NONE;
		    }

		    code = f->ret_code;
		    size = f->ret_size;
		    _frame = PREV_FRAME(_frame);

		    STACK_MOVE(-f->argc + 1);
		    STACK_VAL(-1) = val;

		    break;
		}
		case POP_COUNT:
		    tmp = *code++;
		    goto dopoptmp;
		case POP_ONE:
		    tmp = 1;
		dopoptmp:
		    TR("pop %d", tmp);
		    STACK_MOVE(-tmp);
		    break;
		}
		break;
	    }
	    break;
	}

    }

    vm->func_call_result = 0;
    return CSP_ERR_NONE;
}



static int find_cache_item(int cachetype, int idx)
{
    int i;

    if (cachetype == CACHE_ID_MAP){
	if (idx < vm->maps_cnt)
	    return vm->cache_maps[idx].offset;
    } else if (cachetype == CACHE_ID_FUNC){
	for (i=0;i<vm->funcs_cnt;i++){
	    if (vm->cache_funcs[i].idx == idx){
		return vm->cache_funcs[i].offset;
	    }
	}
    }

    return -1;

}

int CSP_EXTERNAL csp_vm_init(uint8_t *vmbuf, int size)
{
    if (size < sizeof(struct vm))
	return -CSP_ERR_VM_GENERIC;

    vm = (struct vm*)vmbuf;
    memset(vm, 0, size);

    vm->vm_bufsize = size;

    return 0;
}

int CSP_EXTERNAL csp_vm_load_program(uint8_t *code_start, int progsize, int api_fncnt)
{
    struct cache_item *cache;
    uint8_t *code = code_start;
    int size, fsize, i;
    int p;
    int idx;
    char *vm_end;

    program = code_start;
    vm->api_func_cnt = api_fncnt + MACRO_OP_BUILTIN_COUNT;

    vm_end = ((char*)vm) + vm->vm_bufsize - sizeof(struct cache_item);

    size = LD_UINT16(program, 0);
    vm->globals_cnt = program[2];
    vm->maps_cnt = program[3];

    /* cache maps */
    cache =  (struct cache_item *)&vm->global_vars[vm->globals_cnt]; /* just after globals */
    vm->cache_maps = cache;
    p = PROGRAM_HEADER_SIZE;

    if ((char*)&cache[vm->maps_cnt] >= vm_end)
        return -CSP_ERR_VMBUF_OVERFLOW;

    idx = 0;
    for (i=0;i<vm->maps_cnt && p<size;i++){

	/* printf(": cache_maps %d 0x%02x\n", idx, p); */
	cache[idx].offset = p;
	cache[idx].idx = idx;
	idx++;

	if (code[p] == MAPPING_TYPE_ARRAY){
	    int min = code[p+2];
	    int max = code[p+3];
	    p += SIZE_STR_MAPPING + (max - min + 1);
	} else if (code[p] == MAPPING_TYPE_PAIRS){
	    int count = code[p+1];
	    p += SIZE_STR_MAPPING + (count * 2);
	} else {
	    return -CSP_ERR_MAP_INVALID;
	}
    }


    /* cache functions */
    cache = &vm->cache_maps[vm->maps_cnt]; /* just after maps cache */
    vm->cache_funcs = cache;
    idx = 0;

    while (p < size){
	int body_size;

	if ((char*)cache >= vm_end)
	    return -CSP_ERR_VMBUF_OVERFLOW;

	body_size = LD_UINT16(code, p+2);

	if (body_size == 0){
	    p += SIZE_OP_FUNC;
	    break;
	}

	/* printf(": cache_funcs %d 0x%02x %d\n", idx, p, idx+api_fncnt); */
	cache[idx].offset = p;
	cache[idx].idx = idx;
	idx++;

	fsize = body_size + SIZE_OP_FUNC;
	p += fsize;
    }
    vm->funcs_cnt = idx;

    vm->func_names = &code[p];
    vm->func_names_len = progsize - p;

#if CSP_ARRAYS_ENABLE && !CSP_ARRAYS_USE_MALLOC
    vm->arrays_ptr = (uint8_t*)&vm->cache_funcs[idx]; /* just after funcs cache */
    vm->tail_ptr = vm->arrays_ptr;
#endif
    return 0;
}

int CSP_EXTERNAL csp_vm_find_func(const char *name)
{
    uint8_t *p = vm->func_names;
    uint8_t *buf_end = p + vm->func_names_len;
    int idx = 0;
    int namelen;
    uint8_t len;

    len = *p++;
    namelen = strlen(name);

    while (len > 0 && (p < buf_end)){
	if ((len == namelen) && strncmp((char*)p, name, namelen) == 0){
	    return idx;
	}
	idx++;
	p += len;
	len = *p++;
    }

    return -1;
}

int CSP_EXTERNAL csp_vm_run_function(int stack_size, int fn, int argc, int *argv)
{
    uint8_t *vmbuf;
    uint8_t *code;
    int args, vars, size;
    int i, res;
    struct vm_frame *frame;


    i = find_function(fn);
    if (i<0)
	return -CSP_ERR_CALL_UNK_FUNC;

    code = &program[i];

    args = code[0];
    vars = code[1];
    size = LD_UINT16(code, 2);

    if (argc != args){
	return -CSP_ERR_CALL_ARGC_MISMATCH;
    }

    stack_size &= ~3;
    vmbuf = alloca(stack_size);

    frame = create_first_frame(vmbuf, stack_size, vars);

    frame->argv = argv;
    frame->argc = argc;

    res = vm_execute(frame, code+SIZE_OP_FUNC, size);

    return res;
}

int CSP_EXTERNAL csp_vm_get_call_result(void)
{
    return vm->func_call_result;
}

