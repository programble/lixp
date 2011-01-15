ArityException: class extends Exception {
    init: func (=origin, name: String, expected: Int, got: Int) {
        init(name, expected, got)
    }

    init: func ~noOrigin (name: String, expected: Int, got: Int) {
        message = "Wrong number of arguments to %s: expected %d, got %d" format(name, expected, got)
    }
}

TypeException: class extends Exception {
    init: func (=origin, name: String, expected: Class, got: Class) {
        init(name, expected, got)
    }

    init: func ~noOrigin (name: String, expected: Class, got: Class) {
        message = "%s is of wrong type: expected %s, got %s" format(name, expected name, got name)
    }
}
