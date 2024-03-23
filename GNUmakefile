# Selects the production "centaur" build.  Override on the command line
# to build for development without a DGT board, e.g., "make centaur="
centaur  = centaur
build    = $(or $(centaur),stub)

src_dirs = src src/chess src/fonts src/thc src/utility src/$(build)
sources  = $(foreach d,$(src_dirs),$(wildcard $d/*.cpp))

obj_dirs = $(patsubst src%,obj%,$(src_dirs))
objects  = $(foreach s,$(sources),$(patsubst src/%.cpp,obj/%.o,$s))

CPPFLAGS = -D_GNU_SOURCE -DPCRE2_CODE_UNIT_WIDTH=8
CFLAGS   = -Wall -Wextra -Wpedantic -pthread
CFLAGS  += -Werror -g
#CFLAGS  += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable
LDLIBS   = $(if $(centaur),-lpigpio,)
LDLIBS  += -ljansson -lmicrohttpd -lpcre2-8 -lpng -lsqlite3 -lrt -lm

$(shell mkdir -p bin $(obj_dirs))
all: bin/rcm

clean:
	$(RM) -R bin/rcm obj/*

bin/rcm: $(objects)
	$(LINK.cc) $^ $(LDLIBS) -o $@

obj/%.o: src/%.cpp
	$(COMPILE.cc) $< -o $@
