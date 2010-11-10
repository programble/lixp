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
#include <string.h>

int LixpNumber_equals(VALUE a, VALUE b)
{
    return LixpNumber_value(a) == LixpNumber_value(b);
}

int LixpCharacter_equals(VALUE a, VALUE b)
{
    return LixpCharacter_value(a) == LixpCharacter_value(b);
}

int LixpString_equals(VALUE a, VALUE b)
{
    return strcmp(LixpString_value(a), LixpString_value(b)) == 0;
}

int LixpSymbol_equals(VALUE a, VALUE b)
{
    return LixpString_equals(a, b);
}

int LixpKeyword_equals(VALUE a, VALUE b)
{
    return LixpString_equals(a, b);
}

int LixpCons_equals(VALUE a, VALUE b)
{
    /* TODO: Compare each item? */
    return LixpCons_car(a) == LixpCons_car(b) && LixpCons_cdr(a) == LixpCons_cdr(b);
}

int LixpBuiltin_equals(VALUE a, VALUE b)
{
    return LixpBuiltin_value(a) == LixpBuiltin_value(b);
}

int LixpError_equals(VALUE a, VALUE b)
{
    return LixpString_equals(a, b);
}

int LixpFn_equals(VALUE a, VALUE b)
{
    /* TODO: Should fns be equal only if they are the exact same fn or not? */
    return a == b;
}

int LixpValue_equals(VALUE a, VALUE b)
{
    if (a->type != b->type)
        return 0;
    switch (a->type)
    {
    case LixpType_number:
        return LixpNumber_equals(a, b);
    case LixpType_character:
        return LixpCharacter_equals(a, b);
    case LixpType_string:
        return LixpString_equals(a, b);
    case LixpType_symbol:
        return LixpSymbol_equals(a, b);
    case LixpType_keyword:
        return LixpKeyword_equals(a, b);
    case LixpType_cons:
        return LixpCons_equals(a, b);
    case LixpType_builtin:
        return LixpBuiltin_equals(a, b);
    case LixpType_error:
        return LixpError_equals(a, b);
    case LixpType_fn:
        return LixpFn_equals(a, b);
    }
    return 0;
}
