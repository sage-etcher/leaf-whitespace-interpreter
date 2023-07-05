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

#include "ws_program.h"

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


/* globals */
wsInstDefinition WS_INST[] =
{
    /* index,      inst_name,     inst,       len  param   function         comment      */
    { /*WS_PUSH,   */  "WS_PUSH",     "  ",        2,  true,   wsi_push     },  /*  SS  N L  */
    { /*WS_DUP,    */  "WS_DUP",      " \n ",      3,  false,  wsi_dup      },  /*  SLS      */
    { /*WS_COPY,   */  "WS_COPY",     " \t ",      3,  true,   wsi_copy     },  /*  STS N L  */
    { /*WS_SWAP,   */  "WS_SWAP",     " \n\t",     3,  false,  wsi_swap     },  /*  SLT      */
    { /*WS_POP,    */  "WS_POP",      " \n\n",     3,  false,  wsi_pop      },  /*  SLL      */
    { /*WS_SLIDE,  */  "WS_SLIDE",    " \t\n",     3,  true,   wsi_slide    },  /*  STL N L  */
    { /*WS_ADD,    */  "WS_ADD",      "\t   ",     4,  false,  wsi_add      },  /*  TSSS     */
    { /*WS_SUB,    */  "WS_SUB",      "\t  \t",    4,  false,  wsi_sub      },  /*  TSST     */
    { /*WS_MULT,   */  "WS_MULT",     "\t  \n",    4,  false,  wsi_mult     },  /*  TSSL     */
    { /*WS_DIV,    */  "WS_DIV",      "\t \t ",    4,  false,  wsi_idiv     },  /*  TSTS     */
    { /*WS_MOD,    */  "WS_MOD",      "\t \t\t",   4,  false,  wsi_mod      },  /*  TSTT     */
    { /*WS_STORE,  */  "WS_STORE",    "\t\t ",     3,  false,  wsi_store    },  /*  TTS      */
    { /*WS_RESTORE,*/  "WS_RESTORE",  "\t\t\t",    3,  false,  wsi_restore  },  /*  TTT      */
    { /*WS_LABEL,  */  "WS_LABEL",    "\n  ",      3,  true,   wsi_label    },  /*  LSS N L  */
    { /*WS_CALL,   */  "WS_CALL",     "\n \t",     3,  true,   wsi_call     },  /*  LST N L  */
    { /*WS_JMP,    */  "WS_JMP",      "\n \n",     3,  true,   wsi_jmp      },  /*  LSL N L  */
    { /*WS_JZ,     */  "WS_JZ",       "\n\t ",     3,  true,   wsi_jz       },  /*  LTS N L  */
    { /*WS_JN,     */  "WS_JN",       "\n\t\t",    3,  true,   wsi_jn       },  /*  LTT N L  */
    { /*WS_RET,    */  "WS_RET",      "\n\t\n",    3,  false,  wsi_ret      },  /*  LTL      */
    { /*WS_END,    */  "WS_END",      "\n\n\n",    3,  false,  wsi_end      },  /*  LLL      */
    { /*WS_PUTC,   */  "WS_PUTC",     "\t\n  ",    4,  false,  wsi_putc     },  /*  TLSS     */
    { /*WS_PUTI,   */  "WS_PUTI",     "\t\n \t",   4,  false,  wsi_puti     },  /*  TLST     */
    { /*WS_READC,  */  "WS_READC",    "\t\n\t ",   4,  false,  wsi_readc    },  /*  TLTS     */
    { /*WS_READI,  */  "WS_READI",    "\t\n\t\t",  4,  false,  wsi_readi    },  /*  TLTT     */
    { /*WS_DPRINT, */  "WS_DPRINT",   "\t\t\n",    3,  false,  wsi_dprint   }   /*  TTL      */
};


