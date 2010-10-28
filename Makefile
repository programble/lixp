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

GCPREFIX=/usr

INCLUDES=-Iinclude/ -I$(GCPREFIX)/include
WARNINGS=-Wall -Wextra -Wno-unused-parameter
DEFINES=-D_GNU_SOURCE -DREADLINE
FLAGS=-std=c99
CFLAGS=$(FLAGS) $(DEFINES) $(WARNINGS) $(INCLUDES)

DFLAGS=-ggdb -O0
DDEFINES=-DDEBUG

LIBPATHS=-L$(GCPREFIX)/lib
LIBS=-lreadline -lgc
LDFLAGS=$(LIBPATHS) $(LIBS)

SOURCES:=$(wildcard src/*.c)
OBJECTS:=$(SOURCES:%.c=%.o)

OUTPUT=lixp

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): include/version.h $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

include/version.h: $(SOURCES) Makefile
	@if [ "$(shell git tag --contains $(shell git rev-parse HEAD))" ]; then \
	echo "#define VERSION \"$(shell git tag --contains $(shell git rev-parse HEAD) | cut -c2-)\"" > $@; \
	else \
	echo "#define VERSION \"$(shell git tag | cut -c2-).r$(shell git rev-parse HEAD | cut -c-8)\"" > $@; \
	fi
	@echo "#define COMPILER \"$(CC) \" __VERSION__" >> $@

no-readline:
	@$(MAKE) $(MFLAGS) DEFINES="$(DEFINES:-DREADLINE=)" LIBS="$(LIBS:-lreadline=)"

debug:
	@$(MAKE) $(MFLAGS) FLAGS="$(FLAGS) $(DFLAGS)" DEFINES="$(DEFINES) $(DDEFINES)"

clean:
	rm -f include/version.h
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

git-stats:
	@echo "$(shell git log --oneline | wc -l) commits"
	@echo "$(shell git log -p | grep -v '+++' | grep -c '^+') additions"
	@echo "$(shell git log -p | grep -v -e '---' | grep -c '^-') deletions"

.PHONY: debug no-readline clean distclean check-syntax todo sloc git-stats
