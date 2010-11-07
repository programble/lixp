/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of Lixp.
 *
 *  Lixp is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  Lixp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Lixp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "strutils.h"

#include <stdio.h>
#include <stdarg.h>
#include <gc.h>

int str_has_char(const char *str, char c)
{
    for (const char *sc = str; *sc != 0; sc++)
        if (*sc == c)
            return 1;
    return 0;
}

int asprintf(char **ret, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int length = vsnprintf(NULL, 0, format, ap);
    char *s = GC_MALLOC_ATOMIC(length + 1);
    vsnprintf(s, length + 1, format, ap);
    va_end(ap);
    *ret = s;
    /* ? */
    return 0;
}
