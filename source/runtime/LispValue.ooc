import structs/ArrayList

import ../Scope
import exceptions

LispValue: abstract class {

    toString: abstract func -> String
    
    equals?: abstract func (other: LispValue) -> Bool 
    
    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        NotCallableException new(this class) throw()
    }
}

operator == (a, b: LispValue) -> Bool {
    a equals?(b)
}

operator != (a, b: LispValue) -> Bool {
    !a equals?(b)
}
