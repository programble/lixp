import LispValue
import ../Scope

LispKeyword: class extends LispValue {
    value: String { get set }

    init: func (=value)

    toString: func -> String {
        format(":%s", value)
    }

    equals?: func (other: LispValue) {
        // TODO: Implement
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}
