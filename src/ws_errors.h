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
#ifndef _WSERRORS_H_
#define _WSERRORS_H_


/* libraries */
#include <stdio.h>
#include <stdint.h>


/* constants */
/* wsError type definition (0 - 2^16) */

#define wsError enum WS_ERR_INDEX
/* errorcode index */
enum WS_ERR_INDEX
{
    WS_SUCCESS,
    WS_ERR_FAILURE,
    WS_ERR_BADINSTRUCTION,
    WS_ERR_BADPARAM,
    WS_ERR_TOOFEWITEMS,
    WS_ERR_OUTOFRANGE,
    WS_ERR_FULLSTACK,
    WS_ERR_EMPTYSTACK,
    WS_ERR_NOMATCH,
    WS_ERR_MAXRECURSIONDEPTH,
    WS_ERR_NOLABELFOUND,
    WS_ERR_RETURNFROMMAIN,
    WS_ERR_ENDOFFILE
};

/* error strings */
extern char *WS_ERRSTR[];


/* functions prototypes */
void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num);


#endif