# Compiler settings
CC      ?= gcc

CFLAGS  += -fPIC -Wall -Wextra -ansi -Wno-variadic-macros
ifndef DEBUG
ADDITIONAL_CFLAGS  ?= -O2
else
ADDITIONAL_CFLAGS  ?= -g
endif

CFLAGS += ${ADDITIONAL_CFLAGS} -Isrc

INSTALL_PREFIX ?= /usr/local
INSTALL ?= cp  -a

# Ar settings to build the library
AR	    ?= ar
ARFLAGS = rcv

SHLD = ${CC} ${CFLAGS}

# Set RANLIB to ranlib on systems that require it (Sun OS < 4, Mac OSX)
# RANLIB  = ranlib
RANLIB = true

RM      ?= rm -f


# Implicit rules
SUFFIXES = .o .c .h .a 

COMPILE.c	?= $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

ifndef V
QUIET_AR	= @echo "AR	$@";
QUIET_CC	= @echo "CC	$@";
QUIET_LINK	= @echo "LINK	$@";
QUIET_RANLIB	= @echo "RANLIB	$@";
endif

.c.o:
	$(QUIET_CC)$(COMPILE.c) $(OUTPUT_OPTION) $<


SRCS = src/vlog.c \

OBJS = $(SRCS:.c=.o)


default: libvutils.a

libvutils.a: $(OBJS)
	$(QUIET_AR)$(AR) $(ARFLAGS) $@ $^
	$(QUIET_RANLIB)$(RANLIB) $@

clean:
	$(RM) $(OBJS)
	$(RM) libvutils.a

install: libvutils.a
	mkdir -p $(INSTALL_PREFIX)/include
	mkdir -p $(INSTALL_PREFIX)/lib
	$(INSTALL) src/vlist.h $(INSTALL_PREFIX)/include
	$(INSTALL) src/vlog.h  $(INSTALL_PREFIX)/include
	$(INSTALL) libvutils.a $(INSTALL_PREFIX)/lib

uninstall:
	rm -f $(INSTALL_PREFIX)/include/vlist.h
	rm -f $(INSTALL_PREFIX)/include/vlog.h
	rm -f $(INSTALL_PREFIX)/lib/libvutils.a

    

