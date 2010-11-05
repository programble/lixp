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

#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include "scope.h"

enum LixpTypes
{
    LixpType_number,
    LixpType_character,
    LixpType_string,
    LixpType_symbol,
    LixpType_keyword,
    LixpType_cons,
    LixpType_builtin,
    LixpType_error,
    /*
    LixpType_lambda,
    LixpType_macro,
    */
};

enum LixpBuiltins
{
    LixpBuiltin_quote,
    LixpBuiltin_eval,
    LixpBuiltin_car,
    LixpBuiltin_cdr,
    LixpBuiltin_cons,
    LixpBuiltin_cond,
    LixpBuiltin_eq,
    LixpBuiltin_def,
};

struct LixpValue;

union LixpValueValue
{
    int int_value;
    char char_value;
    char *str_value;
    struct LixpValue *lixpvalue_value;
    enum LixpBuiltins builtin_value;
};

typedef struct LixpValue
{
    enum LixpTypes type;
    union LixpValueValue value1;
    union LixpValueValue value2;
} LixpValue;

#define VALUE LixpValue*

VALUE LixpNumber_new(int value);
VALUE LixpCharacter_new(char value);
VALUE LixpString_new(char *value);
VALUE LixpSymbol_new(char *value);
VALUE LixpKeyword_new(char *value);
VALUE LixpCons_new(VALUE car, VALUE cdr);
VALUE LixpBuiltin_new(enum LixpBuiltins value);
VALUE LixpError_new(char *value);

VALUE LixpValue_copy(VALUE value);

#define LixpNumber_value(x) x->value1.int_value
#define LixpCharacter_value(x) x->value1.char_value
#define LixpString_value(x) x->value1.str_value
#define LixpSymbol_value(x) x->value1.str_value
#define LixpKeyword_value(x) x->value1.str_value
#define LixpCons_car(x) x->value1.lixpvalue_value
#define LixpCons_cdr(x) x->value2.lixpvalue_value
#define LixpBuiltin_value(x) x->value1.builtin_value
#define LixpError_value(x) x->value1.str_value

char *LixpNumber_inspect(VALUE value);
char *LixpCharacter_inspect(VALUE value);
char *LixpString_inspect(VALUE value);
char *LixpSymbol_inspect(VALUE value);
char *LixpKeyword_inspect(VALUE value);
char *LixpCons_inspect(VALUE value);
char *LixpBuiltin_inspect(VALUE value);
char *LixpError_inspect(VALUE value);

char *LixpValue_inspect(VALUE value);

VALUE LixpSymbol_evaluate(VALUE value, Scope *scope);
VALUE LixpCons_evaluate(VALUE value, Scope *scope);

VALUE LixpValue_evaluate(VALUE value, Scope *scope);

#endif
