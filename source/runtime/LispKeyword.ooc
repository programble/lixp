import LispValue
import ../Scope

LispKeyword: class extends LispValue {
    value: String { get set }

    init: func (=value)

    toString: func -> String {
        format(":%s", value)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && value == other as This value
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}
