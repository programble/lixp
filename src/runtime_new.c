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
#include <gc.h>

VALUE LixpNumber_new(int value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_number;
    LixpNumber_value(new) = value;
    return new;
}

VALUE LixpCharacter_new(char value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_character;
    LixpCharacter_value(new) = value;
    return new;
}

VALUE LixpString_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_string;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpSymbol_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_symbol;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpKeyword_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_keyword;
    LixpString_value(new) = value;
    return new;
}

VALUE LixpCons_new(VALUE car, VALUE cdr)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_cons;
    LixpCons_car(new) = car;
    LixpCons_cdr(new) = cdr;
    return new;
}

VALUE LixpBuiltin_new(enum LixpBuiltins value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_builtin;
    LixpBuiltin_value(new) = value;
    return new;
}

VALUE LixpError_new(char *value)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_error;
    LixpError_value(new) = value;
    return new;
}

VALUE LixpFn_new(VALUE arglist, VALUE body)
{
    VALUE new = GC_MALLOC(sizeof(LixpValue));
    new->type = LixpType_fn;
    LixpFn_arglist(new) = arglist;
    LixpFn_body(new) = body;
    return new;
}
