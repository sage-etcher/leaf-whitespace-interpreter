/*
    This file is part of LeafWSI.

    LeafWSI - a C based, cross-platform, whitespace lang interpretter.
    Copyright (C) 2023  Sage I. Hendricks

    LeafWSI is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version.

    LeafWSI is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License 
    along with LeafWSI. If not, see <https://www.gnu.org/licenses/>. 
*/

/* 
Contact Information:
    Email   sage.codes@email.com
    Github  sage-etcher
    Discord sage4424
*/

/* only run once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _WSPROGRAM_H_
#define _WSPROGRAM_H_

/* configuration file */
#include "config.h"

/* include headers */
#include "ws_errors.h"
#include "hashmap.h"

/* include libraries */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


/* constants */
#define WS_MAX_INST_LEN 4

/* enum of whitespace instruction indexes */
/* works for WS_INST and ffor WS_INST_NAME */
typedef enum
{
    WS_PUSH,
    WS_DUP,
    WS_COPY,
    WS_SWAP,
    WS_POP,
    WS_SLIDE,
    WS_ADD,
    WS_SUB,
    WS_MULT,
    WS_DIV,
    WS_MOD,
    WS_STORE,
    WS_RESTORE,
    WS_LABEL,
    WS_CALL,
    WS_JMP,
    WS_JZ,
    WS_JN,
    WS_RET,
    WS_END,
    WS_PUTC,
    WS_PUTI,
    WS_READC,
    WS_READI,
    WS_DPRINT,
    WS_INST_COUNT
} WS_INST_INDEX;

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

typedef struct
{
    WS_INST_INDEX index;
    char *inst_name;
    char *inst_string;
    size_t inst_string_len;
    bool takes_parameter;
    wsError (*inst_function)(wsProgram*);
} wsInstDefinition;

/* global variables */
extern wsInstDefinition WS_INST[];


/* function prototypes */
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
wsError wsi_jmp            (wsProgram *program);
wsError wsi_jz             (wsProgram *program);
wsError wsi_jn             (wsProgram *program);
wsError wsi_ret            (wsProgram *program);
wsError wsi_end            (wsProgram *program);
wsError wsi_putc           (wsProgram *program);
wsError wsi_puti           (wsProgram *program);
wsError wsi_readc          (wsProgram *program);
wsError wsi_readi          (wsProgram *program);
/* debug functions */
wsError wsi_dprint         (wsProgram *program);

/* frees wsProgram variable */
void free_wsProgram (wsProgram *program);

#endif
