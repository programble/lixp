import ../Scope

LispValue: abstract class {
    toString: abstract func -> String
    equals?: abstract func (other: This) -> Bool
    evaluate: abstract func (scope: Scope<LispValue>) -> LispValue
}

operator == (a: LispValue, b: LispValue) -> Bool {
    a equals?(b)
}

operator != (a: LispValue, b: LispValue) -> Bool {
    !a equals?(b)
}
