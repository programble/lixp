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
#include <getopt.h>

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

void opt_help(char *self)
{
    printf("Usage: %s [option] ... [file]\n"
           "Options:\n"
           "  -r, --repl                  Start a REPL\n"
           "  -e EXPR, --evaluate=EXPR    Evaluate a single expression\n"
           "  --version                   Print version information\n",
           self);
}

void repl()
{
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
            exp = LixpValue_evaluate(exp, global_scope);
            char *inspect = LixpValue_inspect(exp);
            printf("%s\n", inspect);
        }
    }
}

int main(int argc, char **argv)
{
    GC_INIT();
    struct option options[] =
    {
        {"version", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {"evaluate", required_argument, NULL, 'e'},
        {"repl", no_argument, NULL, 'r'},
        {0, 0, 0, 0}
    };

    int o;
    while (1)
    {
        o = getopt_long(argc, argv, "he:r", options, NULL);
        if (o == -1)
            break;

        switch(o)
        {
        case 'v':
            version_info();
            return 0;
        case 'h':
            opt_help(argv[0]);
            return 0;
        case 'e':
            /* TODO: Stuffs */
        case 'r':
            repl();
            return 0;
        default:
            return 1;
        }
    }
    /* Start a repl by default */
    repl();
    return 0;
}
