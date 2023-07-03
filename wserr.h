/* only run once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _WSERR_H_
#define _WSERR_H_

#include <stdint.h>

/* limits total number of errors to at max 2^16 - 1 */
#define wsError uint16_t

enum WS_ERRCODE
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

const char *WS_ERRSTR[] =
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


#endif
