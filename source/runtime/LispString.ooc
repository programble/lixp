import text/EscapeSequence

import LispValue
import ../Scope

LispString: class extends LispValue {
    value: String { get set }

    init: func (=value)

    toString: func -> String {
        format("\"%s\"", EscapeSequence escape(value))
    }

    equals?: func (other: LispValue) {
        // TODO: Implement
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}

