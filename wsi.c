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

uint64_t g_line_pos = 1;
uint64_t g_char_pos = 1;


int main (int argc, char **argv)
{
    /* file storage container */
    char *file_contents;

    /* interpretted program */
    wsProgram program;

    /* store error code */
    wsError error_code;  


    /* process console arguements, get file name */
    /* check that file console arguement was given */
    if (argc < CONARG_FILE + 1)
    {
        printf ("error: WSI expects one(1) console arguement. \nExample: \"wsi filename.ws<cr>\"\n");
        exit (EXIT_FAILURE);
    }


    /* attempt to get file contents */
    /* if cannot get file contenets return NULL, for example if file doesnt exsist */
    file_contents = fio_file_contents (argv[CONARG_FILE]);
    /* if raw_file_contents is equal to NULL, then the file could not be opened */
    if (file_contents == NULL)
    {
        printf ("error: could not open \"%s\"\n", argv[CONARG_FILE]);
        exit (EXIT_FAILURE);
    }

   
    /* interpret program into an easier to work with format */
    error_code = interpret_file (file_contents, &program);
    /* file_contents is no longer needed, so we free it */
    free (file_contents);    
    /* check if interpret_file ran correctly */
    if (error_code != WS_SUCCESS)
    {
        log_error (error_code, "", g_line_pos, g_char_pos);
        free_wsProgram (&program);
        exit (error_code);
    }

    /* run the program */
    printf ("program interpretted correctly \n");

    /* exit gracefully */
    /* free allocated memory */ 
    free_wsProgram (&program);

    /* send a newline statement to fix linux being mean */
    printf ("\n");

    /* exit with error code */
    exit (error_code);
}


