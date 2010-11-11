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

#include "fn.h"

#include "consutils.h"
#include "builtins.h"

VALUE LixpFn_call(VALUE value, VALUE params, Scope *scope)
{
    VALUE arglist = LixpFn_arglist(value);
    VALUE body = LixpFn_body(value);

    Scope *fn_scope = Scope_new(scope);

    /* TODO: Optional and Rest args */
    if (LixpCons_length(arglist) != LixpCons_length(params))
        return LixpError_new("wrong-number-of-arguments");

    VALUE aiter = arglist;
    VALUE viter = params;
    while (!LixpCons_nil(aiter))
    {
        VALUE bind = LixpCons_car(aiter);
        VALUE value = LixpCons_car(viter);
        Scope_set(fn_scope, LixpSymbol_value(bind), LixpValue_evaluate(value, scope));
        aiter = LixpCons_cdr(aiter);
        viter = LixpCons_cdr(viter);
    }
    return LixpValue_evaluate(body, fn_scope);
}
