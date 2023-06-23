CC = gcc
CFLAGS = -I$@/include
SRC = src
BINDIR = bin
DIRECTORIES = $(patsubst %/, %, $(patsubst $(OBJ)/,, $(patsubst $(BINDIR)/,, $(wildcard */))))
SRCS = $(wildcard $@/$(SRC)/*.c)
BIN = $(BINDIR)/$@.bin

all: $(DIRECTORIES)

.PHONY: release debug
release:
	$(eval CFLAGS += -O2)
debug:
	$(eval CFLAGS += -g)

.SECONDEXPANSION:
$(DIRECTORIES): $$@/$(SRC)/*.c
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

.PHONY: clean
clean:
	rm $(BINDIR)/*