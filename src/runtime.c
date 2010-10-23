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

#include "runtime.h"

#include <stdlib.h>
#include <stdio.h>

VALUE LixpNumber_new(int value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_number;
    LixpNumber_value(new) = value;
    return new;
}

VALUE LixpCharacter_new(char value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_character;
    LixpCharacter_value(new) = value;
    return new;
}

VALUE LixpString_new(char *value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_string;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpSymbol_new(char *value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_symbol;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpKeyword_new(char *value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_keyword;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpCons_new(VALUE car, VALUE cdr)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_cons;
    LixpCons_car(new) = car;
    LixpCons_cdr(new) = cdr;
    return new;
}

VALUE LixpBuiltin_new(enum LixpBuiltins value)
{
    VALUE new = malloc(sizeof(VALUE));
    new->type = LixpType_builtin;
    LixpBuiltin_value(new) = value;
    return new;
}

void LixpNumber_destroy(VALUE value)
{
    free(value);
}

void LixpString_destroy(VALUE value)
{
    free(LixpString_value(value));
    free(value);
}

void LixpCons_destroy(VALUE value)
{
    if (LixpCons_car(value))
        LixpValue_destroy(LixpCons_car(value));
    if (LixpCons_cdr(value))
        LixpValue_destroy(LixpCons_cdr(value));
    free(value);
}

void LixpValue_destroy(VALUE value)
{
    switch (value->type)
    {
    case LixpType_number:
    case LixpType_character:
    case LixpType_builtin:
        LixpNumber_destroy(value);
        break;
    case LixpType_string:
    case LixpType_symbol:
    case LixpType_keyword:
        LixpString_destroy(value);
        break;
    case LixpType_cons:
        LixpCons_destroy(value);
        break;
    }
}

/* I'm sorry for using a GNU extension, but I'm lazy */

char *LixpNumber_inspect(VALUE value)
{
    char *str;
    asprintf(&str, "%d", LixpNumber_value(value));
    return str;
}

char *LixpCharacter_inspect(VALUE value)
{
    char *str;
    /* TODO: Special cases (space, etc) */
    asprintf(&str, "\\%c", LixpCharacter_value(value));
    return str;
}

char *LixpString_inspect(VALUE value)
{
    char *str;
    /* TODO: Escapes (\\, \n, etc) */
    asprintf(&str, "\"%s\"", LixpString_value(value));
    return str;
}

char *LixpSymbol_inspect(VALUE value)
{
    char *str;
    /* TODO: Check for spaces if we are allowing |foo bar| type symbols */
    asprintf(&str, "%s", LixpSymbol_value(value));
    return str;
}

char *LixpKeyword_inspect(VALUE value)
{
    char *str;
    asprintf(&str, ":%s", LixpKeyword_value(value));
    return str;
}

char *LixpCons_inspect_(VALUE value)
{
    char *car, *str;
    /* End of list */
    if (LixpCons_car(LixpCons_cdr(value)) == NULL && LixpCons_cdr(LixpCons_cdr(value)) == NULL)
    {
        car = LixpValue_inspect(LixpCons_car(value));
        asprintf(&str, "%s)", car);
        free(car);
    }
    else
    {
        car = LixpValue_inspect(LixpCons_car(value));
        char *cdr = LixpCons_inspect_(LixpCons_cdr(value));
        asprintf(&str, "%s %s", car, cdr);
        free(car);
        free(cdr);
    }
    return str;
}

char *LixpCons_inspect(VALUE value)
{
    char *str;
    /* nil */
    if (LixpCons_car(value) == NULL && LixpCons_cdr(value) == NULL)
        asprintf(&str, "nil");
    /* Improper list */
    else if (LixpCons_cdr(value)->type != LixpType_cons)
    {
        char *car, *cdr;
        car = LixpValue_inspect(LixpCons_car(value));
        cdr = LixpValue_inspect(LixpCons_cdr(value));
        asprintf(&str, "(%s . %s)", car, cdr);
        free(car);
        free(cdr);
    }
    /* Proper List */
    else
    {
        char *inspect_ = LixpCons_inspect_(value);
        asprintf(&str, "(%s", inspect_);
        free(inspect_);
    }
    return str;
}

char *LixpBuiltin_inspect(VALUE value)
{
    /* TODO: Change this maybe? */
    char *str;
    asprintf(&str, "<LixpBuiltin:%d@%#08x>", LixpBuiltin_value(value), (unsigned int)value);
    return str;
}

char *LixpValue_inspect(VALUE value)
{
    switch (value->type)
    {
    case LixpType_number:
        return LixpNumber_inspect(value);
    case LixpType_character:
        return LixpCharacter_inspect(value);
    case LixpType_string:
        return LixpString_inspect(value);
    case LixpType_symbol:
        return LixpSymbol_inspect(value);
    case LixpType_keyword:
        return LixpKeyword_inspect(value);
    case LixpType_cons:
        return LixpCons_inspect(value);
    case LixpType_builtin:
        return LixpBuiltin_inspect(value);
    default:
        return NULL;
    }
}
