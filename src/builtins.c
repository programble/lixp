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

#include "version.h"

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
    /* Constants */
    Scope_set(scope, "*lixp-version*", LixpString_new(VERSION));
    /* Builtin functions */
    Scope_set(scope, "quote", LixpBuiltin_new(LixpBuiltin_quote));
    Scope_set(scope, "eval", LixpBuiltin_new(LixpBuiltin_eval));
    Scope_set(scope, "car", LixpBuiltin_new(LixpBuiltin_car));
    Scope_set(scope, "cdr", LixpBuiltin_new(LixpBuiltin_cdr));
    Scope_set(scope, "cons", LixpBuiltin_new(LixpBuiltin_cons));
    Scope_set(scope, "cond", LixpBuiltin_new(LixpBuiltin_cond));
    Scope_set(scope, "=", LixpBuiltin_new(LixpBuiltin_eq));
    
    Scope_set(scope, "def", LixpBuiltin_new(LixpBuiltin_def));
    Scope_set(scope, "undef!", LixpBuiltin_new(LixpBuiltin_undef));
    Scope_set(scope, "set!", LixpBuiltin_new(LixpBuiltin_set));
    Scope_set(scope, "unset!", LixpBuiltin_new(LixpBuiltin_unset));

    Scope_set(scope, "list", LixpBuiltin_new(LixpBuiltin_list));
    Scope_set(scope, "do", LixpBuiltin_new(LixpBuiltin_do));
    Scope_set(scope, "do-while-wearing-blue-underwear", LixpBuiltin_new(LixpBuiltin_do));
    Scope_set(scope, "let", LixpBuiltin_new(LixpBuiltin_let));

    Scope_set(scope, "number?", LixpBuiltin_new(LixpBuiltin_numberp));
    Scope_set(scope, "character?", LixpBuiltin_new(LixpBuiltin_characterp));
    Scope_set(scope, "string?", LixpBuiltin_new(LixpBuiltin_stringp));
    Scope_set(scope, "symbol?", LixpBuiltin_new(LixpBuiltin_symbolp));
    Scope_set(scope, "keyword?", LixpBuiltin_new(LixpBuiltin_keywordp));
    Scope_set(scope, "cons?", LixpBuiltin_new(LixpBuiltin_consp));
    Scope_set(scope, "builtin?", LixpBuiltin_new(LixpBuiltin_builtinp));
    Scope_set(scope, "error?", LixpBuiltin_new(LixpBuiltin_errorp));
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
    case LixpBuiltin_undef:
        return LixpBuiltin_undef_call(params, scope);
    case LixpBuiltin_set:
        return LixpBuiltin_set_call(params, scope);
    case LixpBuiltin_unset:
        return LixpBuiltin_unset_call(params, scope);
    case LixpBuiltin_list:
        return LixpBuiltin_list_call(params, scope);
    case LixpBuiltin_do:
        return LixpBuiltin_do_call(params, scope);
    case LixpBuiltin_let:
        return LixpBuiltin_let_call(params, scope);
    case LixpBuiltin_numberp:
        return LixpBuiltin_numberp_call(params, scope);
    case LixpBuiltin_characterp:
        return LixpBuiltin_characterp_call(params, scope);
    case LixpBuiltin_stringp:
        return LixpBuiltin_stringp_call(params, scope);
    case LixpBuiltin_symbolp:
        return LixpBuiltin_symbolp_call(params, scope);
    case LixpBuiltin_keywordp:
        return LixpBuiltin_keywordp_call(params, scope);
    case LixpBuiltin_consp:
        return LixpBuiltin_consp_call(params, scope);
    case LixpBuiltin_builtinp:
        return LixpBuiltin_builtinp_call(params, scope);
    case LixpBuiltin_errorp:
        return LixpBuiltin_errorp_call(params, scope);
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
    /* (car nil) ;-> nil */
    if (LixpCons_car(list) == NULL && LixpCons_cdr(list) == NULL)
        return list;
    return LixpCons_car(list);
}

VALUE LixpBuiltin_cdr_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    VALUE list = LixpValue_evaluate(LixpCons_car(params), scope);
    if (list->type != LixpType_cons)
        return LixpError_new("unexpected-type"); /* TODO: Better error */
    /* (cdr nil) ;-> nil */
    if (LixpCons_car(list) == NULL && LixpCons_cdr(list) == NULL)
        return list;
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

    VALUE a = LixpValue_evaluate(LixpCons_car(params), scope);
    VALUE b = LixpValue_evaluate(LixpCons_car(LixpCons_cdr(params)), scope);

    return LixpValue_equals(a, b) ? t : nil;
}

