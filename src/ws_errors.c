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

