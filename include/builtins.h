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

#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#include "runtime.h"
#include "scope.h"

void bind_builtins(Scope *scope);

VALUE LixpBuiltin_call(VALUE builtin, VALUE params, Scope *scope);

VALUE LixpBuiltin_quote_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_eval_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_car_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_cdr_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_cons_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_if_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_eq_call(VALUE params, Scope *scope);
VALUE LixpBuiltin_def_call(VALUE params, Scope *scope);

#endif
