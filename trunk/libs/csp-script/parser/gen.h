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

#ifndef GEN_H
#define GEN_H

#include "csp_api.h"

/***************************************************/

struct array_descr {
    uint8_t type;
    uint8_t _reserved;
    uint16_t count;
};

/***************************************************/

int global_var_reg(const char *name, int namelen);

int local_var_get(const char *name, int namelen);
int local_var_reg(const char *name, int namelen);

int visible_var_get(const char *name, int namelen);
int visible_var_get_name(int idx, const char **name, int *namelen);

int func_get(const char *name, int namelen);
int func_reg(const char *name, int namelen);

int get_pc(int shift);
/***************************************************/

int prog_check_free_space(void);
void program_finish(void);

/***************************************************/

#define LARGEST_INSTR_SIZE	6
#define PROGRAM_HEADER_SIZE	4

#define OPCODE_XMASK1		(0xc0)
#define OPCODE_XSHIFT		6
#define OPCODE_VALMASK1		(0x3f)

#define OPCODE_IMMED_N		(0<<6)	/* 6 lower bits - signed value, values: 0..63 */
#define PUSH_VARNUM_N		(1<<6)	/* 6 lower bits - varuable number, values: 64..127 */

#define OPCODE_IMMED_8		0x80	/* 8 bit signed */
#define OPCODE_IMMED_16		0x81	/* 16 bit signed, LE */
#define OPCODE_IMMED_32		0x82	/* 32 bit signed, LE */
#define OPCODE_IMMED_STR_N	0x83	/* reserved, string */
void push_num(long n);
void push_str(long n, const char *s);

#define PUSH_VARNUM		0x84	/* 8 bit variable number */
void push_var(int varnum);
#define SET_VARNUM		0x85	/* 8 bit variable number */
void set_var(int varnum);


#define OP_BIN_ADD		0x90
#define OP_BIN_SUB		0x91
#define OP_BIN_MUL		0x92
#define OP_BIN_DIV		0x93
#define OP_BIN_GR		0x94
#define OP_BIN_LS		0x95
#define OP_BIN_GE		0x96
#define OP_BIN_LE		0x97
#define OP_BIN_EQ		0x98
#define OP_BIN_NE		0x99
#define OP_BIN_OROR		0x9a
#define OP_BIN_ANDAND		0x9b
#define OP_BIN_OR		0x9c
#define OP_BIN_AND		0x9d
#define OP_BIN_XOR		0x9e
#define OP_BIN_MOD		0x9f
void binary_op(int op);

#define OP_UN_NEG		0xa0
#define OP_UN_NOT		0xa1
#define OP_UN_BITNEG		0xa2
void unary_op(int op);


#define OP_CALL			0xb0	/* 8 bit funciton number */
void op_call(int funcn);



#define OP_IF			(0xe0|0)
#define SIZE_OP_IF		6
void op_if(int exp_start, int then_start, int else_start);

#define OP_WHILE		(0xe0|1)
#define SIZE_OP_WHILE		4
void op_while(int exp_start, int body_start);

#define OP_DOWHILE		(0xe0|2)
#define SIZE_OP_DOWHILE		4
void op_dowhile(int exp_start, int body_start);

#define OP_FOR			(0xe0|3)
#define SIZE_OP_FOR		5
void op_for(int exp_start, int post_start, int body_start);


#define OP_CONTINUE		(0xf8|1)
#define OP_BREAK		(0xf8|2)
void op_loopctl(int op);

#define OP_RETURN		0xfd
void op_return(void);

#define POP_COUNT		0xfe
#define POP_ONE			0xff
void pop_count(int count);

/***************************************************/

#define MACRO_OP_FUNC_GETIDX 0
#define MACRO_OP_FUNC_SETIDX 1
#define MACRO_OP_FUNC_RESERVED1 2
#define MACRO_OP_FUNC_RESERVED2 3
#define MACRO_OP_BUILTIN_COUNT 4

#define SIZE_OP_FUNC 4
int ctx_sw_func_reg(const char *name, int namelen);
int ctx_get_local_vars_count(void);
void ctx_cur_func_finalize(int func_pc, int argn);

/***************************************************/

#define SIZE_STR_MAPPING 4
#define MAPPING_TYPE_ARRAY 1
#define MAPPING_TYPE_PAIRS 2
#define MAPPING_TYPE_ARR_BIT 0x10
#define MAPPING_TYPE_ARR_CHAR 0x11
#define MAPPING_TYPE_ARR_SHORT 0x12
#define MAPPING_TYPE_ARR_INT 0x13

int ctx_sw_map_reg(const char *name, int namelen);
void ctx_cur_map_finalize(int func_pc);
void ctx_cur_map_add(int from, int to);

/***************************************************/

void gen_init(uint8_t *prog_buffer, int size);
void gen_set_namespaces(struct csp_names *global, struct csp_names *functions, struct csp_names *local);

#endif /* GEN_H */
