CC       := cc
CPPFLAGS :=
CFLAGS   := -std=c99 -Wall -Wextra
LDFLAGS  := 
LDLIBS   :=

BIN := main
SRC := $(wildcard *.c) 
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d) 

DEV := 0

ifeq ($(DEV), 1)
	CFLAGS += -O0 -g -MMD
else
	CFLAGS += -O3
endif

$(BIN): $(OBJ)

-include $(DEP)

.PHONY: run
run: $(BIN)
	./$(BIN)

.PHONY: clean
clean:
	rm -rf $(DEP) $(OBJ) $(BIN)
