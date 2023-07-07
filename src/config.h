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
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* library for wsInt typedef */
#include <stdint.h>


/* CONFIG.H START editting */
/* edit BELLOW this point */


/* --- General Config ------------------------------------------------------ */
/* Sets the size of wsInt */
/* Used durring runtime for the stack and heap aswell as for during first step
 * interpretting for parameters */
typedef int64_t wsInt;


/* --- Interpretter Constants ---------------------------------------------- */
/* max character length of instruction commands, should match wsInt */
/* ex1. if wsInt == int64_t; then MAX_PARAM_LEN = 64 */
/* ex2. if wsInt == int32_t; then MAX_PARAM_LEN = 32 */
#define MAX_PARAM_LEN 64 

/* Strict instruction parameters */
/* for maximum compatibility, defaults to false */
/* true(1):  If an instruction takes a parameter, you must specify the 
 * instruction. otherwise program will throw error on interprettation */
/* false(0): If no parameter is given, assume 0 */
#define STRICT_INSTRUCTION_PARAMETERS 0 


/* --- Runtime Constantsd -------------------------------------------------- */
/* Number of items allowed in the stack (stack depth) */
#define STACK_LEN 255

/* Size of the program->program_control array, how many times you can CALL a 
 * label before you need to return */
#define MAX_RECURSION_DEPTH 255


/* --- WIP Character Locale Support ---------------------------------------- */
/* enable more characters, WIP */
#define SETLOCALE_LC_ALL 1

/* prints each instruction to console before running it */
#define DEBUG_PRINT_INSTRUCTION 0

/* prints stack after each instruction is ran */
#define DEBUG_PRINT_STACK 0

/* edit ABOVE this point */
/* CONFIG STOP ediiting */

#endif