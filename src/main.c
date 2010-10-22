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

#include <stdlib.h>
#include <stdio.h>

#include "ast.h"

int main(int argc, char **argv)
{
#ifdef DEBUG    
    /*VALUE foo = LixpCons_new(LixpKeyword_new("foo"), LixpCons_new(LixpKeyword_new("bar"), LixpCons_new(LixpKeyword_new("baz"), LixpCons_new(NULL, NULL))));*/
    VALUE foo = LixpCons_new(NULL, NULL);
    char *str = LixpValue_inspect(foo);
    printf("%s\n", str);
    free(str);
#endif    
    return 0;
}
