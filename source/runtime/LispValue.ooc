import structs/ArrayList

import ../Scope

LispValue: abstract class {
    toString: abstract func -> String
    
    equals?: abstract func (other: LispValue) -> Bool 
    
    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        // TODO: We should eventually implement our own error system
        raise(This, "Cannot call")
    }
}

operator == (a, b: LispValue) -> Bool {
    a equals?(b)
}

operator != (a, b: LispValue) -> Bool {
    !a equals?(b)
}
