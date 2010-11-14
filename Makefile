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

ROCK:=rock


all:
	$(ROCK) -v source/lixp.ooc

clean:
	rm -f lixp

libsclean: clean
	rm -rf .libs/

distclean: clean
	rm -f $(wildcard *~)
	rm -f $(wildcard source/*~)
	rm -f $(wildcard source/*/*~)

todo:
	@grep -rInso 'TODO: \([^*]\+\)' source/

sloc:
	@sloccount source/ | grep "(SLOC)"

git-stats:
	@echo "$(shell git log --oneline | wc -l) commits"
	@echo "$(shell git log -p | grep -v '+++' | grep -c '^+') additions"
	@echo "$(shell git log -p | grep -v -e '---' | grep -c '^-') deletions"

.PHONY: clean distclean todo sloc git-stats
