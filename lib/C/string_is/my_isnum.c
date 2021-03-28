/*
** BENSUPERPC, 2019
**
** File description:
** my_isnum.c
*/

#include "my_isnum.h"

int my_isnum(char c)
{
    if (c == '\0')
        return (0);
    if (c < '0' || c > '9')
        return (0);
    return (1);
}

bool my_isnum_bool(char c)
{
    if (c == '\0')
        return false;
    if (c < '0' || c > '9')
        return false;
    return true;
}
