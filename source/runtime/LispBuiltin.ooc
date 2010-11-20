import structs/ArrayList
import ../Scope
import LispValue

LispBuiltin: class extends LispValue {
    //value: Func (ArrayList<LispValue>, Scope<LispValue>) -> LispValue
    value: Func
    name: String

    // All of these kill rock
    //init: func (=name, =value)
    //init: func (=name, .value) {
    //init: func (=name, value: Func (ArrayList<LispValue>, Scope<LispValue>) -> LispValue) {
    //init: func (=name, value: Func) {
    init: func (value: Func, name: String) {
        this value = value
    }

    toString: func -> String {
        "%s" format(name)
    }

    equals?: func (other: LispValue) -> Bool {
        other class == This && this value == other as This value
    }

    call: func (arguments: ArrayList<LispValue>, scope: Scope<LispValue>) -> LispValue {
        value(arguments, scope)
    }
}
