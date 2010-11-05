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

#define params_require_1(params) if (LixpCons_car(params) == NULL) return LixpError_new("wrong-number-of-arguments");
#define params_require_2(params) if (LixpCons_car(params) == NULL || LixpCons_car(LixpCons_cdr(params)) == NULL) return LixpError_new("wrong-number-of-arguments");
#define params_require_3(params) if (LixpCons_car(params) == NULL || LixpCons_car(LixpCons_cdr(params)) == NULL || LixpCons_car(LixpCons_cdr(LixpCons_cdr(params))) == NULL) return LixpError_new("wrong-number-of-arguments");

#define t LixpSymbol_new("t")
#define nil LixpCons_new(NULL, NULL)

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
    Scope_set(scope, "cond", LixpBuiltin_new(LixpBuiltin_cond));
    Scope_set(scope, "=", LixpBuiltin_new(LixpBuiltin_eq));
    Scope_set(scope, "def", LixpBuiltin_new(LixpBuiltin_def));
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
        /*case LixpBuiltin_if:
          return LixpBuiltin_if_call(params, scope);*/
    case LixpBuiltin_cond:
        return LixpBuiltin_cond_call(params, scope);
    case LixpBuiltin_eq:
        return LixpBuiltin_eq_call(params, scope);
    case LixpBuiltin_def:
        return LixpBuiltin_def_call(params, scope);
    default:
        return LixpError_new("unknown-builtin");
    }
}

VALUE LixpBuiltin_quote_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    return LixpCons_car(params);
}

VALUE LixpBuiltin_eval_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    return LixpValue_evaluate(LixpValue_evaluate(LixpCons_car(params), scope), scope);
}


VALUE LixpBuiltin_car_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    VALUE list = LixpValue_evaluate(LixpCons_car(params), scope);
    if (list->type != LixpType_cons)
        return LixpError_new("unexpected-type"); /* TODO: Better error */
    return LixpCons_car(list);
}

VALUE LixpBuiltin_cdr_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    VALUE list = LixpValue_evaluate(LixpCons_car(params), scope);
    if (list->type != LixpType_cons)
        return LixpError_new("unexpected-type"); /* TODO: Better error */
    return LixpCons_cdr(list);
}

VALUE LixpBuiltin_cons_call(VALUE params, Scope *scope)
{
    params_require_2(params);
    return LixpCons_new(LixpValue_evaluate(LixpCons_car(params), scope), LixpValue_evaluate(LixpCons_car(LixpCons_cdr(params)), scope));
}

VALUE LixpBuiltin_cond_call(VALUE params, Scope *scope)
{
    if (LixpCons_car(params) == NULL && LixpCons_cdr(params) == NULL)
        return nil;

    VALUE iter = params;
    while (LixpCons_car(iter) != NULL && LixpCons_cdr(iter) != NULL)
    {
        VALUE pair = LixpCons_car(iter);
        params_require_1(pair);
        VALUE predicate = LixpCons_car(pair);
        VALUE then = LixpCons_car(LixpCons_cdr(pair));
        if (!then)
            then = predicate;
        VALUE result = LixpValue_evaluate(predicate, scope);
        if (result->type != LixpType_cons || LixpCons_car(result) != NULL || LixpCons_cdr(result) != NULL)
            return LixpValue_evaluate(then, scope);
        iter = LixpCons_cdr(iter);
    }
    return nil;
}

VALUE LixpBuiltin_eq_call(VALUE params, Scope *scope)
{
    params_require_2(params);

    VALUE a = LixpCons_car(params);
    VALUE b = LixpCons_car(LixpCons_cdr(params));

    if (a->type != b->type)
        return nil;
    if (a->value1.int_value != b->value1.int_value)
        return nil;
    if (a->type == LixpType_cons && a->value2.int_value != b->value2.int_value)
        return nil;
    return t;
}

VALUE LixpBuiltin_def_call(VALUE params, Scope *scope)
{
    params_require_2(params);

    VALUE symbol = LixpCons_car(params);
    VALUE value = LixpCons_car(LixpCons_cdr(params));

    if (symbol->type != LixpType_symbol)
        return LixpError_new("cannot-bind-to-non-symbol");
    if (Scope_get(scope, LixpSymbol_value(symbol)))
        return LixpError_new("symbol-already-bound");

    Scope_set(scope, LixpSymbol_value(symbol), LixpValue_evaluate(value, scope));
    return symbol;
}
