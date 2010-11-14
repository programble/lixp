import LispValue

LispCharacter: class extends LispValue {
    value: Char

    init: func (=value)

    toString: func -> String {
        "\\%c" format(value)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }
}
