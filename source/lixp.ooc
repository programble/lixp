import structs/ArrayList

import runtime/LispValue
import Scope
import LispReader

main: func {
    "Lixp (ooc branch)" println()
    scope := Scope<LispValue> new()
    while (stdin hasNext?()) {
        "=> " print()
        input := stdin readLine()
        reader := LispReader new(input)
        exprs := reader readAll()
        for (expr in exprs) {
            expr evaluate(scope) toString() println()
        }
    }
}
