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

#include "readline.h"

#include <gc.h>

#ifndef READLINE

char *readline(const char *prompt)
{
    printf("%s", prompt);
    int i = 0;
    char *line = GC_MALLOC_ATOMIC(i);
    while (1)
    {
        int c = getchar();
        if (c == EOF && i == 0)
            return NULL;
        else if (c == EOF || c == '\n')
            break;
        line = GC_REALLOC(line, ++i);
        line[i-1] = (char) c;
    }
    line = GC_REALLOC(line, i+1);
    line[i] = 0;
    return line;
}

void add_history(char *line)
{
    /* nop, no history without GNU Readline */
    return;
}

#endif
