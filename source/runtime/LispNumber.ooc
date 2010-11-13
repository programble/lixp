import LispValue
import ../Scope

LispNumber: class extends LispValue {
    value: Int { get set }
    
    init: func (=value) 
    
    toString: func -> String {
        value toString()
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && value == other as This value
    }

    evaluate: func (scope: Scope<LispValue>) -> LispValue {
        this
    }
}
        