/* functions */
/* whitespace instructions */
wsError wsi_push(wsProgram *program)
{
    wsInt new_item;

    /* cannot add item to stack if stack is full */
    if (program->stack_index >= STACK_LEN)
        return WS_ERR_FULLSTACK;

    /* get new item from the push command's paramter */
    new_item = program->current_instruction->parameter;

    /* add an element to the top of the stack */
    program->stack[program->stack_index] = new_item;
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_dup(wsProgram *program)
{
    /* stack must have atleast 1 element */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;
    /* cannot duplicate item on stack if stack is full */
    else if (program->stack_index >= STACK_LEN)
        return WS_ERR_FULLSTACK;

    /* duplicate the top element on the stack */
    program->stack[program->stack_index] = program->stack[program->stack_index - 1];
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_copy(wsProgram *program)
{
    wsInt copy_index;

    /* must have atleast 1 element on the stack */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;
    /* cannot copy a new item to the stack if the stack is full */
    else if (program->stack_index >= STACK_LEN)
        return WS_ERR_FULLSTACK;

    /* get the index of the element we want to copy from the paramter */
    copy_index = program->current_instruction->parameter;

    /* check if copy index is out of range */
    if (copy_index >= program->stack_index ||
        copy_index < 0)
        return WS_ERR_OUTOFRANGE;

    /* copy the item at that index to the top of the stack */
    program->stack[program->stack_index] = program->stack[copy_index];
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_swap(wsProgram *program)
{
    wsInt item_overhead;

    /* cannot swap top two elements if there arent atleast 2 on the stack */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* get top item from stack and store in an overhead variable */
    item_overhead = program->stack[program->stack_index - 1];

    /* set the element 2nd from the top, to the top place */
    program->stack[program->stack_index - 1] = program->stack[program->stack_index - 2];
    /* set origonal top of stack to be the 2nd from the top, using overhead */
    program->stack[program->stack_index - 2] = item_overhead;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_pop(wsProgram *program)
{
    /* if there is not atleast 1 element in the stack, then we cant pop anything */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;

    /* move the stack index back one place */
    /* no need to clear the popped value, as only way to go foreward in stack
     * is to rewrite a new value to it */
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_slide(wsProgram *program)
{
    wsInt slide_n;

    /* check that the stack has atleast 1 element. even with a slide 0, this
     * function requires there to be atleast one element for the top of the
     * stack */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;

    /* get the number of elements we want to slide off the stack, from the
     * given instruction parameter */
    slide_n = program->current_instruction->parameter;

    /* check that slide_n is in a valid range */
    if (slide_n >= program->stack_index ||
        slide_n < 0)
        return WS_ERR_OUTOFRANGE;

    /* put old top value into the new top index after a slide */
    /* magickal fuckery, but it works. but it works soo :/ */
    program->stack[program->stack_index - (1 + slide_n)] = program->stack[program->stack_index - 1];
    /* move index to the new top */
    program->stack_index -= slide_n;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_add(wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* add the top element of the stack to the 2nd to the top */
    program->stack[program->stack_index - 2] += program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_sub(wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* subtract the 2nd element of the stack by the top element */
    program->stack[program->stack_index - 2] -= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_mult(wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* multiply the top element of the stack by the 2nd to the top */
    program->stack[program->stack_index - 2] *= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_idiv(wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* divide the element 2nd from the top by the top element */
    program->stack[program->stack_index - 2] /= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_mod(wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* get the remainder/modulo of dividing the element 2nd from the top by the
     * top element */
    program->stack[program->stack_index - 2] %= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_store(wsProgram *program)
{
    wsInt value;
    wsInt key_int;
    char *key_string;

    /* guard clauses */
    if (program->stack_index < 2)
        return WS_ERR_TOOFEWITEMS;

    /* get the key and value from the stack */
    /* top item is key */
    /* 2nd to top is value */
    key_int = program->stack[program->stack_index - 2];
    value = program->stack[program->stack_index - 1];

    /* take the top 2 items off the stack */
    program->stack_index -= 2;

    /* allocate and convert int into key_string */
    key_string = (char *)malloc((get_places(key_int, 10) + 1) * sizeof(char));
    sprintf(key_string, "%d", key_int);

    /* add pair to the heap */
    hash_set(program->heap, key_string, &value, sizeof(value));

    /* free the temporary key */
    free(key_string);
    return WS_SUCCESS;
}

wsError wsi_restore(wsProgram *program)
{
    wsInt value;
    wsInt key_int;
    char *key_string;
    bool match_found;

    /* get the key from the stack */
    key_int = program->stack[program->stack_index - 1];

    /* allocate and convert int into key_string */
    key_string = (char *)malloc((get_places(key_int, 10) + 1) * sizeof(char));
    sprintf(key_string, "%d", key_int);

    /* look for a match */
    match_found = hash_search(program->heap, key_string, &value);

    /* if no match is found return with error code */
    if (!match_found)
        return WS_ERR_NOMATCH;

    /* add item to stack */
    program->stack[program->stack_index - 1] = value;

    /* free key_string */
    free(key_string);
    return WS_SUCCESS;
}

/* not implemented start */
/* need to figure out how to get labels to work */
wsError wsi_label(wsProgram *program)
{
    /* i dont think we need to do anything here, we should be able to just
     * skip over this */

    /* old */
    /* printf("labels and calls are not yet implemented\n"); */

    return WS_SUCCESS;
}

wsError wsi_call(wsProgram *program)
{
    uint64_t i = 0;

    /* loop through the label_indexes list */
    for (; i < program->label_count; i++)
    {
        /* if call param matches the parameter of the label at label_index */
        if (program->current_instruction->parameter == program->instructions[program->label_indexes[i]].parameter)
        {
            /* increment program_control_index */
            program->program_control_index++;
            /* set new program_control at the instruction label index */
            program->program_control[program->program_control_index] = program->label_indexes[i];

            /* return error success */
            return WS_SUCCESS;
        }
    }

    /* return error that no label was found */
    return WS_ERR_NOLABELFOUND;
}

wsError wsi_jmp(wsProgram *program)
{
    uint64_t i = 0;

    /* loop through the label_indexes list */
    for (; i < program->label_count; i++)
    {
        /* if jump param matches the parameter of the label at label_index */
        if (program->current_instruction->parameter == program->instructions[program->label_indexes[i]].parameter)
        {
            /* set new program_control at the instruction label index */
            program->program_control[program->program_control_index] = program->label_indexes[i];

            /* return error success */
            return WS_SUCCESS;
        }
    }

    /* return error that no label was found */
    return WS_ERR_NOLABELFOUND;
}

wsError wsi_jz(wsProgram *program)
{
    uint64_t i = 0;

    /* make sure there is atleast 1 item in the stack to compare against */
    if (program->inst_count < 1)
        return WS_ERR_TOOFEWITEMS;

    /* pop the top element of the stack */
    program->stack_index--;

    /* if top of stack isn't a zero, return early without jumping */
    if (!(program->stack[program->stack_index] == 0))
        return WS_SUCCESS;

    /* loop through the label_indexes list */
    for (; i < program->label_count; i++)
    {
        /* if jump param matches the parameter of the label at label_index */
        if (program->current_instruction->parameter == program->instructions[program->label_indexes[i]].parameter)
        {
            /* set new program_control at the instruction label index */
            program->program_control[program->program_control_index] = program->label_indexes[i];

            /* return error success */
            return WS_SUCCESS;
        }
    }

    /* return error that no label was found */
    return WS_ERR_NOLABELFOUND;
}

wsError wsi_jn(wsProgram *program)
{
    uint64_t i = 0;

    /* make sure there is atleast 1 item in the stack to compare against */
    if (program->inst_count < 1)
        return WS_ERR_TOOFEWITEMS;

    /* pop the top element of the stack */
    program->stack_index--;

    /* if top of stack isn't negative return early without jumping */
    if (!(program->stack[program->stack_index] < 0))
        return WS_SUCCESS;

    /* loop through the label_indexes list */
    for (; i < program->label_count; i++)
    {
        /* if jump param matches the parameter of the label at label_index */
        if (program->current_instruction->parameter == program->instructions[program->label_indexes[i]].parameter)
        {
            /* set new program_control at the instruction label index */
            program->program_control[program->program_control_index] = program->label_indexes[i];

            /* return error success */
            return WS_SUCCESS;
        }
    }

    /* return error that no label was found */
    return WS_ERR_NOLABELFOUND;
}
/* not implemented end */

wsError wsi_ret(wsProgram *program)
{
    /* cant return from the main process */
    if (program->program_control_index <= 0)
        return WS_ERR_RETURNFROMMAIN;

    program->program_control_index--;

    return WS_SUCCESS;
}

wsError wsi_end(wsProgram *program)
{
    program->exit = false;

    return WS_SUCCESS;
}

wsError wsi_putc(wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index <= 0)
        return WS_ERR_TOOFEWITEMS;

    printf("%c", program->stack[program->stack_index - 1]);
    program->stack_index--;

    return WS_SUCCESS;
}

wsError wsi_puti(wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index <= 0)
        return WS_ERR_TOOFEWITEMS;

    printf("%d", program->stack[program->stack_index - 1]);
    program->stack_index--;

    return WS_SUCCESS;
}

wsError wsi_readc(wsProgram *program)
{
    char value;
    int key_int;
    char *key_str;

    /* check that there is atleast 1 element in the stack */
    /* top element of stack specifies where in the heap the input is to reside */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;

    /* get input from stdin, and store it in a temporary variable */
    scanf("%c", &value);

    /* get key_int from top of stack */
    key_int = program->stack[program->stack_index - 1];

    /* pop the top element off the stack */
    program->stack_index--;

    /* allocate and convert int into key_string */
    key_str = (char *)malloc((get_places(key_int, 10) + 1) * sizeof(char));
    sprintf(key_str, "%d", key_int);

    /* add pair to the heap */
    hash_set(program->heap, key_str, &value, sizeof(value));

    /* exit gracefully */
    free (key_str);
    return WS_SUCCESS;
}

wsError wsi_readi(wsProgram *program)
{
    int value;
    int key_int;
    char *key_str;

    /* check that there is atleast 1 element in the stack */
    /* top element of stack specifies where in the heap the input is to reside */
    if (program->stack_index < 1)
        return WS_ERR_TOOFEWITEMS;

    /* get input from stdin, and store it in a temporary variable */
    scanf("%d", &value);

    /* get key_int from top of stack */
    key_int = program->stack[program->stack_index - 1];

    /* pop the top element off the stack */
    program->stack_index--;

    /* allocate and convert int into key_string */
    key_str = (char *)malloc((get_places(key_int, 10) + 1) * sizeof(char));
    sprintf(key_str, "%d", key_int);

    /* add pair to the heap */
    hash_set(program->heap, key_str, &value, sizeof(value));

    /* exit gracefully */
    free (key_str);
    return WS_SUCCESS;
}


/* debug functions */
wsError wsi_dprint(wsProgram *program)
{
    wsInt i = 0;

    printf ("stack = [");
    for (; i < program->stack_index; i++)
    {
        printf ("%d, ", program->stack[i]);
    }
    printf ("]\n");

    return WS_SUCCESS;
}


/* free structure functions */
void free_wsProgram (wsProgram *program)
{
    uint64_t i = 0;

    /* free the hashMpa*/
    free_hashMap (program->heap);

    /* free the instruction set */
    free (program->instructions);
    program->inst_count = 0;
    program->inst_size = 0;

    /* free label index list */
    free (program->label_indexes);
    program->label_count = 0;
    program->label_size = 0;

}
