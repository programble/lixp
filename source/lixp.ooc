import structs/ArrayList

import runtime/[LispValue, LispBuiltin]
import Scope
import LispReader

// HACK
EOF: extern Int
readLine: func -> String {
    buf := Buffer new(1023)
    while (true) {
        c := fgetc(stdin)
        if (c == EOF) {
            return null
        }
        if (c == '\n') {
            break
        }
        buf append((c & 0xFF) as Char)
    }
    buf toString()
}

main: func {
    "Lixp (ooc branch)" println()
    scope := Scope<LispValue> new()
    LispBuiltin bindAll(scope)
    while (true) {
        "=> " print()
        input := readLine()
        if (input == null) {
            break
        }
        reader := LispReader new(input)
        exprs := reader readAll()
        for (expr in exprs) {
            expr evaluate(scope) toString() println()
        }
    }
}
