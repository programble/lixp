Ideas
=====

and tacos.

Multiple lambda definitions with haskell-like matching
------------------------------------------------------

    (defn factorial
      (0) 1
      (n) (* n (factorial (- n 1))))

    (defn lottery
      ; Chosen by fair dice roll
      (4) "A WINNER IS YOU!"
      (_) "You lose.")

Or if lambda bodies are multiple expressions,

    (defn factorial
      ((0) 1)
      ((n) (* n (factorial (- n 1)))))

Currying would be cool too
--------------------------

    (defn add (x y)
      (+ x y))
    (def add2 (add 2))
    (add2 5) ;-> 7

Lazy lists would be nice wouldn't they
--------------------------------------

No I have no idea how I would implement them.

Errors
------

If a function fails, it evaluates to an error. Perhaps something like

    (!error-type "error details" function-of-origin line-number "filename")

Then if another error arises from that error, the new error should be prepended either like

    ((error-1) (error-2))

or some other way. This will provide a sort of traceback.

Or something completely different!

I want to allow reader macros
-----------------------------

Yes.

Namespacing
-----------

Store all loaded namespaces in a global treemap.
Each namespace has a list of 'use' namespaces, like parent scopes. A
symbol without a namespace specifier would first be looked for in the
current namespace, then in its 'use' namespaces.

`(use foo)` would add the foo namespace as a 'use' to the current
namespace.
`(require foo)` would simply load the namespace into the global
namespace treemap.

### Arbitrary namespacing?

Not really leaning towards this as it would require searching files on
some sort of classpath to find the namespace.

### File-based namespacing

A lot easier, since `(use foo)` could easily be found by looking in
the file `foo.lx` or whatever extension I end up going with. Then the
problem arises of how to nest them, for libraries and things. Would
`(use foo.bar)` look in `/foo/bar.lx`? And then how is that namespace
stored? Flat, as simply "foo.bar", or nested as well? I am thinking
that flat would be much much easier.

### Symbols

A regular symbol, for example `foo`, would be looked up in the current
lexical scope in the current namespace. A namespace specified symbol,
for example `foo.bar/baz` (yes, going the way of Clojure), would be
looked up in the `foo.bar` namespace.

### Use stuff

Every namespace automatically uses the `main` namespace.

#### Examples

    (require foo)
    (use bar)

    (foo/baz 1)
    (bar/baz 2)
    (baz 2) ; This is equivalent to (bar/baz 2) since the namespace is 'used'

### About that global treemap

Global variables are bad. But I can't think of any other way to do
it. Passing around a namespace parameter is disgusting. I suppose
we'd need to have one variable for the namespace treemap, and one for
the current namespace.

### Blending scopes and namespaces

Namespaces need to be compatible with scopes. The 'global scope' would
be replaced by a namespace, so the other scopes need to be able to
call on their namespace to look up a symbol. Namespaces will probably
just be like scopes with more than one parent, the 'used'
namespaces. The only real difference would be that where a scope calls
on its parent to find a symbol, a namespace would call on each of its
'used' namespaces to find a symbol. Only once a symbol look up reaches
the `main` namespace and is not found will it officially be undefined.
