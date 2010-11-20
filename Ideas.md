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

The second example is unlikely, not only because I am lazy, but because
it seems there are unnecessary parentheses. If you want multiple
expressions in a lambda then use a `do`, simple as that.

Currying would be cool too
--------------------------

    (defn add (x y)
      (+ x y))
    (def add2 (add 2))
    (add2 5) ;-> 7

### No, it wouldn't

Currying can easily be defined as a macro then used as `(curry (foo
bar baz))`. Having it built in makes no sense at all.

Lazy lists would be nice wouldn't they
--------------------------------------

A lazy list would need a generator function. The function would need
to take parameters of the current index and the last item in the list,
and would need to return a new item.

### Problem

Generators for things like `map` would need some extra data (the
original list and the function to apply). How would we handle that?
Perhaps if lazy generators were classes, the `map` function could
return a special subclass of `LazyGenerator` that stores those
things. I think that would be the best solution.

Actually, `map` can just return a subclass of `LazyList`, and
`LazyList` would have an abstract function `generate` that the
subclass would implement.

### Hm...

Actually, the generate function should take an index of what to
evaluate up to. See, `LazyList` would be a subclass of `List`, so what
the generate function must do is populate its `items` member until its
length is the index passed to it. Does that make sense?

So, the `take` function for example, when given the index of 5, would
call on the lazy list to generate up to index 5, then return that
value.

But what about incremental things? If 3 are already generated, and now
we want 5, we would need to figure out which ones actually need to be
generated.

And how about telling the actual length of the list. Like, when the
list is actually ended. Hm...

How, how, how?
    
I want to allow reader macros
-----------------------------

This would require exposing the entire reader to Lixp. That would
require the reader to be a bit flexible, which is not likely to just
happen. This will need work.

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

### Oh oh oh

The scope can have a pointer to the namespace it's in! That should
mean only one global variable, for the treemap of namespaces.
