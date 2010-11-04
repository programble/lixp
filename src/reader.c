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
#include <gc.h>

#include "strutils.h"

#define WHITESPACE " \t\n\r"

Reader *Reader_new(char *source)
{
    Reader *reader = GC_MALLOC(sizeof(Reader));
    reader->source = source;
    reader->index = 0;
    reader->error = NULL;
    return reader;
}

char *Reader_read_while(Reader *reader, const char term[])
{
    /* TODO: Clean up */
    int i = 0;
    char *str = GC_MALLOC_ATOMIC(i);
    while ((unsigned)reader->index < strlen(reader->source))
    {
        if (!str_has_char(term, reader->source[reader->index]))
            break;
        str = GC_REALLOC(str, ++i);
        str[i-1] = reader->source[reader->index];
        reader->index++;
    }
    str = GC_REALLOC(str, i+1);
    str[i] = 0;
    return str;
}

char *Reader_read_until(Reader *reader, const char term[])
{
    /* TODO: Clean this up */
    int i = 0;
    char *str = GC_MALLOC_ATOMIC(i);
    while ((unsigned)reader->index < strlen(reader->source))
    {
        if (str_has_char(term, reader->source[reader->index]))
            break;
        str = GC_REALLOC(str, ++i);
        str[i-1] = reader->source[reader->index];
        reader->index++;
    }
    str = GC_REALLOC(str, i+1);
    str[i] = 0;
    return str;
}

void Reader_skip_whitespace(Reader *reader)
{
    while (str_has_char(WHITESPACE, reader->source[reader->index]))
    {
        reader->index++;
    }
}

VALUE Reader_read_list(Reader *reader)
{
    VALUE cons = LixpCons_new(NULL, NULL);

    /* Skip ( */
    reader->index++;
    Reader_skip_whitespace(reader);

    if ((unsigned)reader->index >= strlen(reader->source))
    {
        reader->error = "Unexpected EOF";
        return NULL;
    }

    /* Empty list */
    if (reader->source[reader->index] == ')')
    {
        reader->index++;
        return cons;
    }

    /* Read the car */
    VALUE car = Reader_read(reader);
    /* Return NULL if car returned NULL (error) */
    if (car == NULL)
        return NULL;
    LixpCons_car(cons) = car;

    Reader_skip_whitespace(reader);

    /* Improper list */
    if (reader->source[reader->index] == '.')
    {
        reader->index++;
        /* Read the cdr */
        VALUE cdr = Reader_read(reader);
        /* Error if error */
        if (cdr == NULL)
            return NULL;
        LixpCons_cdr(cons) = cdr;
        /* Skip over ) */
        reader->index++;
        
        return cons;
    }

    /* Proper list */
    Reader_skip_whitespace(reader);
    /* Step back one to simulate opening ( */
    reader->index--;
    /* Read in the rest of the list recursively */
    VALUE cdr = Reader_read_list(reader);
    /* Error if error */
    if (cdr == NULL)
        return NULL;
    LixpCons_cdr(cons) = cdr;
    return cons;
}

VALUE Reader_read_number(Reader *reader)
{
    /* TODO: CLean up */
    /*char *ns = Reader_read_until(reader, WHITESPACE ")");*/
    char *ns = Reader_read_while(reader, "-0123456789");
    int n, r;
    r = sscanf(ns, "%d", &n);
    /* TODO: Handle errors better somehow? */
    if (r < 1)
    {
        reader->error = "Invalid number literal";
        return NULL;
    }
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
    if (reader->source[reader->index] == '|')
    {
        /* TODO: Allow \| escaping? */
        /* Skip first | */
        reader->index++;
        VALUE value = LixpSymbol_new(Reader_read_until(reader, "|"));
        /* Skip over ending | */
        reader->index++;
        return value;
    }
    return LixpSymbol_new(Reader_read_until(reader, WHITESPACE ")"));
}

VALUE Reader_read_quote(Reader *reader)
{
    /* Skip ' */
    reader->index++;

    /* Read the quoted expression */
    VALUE expression = Reader_read(reader);

    return LixpCons_new(LixpBuiltin_new(LixpBuiltin_quote), LixpCons_new(expression, LixpCons_new(NULL, NULL)));
}

VALUE Reader_read(Reader *reader)
{
    /* No more to read */
    if ((unsigned) reader->index >= strlen(reader->source))
        return NULL;

    Reader_skip_whitespace(reader);
    char c = reader->source[reader->index];

    /* Unexpected close-paren */
    if (c == ')')
    {
        reader->error = "Mismatched parenthesis";
        return NULL;
    }
    
    if (c == '(')
        return Reader_read_list(reader);
    if ((c >= '0' && c <= '9') || c == '-')
        return Reader_read_number(reader);
    if (c == '\\')
        return Reader_read_character(reader);
    if (c == '"')
        return Reader_read_string(reader);
    if (c == ':')
        return Reader_read_keyword(reader);
    if (c == '\'')
        return Reader_read_quote(reader);
    return Reader_read_symbol(reader);
}
