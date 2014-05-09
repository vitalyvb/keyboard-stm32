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
#include "csp_api.h"
#include "csp_internal.h"
#include "gen.h"

#define ST_UINT16(ptr, offset) (*(uint16_t*)((ptr) + (offset)))
#define ST_INT16(ptr, offset) (*(int16_t*)((ptr) + (offset)))

#define ST_INT32(ptr, offset) (*(int32_t*)((ptr) + (offset)))

static uint8_t *prog;
static uint32_t pc;
static uint32_t pc_end;
static uint8_t globals_finished;
static uint32_t funcnames_offset;

static struct csp_names *global_vars;
static struct csp_names *local_vars;
static struct csp_names *funcnames;

/***************************************************************************/

int get_pc(int shift)
{
    int xpc = pc;
    pc += shift;
    return xpc;
}

int prog_check_free_space(void)
{
    /* check if there is enough place for a largest VM inctruction */
    if (pc < pc_end - LARGEST_INSTR_SIZE)
	return 0;

    return -1;
}


 

static int ns_get_idx(const char *name, int namelen, struct csp_names *ns)
{
    uint8_t *p = ns->buf;
    uint8_t *buf_end = p + ns->tail;
    int idx = 0;
    uint8_t len;

    len = *p++;
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

static int ns_register(const char *name, int namelen, struct csp_names *ns)
{
    int freespece = ns->size - ns->tail;

    if ((freespece-1) < namelen){
	csp_set_error(CSP_ERR_LEX_OUT_NAMESPACE, 0, "Out of namespace buffer");
	return -1;
    }

    ns->buf[ns->tail++] = namelen;
    strncpy((char*)(&ns->buf[ns->tail]), name, namelen);
    ns->tail += namelen;

    return ns->idx++;
}

static int ns_get_name(int idx, const char **name, int *namelen, struct csp_names *ns)
{
    uint8_t *p = ns->buf;
    uint8_t *buf_end = p + ns->tail;
    uint8_t len;

    len = *p++;
    while (len > 0 && (p < buf_end)){
	if (idx == 0){
	    if (name)
		*name = (char*)p;
	    if (namelen)
		*namelen = len;
	    return 0;
	}

	idx--;
	p += len;
	len = *p++;
    }

    return -1;
}


int global_var_get(const char *name, int namelen)
{
    return ns_get_idx(name, namelen, global_vars);
}

static int global_var_get_name(int idx, const char **name, int *namelen)
{
    return ns_get_name(idx, name, namelen, global_vars);
}

int global_var_reg(const char *name, int namelen)
{
    return ns_register(name, namelen, global_vars);
}


int local_var_get(const char *name, int namelen)
{
    int idx = ns_get_idx(name, namelen, local_vars);

    if (idx >= 0)
	idx += global_vars->idx;

    return idx;
}

int local_var_reg(const char *name, int namelen)
{
    return ns_register(name, namelen, local_vars) + global_vars->idx;
}

static int local_var_get_name(int idx, const char **name, int *namelen)
{
    return ns_get_name(idx, name, namelen, local_vars);
}

int visible_var_get(const char *name, int namelen)
{
    int idx = global_var_get(name, namelen);
    if (idx < 0)
	idx = local_var_get(name, namelen);
    return idx;
}

int visible_var_get_name(int idx, const char **name, int *namelen)
{
    int res = global_var_get_name(idx, name, namelen);
    if (res < 0)
	res = local_var_get_name(idx, name, namelen);
    return res;
}


int func_get(const char *name, int namelen)
{
    int idx = ns_get_idx(name, namelen, funcnames);
    if (idx >= 0)
	idx += MACRO_OP_BUILTIN_COUNT;
    return idx;
}

int func_reg(const char *name, int namelen)
{
    return ns_register(name, namelen, funcnames) + MACRO_OP_BUILTIN_COUNT;
}

int map_get(const char *name, int namelen)
{
    return ns_get_idx(name, namelen, global_vars);
}

int map_reg(const char *name, int namelen)
{
    return ns_register(name, namelen, global_vars);
}


/***************************************************************************/

void program_start(void)
{
    prog[0] = 0;		/* 16bit program size */
    prog[1] = 0;
    prog[2] = 0;		/* number of globals */
    prog[3] = 0;		/* number of maps */
    pc = PROGRAM_HEADER_SIZE;
}

void program_globals_finish(void)
{
    if (!globals_finished){
	prog[2] = global_vars->idx;
	prog[3] = global_vars->idx;
	globals_finished = 1;
    }
}

void program_finish(void)
{
    int nsize;
    pc += SIZE_OP_FUNC;

    nsize = funcnames->tail - funcnames_offset;
    memcpy(&prog[pc], &funcnames->buf[funcnames_offset], nsize);
    pc += nsize;

    ST_UINT16(prog, 0) = pc;

    prog[3] = global_vars->idx - prog[3];
}

/***************************************************************************/

int ctx_sw_func_reg(const char *name, int namelen)
{
    /* register function and switch parser context */
    int fn = func_reg(name, namelen);
    int xpc = get_pc(SIZE_OP_FUNC);

    program_globals_finish();

    prog[xpc+2] = fn; /* temporary store */

    local_vars->tail = 0;
    local_vars->idx = 0;
    local_vars->buf[0] = 0;

    return xpc;
}

int ctx_get_local_vars_count(void)
{
    return local_vars->idx;
}

void ctx_cur_func_finalize(int func_pc, int argn)
{
    int func_size = pc - func_pc - SIZE_OP_FUNC;

    prog[func_pc] = argn;	/* arguments count*/
    prog[func_pc+1] = local_vars->idx; /* total local variables count */
    ST_UINT16(prog, func_pc+2) = func_size;
}

/***************************************************************************/

int ctx_sw_map_reg(const char *name, int namelen)
{
    int mn;
    int xpc = get_pc(SIZE_STR_MAPPING);
    struct csp_mapping *map = (struct csp_mapping *)&prog[pc];

    program_globals_finish();

    if ((pc_end - pc) <= sizeof(struct csp_mapping)){
	csp_set_error(CSP_ERR_CODE_OVERFLOW, 0, "Out of program space");
	return -1;
    }

    mn = map_reg(name, namelen);

    prog[xpc+2] = mn; /* temporary store */

    memset(map, 0, sizeof(struct csp_mapping));
    map->min = MAPPING_SIZE-1;

    return xpc;

}

static int map_size_as_pairs(const struct csp_mapping *map)
{
    return map->count * 2;
}

static int map_size_as_array(const struct csp_mapping *map)
{
    return map->max - map->min + 1;
}

static void map_covert_to_pairs(const struct csp_mapping *map, uint8_t *dest)
{
    int i, max = map->max;
    int val;

    for (i=map->min; i<=max; i++) {
	val = map->data[i];

	if (val){
	    *dest++ = i;
	    *dest++ = val;
	}
    }
}

void ctx_cur_map_finalize(int map_pc)
{
    struct csp_mapping *map = (struct csp_mapping *)&prog[pc];
    int pairs_size, mapping_size;

    if (map->count <= 0){
	csp_set_error(CSP_ERR_EMPTY_MAP, 0, "Empty map");
	return;
    }

    pairs_size = map_size_as_pairs(map);
    mapping_size = map_size_as_array(map);

    /* compress mapping */
    if ((map->count > MAPPING_SIZE/4) || (pairs_size >= mapping_size)) {
	/* prefer array */
	prog[map_pc] = MAPPING_TYPE_ARRAY;
	prog[map_pc+1] = map->count;
	prog[map_pc+2] = map->min;
	prog[map_pc+3] = map->max;
	memmove(&prog[pc], &map->data[map->min], mapping_size);
	pc += mapping_size;
    } else {
	/* prefer pairs */
	prog[map_pc] = MAPPING_TYPE_PAIRS;
	prog[map_pc+1] = map->count;
	prog[map_pc+2] = map->min;
	prog[map_pc+3] = map->max;

	if (pairs_size < map->min){
	    /* convert inplace */
	    map_covert_to_pairs(map, &prog[pc]);
	} else {
	    uint8_t *buf = alloca(pairs_size);
	    map_covert_to_pairs(map, buf);
	    memcpy(&prog[pc], buf, pairs_size);
	}
	pc += pairs_size;
    }
}

void ctx_cur_map_add(int from, int to)
{
    struct csp_mapping *map = (struct csp_mapping *)&prog[pc];

    if (from >= MAPPING_SIZE || to >= MAPPING_SIZE || from < 0 || to < 0){
	csp_set_error(CSP_ERR_MAP_ITEM, 0, "Bad map item");
	return;
    }

    if (map->data[from] != to) {

	map->data[from] = to;
	map->count++;

	if (map->min > from)
	    map->min = from;
	if (map->max < from)
	    map->max = from;
    }
}

/***************************************************************************/

void push_num(long n)
{
    if (-32 <= n && n <= 31){
	prog[pc++] = OPCODE_IMMED_N | (n & 0x3f);
    } else if (INT8_MIN <= n && n <= INT8_MAX){
	prog[pc] = OPCODE_IMMED_8;
	prog[pc+1] = n;
	pc += 2;
    } else if (INT16_MIN <= n && n <= INT16_MAX){
	prog[pc] = OPCODE_IMMED_16;
	ST_INT16(prog, pc+1) = n;
	pc += 3;
    } else {
	prog[pc] = OPCODE_IMMED_32;
	ST_INT32(prog, pc+1) = n;
	pc += 5;
    }
}

void push_str(long n, const char *s)
{

}


void binary_op(int op)
{
    prog[pc++] = op;
}

void unary_op(int op)
{
    prog[pc++] = op;
}

void push_var(int varnum)
{
    if (varnum <= 63){
	prog[pc++] = PUSH_VARNUM_N | (varnum & 0x3f);
    } else {
	prog[pc] = PUSH_VARNUM;
	prog[pc+1] = varnum;
	pc += 2;
    }
}

void set_var(int varnum)
{
    prog[pc] = SET_VARNUM;
    prog[pc+1] = varnum;
    pc += 2;
}

void pop_count(int count)
{
    if (count == 1) {
	prog[pc++] = POP_ONE;
    } else {
	prog[pc] = POP_COUNT;
	prog[pc+1] = count;
	pc += 2;
    }
}

/***************************************************************************/

void op_if(int exp_start, int then_start, int else_start)
{
    int stmt_end = pc;
    int len_exp = then_start - exp_start - SIZE_OP_IF;
    int len_then = else_start - then_start;
    int len_else = stmt_end - else_start;

    prog[exp_start] = OP_IF;
    prog[exp_start+1] = len_exp;
    ST_UINT16(prog, exp_start+2) = len_then;
    ST_UINT16(prog, exp_start+4) = len_else;
}

void op_while(int exp_start, int body_start)
{
    int stmt_end = pc;
    int len_exp = body_start - exp_start - SIZE_OP_WHILE;
    int len_body = stmt_end - body_start;

    prog[exp_start] = OP_WHILE;
    prog[exp_start+1] = len_exp;
    ST_UINT16(prog, exp_start+2) = len_body;
}

void op_dowhile(int exp_start, int body_start)
{
    int stmt_end = pc;
    int len_body = exp_start - body_start - SIZE_OP_DOWHILE;
    int len_exp = stmt_end - exp_start;

    prog[body_start] = OP_DOWHILE;
    prog[body_start+1] = len_exp;
    ST_UINT16(prog, body_start+2) = len_body;
}

void op_for(int exp_start, int post_start, int body_start)
{
    int stmt_end = pc;
    int len_exp = post_start - exp_start - SIZE_OP_FOR;
    int len_post = body_start - post_start;
    int len_body = stmt_end - body_start;

    prog[exp_start] = OP_FOR;
    prog[exp_start+1] = len_exp;
    ST_UINT16(prog, exp_start+2) = len_body;
    prog[exp_start+4] = len_post;
}

void op_loopctl(int op)
{
    prog[pc++] = op;
}

void op_call(int funcn)
{
    prog[pc] = OP_CALL;
    prog[pc+1] = funcn;
    pc += 2;
}

void op_return(void)
{
    prog[pc++] = OP_RETURN;
}

/***************************************************************************/

void gen_init(uint8_t *prog_buffer, int size)
{
    prog = prog_buffer;

    memset(prog, 0, size);
    pc = 0;
    globals_finished = 0;

    /* XXX reserve program space for metadata */
    program_start();
    pc_end = size;

}

void gen_set_namespaces(struct csp_names *global, struct csp_names *functions, struct csp_names *local)
{
    global_vars = global;
    funcnames = functions;
    local_vars = local;

    funcnames_offset = funcnames->tail;
}

