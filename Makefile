CC = gcc
CFLAGS = -Wall -Wextra -D_Default_SOURCE -Ilib/include -Iinclude

SRCDIR = src
LIBDIR = lib
OBJDIR = obj
BINDIR = bin

SRCS := $(wildcard $(SRCDIR)/**/*.c)
SRCS += $(wildcard $(SRCDIR)/*.c)
SRCS += $(wildcard $(LIBDIR)/**/*.c)
SRCS += $(wildcard $(LIBDIR)/*.c)

OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

TARGET = $(BINDIR)/graphing_calculator


all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lglfw


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean