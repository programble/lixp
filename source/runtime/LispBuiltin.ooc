import structs/ArrayList
import ../Scope
import [LispValue, LispList, LispSymbol, LispNumber]

LispBuiltins: enum {
    quote,
    eval,
    car,
    cdr,
    cons,
    cond,

    eq,
    gt
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
        scope["cond"] = This new(LispBuiltins cond, "cond")

        scope["="] = This new(LispBuiltins eq, "=")
        scope[">"] = This new(LispBuiltins gt, ">")
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
            case LispBuiltins cond => cond(arguments, scope)

            case LispBuiltins eq => eq(arguments, scope)
            case LispBuiltins gt => gt(arguments, scope)
        }
    }

    // Builtins start here

    quote: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        arguments[0]
    }

    eval: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            raise(This, "Wrong number of arguments") // TODO: Specific error type
        }
        arguments[0] evaluate(scope) evaluate(scope)
    }

    car: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
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

    cdr: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
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

    cons: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
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

    cond: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        for (pair in arguments) {
            if (pair class != LispProperList) {
                raise(This, "Unexpected type and i need an error system") // TODO: Error
            }
            // First case, (foo) -> foo
            if (pair as LispProperList items size == 1) {
                return pair as LispProperList items[0] evaluate(scope)
            }
            // Second case, (foo bar) -> bar if foo != nil
            if (pair as LispProperList items size == 2) {
                if (!pair as LispProperList items[0] evaluate(scope) equals?(LispList new())) {
                    return pair as LispProperList items[1] evaluate(scope)
                }
            }
            // Third case, too many items
            if (pair as LispProperList items size > 2) {
                raise(This, "TODO: SOme error") // TODO: Error
            }
        }
        return LispList new()
    }

    eq: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            raise(This, "Wrong number of arguments") // TODO: Proper error
        }
        if (arguments[0] evaluate(scope) equals?(arguments[1] evaluate(scope))) {
            return LispSymbol new("t")
        } else {
            return LispList new()
        }
    }

    gt: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            raise(This, "Wrong number of arguments") // TODO: Proper error
        }
        x := arguments[0] evaluate(scope)
        y := arguments[1] evaluate(scope)
        if (x class != LispNumber || y class != LispNumber) {
            raise(This, "OMG WRONG TYPES") // TODO: Error
        }
        // TODO: Handle floats, if we ever get them working
        if (x as LispNumber ivalue > y as LispNumber ivalue) {
            return LispSymbol new("t")
        } else {
            return LispList new()
        }
    }
}
