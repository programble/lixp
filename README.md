Lixp
====

Some kind of Lisp in C.

Dependencies
------------

 * [Boehm GC](http://www.hpl.hp.com/personal/Hans_Boehm/gc/)
 * [GNU Readline](http://tiswww.case.edu/php/chet/readline/rltop.html) (Optional: `make no-readline`)

Building
--------

### Building Boehm GC

 1. Download http://www.hpl.hp.com/personal/Hans_Boehm/gc/gc_source/gc.tar.gz
 2. Decompress archive
 3. `cd` into `gc-version` directory
 4. Run `./configure --prefix=/usr --disable-threads`
 5. Run `make`
 6. As root, run `make install`

### Building Lixp with GNU Readline

    make

### Building Lixp without GNU Readline

    make no-readline

### Make variables

 * `CC`: C compiler to use when building Lixp
 * `GCPREFIX`: Prefix to search for libgc in (the path supplied to `./confure --prefix=` when building the GC)
 * `OUTPUT`: Output executable file

For example, if you want to use GCC, installed the GC to `/usr/local`, and want the executable named `lixp-1`:
    make CC=gcc GCPREFIX=/usr/local OUTPUT=lixp-1
