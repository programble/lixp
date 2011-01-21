import LispValue
import ../Scope
import exceptions

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
            UndefinedException new(value) throw()
        } else {
            return bind
        }
    }
}
            
