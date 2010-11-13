import LispValue
import ../Scope

LispNumber: class extends LispValue {
    value: Int { get set }
    
    init: func (=value) 
    
    toString: func -> String {
        value toString()
    }

    equals?: func (other: LispValue) {
        // TODO: Imeplement
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}
        
