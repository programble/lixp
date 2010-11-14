import LispValue
import ../Scope

LispSymbol: class extends LispValue {
    value: String

    init: func (=value)

    toString: func -> String {
        value
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        bind := scope[value]
        if (bind == null) {
            raise(This, "Undefined symbol")
        } else {
            return bind
        }
    }
}
            
