SRCS = src/main.c src/parser.c src/display.c src/slides.o
OBJS = $(SRCS:.c=.o)

PREFIX ?= /usr/local
INSTALL ?= install
INSTALL_PROGRAM ?= $(INSTALL)

ifneq "$(DESTDIR)" ""
  PREFIX = $(DESTDIR)
endif

# Lets users pass custom options in
# without overwriting ours
CFLAGS ?=
CFLAGS += -Wall -Wextra

CPPFLAGS ?=
CPPFLAGS += -DPROGNAME=\"dss\"

LDFLAGS ?=
LDFLAGS += -lncurses

ifneq "$(DEBUG)" ""
  CFLAGS += -O0 -g -ggdb
  CPPFLAGS += -DDSS_DEBUG=1
else
  CFLAGS += -O2
endif

# check for getline
ifneq (, $(shell ldconfig -p | grep 'libc\b' \
  | awk '{print $$NF}' \
  | xargs nm -g 2> /dev/null \
  | grep getline -m 1 -q \
  && echo HAS_GETLINE))
  CPPFLAGS += -DHAVE_GETLINE=1
else
  CPPFLAGS += -DHAVE_GETLINE=0
endif

# check for getpagesize
ifneq (, $(shell ldconfig -p | grep 'libc\b' \
  | awk '{print $$NF}' \
  | xargs -nm -g 2> /dev/null \
  | grep getpagesize -m 1 -q \
  && echo HAS_GETPAGESIZE))
  CPPFLAGS += -DHAVE_GETPAGESIZE=1
else
  CPPFLAGS += -DHAVE_GETPAGESIZE=0
endif

all: dss

# XXX only needed because of -I ./include:
# generally speaking, Make knows
# how to build C programs.
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -I ./include $< -o $@

dss: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o dss $(LDFLAGS)

tidy:
	$(RM) *.o
	$(RM) src/*.o

vars:
	echo $(CPPFLAGS)

clean: tidy
	$(RM) dss

install: dss
	## TODO Determine what use the installation
	## categories have.
	#$(PRE_INSTALL)
	$(INSTALL_PROGRAM) -d $(PREFIX)/bin

	#$(NORMAL_INSTALL)
	$(INSTALL_PROGRAM) -m755 dss $(PREFIX)/bin

uninstall:
	#$(NORMAL_UNINSTALL)
	$(RM) $(PREFIX)/bin/dss
