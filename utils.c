#include "utils.h"


int util_from_binary (const char *s)
{
    return (int) strtol (s, NULL, 2);
}

/* gets number of place values a number of base n has */
/* for example: (int)255 in base 10 has 3 places, while if negative it would have 4.*/
uint8_t get_places (intmax_t num, uint8_t base)
{
    /* start increment counter at base value */
    uintmax_t incrementer = base;

    /* if num is negative, account for sign in length counter */
    /* all numbers will be ATLEAST 1 in length */
    uint8_t length = num < 0 ? 2 : 1;

    /* get absolute value of num, for modulo comparison of negative numbers */
    /* defined in a variable, as to avoid ternary operations on each loop */
    uintmax_t abs_num = ABS(num);


    /* loop through incrementing length and incrementer, */
    /* loop until $abs_num modulo $incrementer is the same as plain $abs_num*/
    /* must use abs_num over normal num. uint comparison to signed int causes issues */
    while ((abs_num % incrementer) != abs_num)
    {
        /* set increment as the minimum value for the next place value */
        incrementer *= base;

        /* increment length for each iteration, and ie, place value */
        length++;
    }

    /* return length of number */
    return length;
}