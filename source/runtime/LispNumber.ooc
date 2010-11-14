import LispValue
import ../Scope

LispNumber: class extends LispValue {
    ivalue: Int
    fvalue: Float
    floating: Bool

    init: func~int (=ivalue) {
        floating = false
    }

    init: func~float (=fvalue) {
        floating = true
    }

    toString: func -> String {
        if (floating) {
            fvalue toString()
        } else {
            ivalue toString()
        }
    }

    equals?: func (other: LispValue) -> Bool {
        if (other class == This && this floating == other as This floating) {
            if (floating) {
                return this fvalue == other as This fvalue
            } else {
                return this ivalue == other as This ivalue
            }
        }
        false
    }   
}
