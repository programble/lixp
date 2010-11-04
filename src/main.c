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
#include <gc.h>

#include "readline.h"

#include "version.h"

#include "runtime.h"
#include "reader.h"
#include "builtins.h"
#include "scope.h"

#define S(x) #x
#define SS(x) S(x)
#define GC_VERSION SS(GC_VERSION_MAJOR) "." SS(GC_VERSION_MINOR)
void version_info()
{
    printf("Lixp " VERSION "\n");
    printf("Boehm GC " GC_VERSION "\n");
    printf(__DATE__ " " COMPILER "\n");
}

int main(int argc, char **argv)
{
    GC_INIT();
    printf("Lixp %s\n", VERSION);
    Scope *global_scope = Scope_new(NULL);
    bind_builtins(global_scope);
    while (1)
    {
        char *input = readline("=> ");
        if (input == NULL)
            break;
        /* Add to history if not blank */
        if (*input)
            add_history(input);
        
        Reader *reader = Reader_new(input);
        while (1)
        {
            VALUE exp = Reader_read(reader);
            if (exp == NULL)
            {
                if (reader->error)
                    printf("Error: %s\n", reader->error);
                break;
            }
            LixpValue_evaluate(exp, global_scope);
            char *inspect = LixpValue_inspect(exp);
            printf("%s\n", inspect);
        }
    }
    return 0;
}
