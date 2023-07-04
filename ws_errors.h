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
    WS_ERR_FULLSTACK,
    WS_ERR_EMPTYSTACK,
    WS_ERR_TOOFEWITEMS,
    WS_ERR_OUTOFRANGE,
    WS_ERR_RETURNFROMMAIN,
    WS_ERR_ENDOFFILE,
    WS_ERR_BADINSTRUCTION,
    WS_ERR_NOMATCH,
    WS_ERR_BADPARAM
};

/* error strings */
extern char *WS_ERRSTR[];


/* functions prototypes */
void log_error (wsError error_code, char *instruction, uint64_t line_num, uint64_t char_num);


#endif