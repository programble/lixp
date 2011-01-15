import structs/ArrayList

import LispValue
import ../Scope

LispList: abstract class extends LispValue {
    new: static func ~proper (items: ArrayList<LispValue>) -> LispList {
        LispProperList new(items)
    }

    new: static func ~improper (car, cdr: LispValue) -> LispList {
        LispImproperList new(car, cdr)
    }

    new: static func ~nil -> LispList {
        LispProperList new(ArrayList<LispValue> new())
    }
}

LispProperList: class extends LispList {
    items: ArrayList<LispValue>
    
    init: func (=items)

    toString: func -> String {
        // Show an empty list as 'nil'
        if (items size == 0) {
            return "nil"
        }
        stritems := items map(|x| x toString())
        "(%s)" format(stritems join(" "))
    }

    equals?: func (other: LispValue) -> Bool {
        if (other class != This) {
            return false
        }
        if (other as This items size != this items size) {
            return false
        }

        acc := true
        for (i in 0..items size) {
            if (!this items get(i) equals?(other as This items get(i))) {
                acc = false
                break
            }
        }
        return acc
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        // nil evaluates to nil
        if (items size == 0) {
            return this
        }
        callee := items[0] evaluate(scope)
        args: ArrayList<LispValue>
        if (items size == 1) {
            args = ArrayList<LispValue> new()
        } else {
            args = items[1..-1]
        }
        callee call(args, scope)
    }

    nil?: func -> Bool {
        items size == 0
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
