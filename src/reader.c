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

#include "reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Reader *Reader_new(char *source)
{
    Reader *reader = malloc(sizeof(Reader));
    reader->source = source;
    reader->index = 0;
    return reader;
}

void Reader_destroy(Reader *reader)
{
    free(reader->source);
    free(reader);
}

int char_in(char c, const char clist[])
{
    /* TODO: Clean up/rename/move to strutils.h or something? */
    for (unsigned int i = 0; i < strlen(clist); i++)
        if (c == clist[i])
            return 1;
    return 0;
}

char *Reader_read_until(Reader *reader, const char term[])
{
    /* TODO: Clean this up */
    int i = 0;
    char *str = malloc(i);
    while ((unsigned)reader->index < strlen(reader->source))
    {
        str = realloc(str, ++i);
        if (char_in(reader->source[reader->index], term))
            break;
        str[i-1] = reader->source[reader->index];
        reader->index++;
    }
    str[i] = 0;
    return str;
}

VALUE Reader_read_list(Reader *reader);
VALUE Reader_read_string(Reader *reader);
VALUE Reader_read_number(Reader *reader);
VALUE Reader_read_keyword(Reader *reader);
VALUE Reader_read_character(Reader *reader);

VALUE Reader_read_symbol(Reader *reader)
{
    /* TODO: Check for first character and stuff */
    return LixpSymbol_new(Reader_read_until(reader, ") \t\n\r"));
}

VALUE Reader_read(Reader *reader);
