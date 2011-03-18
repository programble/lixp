import structs/HashMap

Scope: class <V> {
    parent: This<V>
    map: HashMap<String, V>

    init: func ~withParent (=parent) {
        map = HashMap<String, V> new()
    }

    init: func {
        init(null)
    }

    get: func (key: String) -> V {
        if (map contains?(key)) {
            map get(key)
        } else if (parent == null) {
            null
        } else {
            parent get(key)
        }
    }

    contains?: func (key: String) -> Bool {
        here := map contains?(key)
        if (!here && parent) {
            parent contains?(key)
        } else {
            here
        }
    }

    put: func (key: String, value: V) -> Bool {
        map put(key, value)
    }

    remove: func (key: String) -> Bool {
        map remove(key)
    }
}

operator [] <V> (scope: Scope<V>, key: String) -> V {
    scope get(key)
}

operator []= <V> (scope: Scope<V>, key: String, value: V) {
    scope put(key, value)
}
