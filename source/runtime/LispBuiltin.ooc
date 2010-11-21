import structs/ArrayList
import ../Scope
import [LispValue, LispList]

LispBuiltins: enum {
    car,
    cdr
}

LispBuiltin: class extends LispValue {
    value: LispBuiltins

    init: func (=value)

    toString: func -> String {
        "%s" format(value)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        match (value) {
            case LispBuiltins car => car(arguments, scope)
            case LispBuiltins cdr => cdr(arguments, scope)
        }
    }

    car: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        list := arguments[0] evaluate(scope)
        if (list class != LispProperList && list class != LispImproperList) {
            raise(This, "Expecting type List, got blah") // TODO: Proper error
        }
        if (list class == LispProperList) {
            list as LispProperList items[0]
        } else {
            list as LispImproperList car
        }
    }

    cdr: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        list := arguments[0] evaluate(scope)
        if (list class != LispProperList && list class != LispImproperList) {
            raise(This, "Expecting type List, got blah") // TODO: Proper error
        }
        if (list class == LispProperList) {
            LispList new(list as LispProperList items[1..-1])
        } else {
            list as LispImproperList cdr
        }
    }
}
