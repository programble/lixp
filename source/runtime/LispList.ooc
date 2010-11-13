import structs/ArrayList

import LispValue
import ../Scope

LispList: abstract class extends LispValue {

    new: static func ~proper (items: ArrayList<LispValue>) -> LispList {
        LispProperList new(items)
    }

    new: static func ~improper (car: LispValue, cdr: LispValue) -> ListList {
        LispImproperList new(car, cdr)
    }
}

LispProperList: class extends LispList {
    items: ArrayList<LispValue> { get set }

    init: func (=items)

    toString: func -> String {
        format("(%s)", items join(" "))
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && items == other as This items
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        // TODO: Implement
    }
}
