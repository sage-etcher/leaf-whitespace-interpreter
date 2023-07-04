#include "ws_errors.h"

/* libraries */
#include <stdio.h>
#include <stdint.h>


/* global variables */
char *WS_ERRSTR[] =
{
    "success",
    "failure",
    "stack is full",
    "stack is empty",
    "too few elements in stack",
    "index out of range",
    "return from main processes",
    "end of file reached. no END statement (LLL)",
    "bad instruction",
    "couldn't find a match for that key",
    "there was an issue getting the provided parameter"
};


/* functions */
void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num)
{
    printf ("\n\nerror #%llu : L%llu C%llu : %s \"%s\"\n", error_code, line_num, char_num, instruction, WS_ERRSTR[error_code]);
}