VALUE LixpBuiltin_def_call(VALUE params, Scope *scope)
{
    params_require_2(params);

    /* Travel up the scope tree to the global scope */
    while (scope->parent != NULL)
        scope = scope->parent;

    VALUE symbol = LixpCons_car(params);
    VALUE value = LixpCons_car(LixpCons_cdr(params));

    if (symbol->type != LixpType_symbol)
        return LixpError_new("cannot-bind-to-non-symbol");
    if (Scope_get(scope, LixpSymbol_value(symbol)))
        return LixpError_new("symbol-already-bound");

    Scope_set(scope, LixpSymbol_value(symbol), LixpValue_evaluate(value, scope));
    return symbol;
}

VALUE LixpBuiltin_undef_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    /* Travel up the scope tree to the global scope */
    while (scope->parent != NULL)
        scope = scope->parent;

    VALUE symbol = LixpCons_car(params);
    if (symbol->type != LixpType_symbol)
        return LixpError_new("cannot-bind-to-non-symbol");

    VALUE value = Scope_get(scope, LixpSymbol_value(symbol));
    if (!value)
        value = nil;
    
    Scope_del(scope, LixpSymbol_value(symbol));
    return value;
}

VALUE LixpBuiltin_set_call(VALUE params, Scope *scope)
{
    params_require_2(params);

    VALUE symbol = LixpCons_car(params);
    VALUE value = LixpCons_car(LixpCons_cdr(params));

    if (symbol->type != LixpType_symbol)
        return LixpError_new("cannot-bind-to-non-symbol");

    VALUE previous = Scope_get(scope, LixpSymbol_value(symbol));
    if (!previous)
        previous = nil;

    Scope_set(scope, LixpSymbol_value(symbol), LixpValue_evaluate(value, scope));
    return previous;
}

VALUE LixpBuiltin_unset_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    VALUE symbol = LixpCons_car(params);
    if (symbol->type != LixpType_symbol)
        return LixpError_new("cannot-bind-to-non-symbol");

    VALUE value = Scope_get(scope, LixpSymbol_value(symbol));
    if (!value)
        value = nil;
    
    Scope_del(scope, LixpSymbol_value(symbol));
    return value;
}

VALUE LixpBuiltin_list_call(VALUE params, Scope *scope)
{
    VALUE piter = params;
    VALUE list = nil;
    VALUE liter = list;
    while (LixpCons_car(piter) != NULL && LixpCons_cdr(piter) != NULL)
    {
        LixpCons_car(liter) = LixpValue_evaluate(LixpCons_car(piter), scope);
        piter = LixpCons_cdr(piter);
        VALUE next = nil;
        LixpCons_cdr(liter) = next;
        liter = next;
    }
    return list;
}

VALUE LixpBuiltin_do_call(VALUE params, Scope *scope)
{
    params_require_1(params);
    
    VALUE iter = params;
    while (LixpCons_car(LixpCons_cdr(iter)) != NULL && LixpCons_cdr(LixpCons_cdr(iter)) != NULL)
    {
        LixpValue_evaluate(LixpCons_car(iter), scope);
        iter = LixpCons_cdr(iter);
    }
    return LixpValue_evaluate(LixpCons_car(iter), scope);
}

VALUE LixpBuiltin_let_call(VALUE params, Scope *scope)
{
    params_require_2(params);
    
    VALUE bindings = LixpCons_car(params);

    Scope *local_scope = Scope_new(scope);

    VALUE iter = bindings;
    while (LixpCons_car(iter) != NULL && LixpCons_cdr(iter) != NULL)
    {
        VALUE pair = LixpCons_car(iter);
        params_require_2(pair);
        LixpBuiltin_set_call(pair, local_scope);
        iter = LixpCons_cdr(iter);
    }

    return LixpBuiltin_do_call(LixpCons_cdr(params), local_scope);
}

VALUE LixpBuiltin_numberp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_number ? t : nil;
}

VALUE LixpBuiltin_characterp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_character ? t : nil;
}

VALUE LixpBuiltin_stringp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_string ? t : nil;
}

VALUE LixpBuiltin_symbolp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_symbol ? t : nil;
}

VALUE LixpBuiltin_keywordp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_keyword ? t : nil;
}

VALUE LixpBuiltin_consp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_cons ? t : nil;
}

VALUE LixpBuiltin_builtinp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_builtin ? t : nil;
}

VALUE LixpBuiltin_errorp_call(VALUE params, Scope *scope)
{
    params_require_1(params);

    return LixpValue_evaluate(LixpCons_car(params), scope)->type == LixpType_error ? t : nil;
}
