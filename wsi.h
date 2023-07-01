/* only run once */
/* modern */
#pragma once
/* legacy backup */
#ifndef _WSI_H_
#define _WSI_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

#include "fileio.h"
#include "utils.h"
#include "whitespace.h"
#include "config.h"
#include "wserr.h"
#include "hashmap.h"

/* typedefinition for default int size for whitespace script */
typedef int32_t wsInt;

/* whitespace instruction container */
typedef struct 
{
    WS_INST_INDEX instruction;
    wsInt parameter;

    uint64_t line_pos;
    uint64_t char_pos;
} wsInstruction;


/* program container */
typedef struct
{
    /* program instructions */
    wsInstruction **instruction_list;
    uint64_t list_len;
    uint64_t list_count;

    /* program runtime stack */
    wsInt stack[STACK_LEN];
    wsInt stack_index;

    /* program runtime heap */
    hashMap *heap;
} wsProgram;


/* helper functions */

wsInt get_parameter (char *file_contents, uint64_t *program_counter);
wsError interpret_file (char *file_contents, wsProgram *program);
void inc_cursor_position (char character);
void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num);
void print_stack (wsInt stack[], wsInt stack_index);
void free_wsProgram (wsProgram *program);

/* whitespace instruction set */
wsError wsi_push           (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_dup            (wsInt stack[], wsInt *stack_index);
wsError wsi_copy           (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_swap           (wsInt stack[], wsInt *stack_index);
wsError wsi_pop            (wsInt *stack_index);
wsError wsi_slide          (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_add            (wsInt stack[], wsInt *stack_index);
wsError wsi_sub            (wsInt stack[], wsInt *stack_index);
wsError wsi_mult           (wsInt stack[], wsInt *stack_index);
wsError wsi_idiv           (wsInt stack[], wsInt *stack_index);
wsError wsi_mod            (wsInt stack[], wsInt *stack_index);
wsError wsi_store          (wsInt stack[], wsInt *stack_index, hashMap *heap);
wsError wsi_restore        (wsInt stack[], wsInt *stack_index, hashMap *heap);
wsError wsi_label          (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_call           (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump           (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump_zero      (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump_negative  (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_ret            (uint64_t *program_counter_index);
wsError wsi_end            (bool *runtime_bool);
wsError wsi_putc           (wsInt stack[], wsInt *stack_index);
wsError wsi_puti           (wsInt stack[], wsInt *stack_index);
wsError wsi_readc          (wsInt stack[], wsInt *stack_index);
wsError wsi_readi          (wsInt stack[], wsInt *stack_index);

#endif
