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

#ifndef __READLINE_H__
#define __READLINE_H__

#include <stdlib.h>
#include <stdio.h>

#ifdef READLINE

#include <readline/readline.h>
#include <readline/history.h>

#else

/* Drop-in replacements for readline functions */

char *readline(const char *prompt);
void add_history(char *line);

#endif

#endif
