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

#ifndef __TREEMAP_H__
#define __TREEMAP_H__

typedef struct TreeMapNode
{
    const char *key;
    void *value;
    struct TreeMapNode *lesser, *greater, *parent;
} TreeMapNode;

typedef struct TreeMap
{
    TreeMapNode *root;
} TreeMap;

TreeMap *TreeMap_new();
void TreeMap_set(TreeMap *map, const char *key, void *value);
void *TreeMap_get(TreeMap *map, const char *key);
void TreeMap_del(TreeMap *map, const char *key);

#endif
