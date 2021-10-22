# program executable name
TARGET = sync

XENO_DESTDIR:=
XENO_CONFIG:=$(XENO_DESTDIR)/usr/bin/xeno-config
XENO_CFLAGS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --cflags)
XENO_LIBS:=$(shell DESTDIR=$(XENO_DESTDIR) $(XENO_CONFIG) --skin native --ldflags)

# compiler flags
CFLAGS = -g -Wall $(XENO_CFLAGS)

# linker flags
LDFLAGS = -g $(XENO_LIBS) 

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	gcc -o $@ $^ $(LDFLAGS)

# compile
%.o : %.c
	gcc -c -o $@ $< $(CFLAGS) 

# cleaning
clean:
	rm -f $(TARGET) $(OBJECTS)
