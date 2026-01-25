CC       := cc
CPPFLAGS :=
CFLAGS   := -std=c99 -Wall -Werror -Wextra
LDFLAGS  :=
LDLIBS   := -lcurses

TARGET := bin/tetris
SRC    := $(wildcard *.c)
OBJS   := $(SRC:.c=.o)
DFS    := $(SRC:.c=.d)

DEV := 0

ifeq ($(DEV), 1)
	CFLAGS += -O0 -g -MMD
else
	CFLAGS += -O3
endif

$(TARGET): $(OBJS)
	mkdir -p $$(dirname $(TARGET))
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

-include $(DFS)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(DFS) $(TARGET)
