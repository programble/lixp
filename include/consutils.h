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

#ifndef __CONSUTILS_H__
#define __CONSUTILS_H__

#include "runtime.h"

int LixpCons_nil(VALUE cons);
int LixpCons_improper(VALUE cons);
int LixpCons_proper(VALUE cons);

int LixpCons_length(VALUE cons);

#define LixpCons_each(cons, iter) for(VALUE iter = cons; !LixpCons_nil(iter); iter = LixpCons_cdr(iter))

#endif
