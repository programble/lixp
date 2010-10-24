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

void bind_builtins(Scope *scope)
{
    Scope_set(scope, "quote", LixpBuiltin_new(LixpBuiltin_quote));
    Scope_set(scope, "eval", LixpBuiltin_new(LixpBuiltin_eval));
    Scope_set(scope, "car", LixpBuiltin_new(LixpBuiltin_car));
    Scope_set(scope, "cdr", LixpBuiltin_new(LixpBuiltin_cdr));
}

void LixpBuiltin_call(VALUE ret, VALUE builtin, VALUE params, Scope *scope)
{
    switch (LixpBuiltin_value(builtin))
    {
    case LixpBuiltin_quote:
        LixpBuiltin_quote_call(ret, params, scope);
        break;
    case LixpBuiltin_eval:
        LixpBuiltin_eval_call(ret, params, scope);
    case LixpBuiltin_car:
        LixpBuiltin_car_call(ret, params, scope);
        break;
    case LixpBuiltin_cdr:
        LixpBuiltin_cdr_call(ret, params, scope);
        break;
    }
}

void LixpBuiltin_quote_call(VALUE ret, VALUE params, Scope *scope)
{
    VALUE arg = LixpCons_car(params);
    ret->type = arg->type;
    ret->value1 = arg->value1;
    ret->value2 = arg->value2;
    LixpValue_destroy(params);
}

void LixpBuiltin_eval_call(VALUE ret, VALUE params, Scope *scope)
{
    VALUE arg = LixpCons_car(params);
    LixpValue_evaluate(arg, scope);
    LixpValue_evaluate(arg, scope);
    ret->type = arg->type;
    ret->value1 = arg->value1;
    ret->value2 = arg->value2;
    LixpValue_destroy(params);
}

void LixpBuiltin_car_call(VALUE ret, VALUE params, Scope *scope)
{
    VALUE list = LixpCons_car(params);
    LixpValue_evaluate(list, scope);
    if (list->type != LixpType_cons)
        /* TODO: Error */ return;
    VALUE car = LixpCons_car(list);
    ret->type = car->type;
    ret->value1 = car->value1;
    ret->value2 = car->value2;
    LixpValue_destroy(params);
}

void LixpBuiltin_cdr_call(VALUE ret, VALUE params, Scope *scope)
{
    VALUE list = LixpCons_car(params);
    LixpValue_evaluate(list, scope);
    if (list->type != LixpType_cons)
        /* TODO: Error */ return;
    VALUE cdr = LixpCons_cdr(list);
    ret->type = cdr->type;
    ret->value1 = cdr->value1;
    ret->value2 = cdr->value2;
    /* TODO: Fix this horrible memleak (somehow) */
    /*LixpValue_destroy(params);*/
}
