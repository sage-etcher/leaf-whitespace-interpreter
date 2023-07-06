/*
    This file is part of LeafWSI.

    LeafWSI - a C based, cross-platform, whitespace language interpretter.
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

#include "wsi.h"

/* header files */
#include "fileio.h"
#include "ws_program.h"
#include "utils.h"

/* libraries */
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


/* static definitions */
static void inc_cursor_position (char character);
static wsError get_parameter (char *file_contents, uint64_t *file_cursor, wsInt *return_address);
static wsError interpret_file (char *file_contents, wsProgram *program);
static wsError run_program (wsProgram *program);

static uint64_t g_line_pos = 1;
static uint64_t g_char_pos = 1;


/* main entry point */
int main (int argc, char **argv)
{
    /* file storage container */
    char *file_contents;

    /* interpretted program */
    wsProgram program;

    /* store error code */
    wsError error_code;
    
    /* enable more characters */
    #ifdef SETLOCALE_LC_ALL
        setlocale(LC_ALL, "");
    #endif

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
        free_wsProgram (&program);
        exit (error_code);
    }

    error_code = run_program (&program);
    if (error_code != WS_SUCCESS)
    {
        free_wsProgram (&program);
        exit (error_code);
    }

    /* exit gracefully */
    /* free allocated memory */
    free_wsProgram (&program);

    /* send a newline statement to fix linux being mean */
    printf ("\n");

    /* exit with error code */
    exit (error_code);
}


/* helper functions */
static void inc_cursor_position (char character)
{
    g_char_pos++;
    if (character == '\n')
    {
        g_char_pos = 1;
        g_line_pos++;
    }
}

static wsError get_parameter (char *file_contents, uint64_t *file_cursor, wsInt *return_address)
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


    /* if strict instruction flag is enabled, log error when the 
     * instruction_string_count is equal to 0.*/
    #if STRICT_INSTRUCTION_PARAMETERS
    
    /* check if variable is 0 */
    if (binary_string_count == 0)
    {
        /* set error code to BADPARAM */
        err_code = WS_ERR_BADPARAM;
    }

    #endif 

    /* if errorcode success then convert it normally*/
    if (err_code == WS_SUCCESS)
    {
        /* convert the string to binary and return the integer result */
        *return_address = util_from_binary (binary_string);
    }
    return err_code;
}

static wsError interpret_file (char *file_contents, wsProgram *program)
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
    wsError syntax_err_code = WS_SUCCESS;

    /* runtime variable */
    bool runtime = true;


    /* allocate program_list */
    program->inst_count = 0;
    program->inst_size = 2;
    program->instructions = malloc (program->inst_size * sizeof (wsInstruction));
    assert (program->instructions != NULL);

    /* allocate label indexes */
    program->label_count = 0;
    program->label_size = 2;
    program->label_indexes = malloc (program->label_size * sizeof (uint64_t));
    assert (program->label_indexes != NULL);

    /* allocate instruction string */
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

            /* check if item is a wsLabel, if so add the index to the
             * label_index list to quickly search for labels durring a jump */
            if (inst_check == WS_LABEL)
            {
                /* reallocate memory for the label_indexes list */
                if (program->label_count == program->label_size)
                {
                    program->label_size *= 2;
                    program->label_indexes = realloc (program->label_indexes, program->label_size * sizeof (uint64_t));
                }

                /* add instruction index to labelindex */
                program->label_indexes[program->label_count] = program->inst_count;

                /* increment label_count */
                program->label_count++;
            }

            /* if program_list runs out of allocated length, extend it */
            if (program->inst_count == program->inst_size)
            {
                program->inst_size *= 2;
                program->instructions = realloc (program->instructions, program->inst_size * sizeof (wsInstruction));
            }

            /* set the instruction type */
            program->instructions[program->inst_count].instruction = inst_check;
            /* set the instruction start position */
            program->instructions[program->inst_count].char_pos = instruction_char_pos;
            program->instructions[program->inst_count].line_pos = instruction_line_pos;

            /* if the instruction takes a parameter, get it */
            if (WS_INST[inst_check].takes_parameter)
            {
                /* otherwise, if the instruction does take a paramter, get it */
                /* get a paramter and set it */
                syntax_err_code = get_parameter (file_contents,
                                                 &cur_pos,
                                                 &program->instructions[program->inst_count].parameter);
            }

            /* increment count */
            program->inst_count++;

            /* log that a command was just run */
            instruction_pos_flag = true;

            /* reset the instruction */
            instruction_count = 0;
            memset (instruction_str, 0, instruction_size * sizeof (instruction_str[0]));

            /* stop checking for instruction_str having a match */
            break;
        }
    }


    if (syntax_err_code != WS_SUCCESS)
    {
        log_error (syntax_err_code, "", g_line_pos, g_char_pos);
    }


    /* exit gracefully */
    /* free unneeded memory */
    free (instruction_str);

    /* return error code */
    return syntax_err_code;
}

static wsError run_program (wsProgram *program)
{
    /* runtime error code */
    wsError runtime_err_code = WS_SUCCESS;
    uint64_t *program_counter;

    /* set the stack index to default value of 0 */
    program->stack_index = 0;

    /* initiate the programs heap */
    program->heap = new_hashMap ();
    program->exit = false;

    /* loop through instructions */
    program->program_control_index = 0;
    program->program_control[program->program_control_index] = 0;

    /* loop through all of the program instructions */
    do
    {
        /* make a pointer to the current executing location in the program */
        program_counter = &program->program_control[program->program_control_index];

        /* set current instrution pointer */
        program->current_instruction = &program->instructions[*program_counter];

        /* run the current instruction's dedicated function */
        runtime_err_code = WS_INST[program->current_instruction->instruction].inst_function (program);
        
        /* if the function runs poorly or sends an end command then exit the program */
        if (program->exit == true || runtime_err_code != WS_SUCCESS)
        {
            break;
        }

        /* increment to the next instruction */
        (*program_counter)++;
    } /* check if we have processed all the instructions */
    while (*program_counter < program->inst_count);

    /* if there was an issue proccessing the code, throw an error */
    if (runtime_err_code != WS_SUCCESS)
    {
        log_error (runtime_err_code, WS_INST[runtime_err_code].inst_name, g_line_pos, g_char_pos);
    }

    /* return the error code */
    return runtime_err_code;
}

