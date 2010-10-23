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

#include "strutils.h"

#define WHITESPACE " \t\n\r"

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

char *Reader_read_until(Reader *reader, const char term[])
{
    /* TODO: Clean this up */
    int i = 0;
    char *str = malloc(i);
    while ((unsigned)reader->index < strlen(reader->source))
    {
        if (str_has_char(term, reader->source[reader->index]))
            break;
        str = realloc(str, ++i);
        str[i-1] = reader->source[reader->index];
        reader->index++;
    }
    str[i] = 0;
    return str;
}

VALUE Reader_read_list(Reader *reader)
{
    /* Skip over ( */
    reader->index++;

    /* Empty List? */
    if (reader->source[reader->index] == ')')
    {
        /* Skip ) */
        reader->index++;
        return LixpCons_new(NULL, NULL);
    }

    VALUE car = Reader_read(reader);

    /* Improper list */
    if (reader->source[reader->index + 1] == '.')
    {
        /* Skip over . */
        reader->index += 2;
        VALUE cdr = Reader_read(reader);
        /* Skip ) */
        reader->index++;
        return LixpCons_new(car, cdr);
    }

    /* Proper list */
    reader->index--;
    return LixpCons_new(car, Reader_read_list(reader));
}

VALUE Reader_read_number(Reader *reader)
{
    /* TODO: CLean up */
    char *ns = Reader_read_until(reader, WHITESPACE ")");
    int n, r;
    r = sscanf(ns, "%d", &n);
    /* TODO: Handle errors better somehow? */
    if (r < 1)
        return NULL;
    return LixpNumber_new(n);
}

VALUE Reader_read_character(Reader *reader)
{
    /* TODO: Special cases? */
    /* Skip over \ */
    reader->index++;
    /* TODO: Check for EOF */
    return LixpCharacter_new(reader->source[reader->index++]);
}

VALUE Reader_read_string(Reader *reader)
{
    /* Skip over opening " */
    reader->index++;
    /* TODO: Escapes? */
    VALUE value = LixpString_new(Reader_read_until(reader, "\""));
    /* Skip over closing " */
    reader->index++;
    return value;
}

VALUE Reader_read_keyword(Reader *reader)
{
    /* TODO: Also check for EOF maybe? */
    /* Skip over : */
    reader->index++;
    return LixpKeyword_new(Reader_read_until(reader, WHITESPACE ")"));
}

VALUE Reader_read_symbol(Reader *reader)
{
    /* TODO: Check for starting with |*/
    return LixpSymbol_new(Reader_read_until(reader, WHITESPACE ")"));
}

VALUE Reader_read(Reader *reader)
{
    /* No more to read */
    if ((unsigned) reader->index >= strlen(reader->source))
        return NULL;
    
    char c = reader->source[reader->index];
    /* Skip Whitespace */
    while (str_has_char(WHITESPACE, c))
    {
        reader->index++;
        c = reader->source[reader->index];
    }

    /* Unexpected close-paren */
    if (c == ')')
        /* TODO: Handle error somehow */ return NULL;
    
    if (c == '(')
        return Reader_read_list(reader);
    if (c >= '0' && c <= '9')
        return Reader_read_number(reader);
    if (c == '\\')
        return Reader_read_character(reader);
    if (c == '"')
        return Reader_read_string(reader);
    if (c == ':')
        return Reader_read_keyword(reader);
    return Reader_read_symbol(reader);
}
