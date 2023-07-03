/* run only once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _WHITESPACE_H_
#define _WHITESPACE_H_

#include <stdbool.h>


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


typedef struct
{
    WS_INST_INDEX index;
    char *inst_name;
    char *inst_string;
    size_t inst_string_len;
    bool takes_parameter;
} wsInstDefinition;

#define WS_MAX_INST_LEN 4

const wsInstDefinition WS_INST[] = 
{
    /* index,       inst_name,      inst,      len  param         comment  */
    { WS_PUSH,      "WS_PUSH",      "  ",       2,  true  },  /*  SS  N L  */ 
    { WS_DUP,       "WS_DUP",       " \n ",     3,  false },  /*  SLS      */ 
    { WS_COPY,      "WS_COPY",      " \t ",     3,  true  },  /*  STS N L  */ 
    { WS_SWAP,      "WS_SWAP",      " \n\t",    3,  false },  /*  SLT      */ 
    { WS_POP,       "WS_POP",       " \n\n",    3,  false },  /*  SLL      */ 
    { WS_SLIDE,     "WS_SLIDE",     " \t\n",    3,  true  },  /*  STL N L  */ 
    { WS_ADD,       "WS_ADD",       "\t   ",    4,  false },  /*  TSSS     */ 
    { WS_SUB,       "WS_SUB",       "\t  \t",   4,  false },  /*  TSST     */ 
    { WS_MULT,      "WS_MULT",      "\t  \n",   4,  false },  /*  TSSL     */ 
    { WS_DIV,       "WS_DIV",       "\t \t ",   4,  false },  /*  TSTS     */ 
    { WS_MOD,       "WS_MOD",       "\t \t\t",  4,  false },  /*  TSTT     */ 
    { WS_STORE,     "WS_STORE",     "\t\t ",    3,  false },  /*  TTS      */ 
    { WS_RESTORE,   "WS_RESTORE",   "\t\t\t",   3,  false },  /*  TTT      */ 
    { WS_LABEL,     "WS_LABEL",     "\n  ",     3,  true  },  /*  LSS N L  */ 
    { WS_CALL,      "WS_CALL",      "\n \t",    3,  true  },  /*  LST N L  */ 
    { WS_JMP,       "WS_JMP",       "\n \n",    3,  true  },  /*  LSL N L  */ 
    { WS_JZ,        "WS_JZ",        "\n\t ",    3,  true  },  /*  LTS N L  */ 
    { WS_JN,        "WS_JN",        "\n\t\t",   3,  true  },  /*  LTT N L  */ 
    { WS_RET,       "WS_RET",       "\n\t\n",   3,  false },  /*  LTL      */ 
    { WS_END,       "WS_END",       "\n\n\n",   3,  false },  /*  LLL      */ 
    { WS_PUTC,      "WS_PUTC",      "\t\n  ",   4,  false },  /*  TLSS     */ 
    { WS_PUTI,      "WS_PUTI",      "\t\n \t",  4,  false },  /*  TLST     */ 
    { WS_READC,     "WS_READC",     "\t\n\t ",  4,  false },  /*  TLTS     */ 
    { WS_READI,     "WS_READI",     "\t\n\t\t", 4,  false },  /*  TLTT     */ 
    { WS_DPRINT,    "WS_DPRINT",    "\t\t\n",   3,  false }   /*  TTL      */ 
};


#endif
