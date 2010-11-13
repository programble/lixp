import Scope

main: func {
    test := Scope<String> new()
    test["foo"] = "bar"
    test["bar"] = "baz"
    test2 := Scope<String> new(test)
    test2["bar"] = "moo"
    
    test2["bar"] println()
    test2["foo"] println()
    test["bar"] println()
}