/* helper functions */
wsInt get_parameter (char *file_contents, uint64_t *file_cursor)
{
    char current_char;
    char binary_string[MAX_PARAM_LEN];
    const int8_t binary_string_len = MAX_PARAM_LEN;
    int8_t binary_string_count = 0;

    /* set binary string as all zeroes */
    memset (binary_string, 0, binary_string_len * sizeof (binary_string[0]));

    /* set current_char */
    current_char = file_contents[*file_cursor];

    /* loop through characters until you get to end of file or end of number */
    while (current_char != '\0' && current_char != '\n')
    {
        /* set current_char */
        current_char = file_contents[*file_cursor];
        /* increment program counter */
        (*file_cursor)++;

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

wsError interpret_file (char *file_contents, wsProgram *program)
{
    /* cursor position in file */
    uint64_t cur_pos = 0;
    char cur_char;

    /* instruction string */
    char *instruction_str;
    uint8_t instruction_len = WS_MAX_INST_LEN;
    uint8_t instruction_count = 0;

    /* storage container for the current instruction we are compareing */
    WS_INST_INDEX inst_check;

    /* error logging variables */
    /* update instruction position variables flag */
    bool instruction_pos_flag;
    uint64_t instruction_line_pos = 1;
    uint64_t instruction_char_pos = 1;
    /* syntax error container */
    wsError syntax_err_code;

    /* runtime variable */
    bool runtime = true;


    /* allocate program_list */
    program->list_count = 0;
    program->list_len = 1;
    program->instruction_list = malloc (program->list_len * sizeof (wsInstruction *));


    printf ("here\n");
    /* allocate instructin string */
    instruction_str = malloc ((instruction_len + 1) * sizeof (char));  
    printf ("here\n");
    /* set instruction_str to all 0s */
    /* memcpy (instruction_str, NULL, instruction_len * sizeof (char));

    printf ("here\n");


    /* set instruction pos flag to default to update on first run */
    instruction_pos_flag = true;


    /* interpretter runtime loop */
    while (runtime)
    {
        /* increment through the file */
        cur_char = file_contents[cur_pos];
        cur_pos++;

        /* update file location information (for error logging) */
        inc_cursor_position (cur_char);

        /* proccess out comments */
        if (cur_char != ' '  ||
            cur_char != '\t' ||
            cur_char != '\n' )
        {
            /* if the cursor char isn't a white space caracter 
             * loop back to the start of the loop */
            continue;
        }

        /* if there was an instruction ran on last run, update instructin pos
         * variables, only update for whitespace characters. */
        if (instruction_pos_flag)
        {
            /* update instruction positional variables */
            instruction_line_pos = g_line_pos;
            instruction_char_pos = g_char_pos;

            /* reset flag */
            instruction_pos_flag = false;
        }

        /* check if the instructon length in about to overflow */
        if (instruction_count == instruction_len)
        {
            /* set global file possitioning to the start of error */
            g_char_pos = instruction_char_pos;
            g_line_pos = instruction_line_pos;

            /* set error code */
            syntax_err_code = WS_ERR_BADINSTRUCTION;

            /* tell the loop to exit */
            runtime = false;
            continue;
        }

        /* add current character to the instruction_str */
        instruction_str[instruction_count] = cur_char;
        instruction_count++;

        /* check instruction to see if it matches a known instruction */
        /* loop through all instructions, starting from the top */
        for (inst_check = WS_INST_COUNT - 1; inst_check >= 0; inst_check--)
        {
            /* check if instruction_str matches a known instruction */
            if (strncmp (instruction_str, 
                         WS_INST[inst_check].inst_string, 
                         WS_INST[inst_check].inst_string_len) != 0)
            {
                /* if instruction doesnt match inst_check, return to 
                 * the top of the loop */
                continue;
            }

            /* if program_list runs out of allocated length, extend it */
            if (program->list_count == program->list_len)
            {
                /* double the allocated length, and realloc the list */
                program->list_len *= 2;
                program->instruction_list = realloc (program->instruction_list, program->list_len * sizeof (wsInstruction *));
                assert (program->instruction_list != NULL);
            }

            /* allocate a new instruction */
            program->instruction_list[program->list_count] = malloc (sizeof (wsInstruction));

            /* set the instruction type */
            program->instruction_list[program->list_count]->instruction = inst_check;
            /* set the instruction start position */
            program->instruction_list[program->list_count]->char_pos = instruction_char_pos;
            program->instruction_list[program->list_count]->line_pos = instruction_line_pos;

            /* if the instruction takes a parameter, get it */
            if (WS_INST[inst_check].takes_parameter)
            {
                /* otherwise, if the instruction does take a paramter, get it */ 
                /* get a paramter and set it */
                program->instruction_list[program->list_count]->parameter = get_parameter ( 
                    file_contents, &cur_pos);
            }

            /* reset the instruction */
            instruction_count = 0;
            /* memcpy (instruction_str, 0, instruction_len); */
            
            /* stop checking for instruction_str having a match */
            break;
        } 
    }


    /* exit gracefully */
    /* free unneeded memory */
    free (instruction_str);

    /* return error code */
    return syntax_err_code;
}

void print_stack (wsInt stack[], wsInt stack_index)
{
    wsInt i = 0;

    printf ("stack = [");
    for (; i < stack_index; i++)
    {
        printf ("%d, ", stack[i]);
    }
    printf ("]\n");

    return;
}

void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num)
{
    printf ("\n\nerror #%llu : L%llu C%llu : %s \"%s\"\n", error_code, line_num, char_num, instruction, WS_ERRSTR[error_code]);
}

void inc_cursor_position (char character)
{
    g_char_pos++;
    if (character == '\n')
    {
        g_char_pos = 1;
        g_line_pos++;
    }
}

void free_wsProgram (wsProgram *program)
{
    uint64_t i = 0;

    /* free the hashMpa*/
    free_hashMap (program->heap);

    /* free each instruction */
    for (; i < program->list_count; i++)
        free (program->instruction_list[i]);

    /* free the instruction set */
    free (program->instruction_list);
}



/* whitespace instructions */
wsError wsi_push (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    wsInt new_value;

    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    new_value = get_parameter (file_contents, program_counter);
    stack[(*stack_index)] = new_value;
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_dup (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) == 0) return WS_ERR_EMPTYSTACK;
    else if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    stack[(*stack_index)] = stack[(*stack_index) - 1];
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_copy (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    wsInt copy_index;

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

wsError wsi_swap (wsInt stack[], wsInt *stack_index)
{
    wsInt item_overhead;

    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    item_overhead = stack[(*stack_index) - 1];
    stack[(*stack_index) - 1] = stack[(*stack_index) - 2];
    stack[(*stack_index) - 2] = item_overhead;

    return WS_SUCCESS;
}

wsError wsi_pop (wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_EMPTYSTACK;

    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_slide (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    wsInt slide_n;

    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_EMPTYSTACK;

    slide_n = get_parameter (file_contents, program_counter);

    /* check that slide_n is valid */
    if (slide_n < 0 || slide_n >= (*stack_index)) return WS_ERR_OUTOFRANGE;

    stack[(*stack_index) - (1 + slide_n)] = stack[(*stack_index) - 1];
    (*stack_index) -= slide_n;

    return WS_SUCCESS;
}

wsError wsi_add (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] += stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_sub (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] -= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_mult (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] *= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_idiv (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] /= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_mod (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) < 2) return WS_ERR_TOOFEWITEMS;

    stack[(*stack_index) - 2] %= stack[(*stack_index) - 1];
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_store (wsInt stack[], wsInt *stack_index, hashMap *heap)
{
    wsInt value;
    wsInt key_int;
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

wsError wsi_restore (wsInt stack[], wsInt *stack_index, hashMap *heap)
{
    wsInt value;
    wsInt key_int;
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
wsError wsi_label (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_call (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_zero (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_negative (wsInt stack[], wsInt *stack_index, char *file_contents, uint64_t *program_counter)
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

wsError wsi_putc (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%c", stack[(*stack_index - 1)]);
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_puti (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%d", stack[(*stack_index - 1)]);
    (*stack_index)--;

    return WS_SUCCESS;
}

wsError wsi_readc (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%c", &stack[(*stack_index)]);
    (*stack_index)++;

    return WS_SUCCESS;
}

wsError wsi_readi (wsInt stack[], wsInt *stack_index)
{
    /* guard clauses */
    if ((*stack_index) >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%d", &stack[(*stack_index)]);
    (*stack_index)++;

    return WS_SUCCESS;
}
