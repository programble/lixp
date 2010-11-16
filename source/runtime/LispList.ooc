import structs/ArrayList

import LispValue
import ../Scope

LispList: abstract class extends LispValue {
    new: static func~proper (items: ArrayList<LispValue>) -> LispList {
        LispProperList new(items)
    }

    new: static func~improper (car, cdr: LispValue) -> LispList {
        LispImproperList new(car, cdr)
    }

    new: static func~nil -> LispList {
        LispProperList new(ArrayList<LispValue> new())
    }
}

LispProperList: class extends LispList {
    items: ArrayList<LispValue>
    
    init: func (=items)

    toString: func -> String {
        stritems := items map(|x| x toString())
        "(%s)" format(stritems join(" "))
    }

    equals?: func (other: LispValue) -> Bool {
        // TODO: Implement
        //other class == This && this items equals?(other as This items)
        false
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        callee := items first()
        args := items[1..items size]
        callee call(args, scope)
    }
}

LispImproperList: class extends LispList {
    car, cdr: LispValue

    init: func (=car, =cdr)

    toString: func -> String {
        "(%s . %s)" format(car toString(), cdr toString())
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this car equals?(other as This car) && this cdr equals?(other as This cdr)
    }
}
