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
void inc_cursor_position (char character)
{
    g_char_pos++;
    if (character == '\n')
    {
        g_char_pos = 1;
        g_line_pos++;
    }
}

wsError get_parameter (char *file_contents, uint64_t *file_cursor, wsInt *return_address)
{
    char current_char;
    char binary_string[MAX_PARAM_LEN];
    const int8_t binary_string_len = MAX_PARAM_LEN;
    int8_t binary_string_count = 0;
    wsError err_code = WS_SUCCESS;

    /* set binary string as all zeroes */
    memset (binary_string, 0, binary_string_len * sizeof (binary_string[0]));

    /* set current_char */
    current_char = file_contents[*file_cursor];

    /* loop through characters until you get to end of file or end of number */
    while (current_char != '\n')
    {
        /* set current_char */
        current_char = file_contents[*file_cursor];
        /* increment program counter */
        (*file_cursor)++;

        /* get line number and char in line for error logging */
        inc_cursor_position (current_char);

        if (current_char == '\0')
        {
            err_code = WS_ERR_BADPARAM;
            break;
        }

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


    if (binary_string_count == 0)
    {
        err_code = WS_ERR_BADPARAM;
    }

    /* check that there was a value given */
    if (err_code == WS_SUCCESS)
    {
        /* convert the string to binary and return the integer result */
        *return_address = util_from_binary (binary_string);
    }

    return err_code;
}

wsError interpret_file (char *file_contents, wsProgram *program)
{
    /* cursor position in file */
    uint64_t cur_pos = 0;
    char cur_char;

    /* instruction string */
    char *instruction_str;
    uint8_t instruction_size = WS_MAX_INST_LEN;
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
    program->count = 0;
    program->size = 2;
    program->instructions = malloc (program->size * sizeof (wsInstruction));
    assert (program->instructions != NULL);

    /* allocate instructin string */
    instruction_str = malloc ((instruction_size + 1) * sizeof (char));
    assert (instruction_str != NULL);
    /* set instruction_str to all 0s */
    memset (instruction_str, 0, instruction_size * sizeof (instruction_str[0]));

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

        /* if there was an instruction ran on last run, update instructin pos
         * variables, only update for whitespace characters. */
        if (instruction_pos_flag && syntax_err_code == WS_SUCCESS)
        {
            /* update instruction positional variables */
            instruction_line_pos = g_line_pos;
            instruction_char_pos = g_char_pos;

            /* reset flag */
            instruction_pos_flag = false;
        }

        /* check if the instructon string is about to overflow */
        /* or if end of file reached while in the middle of an instruction */
        if (instruction_count == instruction_size ||
            (cur_char == '\0' && instruction_count != 0))
        {
            syntax_err_code = WS_ERR_BADINSTRUCTION;
        }


        /* if an error shows up, do stuff with it, cause we dont want that here */
        if (syntax_err_code != WS_SUCCESS)
        {
            /* printf ("bad instruction\n%d, %d", instruction_count, instruction_size); */
            /* set global file possitioning to the start of error */
            g_char_pos = instruction_char_pos;
            g_line_pos = instruction_line_pos;

            /* tell the loop to exit */
            runtime = false;
            continue;
        }

        /* check to make sure that we arent at the end of the file */
        if (cur_char == '\0')
        {
            runtime = false;
            continue;
        }

        /* proccess out comments */
        if (cur_char != ' '  &&
            cur_char != '\t' &&
            cur_char != '\n' )
        {
            /* if the cursor char isn't a white space caracter
             * loop back to the start of the loop */
            continue;
        }


        /* add current character to the instruction_str */
        instruction_str[instruction_count] = cur_char;
        instruction_count++;

        /* check instruction to see if it matches a known instruction */
        /* loop through all instructions, starting from the top */
        for (inst_check = 0; inst_check < WS_INST_COUNT; inst_check++)
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
            if (program->count == program->size)
            {
                program->size *= 2;
                program->instructions = realloc (program->instructions, program->size * sizeof (wsInstruction));
            }

            /* set the instruction type */
            program->instructions[program->count].instruction = inst_check;
            /* set the instruction start position */
            program->instructions[program->count].char_pos = instruction_char_pos;
            program->instructions[program->count].line_pos = instruction_line_pos;

            /* if the instruction takes a parameter, get it */
            if (WS_INST[inst_check].takes_parameter)
            {
                /* otherwise, if the instruction does take a paramter, get it */
                /* get a paramter and set it */
                syntax_err_code = get_parameter (file_contents,
                                                 &cur_pos,
                                                 &program->instructions[program->count].parameter);
            }

            /* increment count */
            program->count++;

            /* log that a command was just run */
            instruction_pos_flag = true;

            /* reset the instruction */
            instruction_count = 0;
            memset (instruction_str, 0, instruction_size * sizeof (instruction_str[0]));

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

void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num)
{
    printf ("\n\nerror #%llu : L%llu C%llu : %s \"%s\"\n", error_code, line_num, char_num, instruction, WS_ERRSTR[error_code]);
}

wsError run_program (wsProgram *program)
{

}

void free_wsProgram (wsProgram *program)
{
    uint64_t i = 0;

    /* free the hashMpa*/
    free_hashMap (program->heap);

    /* free the instruction set */
    free (program->instructions);
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


/* whitespace instructions */
wsError wsi_push (wsProgram *program)
{
    wsInt new_item;

    /* cannot add item to stack if stack is full */
    if (program->stack_index >= STACK_LEN) return WS_ERR_FULLSTACK;

    /* get new item from the push command's paramter */
    new_item = program->current_instruction->parameter;

    /* add an element to the top of the stack */
    program->stack[program->stack_index] = new_item;
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_dup (wsProgram *program)
{
    /* stack must have atleast 1 element */
    if (program->stack_index < 1) return WS_ERR_TOOFEWITEMS;
    /* cannot duplicate item on stack if stack is full */
    else if (program->stack_index >= STACK_LEN) return WS_ERR_FULLSTACK;

    /* duplicate the top element on the stack */
    program->stack[program->stack_index] = program->stack[program->stack_index - 1];
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_copy (wsProgram *program)
{
    wsInt copy_index;

    /* must have atleast 1 element on the stack */
    if (program->stack_index < 1) return WS_ERR_TOOFEWITEMS;
    /* cannot copy a new item to the stack if the stack is full */
    else if (program->stack_index >= STACK_LEN) return WS_ERR_FULLSTACK;

    /* get the index of the element we want to copy from the paramter */
    copy_index = program->current_instruction->parameter;

    /* check if copy index is out of range */
    if ( copy_index >= program->stack_index ||
         copy_index < 0 ) return WS_ERR_OUTOFRANGE;

    /* copy the item at that index to the top of the stack */
    program->stack[program->stack_index] = program->stack[copy_index];
    program->stack_index++;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_swap (wsProgram *program)
{
    wsInt item_overhead;

    /* cannot swap top two elements if there arent atleast 2 on the stack */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* get top item from stack and store in an overhead variable */
    item_overhead = program->stack[program->stack_index - 1];

    /* set the element 2nd from the top, to the top place */
    program->stack[program->stack_index - 1] = program->stack[program->stack_index - 2];
    /* set origonal top of stack to be the 2nd from the top, using overhead */
    program->stack[program->stack_index - 2] = item_overhead;

    /* return successfully */
    return WS_SUCCESS;
}

wsError wsi_pop (wsProgram *program)
{
    /* if there is not atleast 1 element in the stack, then we cant pop anything */
    if (program->stack_index < 1) return WS_ERR_TOOFEWITEMS;

    /* move the stack index back one place */
    /* no need to clear the popped value, as only way to go foreward in stack
     * is to rewrite a new value to it */
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_slide (wsProgram *program)
{
    wsInt slide_n;

    /* check that the stack has atleast 1 element. even with a slide 0, this
     * function requires there to be atleast one element for the top of the
     * stack */
    if (program->stack_index < 1) return WS_ERR_TOOFEWITEMS;

    /* get the number of elements we want to slide off the stack, from the
     * given instruction parameter */
    slide_n = program->current_instruction->parameter;

    /* check that slide_n is in a valid range */
    if (slide_n >= program->stack_index ||
        slide_n < 0) return WS_ERR_OUTOFRANGE;

    /* put old top value into the new top index after a slide */
    /* magickal fuckery, but it works. but it works soo :/ */
    program->stack[program->stack_index - (1 + slide_n)] = program->stack[program->stack_index - 1];
    /* move index to the new top */
    program->stack_index -= slide_n;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_add (wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* add the top element of the stack to the 2nd to the top */
    program->stack[program->stack_index - 2] += program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_sub (wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* subtract the 2nd element of the stack by the top element */
    program->stack[program->stack_index - 2] -= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_mult (wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* multiply the top element of the stack by the 2nd to the top */
    program->stack[program->stack_index - 2] *= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_idiv (wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* divide the element 2nd from the top by the top element */
    program->stack[program->stack_index - 2] /= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_mod (wsProgram *program)
{
    /* check that there is atleast 2 elements in the stack to add */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* get the remainder/modulo of dividing the element 2nd from the top by the
     * top element */
    program->stack[program->stack_index - 2] %= program->stack[program->stack_index - 1];
    program->stack_index--;

    /* exit successfully */
    return WS_SUCCESS;
}

wsError wsi_store (wsProgram *program)
{
    wsInt value;
    wsInt key_int;
    char *key_string;

    /* guard clauses */
    if (program->stack_index < 2) return WS_ERR_TOOFEWITEMS;

    /* get the key and value from the stack */
    /* top item is key */
    /* 2nd to top is value */
    key_int = program->stack[program->stack_index - 1];
    value = program->stack[program->stack_index - 2];

    /* take the top 2 items off the stack */
    program->stack_index -= 2;

    /* allocate and convert int into key_string */
    key_string = (char *)malloc ((get_places (key_int, 10) + 1) * sizeof (char));
    sprintf (key_string, "%d", key_int);

    /* add pair to the heap */
    hash_set (program->heap, key_string, &value, sizeof (value));

    /* free the temporary key */
    free (key_string);
    return WS_SUCCESS;
}

wsError wsi_restore (wsProgram *program)
{
    wsInt value;
    wsInt key_int;
    char *key_string;
    bool match_found;


    /* get the key from the stack */
    key_int = program->stack[program->stack_index - 1];

    /* allocate and convert int into key_string */
    key_string = (char *)malloc ((get_places (key_int, 10) + 1) * sizeof (char));
    sprintf (key_string, "%d", key_int);

    /* look for a match */
    match_found = hash_search (program->heap, key_string, &value);

    /* if no match is found return with error code */
    if (!match_found) return WS_ERR_NOMATCH;

    /* add item to stack */
    program->stack[program->stack_index - 1] = value;

    /* free key_string */
    free (key_string);
    return WS_SUCCESS;
}

/* not implemented start */
/* need to figure out how to get labels to work */
wsError wsi_label (wsProgram *program)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_call (wsProgram *program)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump (wsProgram *program)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_zero (wsProgram *program)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}

wsError wsi_jump_negative (wsProgram *program)
{
    printf ("labels and calls are not yet implemented\n");

    return WS_SUCCESS;
}
/* not implemented end */

wsError wsi_ret (wsProgram *program)
{
    /* cant return from the main process */
    if (program->program_control_index <= 0) return WS_ERR_RETURNFROMMAIN;

    program->program_control_index--;

    return WS_SUCCESS;
}

wsError wsi_end (wsProgram *program)
{
    program->exit = false;

    return WS_SUCCESS;
}

wsError wsi_putc (wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%c", program->stack[program->stack_index - 1]);
    program->stack_index--;

    return WS_SUCCESS;
}

wsError wsi_puti (wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index <= 0) return WS_ERR_TOOFEWITEMS;

    printf ("%d", program->stack[program->stack_index - 1]);
    program->stack_index--;

    return WS_SUCCESS;
}

wsError wsi_readc (wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%c", &program->stack[program->stack_index]);
    program->stack_index++;

    return WS_SUCCESS;
}

wsError wsi_readi (wsProgram *program)
{
    /* guard clauses */
    if (program->stack_index >= STACK_LEN) return WS_ERR_FULLSTACK;

    scanf("%d", &program->stack[program->stack_index]);
    program->stack_index++;

    return WS_SUCCESS;
}
