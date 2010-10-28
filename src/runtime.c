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
#include <gc.h>

#include "strutils.h"

VALUE LixpNumber_new(int value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_number;
    LixpNumber_value(new) = value;
    return new;
}

VALUE LixpCharacter_new(char value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_character;
    LixpCharacter_value(new) = value;
    return new;
}

VALUE LixpString_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_string;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpSymbol_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_symbol;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpKeyword_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_keyword;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpCons_new(VALUE car, VALUE cdr)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_cons;
    LixpCons_car(new) = car;
    LixpCons_cdr(new) = cdr;
    return new;
}

VALUE LixpBuiltin_new(enum LixpBuiltins value)
{
    VALUE new = GC_MALLOC(sizeof(VALUE));
    new->type = LixpType_builtin;
    LixpBuiltin_value(new) = value;
    return new;
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
    /* TODO: Improve this */
    if (str_has_char(LixpSymbol_value(value), ' ') || str_has_char(LixpSymbol_value(value), ')') || str_has_char(LixpSymbol_value(value), '\t') || str_has_char(LixpSymbol_value(value), '\n') || str_has_char(LixpSymbol_value(value), '\r'))
        asprintf(&str, "|%s|", LixpSymbol_value(value));
    else
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

#include "builtins.h"

void LixpSymbol_evaluate(VALUE value, Scope *scope)
{
    VALUE new = Scope_get(scope, LixpSymbol_value(value));
    /* Undefined */
    if (new == NULL)
    {
        value->type = LixpType_cons;
        LixpCons_car(value) = NULL;
        LixpCons_cdr(value) = NULL;
        return;
    }
    value->type = new->type;
    value->value1 = new->value1;
    value->value2 = new->value2;
}

void LixpCons_evaluate(VALUE value, Scope *scope)
{
    /* nil evaluates to nil */
    if (LixpCons_car(value) == NULL && LixpCons_cdr(value) == NULL)
        return;
    switch (LixpCons_car(value)->type)
    {
    case LixpType_symbol:
    case LixpType_cons:
        LixpValue_evaluate(LixpCons_car(value), scope);
    case LixpType_builtin:
        LixpBuiltin_call(value, LixpCons_car(value), LixpCons_cdr(value), scope);
        break;
    default:
        /* TODO: Handle error somehow */
        return;
    }
}

void LixpValue_evaluate(VALUE value, Scope *scope)
{
    switch (value->type)
    {
    case LixpType_symbol:
        LixpSymbol_evaluate(value, scope);
        break;
    case LixpType_cons:
        LixpCons_evaluate(value, scope);
        break;
    default:
        return;
    }
}
