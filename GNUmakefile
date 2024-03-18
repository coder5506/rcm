# Selects the production "centaur" build.  Override on the command line
# to build for development without a DGT board, e.g., "make centaur="
centaur  = centaur
build    = $(or $(centaur),stub)

src_dirs = src src/chess src/fonts src/utility src/$(build)
sources  = $(foreach d,$(src_dirs),$(wildcard $d/*.c))

obj_dirs = $(patsubst src%,obj%,$(src_dirs))
objects  = $(foreach s,$(sources),$(patsubst src/%.c,obj/%.o,$s))

CPPFLAGS = -DGC_THREADS -D_GNU_SOURCE -DPCRE2_CODE_UNIT_WIDTH=8
CFLAGS   = -Wall -Wextra -Wpedantic -pthread
CFLAGS  += -Werror -g
#CFLAGS  += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable
LDLIBS   = $(if $(centaur),-lpigpio,)
LDLIBS  += -ljansson -lmicrohttpd -lpcre2-8 -lpng -lsqlite3 -lrt -lm

$(shell mkdir -p bin $(obj_dirs))
all: bin/rcm

check: bin/check
	$<

clean:
	$(RM) -R bin/rcm bin/check obj/

bin/rcm: $(objects)
	$(LINK.cc) $^ $(LDLIBS) -o $@

test_sources = \
	$(wildcard src/chess/*.c) \
	src/utility/buffer.c \
	src/utility/kv.c \
	src/utility/list.c \
	src/utility/model.c
bin/check: $(wildcard t/*.c) $(test_sources)
	$(LINK.cc) $^ -o $@ -lcheck -lsubunit -lgc -lrt -lm

obj/%.o: src/%.c
	$(COMPILE.cc) $< -o $@
