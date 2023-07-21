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
$(DIRECTORIES): create_dirs
$(DIRECTORIES): $$@/$(SRC)/*.c
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)

.PHONY: clean new create_dirs
clean:
	rm -r $(BINDIR)

new: clean
new: all

create_dirs:
	@mkdir -p $(BINDIR)