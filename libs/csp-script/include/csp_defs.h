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

#ifndef CSC_PARSER_COMMON_H
#define CSC_PARSER_COMMON_H

/*
 * Text buffer size for a lexical analyzer.
 * Identifier/constant/keyword - anything can not exceed this length.
 * Lexer will return an overflow condition and stop if buffer is exhausted.
 */
#define CSP_LEX_BUFSIZE 64

/*
 * Some error conditions will return a string describing an error.
 */
#define CSP_STRING_ERRORS 1

/*
 * If CSP_STRING_ERRORS is set, this specifies a message buffer size.
 */
#define CSP_ERRSTR_SIZE 128

/*
 * Defines initial and maximal parser stack size. The larger value set
 * here the more complex input parser will be able to handle
 * successfully and more memory (program stack space) it'll
 * require.
 */
#define CSP_PARSER_STACK_DEPTH 192

/*
 * Size of all names in a namespace in a program. Buffer is automatically
 * adjusted for builtin globals and function names;
 */
#define CSP_NAMESPACE_SIZE 256

/*
 * Enable support for arrays
 */
#define CSP_ARRAYS_ENABLE 1

/*
 * If support for arrays is enabled, use malloc() to allocate buffer
 * for arrays. If not set, vmbuf free space is used.
 */
#define CSP_ARRAYS_USE_MALLOC 0
 
/**********************************************************/

#ifndef CSP_LEX_BUFSIZE
#define CSP_LEX_BUFSIZE 64
#endif

#ifndef CSP_STRING_ERRORS
#define CSP_STRING_ERRORS 0
#endif

#ifndef CSP_PARSER_STACK_DEPTH
#define CSP_PARSER_STACK_DEPTH 192
#endif

#ifndef CSP_ARRAYS_ENABLE
#define CSP_ARRAYS_ENABLE 0
#endif

#ifndef CSP_ARRAYS_USE_MALLOC
#define CSP_ARRAYS_USE_MALLOC 0
#endif

#define CSP_NS_GLOBAL_NAMES_SIZE CSP_NAMESPACE_SIZE
#define CSP_NS_FUNCTIONS_NAMES_SIZE CSP_NAMESPACE_SIZE
#define CSP_NS_LOCAL_NAMES_SIZE CSP_NAMESPACE_SIZE

/**********************************************************/

#ifdef  BUILDING_CSP
#define CSP_EXTERNAL __attribute__((externally_visible))
#else
#define CSP_EXTERNAL
#endif

#define CSP_ERR_NONE 0
/* lexer errors */
#define CSP_ERR_LEX_BAD_NUMBER 1
#define CSP_ERR_LEX_OVERFLOW 3
#define CSP_ERR_LEX_BAD_TOKEN 4
#define CSP_ERR_LEX_OUT_NAMESPACE 5
#define CSP_ERR_LEX_CONST_UNDEF 6
/* parser errors */
#define CSP_ERR_PARSER_FAILURE 16 /* likely out of parser stack space */
#define CSP_ERR_PARSE_ERROR 17
/* code generator errors */
#define CSP_ERR_CODE_OVERFLOW 32
#define CSP_ERR_EMPTY_MAP 33
#define CSP_ERR_MAP_ITEM 34
/* vm errors */
#define CSP_ERR_VM_GENERIC 1
#define CSP_ERR_MAP_NOT_FOUND 2
#define CSP_ERR_MAP_INVALID 3
#define CSP_ERR_MAP_CANT_SET 4
#define CSP_ERR_NOARRAY_GET 5
#define CSP_ERR_NOARRAY_SET 6
#define CSP_ERR_MAP_CANT_SET_VALUE 7
#define CSP_ERR_STACK_OVERFLOW 8
#define CSP_ERR_UNEXP_BRK_CONT 9
#define CSP_ERR_IE_UNK_UF 10
#define CSP_ERR_CALL_UNK_FUNC 11
#define CSP_ERR_CALL_ARGC_MISMATCH 12
#define CSP_ERR_VMBUF_OVERFLOW 13
#define CSP_ERR_DIV_BY_ZERO 14
#define CSP_ERR_OUT_OF_BOUND 15
#define CSP_ERR_ARRAY_TYPE 16
#define CSP_ERR_NO_MEMORY 17

#endif /* CSC_PARSER_COMMON_H */

