Ideas
=====

and tacos.

Multiple lambda definitions with haskell-like matching
------------------------------------------------------

    (defn factorial
      (0) 1
      (n) (* n (factorial (- n 1))))

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
