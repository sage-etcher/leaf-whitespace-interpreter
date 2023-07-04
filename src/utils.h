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
#ifndef _UTILS_H_
#define _UTILS_H_


/* libraries */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


/* macros */
#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

#define ABS(a) (a < 0 ? -a : a)


/* function prototypes */
int util_from_binary (const char *s);
uint8_t get_places (intmax_t num, uint8_t base);


#endif