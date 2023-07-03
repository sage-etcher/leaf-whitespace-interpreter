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
    wsInstruction *instructions;
    uint64_t size;
    uint64_t count;

    /* holds information on where in the instructions list we are durring runtime*/
    uint64_t program_control[STACK_LEN];
    uint64_t program_control_index;
    /* holds a pointer to the current executing instruction */
    wsInstruction *current_instruction;

    /* program runtime stack */
    wsInt stack[STACK_LEN];
    wsInt stack_index;

    /* program runtime heap */
    hashMap *heap;

    /* runtime exit sequence */
    bool exit;
} wsProgram;


/* helper functions */

wsError get_parameter (char *file_contents, uint64_t *file_cursor, wsInt *return_address);
/* wsInt get_parameter (char *file_contents, uint64_t *program_counter); */
wsError interpret_file (char *file_contents, wsProgram *program);
void inc_cursor_position (char character);
void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num);
void print_stack (wsInt stack[], wsInt stack_index);
void free_wsProgram (wsProgram *program);

/* whitespace instruction set */
wsError wsi_push           (wsProgram *program);
wsError wsi_dup            (wsProgram *program);
wsError wsi_copy           (wsProgram *program);
wsError wsi_swap           (wsProgram *program);
wsError wsi_pop            (wsProgram *program);
wsError wsi_slide          (wsProgram *program);
wsError wsi_add            (wsProgram *program);
wsError wsi_sub            (wsProgram *program);
wsError wsi_mult           (wsProgram *program);
wsError wsi_idiv           (wsProgram *program);
wsError wsi_mod            (wsProgram *program);
wsError wsi_store          (wsProgram *program);
wsError wsi_restore        (wsProgram *program);
wsError wsi_label          (wsProgram *program);
wsError wsi_call           (wsProgram *program);
wsError wsi_jump           (wsProgram *program);
wsError wsi_jump_zero      (wsProgram *program);
wsError wsi_jump_negative  (wsProgram *program);
wsError wsi_ret            (wsProgram *program);
wsError wsi_end            (wsProgram *program);
wsError wsi_putc           (wsProgram *program);
wsError wsi_puti           (wsProgram *program);
wsError wsi_readc          (wsProgram *program);
wsError wsi_readi          (wsProgram *program);

#endif
