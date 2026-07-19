CC       := cc
CPPFLAGS :=
CFLAGS   := -std=c99 -Wall -Werror -Wextra
LDFLAGS  :=
LDLIBS   :=

TARGET := bin/main
SRC    := $(wildcard *.c)
OBJS   := $(SRC:.c=.o)
DS    := $(SRC:.c=.d)

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

-include $(DS)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(DS) $(TARGET)
