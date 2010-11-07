Lixp
====

Some kind of Lisp in C.

Dependencies
------------

 * [GNU Readline](http://tiswww.case.edu/php/chet/readline/rltop.html) (Optional: `make no-readline`)

System Requirements
-------------------

Lixp has been tested on:

 * Linux 32-bit
 * Linux 64-bit

Building
--------

    make

### Building Lixp without GNU Readline

    make no-readline

### Make variables

 * `CC`: C compiler to use when building Lixp
 * `GC_PATH`: Replace with `-lgc` to dynamically link your own build of Boehm GC
 * `OUTPUT`: Output executable file

Installing
----------

Lixp cannot yet be installed.

License
-------

    Lixp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version. 
    
    Lixp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License, located in the file 'COPYING' for more details.
