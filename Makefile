ROCK:=rock

ROCKFLAGS=-g +-rdynamic -v

SOURCES:=$(wildcard source/*) $(wildcard source/*/*)

lixp: $(SOURCES) Makefile
	$(ROCK) $(ROCKFLAGS) source/lixp.ooc

clean:
	rm -f lixp
	rm -rf rock_tmp

libsclean: clean
	rm -rf .libs/

distclean: libsclean
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
