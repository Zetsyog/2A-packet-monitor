CC ?= gcc
CFLAGS  ?= -Wall -Wextra
LDLIBS  ?= -lpcap

INCLUDE_PATH = ./include
TARGET   = monitor
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
SOURCES  := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCLUDE_PATH)/**/*.h) $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDLIBS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -g -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*
	rm -f $(BINDIR)/$(TARGET)