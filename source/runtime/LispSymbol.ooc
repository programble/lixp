import LispValue
import ../Scope

LispSymbol: class extends LispValue {
    value: String { get set }

    init: func (=value)

    toString: func -> String {
        if (value contains?(' ')) {
            format("|%s|", value)
        } else {
            value
        }
    }

    equals?: func (other: LispValue) {
        // TODO: Implement
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        // TODO: Catch undefined
        scope[value]
    }
}
