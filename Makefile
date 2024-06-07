CC = gcc
CFLAGS = -Wall -Wextra -D_Default_SOURCE -Ilib/include -Iinclude

SRCDIR = src
LIBDIR = lib
OBJDIR = obj
BINDIR = bin

SRCS := $(wildcard $(SRCDIR)/**/*.c) $(wildcard $(SRCDIR)/*.c) $(wildcard $(LIBDIR)/**/*.c) $(wildcard $(LIBDIR)/*.c)

OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TARGET = $(BINDIR)/graphing_calculator

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lglfw

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean