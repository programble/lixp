ArityException: class extends Exception {
    init: func (name: String, expected: Int, got: Int) {
        message = "Wrong number of arguments to %s: expected %d, got %d" format(name, expected, got)
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
