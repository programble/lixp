import text/EscapeSequence

import LispValue

LispString: class extends LispValue {
    value: String

    init: func (=value)

    toString: func -> String {
        "\"%s\"" format(EscapeSequence escape(value))
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }
}
