Version: class {
    major: static const Int = 0
    minor: static const Int = 0
    patch: static const Int = 3

    toString: static func -> String {
        "%d.%d.%d" format(major, minor, patch)
    }
}
