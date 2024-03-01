# Selects the production "centaur" build.  Override on the command line
# to build for development without a DGT board, e.g., "make centaur="
centaur  = centaur
build    = $(or $(centaur),stub)

src_dirs = src src/chess src/fonts src/$(build)
sources  = $(foreach d,$(src_dirs),$(wildcard $d/*.c))

obj_dirs = $(patsubst src%,obj%,$(src_dirs))
objects  = $(foreach s,$(sources),$(patsubst src/%.c,obj/%.o,$s))

CPPFLAGS = -D_GNU_SOURCE -DPCRE2_CODE_UNIT_WIDTH=8
CFLAGS   = -Wall -Wextra -Wpedantic -pthread
CFLAGS  += -Werror -g
LDLIBS   = $(if $(centaur),-lpigpio,) -lmicrohttpd -lpcre2-8 -lpng -lrt -lm

$(shell mkdir -p bin $(obj_dirs))
all: bin/rcm

check: bin/check
	$<

clean:
	$(RM) -R bin/rcm bin/check obj/

bin/rcm: $(objects)
	$(LINK.c) $^ $(LDLIBS) -o $@

test_sources = \
	src/chess/chess.c \
	src/chess/chess_board.c \
	src/chess/chess_fen.c \
	src/chess/chess_game.c \
	src/chess/chess_position.c \
	src/list.c \
	src/mem.c
bin/check: $(wildcard t/*.c) $(test_sources)
	$(LINK.c) $^ -o $@ -lcheck -lsubunit -lm

obj/%.o: src/%.c
	$(COMPILE.c) $< -o $@
