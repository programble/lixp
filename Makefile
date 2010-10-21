# Copyright 2010 Curtis McEnroe <programble@gmail.com>
#
# This file is part of Lixp.
#
# Lixp is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Lixp is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Lixp.  If not, see <http://www.gnu.org/licenses/>.

CC=clang

INCLUDES=-Iinclude/
WARNINGS=-Wall -Wextra
DEFINES=-D_GNU_SOURCE
FLAGS=-std=c99
CFLAGS=$(FLAGS) $(DEFINES) $(WARNINGS) $(INCLUDES)

DFLAGS=-ggdb -O0
DDEFINES=-DDEBUG

SOURCES:=$(wildcard src/*.c)
OBJECTS:=$(SOURCES:%.c=%.o)

OUTPUT=lixp

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

debug:
	@$(MAKE) $(MFLAGS) FLAGS="$(FLAGS) $(DFLAGS)" DEFINES="$(DEFINES) $(DDEFINES)"

clean:
	rm -f $(OBJECTS)
	rm -f $(OUTPUT)

distclean: clean
	rm -f $(wildcard *~)
	rm -f $(wildcard src/*~)
	rm -f $(wildcard include/*~)

check-syntax:
	$(CC) $(CFLAGS) -fsyntax-only $(CHK_SOURCES)

todo:
	@grep -rInso 'TODO: \([^*]\+\)' src/ include/

sloc:
	@sloccount ./ | grep "(SLOC)"

.PHONY: debug clean distclean check-syntax todo sloc
