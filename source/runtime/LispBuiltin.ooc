import structs/ArrayList
import ../Scope
import [LispValue, LispList, LispSymbol]

LispBuiltins: enum {
    quote,
    eval,
    car,
    cdr,
    cons
}

LispBuiltin: class extends LispValue {
    value: LispBuiltins
    str: String

    init: func (=value, =str)

    bindAll: static func (scope: Scope<LispValue>) {
        scope["nil"] = LispList new()
        scope["t"] = LispSymbol new("t")
        
        scope["quote"] = This new(LispBuiltins quote, "quote")
        scope["eval"] = This new(LispBuiltins eval, "eval")
        scope["car"] = This new(LispBuiltins car, "car")
        scope["cdr"] = This new(LispBuiltins cdr, "cdr")
        scope["cons"] = This new(LispBuiltins cons, "cons")
    }

    toString: func -> String {
        "%s" format(str)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        match (value) {
            case LispBuiltins quote => quote(arguments, scope)
            case LispBuiltins eval => eval(arguments, scope)
            case LispBuiltins car => car(arguments, scope)
            case LispBuiltins cdr => cdr(arguments, scope)
            case LispBuiltins cons => cons(arguments, scope)
        }
    }

    quote: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        arguments[0]
    }

    eval: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        arguments[0] evaluate(scope) evaluate(scope)
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

    cons: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            raise(This, "Wrong number of arguments") // TODO: Proper error
        }
        car := arguments[0] evaluate(scope)
        cdr := arguments[1] evaluate(scope)

        // First case, (cons x nil) -> (x)
        if (cdr class == LispProperList && cdr as LispProperList items size == 0) {
            list := ArrayList<LispValue> new()
            list add(car)
            return LispList new(list)
        }
        // Second case, (cons foo (proper list)) -> (foo proper list)
        if (cdr class == LispProperList) {
            list := ArrayList<LispValue> new()
            list add(car)
            list addAll(cdr as LispProperList items)
            return LispList new(list)
        }
        // Third case, (cons foo bar) -> (foo . bar)
        return LispList new(car, cdr)
    }
}
