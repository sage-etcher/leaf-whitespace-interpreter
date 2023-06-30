#include "wsi.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "fileio.h"
#include "utils.h"
#include "whitespace.h"
#include "config.h"
#include "wserr.h"
#include "hashmap.h"


enum console_args {
    CONARG_NAME,
    CONARG_FILE,
    CONARG_LEN
};

uint64_t g_line_num = 1;
uint64_t g_char_num = 1;


int main (int argc, char **argv)
{
    /* storage container for file chars */
    char *file_contents;
    /* current character in file_contents */
    char current_char = '\0';

    /* program counter */
    uint64_t program_counter[STACK_LEN];
    uint64_t program_counter_index = 0;

    /* whitespace stack */
    int32_t program_stack[STACK_LEN];
    int32_t program_stack_index = 0;

    /* whitespace heap */
    hashMap *program_heap;

    /* active instruction variables */
    char instruction[WS_MAX_COMMAND_LEN];                   /* list of chars to compare with instructions */
    const uint8_t instruction_len = WS_MAX_COMMAND_LEN;     /* max length of instruction pointer */
    uint8_t instruction_count = 0;                          /* actual number of chars in pointer */

    /* error logging information */
    /* if last loop ran an instruction, set to true, otherwise assume false */
    bool instruction_flag;

    /* actively updates as different instructions are compared */
    /* used as a prerequisite to last_instruction*/
    wsInst inst_check = WS_NONE;
    /* hold index of the last ran instruction */
    wsInst last_instruction = inst_check;

    /* holds Human readable position of last run instruction */
    int32_t next_inst_line = g_line_num;
    int32_t next_inst_char = g_char_num;

    /* holds the error code returned be the last ran instruction */
    wsError err_code = WS_SUCCESS;

    /* runtime loop var */
    bool runtime = true;


    /* process console arguements, get file name */
    /* check that file console arguement was given */
    if (argc < CONARG_FILE + 1)
    {
        printf ("error: WSI expects one(1) console arguement. \nExample: \"wsi filename.ws<cr>\"\n");
        return EXIT_FAILURE;
    }


    /* attempt to get file contents */
    /* if cannot get file contenets return NULL, for example if file doesnt exsist */
    file_contents = fio_file_contents (argv[CONARG_FILE]);
    /* if raw_file_contents is equal to NULL, then the file could not be opened */
    if (file_contents == NULL)
    {
        printf ("error: could not open \"%s\"\n", argv[CONARG_FILE]);
        return EXIT_FAILURE;
    }


    /* start initializing variables for the interpretter */
    /* initialize the heap */
    program_heap = new_hashMap ();

    /* start program counter at 0 */
    program_counter[program_counter_index] = 0;

    /* first time run, reset/initiate instruction variables */
    instruction_flag = true;


    while (runtime)
    {
        /* file positioning and current character, overall incrementing through file */
        /* get current char */
        current_char = file_contents[program_counter[program_counter_index]];

        /* increment our location in the file */
        program_counter[program_counter_index]++;


        /* check for errors */
        /* check if character is EOF exit */
        if (current_char == '\0')
        {
            last_instruction = WS_NONE;
            err_code = WS_ERR_ENDOFFILE;
        }

        /* if the instruction pointer grows too long (normally a syntax error), throw error and quit */
        if (instruction_count > WS_MAX_COMMAND_LEN ||
            ((!runtime) && (instruction_count != 0)))
        {
            last_instruction = WS_NONE;
            err_code = WS_ERR_BADINSTRUCTION;
        }

        /* if an error is found YELL VIOLENTLY */
        /* if there was an error exectuting an instruction, log the error to the terminal and quit */
        if (err_code != WS_SUCCESS)
        {
            log_error (err_code, last_instruction, next_inst_line, next_inst_char);
            runtime = false;
            /* reset err_code to WS_SUCCESS as a backup */
            /* should not need to rely on this but JUSTINCASE */
            err_code = WS_SUCCESS;
            /* loop back to the start of the loop, and have loop recheck runtime */
            continue;
        }

        /* update error logging information */
        /* get line number and char in line for error logging */
        inc_cursor_position (current_char);


        /* ignore comments */
        /* if character isn't whitespace, early loop back */
        if (! (current_char == ' '  ||
               current_char == '\t' ||
               current_char == '\n' ))
        {
            /* return to start of loop */
            continue;
        }

        /* update variables when an instruction is ran */
        /* reset instruction variables */
        if (instruction_flag)
        {
            /* reset instruction variables */
            memset (instruction, 0, instruction_len * sizeof (instruction[0]));
            instruction_count = 0;
            instruction_flag = false;

            /* once next whitespace character shows up, save location of character */
            /* if an error occures, you have the base of the command */
            next_inst_line = g_line_num;
            next_inst_char = g_char_num - 1;
        }


        /* append current_char to our instruction char list */
        /* add new character to instruction string */
        instruction[instruction_count] = current_char; /* increment instruction */
        instruction_count++;


        /* compare our current instruction char list with defined instructions */
        /* if instruction doesnt match any defined WS_INST, set flag to false at end of if*/
        instruction_flag = true;

        /* instructions */
        /* for each instruction (defined in WS_INST) first change isnt_check to the instruction's index */
        /* then compare the current instruction with the defined WS_INST string. */
        /* if the 2 match, run the associated function */
        if      (inst_check = WS_PUSH,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* push a value onto stack */
            err_code = wsi_push (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_DUP,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* duplicate top value of stack */
            err_code = wsi_dup (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_COPY,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* copy the Nth item to the top of the stack;  */
            err_code = wsi_copy (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_SWAP,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* swap the top 2 items of the stack */
            err_code = wsi_swap (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_POP,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* pop/remove the top item on the stack */
            err_code = wsi_pop (&program_stack_index);
        }
        else if (inst_check = WS_SLIDE,   strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* slide/remove N number of items off of the stack, keeping the top element */
            err_code = wsi_slide (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_ADD,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {   /* add the top 2 items of the stack, destroy both, then push result to the top of the stack*/
            err_code = wsi_add (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_SUB,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_sub (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_MULT,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_mult (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_DIV,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_idiv (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_MOD,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_mod (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_STORE,   strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_store (program_stack, &program_stack_index, program_heap);
        }
        else if (inst_check = WS_RESTORE, strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_restore (program_stack, &program_stack_index, program_heap);
        }
        else if (inst_check = WS_LABEL,   strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_label (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_CALL,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_call (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_JMP,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_jump (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_JZ,      strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_jump_zero (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_JN,      strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_jump_negative (program_stack, &program_stack_index, file_contents, &program_counter[program_counter_index]);
        }
        else if (inst_check = WS_RET,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_ret (&program_counter_index);
        }
        else if (inst_check = WS_END,     strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_end (&runtime);
        }
        else if (inst_check = WS_PUTC,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_putc (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_PUTI,    strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_puti (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_READC,   strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_readc (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_READI,   strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            err_code = wsi_readi (program_stack, &program_stack_index);
        }
        else if (inst_check = WS_DPRINT,  strncmp (instruction, WS_INST[inst_check], WS_INST_LEN[inst_check]) == 0)
        {
            print_stack (program_stack, program_stack_index);
            err_code = WS_SUCCESS;
        }
        else
        {   /* show that no instruction is ran */
            instruction_flag = false;
        }

        /* when an instruction is ran we want to update last_instruction to reflect that */
        /* inst_check will match the last else if statement that was ran. ie the instruction that ran */
        /* so when an instruction is ran (anytime else isnt), update last_instruction */
        if (instruction_flag)
        {
            last_instruction = inst_check;
        }

    } /* end runtime loop */


    /* exit gracefully from the program */
    free (file_contents);
    free_hashMap (program_heap);

    /* after exitting, print new line because linux is adding a weird reverse color, percent sign if not??? */
    printf ("\n");

    /* return the errorcode, for somereason on linux it has weird side effects */ 
    exit (err_code);
}


/* helper functions */
int32_t get_parameter (char *file_contents, uint64_t *program_counter)
{
    char current_char;
    char binary_string[MAX_PARAM_LEN];
    const int32_t binary_string_len = MAX_PARAM_LEN;
    int32_t binary_string_count = 0;

    /* set binary string as all zeroes */
    memset (binary_string, 0, binary_string_len * sizeof (binary_string[0]));

    /* set current_char */
    current_char = file_contents[*program_counter];

    /* loop through characters until you get to end of file or end of number */
    while (current_char != '\0' && current_char != '\n')
    {
        /* set current_char */
        current_char = file_contents[*program_counter];
        /* increment program counter */
        (*program_counter)++;

        /* get line number and char in line for error logging */
        inc_cursor_position (current_char);

        /* if not white space return to top of loop */
        /* if character isn't whitespace, early loop back */
        if (! (current_char == ' ' ||
               current_char == '\t' ||
               current_char == '\n' ))
        {
            /* return to start of loop */
            continue;
        }

        /* if string count is longer than string len return */
        if (binary_string_count >= binary_string_len)
        {
            break;
        }

        /* if space, add a 0, if a tab add a 1 */
        if (current_char == ' ')
        {
            binary_string[binary_string_count] = '0';
            binary_string_count++;
        }
        else if (current_char == '\t')
        {
            binary_string[binary_string_count] = '1';
            binary_string_count++;
        }
    } /* end while loop */


    /* convert the string to binary and return the integer result */
    return util_from_binary (binary_string);
}

void print_stack (int32_t stack[], uintmax_t stack_index)
{
    int32_t i = 0;

    printf ("stack = [");
    for (; i < stack_index; i++)
    {
        printf ("%d, ", stack[i]);
    }
    printf ("]\n");

    return;
}

void log_error (wsError error_code, wsInst instruction, uint64_t line_num, uint64_t char_num)
{
    printf ("\n\nerror #%llu : L%llu C%llu : %s \"%s\"\n", error_code, line_num, char_num, WS_INST_NAME[instruction], WS_ERRSTR[error_code]);
}

void inc_cursor_position (char character)
{
    g_char_num ++;
    if (character == '\n')
    {
        g_char_num = 1;
        g_line_num++;
    }
}


/* whitespace instructions */
wsError wsi_push (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    int32_t new_value;

    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    new_value = get_parameter (file_contents, program_counter);
    stack[(*stack_index)] = new_value;
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_dup (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) == 0) return WS_ERR_EMPTYSTACK;
    else if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    stack[(*stack_index)] = stack[(*stack_index) - 1];
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_copy (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    int32_t copy_index;

    /* guard clauses */
    if ((*stack_index) == 0) return WS_ERR_EMPTYSTACK;
    else if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    copy_index = get_parameter (file_contents, program_counter);

    /* check if index is out of range */
    if ( copy_index >= (*stack_index) || copy_index < 0 ) return WS_ERR_OUTOFRANGE;

    stack[(*stack_index)] = stack[copy_index];
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_swap (int32_t stack[], int32_t *stack_index)
{
    int32_t item_overhead;

    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    item_overhead = stack[(*stack_index) - 1];
    stack[(*stack_index) - 1] = stack[(*stack_index) - 2];
    stack[(*stack_index) - 2] = item_overhead;

    return WS_SUCCESS;
}

wsError wsi_pop (int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_EMPTYSTACK;

    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_slide (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    int32_t slide_n;

    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_EMPTYSTACK;

    slide_n = get_parameter (file_contents, program_counter);

    /* check that slide_n is valid */
    if (slide_n < 0 || slide_n >= (*stack_index)) return WS_ERR_OUTOFRANGE;

    stack[(*stack_index) - (1 + slide_n)] = stack[(*stack_index) - 1];
    (*stack_index) -= slide_n;

    return WS_SUCCESS;
}

wsError wsi_add (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] += stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_sub (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] -= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_mult (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] *= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_idiv (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] /= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_mod (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] %= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_store (int32_t stack[], int32_t *stack_index, hashMap *heap)
{
    int32_t value;
    int32_t key_int;
    char *key_string;

    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    /* get the key and value from the stack */
    /* top item is key */
    /* 2nd to top is value */
    key_int = stack[(*stack_index) - 1];
    value = stack[(*stack_index) - 2];

    /* take the top 2 items off the stack */
    (*stack_index) -= 2;

    /* allocate and convert int into key_string */
    key_string = (char *)malloc ((get_places (key_int, 10) + 1) * sizeof (char));
    sprintf (key_string, "%d", key_int);

    /* add pair to the heap */
    hash_set (heap, key_string, &value, sizeof (value));

    /* free the temporary key */
    free (key_string);
    return WS_SUCCESS;
}

wsError wsi_restore (int32_t stack[], int32_t *stack_index, hashMap *heap)
{
    int32_t value;
    int32_t key_int;
    char *key_string;
    bool match_found;


    /* get the key from the stack */
    key_int = stack[(*stack_index) - 1];

    /* allocate and convert int into key_string */
    key_string = (char *)malloc ((get_places (key_int, 10) + 1) * sizeof (char));
    sprintf (key_string, "%d", key_int);

    /* look for a match */
    match_found = hash_search (heap, key_string, &value);

    /* free key_string */
    free (key_string);

    /* if no match is found return with error code */
    if (!match_found) return WS_ERR_NOMATCH;

    /* add item to stack */
    stack[(*stack_index) - 1] = value;

    return WS_SUCCESS;
}

/* not implemented start */
/* need to figure out how to get labels to work */
wsError wsi_label (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_call (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_zero (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_negative (int32_t stack[], int32_t *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}
/* not implemented end */

wsError wsi_ret (uint64_t *program_counter_index)
{
    /* guard clause */
    if ((*program_counter_index) <= 0) return WS_ERR_RETURNFROMMAIN;

    (*program_counter_index)--;

    return WS_SUCCESS;
}

wsError wsi_end (bool *runtime_bool)
{
    (*runtime_bool) = false;

    return WS_SUCCESS;
}

wsError wsi_putc (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%c", stack[(*stack_index - 1)]);
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_puti (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%d", stack[(*stack_index - 1)]);
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_readc (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%c", &stack[(*stack_index)]);
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_readi (int32_t stack[], int32_t *stack_index)
{
    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%d", &stack[(*stack_index)]);
    (*stack_index)++;

    return WS_SUCCESS;
}
