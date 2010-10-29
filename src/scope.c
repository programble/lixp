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

#include "scope.h"

#include <stdlib.h>
#include <gc.h>

#include "treemap.h"

Scope *Scope_new(Scope *parent)
{
    Scope *scope = GC_MALLOC(sizeof(Scope));
    scope->parent = parent;
    scope->map = TreeMap_new();
    return scope;
}

void Scope_set(Scope *scope, const char *key, void *value)
{
    TreeMap_set(scope->map, key, value);
}

void *Scope_get(Scope *scope, const char *key)
{
    void *value = TreeMap_get(scope->map, key);
    if (!value && !scope->parent)
        return NULL;
    if (!value)
        return Scope_get(scope->parent, key);
    return value;
}

void Scope_del(Scope *scope, const char *key)
{
    TreeMap_del(scope->map, key);
}
