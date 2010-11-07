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

# Find OS and Arch (borrowed from rock)
OS:=$(shell uname -s)
MACHINE:=$(shell uname -m)
ifeq ($(OS), Linux)
    ARCH=linux
else ifeq ($(OS), Darwin)
    ARCH=osx
else ifeq ($(OS), CYGWIN_NT-5.1)
    ARCH=win
else ifeq ($(OS), MINGW32_NT-5.1)
    ARCH=win
else
    $(shell echo "OS ${OS} doesn't have pre-built Boehm GC packages. Please compile and install your own and recompile with GC_PATH=-lgc")
endif
ifneq ($(ARCH), osx)
  ifeq ($(MACHINE), x86_64)
    ARCH:=$(ARCH)64
  else
    ARCH:=$(ARCH)32
  endif
endif

GC_PATH=libs/$(ARCH)/libgc.a

INCLUDES=-Iinclude/ -Ilibs/headers/
WARNINGS=-Wall -Wextra -Wno-unused-parameter
DEFINES=-DREADLINE
FLAGS=-std=c99
CFLAGS=$(FLAGS) $(DEFINES) $(WARNINGS) $(INCLUDES)

DFLAGS=-ggdb -O0
DDEFINES=-DDEBUG

LIBS=-lreadline -lm -lpthread  $(GC_PATH)

SOURCES:=$(wildcard src/*.c)
OBJECTS:=$(SOURCES:%.c=%.o)

OUTPUT=lixp

INSTALL_PREFIX=/usr/local

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): include/version.h $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

include/version.h: $(SOURCES) Makefile
	@echo "#define VERSION \"$(shell git describe --tags HEAD | cut -c2-)\"" > $@
	@echo "#define COMPILER \"$(CC) \" __VERSION__" >> $@

install: $(OUTPUT)
	install -g root -o root $(OUTPUT) $(INSTALL_PREFIX)/bin

uninstall: remove
remove:
	rm -f $(INSTALL_PREFIX)/bin/$(OUTPUT)

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

size:
	@du -h $(OUTPUT)
	@du -sh .

git-stats:
	@echo "$(shell git log --oneline | wc -l) commits"
	@echo "$(shell git log -p | grep -v '+++' | grep -c '^+') additions"
	@echo "$(shell git log -p | grep -v -e '---' | grep -c '^-') deletions"

.PHONY: debug no-readline clean distclean check-syntax todo sloc git-stats
