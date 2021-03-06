ArityException: class extends Exception {
    init: func ~exact (name: String, expected: Int, got: Int) {
        message = "Wrong number of arguments to %s: expected %d, got %d" format(name, expected, got)
    }

    init: func ~range (name: String, min, max, got: Int) {
        message = "Wrong number of arguments to %s: expected between %d and %d, got %d" format(name, min, max, got)
    }
}

ArgumentTypeException: class extends Exception {
    init: func (name: String, expected: Class, got: Class) {
        message = "Argument to %s is of wrong type: expected %s, got %s" format(name, expected name, got name)
    }
}

NotCallableException: class extends Exception {
    init: func (type: Class) {
        message = "Type %s is not callable" format(type name)
    }
}

ZeroDivisionException: class extends Exception {
    init: func {
        message = "Cannot divide by zero"
    }
}

AlreadyDefinedException: class extends Exception {
    init: func (name: String) {
        message = "Symbol %s is already defined" format(name)
    }
}

UndefinedException: class extends Exception {
    init: func (name: String) {
        message = "Symbol %s is not defined" format(name)
    }
}
