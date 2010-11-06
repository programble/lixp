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

#include "scope.h"
#include "builtins.h"

VALUE LixpSymbol_evaluate(VALUE value, Scope *scope)
{
    VALUE get = Scope_get(scope, LixpSymbol_value(value));
    if (!get)
        return LixpError_new("undefined");
    return get;
}

VALUE LixpCons_evaluate(VALUE value, Scope *scope)
{
    /* nil evaluates to nil */
    if (LixpCons_car(value) == NULL && LixpCons_cdr(value) == NULL)
        return LixpCons_new(NULL, NULL);

    switch (LixpCons_car(value)->type)
    {
    case LixpType_symbol:
    case LixpType_cons:
        LixpCons_car(value) = LixpValue_evaluate(LixpCons_car(value), scope);
        return LixpCons_evaluate(value, scope);
    case LixpType_builtin:
        return LixpBuiltin_call(LixpCons_car(value), LixpCons_cdr(value), scope);
    /* TODO: Come up with better names for these errors maybe */
    case LixpType_number:
        return LixpError_new("cannot-call-number");
    case LixpType_character:
        return LixpError_new("cannot-call-character");
    case LixpType_string:
        return LixpError_new("cannot-call-string");
    case LixpType_keyword:
        return LixpError_new("cannot-call-keyword");
    case LixpType_error:
        return LixpError_new("cannot-call-error");
    default:
        return LixpError_new("cannot-call-wtf");
    }
}

VALUE LixpValue_evaluate(VALUE value, Scope *scope)
{
    switch (value->type)
    {
    case LixpType_symbol:
        return LixpSymbol_evaluate(value, scope);
    case LixpType_cons:
        return LixpCons_evaluate(value, scope);
    default:
        return value;
    }
}
