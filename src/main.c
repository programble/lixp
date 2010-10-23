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

#include <stdlib.h>
#include <stdio.h>

#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#else

/* Drop-in replacements for readline functions */

char *readline(const char *prompt)
{
    /* TODO: Implement */
    return NULL;
}

void add_history(char *line)
{
    /* nop, no history without GNU Readline */
    return;
}

#endif

#include "runtime.h"
#include "reader.h"

int main(int argc, char **argv)
{
    while (1)
    {
        char *input = readline("=> ");
        if (input == NULL)
            break;
        Reader *reader = Reader_new(input);
        while (1)
        {
            VALUE exp = Reader_read(reader);
            if (exp == NULL)
                break;
            /* TODO: Evaluate expression */
            char *inspect = LixpValue_inspect(exp);
            printf("%s\n", inspect);
            free(inspect);
        }
        Reader_destroy(reader);
    }
    return 0;
}
