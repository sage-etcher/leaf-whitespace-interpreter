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

#define wsInt int32_t


/* helper functions */
int32_t get_parameter (char *file_contents, uint64_t *program_counter);
void print_stack (int32_t stack[], uint64_t stack_index);
void log_error (wsError error_code, wsInst instruction, uint64_t line_num, uint64_t char_num);
void inc_cursor_position (char character);

/* whitespace instruction set */
wsError wsi_push           (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_dup            (int32_t stack[], int32_t *stack_index);
wsError wsi_copy           (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_swap           (int32_t stack[], int32_t *stack_index);
wsError wsi_pop            (int32_t *stack_index);
wsError wsi_slide          (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_add            (int32_t stack[], int32_t *stack_index);
wsError wsi_sub            (int32_t stack[], int32_t *stack_index);
wsError wsi_mult           (int32_t stack[], int32_t *stack_index);
wsError wsi_idiv           (int32_t stack[], int32_t *stack_index);
wsError wsi_mod            (int32_t stack[], int32_t *stack_index);
wsError wsi_store          (int32_t stack[], int32_t *stack_index, hashMap *heap);
wsError wsi_restore        (int32_t stack[], int32_t *stack_index, hashMap *heap);
wsError wsi_label          (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_call           (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump           (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump_zero      (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_jump_negative  (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter);
wsError wsi_ret            (uint64_t *program_counter_index);
wsError wsi_end            (bool *runtime_bool);
wsError wsi_putc           (int32_t stack[], int32_t *stack_index);
wsError wsi_puti           (int32_t stack[], int32_t *stack_index);
wsError wsi_readc          (int32_t stack[], int32_t *stack_index);
wsError wsi_readi          (int32_t stack[], int32_t *stack_index);

#endif
