import LispValue

LispKeyword: class extends LispValue {
    value: String

    init: func (=value)

    toString: func -> String {
        ":%s" format(value)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }
}
