/* only run once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _WSI_H_
#define _WSI_H_


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


/* constants */
enum console_args {
    CONARG_NAME,
    CONARG_FILE,
    CONARG_LEN
};

#endif