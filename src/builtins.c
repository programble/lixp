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

#include "builtins.h"

#include <stdlib.h>
#include <gc.h>

void bind_builtins(Scope *scope)
{
    /* Standard symbols */
    Scope_set(scope, "nil", LixpCons_new(NULL, NULL));
    Scope_set(scope, "t", LixpSymbol_new("t"));
    Scope_set(scope, "quote", LixpBuiltin_new(LixpBuiltin_quote));
    Scope_set(scope, "eval", LixpBuiltin_new(LixpBuiltin_eval));
    Scope_set(scope, "car", LixpBuiltin_new(LixpBuiltin_car));
    Scope_set(scope, "cdr", LixpBuiltin_new(LixpBuiltin_cdr));
    Scope_set(scope, "cons", LixpBuiltin_new(LixpBuiltin_cons));
}

VALUE LixpBuiltin_call(VALUE builtin, VALUE params, Scope *scope)
{
    switch (LixpBuiltin_value(builtin))
    {
    case LixpBuiltin_quote:
        return LixpBuiltin_quote_call(params, scope);
    case LixpBuiltin_eval:
        return LixpBuiltin_eval_call(params, scope);
    case LixpBuiltin_car:
        return LixpBuiltin_car_call(params, scope);
    case LixpBuiltin_cdr:
        return LixpBuiltin_cdr_call(params, scope);
    case LixpBuiltin_cons:
        return LixpBuiltin_cons_call(params, scope);
    default:
        return LixpError_new("unknown-builtin");
    }
}

VALUE LixpBuiltin_quote_call(VALUE params, Scope *scope)
{
    return LixpCons_car(params);
}

VALUE LixpBuiltin_eval_call(VALUE params, Scope *scope)
{
    return LixpValue_evaluate(LixpValue_evaluate(LixpCons_car(params), scope), scope);
}


VALUE LixpBuiltin_car_call(VALUE params, Scope *scope)
{
    VALUE list = LixpValue_evaluate(LixpCons_car(params), scope);
    if (list->type != LixpType_cons)
        return LixpError_new("unexpected-type"); /* TODO: Better error */
    return LixpCons_car(list);
}

VALUE LixpBuiltin_cdr_call(VALUE params, Scope *scope)
{
    VALUE list = LixpValue_evaluate(LixpCons_car(params), scope);
    if (list->type != LixpType_cons)
        return LixpError_new("unexpected-type"); /* TODO: Better error */
    return LixpCons_cdr(list);
}

VALUE LixpBuiltin_cons_call(VALUE params, Scope *scope)
{
    return LixpCons_new(LixpValue_evaluate(LixpCons_car(params), scope), LixpValue_evaluate(LixpCons_car(LixpCons_cdr(params)), scope));
}
