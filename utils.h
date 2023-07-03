/* only run once */
/* modern solution */
#pragma once
/* legacy backup solution */
#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>
#include <stdint.h>

#include <stdio.h>

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

#define ABS(a) (a < 0 ? -a : a)


int util_from_binary (const char *s);
uint8_t get_places (intmax_t num, uint8_t base);

#endif