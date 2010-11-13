import LispValue
import ../Scope

LispCharacter: class extends LispValue {
    value: Char { get set }

    init: func (=value)

    toString: func -> String {
        format("\\%c", value)
    }

    equals?: func (other: LispValue) {
        other class == This && value == other as This value
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}
