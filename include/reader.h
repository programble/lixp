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

#ifndef __READER_H__
#define __READER_H__

#include "runtime.h"

typedef struct Reader
{
    char *source;
    int index;
    char *error;
} Reader;

Reader *Reader_new(char *source);
void Reader_destroy(Reader *reader);

VALUE Reader_read_list(Reader *reader);
VALUE Reader_read_string(Reader *reader);
VALUE Reader_read_number(Reader *reader);
VALUE Reader_read_keyword(Reader *reader);
VALUE Reader_read_character(Reader *reader);

VALUE Reader_read_symbol(Reader *reader);

VALUE Reader_read(Reader *reader);

#endif
