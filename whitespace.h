/* run only once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _WHITESPACE_H_
#define _WHITESPACE_H_

#include <string.h>

typedef enum WS_INSTRUCTION_INDEX
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
    WS_NONE,
    WS_INST_COUNT
} wsInst;

const char *WS_INST_NAME[] =
{
    "WS_PUSH",
    "WS_DUP",
    "WS_COPY",
    "WS_SWAP",
    "WS_POP",
    "WS_SLIDE",
    "WS_ADD",
    "WS_SUB",
    "WS_MULT",
    "WS_DIV",
    "WS_MOD",
    "WS_STORE",
    "WS_RESTORE",
    "WS_LABEL",
    "WS_CALL",
    "WS_JMP",
    "WS_JZ",
    "WS_JN",
    "WS_RET",
    "WS_END",
    "WS_PUTC",
    "WS_PUTI",
    "WS_READC",
    "WS_READ",
    "WS_DPRINT",
    ""
};

const char *WS_INST[] =
{
/* WS_PUSH    */ "  ",         /*  SS  N L  push [num] onto stack */
/* WS_DUP     */ " \n ",       /*  SLS      duplicate top value on stack */
/* WS_COPY    */ " \t ",       /*  STS N L  copy the [n]th element on stack to the top */
/* WS_SWAP    */ " \n\t",      /*  SLT      swaps the top 2 elements of the stack */
/* WS_POP     */ " \n\n",      /*  SLL      destroys top element on the stack */
/* WS_SLIDE   */ " \t\n",      /*  STL N L  destroy [n] elements off the stack, keep top element */
/* WS_ADD     */ "\t   ",      /*  TSSS     addition */
/* WS_SUB     */ "\t  \t",     /*  TSST     subtraction */
/* WS_MULT    */ "\t  \n",     /*  TSSL     multiplication */
/* WS_DIV     */ "\t \t ",     /*  TSTS     integer division */
/* WS_MOD     */ "\t \t\t",    /*  TSTT     modulo */
/* WS_STORE   */ "\t\t ",      /*  TTS      store in heap */
/* WS_RESTORE */ "\t\t\t",     /*  TTT      restore from heap */
/* WS_LABEL   */ "\n  ",       /*  LSS N L  mark a label in the program */
/* WS_CALL    */ "\n \t",      /*  LST N L  call a subroutine to label */
/* WS_JMP     */ "\n \n",      /*  LSL N L  jump to label */
/* WS_JZ      */ "\n\t ",      /*  LTS N L  jump to label, if top of stack is zero */
/* WS_JN      */ "\n\t\t",     /*  LTT N L  jump to label, if top of stack is negative */
/* WS_RET     */ "\n\t\n",     /*  LTL      end subroutine, return to caller */
/* WS_END     */ "\n\n\n",     /*  LLL      end the program */
/* WS_PUTC    */ "\t\n  ",     /*  TLSS     output top of stack as character to stdout */
/* WS_PUTI    */ "\t\n \t",    /*  TLST     output top of stack as integer to stdout */
/* WS_READC   */ "\t\n\t ",    /*  TLTS     read a character from stdin */
/* WS_READI   */ "\t\n\t\t",   /*  TLTT     read an integer from stdin */

/* WS_DPRINT  */ "\t\t\n",     /*  TTL      prints the stack to the screen for debugging purposes */
/* WS_NONE    */ ""
};

/* longest WhiteSpace command (not counting param), 0 - 255 */
#define WS_MAX_COMMAND_LEN 4

size_t WS_INST_LEN[] =
{
/* WS_PUSH    */ 2,
/* WS_DUP     */ 3,
/* WS_COPY    */ 3,
/* WS_SWAP    */ 3,
/* WS_POP     */ 3,
/* WS_SLIDE   */ 3,
/* WS_ADD     */ 4,
/* WS_SUB     */ 4,
/* WS_MULT    */ 4,
/* WS_DIV     */ 4,
/* WS_MOD     */ 4,
/* WS_STORE   */ 3,
/* WS_RESTORE */ 3,
/* WS_LABEL   */ 3,
/* WS_CALL    */ 3,
/* WS_JMP     */ 3,
/* WS_JZ      */ 3,
/* WS_JN      */ 3,
/* WS_RET     */ 3,
/* WS_END     */ 3,
/* WS_PUTC    */ 4,
/* WS_PUTI    */ 4,
/* WS_READC   */ 4,
/* WS_READI   */ 4,

/* WS_DPRINT  */ 3,
/* WS_NONE    */ 0
};


#endif
