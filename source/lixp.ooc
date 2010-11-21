import structs/ArrayList

import runtime/[LispValue, LispBuiltin]
import Scope
import LispReader

main: func {
    "Lixp (ooc branch)" println()
    scope := Scope<LispValue> new()
    LispBuiltin bindAll(scope)
    while (stdin hasNext?()) {
        "=> " print()
        input := stdin readLine()
        reader := LispReader new(input)
        exprs := reader readAll()
        for (expr in exprs) {
            expr evaluate(scope) toString() println()
        }
        // TODO: Fix evaluation of random data on EOF
    }
}
