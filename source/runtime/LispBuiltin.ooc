import structs/ArrayList
import ../Scope
import [LispValue, LispList, LispSymbol, LispNumber, LispKeyword, LispCharacter, LispString]
import exceptions

LispBuiltins: enum {
    quote,
    eval,
    car,
    cdr,
    cons,
    cond,

    eq,
    gt,
    lt,

    add,
    sub,
    mul,
    div,

    def,
    undef,
    set,
    unset,

    symbolp,
    keywordp,
    numberp,
    characterp,
    stringp,
    listp
}

LispBuiltin: class extends LispValue {
    t: static const LispValue = LispSymbol new("t")
    nil: static const LispValue = LispList new()
    
    value: LispBuiltins
    name: String

    init: func (=value, =name, scope: Scope<LispValue>) {
        scope[name] = this
    }

    bindAll: static func (scope: Scope<LispValue>) {
        scope["nil"] = nil
        scope["t"] = t
        
        This new(LispBuiltins quote, "quote", scope)
        This new(LispBuiltins eval, "eval", scope)
        This new(LispBuiltins car, "car", scope)
        This new(LispBuiltins cdr, "cdr", scope)
        This new(LispBuiltins cons, "cons", scope)
        This new(LispBuiltins cond, "cond", scope)

        This new(LispBuiltins eq, "=", scope)
        This new(LispBuiltins gt, ">", scope)
        This new(LispBuiltins lt, "<", scope)

        This new(LispBuiltins add, "+", scope)
        This new(LispBuiltins sub, "-", scope)
        This new(LispBuiltins mul, "*", scope)
        This new(LispBuiltins div, "/", scope)

        This new(LispBuiltins def, "def", scope)
        This new(LispBuiltins undef, "undef!", scope)
        This new(LispBuiltins set, "set!", scope)
        This new(LispBuiltins unset, "unset!", scope)

        This new(LispBuiltins symbolp, "symbol?", scope)
        This new(LispBuiltins keywordp, "keyword?", scope)
        This new(LispBuiltins numberp, "number?", scope)
        This new(LispBuiltins characterp, "character?", scope)
        This new(LispBuiltins stringp, "string?", scope)
        This new(LispBuiltins listp, "list?", scope)
    }

    toString: func -> String {
        "%s" format(name)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        f := match (value) {
            case LispBuiltins quote => quote 
            case LispBuiltins eval => eval 
            case LispBuiltins car => car 
            case LispBuiltins cdr => cdr 
            case LispBuiltins cons => cons 
            case LispBuiltins cond => cond 

            case LispBuiltins eq => eq 
            case LispBuiltins gt => gt 
            case LispBuiltins lt => lt 

            case LispBuiltins add => add 
            case LispBuiltins sub => sub 
            case LispBuiltins mul => mul
            case LispBuiltins div => div

            case LispBuiltins def => def
            case LispBuiltins undef => undef
            case LispBuiltins set => set
            case LispBuiltins unset => unset

            case LispBuiltins symbolp => symbolp
            case LispBuiltins keywordp => keywordp
            case LispBuiltins numberp => numberp
            case LispBuiltins characterp => characterp
            case LispBuiltins stringp => stringp
            case LispBuiltins listp => listp
        }
        f(arguments, scope)
    }

    // Builtins start here

    quote: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("quote", 1, arguments size) throw()
        }
        arguments[0]
    }

    eval: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("eval", 1, arguments size) throw()
        }
        arguments[0] evaluate(scope) evaluate(scope)
    }

    car: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("car", 1, arguments size) throw()
        }
        list := arguments[0] evaluate(scope)
        if (list class != LispProperList && list class != LispImproperList) {
            ArgumentTypeException new("car", LispList, list class) throw()
        }
        if (list class == LispProperList) {
            list as LispProperList items[0]
        } else {
            list as LispImproperList car
        }
    }

    cdr: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("cdr", 1, arguments size) throw()
        }
        list := arguments[0] evaluate(scope)
        if (list class != LispProperList && list class != LispImproperList) {
            ArgumentTypeException new("cdr", LispList, list class) throw()
        }
        if (list class == LispProperList) {
            LispList new(list as LispProperList items[1..-1])
        } else {
            list as LispImproperList cdr
        }
    }

    cons: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            ArityException new("cons", 2, arguments size) throw()
        }
        car := arguments[0] evaluate(scope)
        cdr := arguments[1] evaluate(scope)

        // First case, (cons x nil) -> (x)
        if (cdr class == LispProperList && cdr as LispProperList nil?()) {
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
                ArgumentTypeException new("cond", LispProperList, pair class) throw()
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
                ArityException new("cond", 2, pair as LispProperList items size) throw() // TODO: Possibly a better error?
            }
        }
        return nil
    }

    eq: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            ArityException new("=", 2, arguments size) throw()
        }
        if (arguments[0] evaluate(scope) equals?(arguments[1] evaluate(scope))) {
            return t
        } else {
            return nil
        }
    }

    gt: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            ArityException new(">", 2, arguments size) throw()
        }
        x := arguments[0] evaluate(scope)
        y := arguments[1] evaluate(scope)
        if (x class != LispNumber || y class != LispNumber) {
            ArgumentTypeException new(">", LispNumber, (x class != LispNumber) ? (x class) : (y class)) throw()
        }
        // TODO: Handle floats, if we ever get them working
        if (x as LispNumber ivalue > y as LispNumber ivalue) {
            return t
        } else {
            return nil
        }
    }

    lt: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            ArityException new("<", 2, arguments size) throw()
        }
        x := arguments[0] evaluate(scope)
        y := arguments[1] evaluate(scope)
        if (x class != LispNumber || y class != LispNumber) {
            ArgumentTypeException new("<", LispNumber, (x class != LispNumber) ? (x class) : (y class)) throw()
        }
        // TODO: Handle floats, if we ever get them working
        if (x as LispNumber ivalue < y as LispNumber ivalue) {
            return t
        } else {
            return nil
        }
    }

    add: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        acc := 0
        for (i in arguments) {
            i = i evaluate(scope)
            if (i class != LispNumber) {
                ArgumentTypeException new("+", LispNumber, i class) throw()
            }
            // TODO: Handle floats, if they ever work, apparently
            acc += i as LispNumber ivalue
        }
        LispNumber new(acc)
    }

    sub: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size == 0) {
            LispNumber new(0)
        } else if (arguments size == 1) {
            x := arguments[0] evaluate(scope)
            if (x class != LispNumber) {
                ArgumentTypeException new("-", LispNumber, x class) throw()
            }
            LispNumber new(x as LispNumber ivalue * -1)
        } else {
            first := arguments[0] evaluate(scope)
            if (first class != LispNumber) {
                ArgumentTypeException new("-", LispNumber, first class) throw()
            }
            // TODO: Float support
            acc := first as LispNumber ivalue
            for (i in arguments[1..-1]) {
                i = i evaluate(scope)
                if (i class != LispNumber) {
                    ArgumentTypeException new("-", LispNumber, i class) throw()
                }
                acc -= i as LispNumber ivalue
            }
            LispNumber new(acc)
        }
    }

    mul: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        acc := 1
        for (i in arguments) {
            i = i evaluate(scope)
            if (i class != LispNumber) {
                ArgumentTypeException new("*", LispNumber, i class) throw()
            }
            // TODO: Floats
            acc *= i as LispNumber ivalue
        }
        LispNumber new(acc)
    }

    div: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size == 0) {
            LispNumber new(1)
        } else if (arguments size == 1) {
            x := arguments[0] evaluate(scope)
            if (x class != LispNumber) {
                ArgumentTypeException new("/", LispNumber, x class) throw()
            }
            if (x as LispNumber ivalue == 0) {
                ZeroDivisionException new() throw()
            }
            // TODO: This should be a float
            LispNumber new(1 / x as LispNumber ivalue)
        } else {
            first := arguments[0] evaluate(scope)
            if (first class != LispNumber) {
                ArgumentTypeException new("/", LispNumber, first class) throw()
            }
            // TODO: Float support
            acc := first as LispNumber ivalue
            for (i in arguments[1..-1]) {
                i = i evaluate(scope)
                if (i class != LispNumber) {
                    ArgumentTypeException new("/", LispNumber, i class) throw()
                }
                if (i as LispNumber ivalue == 0) {
                    ZeroDivisionException new() throw()
                }
                acc /= i as LispNumber ivalue
            }
            LispNumber new(acc)
        }
    }

    def: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        // Climb up the scope tree to find global scope
        while (scope parent) {
            scope = scope parent
        }
        if (arguments size != 2) {
            ArityException new("def", 2, arguments size) throw()
        }
        if (arguments[0] class != LispSymbol) {
            ArgumentTypeException new("def", LispSymbol, arguments[0] class) throw()
        }
        symbol := arguments[0] as LispSymbol value
        value := arguments[1] evaluate(scope)
        if (scope contains?(symbol)) {
            AlreadyDefinedException new(symbol) throw()
        }
        scope[symbol] = value
        arguments[0]
    }

    undef: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        // Climb up the scope tree to find global scope
        while (scope parent) {
            scope = scope parent
        }
        if (arguments size != 1) {
            ArityException new("undef!", 1, arguments size) throw()
        }
        if (arguments[0] class != LispSymbol) {
            ArgumentTypeException new("undef!", LispSymbol, arguments[0] class) throw()
        }
        symbol := arguments[0] as LispSymbol value
        if (!scope contains?(symbol)) {
            UndefinedException new(symbol) throw()
        }
        value := scope[symbol]
        scope remove(symbol)
        value
    }

    set: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 2) {
            ArityException new("set!", 2, arguments size) throw()
        }
        if (arguments[0] class != LispSymbol) {
            ArgumentTypeException new("set!", LispSymbol, arguments[0] class) throw()
        }
        symbol := arguments[0] as LispSymbol value
        value := arguments[1] evaluate(scope)
        if (!scope contains?(symbol)) {
            UndefinedException new(symbol) throw()
        }
        old := scope[symbol]
        scope[symbol] = value
        old
    }

    unset: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("unset!", 1, arguments size) throw()
        }
        if (arguments[0] class != LispSymbol) {
            ArgumentTypeException new("unset!", LispSymbol, arguments[0] class) throw()
        }
        symbol := arguments[0] as LispSymbol value
        if (!scope contains?(symbol)) {
            UndefinedException new(symbol) throw()
        }
        value := scope[symbol]
        scope remove(symbol)
        value
    }

    symbolp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("symbol?", 1, arguments size) throw()
        }
        return (arguments[0] evaluate(scope) class == LispSymbol) ? t : nil
    }

    keywordp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("keyword?", 1, arguments size) throw()
        }
        return (arguments[0] evaluate(scope) class == LispKeyword) ? t : nil
    }

    numberp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("number?", 1, arguments size) throw()
        }
        return (arguments[0] evaluate(scope) class == LispNumber) ? t : nil
    }

    characterp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("character?", 1, arguments size) throw()
        }
        return (arguments[0] evaluate(scope) class == LispCharacter) ? t : nil
    }

    stringp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("string?", 1, arguments size) throw()
        }
        return (arguments[0] evaluate(scope) class == LispString) ? t : nil
    }

    listp: static func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        if (arguments size != 1) {
            ArityException new("list?", 1, arguments size) throw()
        }
        a := arguments[0] evaluate(scope)
        return (a class == LispProperList || a class == LispImproperList) ? t : nil
    }
}
