# Define compiler and flags
CC=gcc
CFLAGS=-Wall -fPIC
LDFLAGS=-ldl
RESOLV_LIB=-lresolv

# Target executable and library
EXEC=test
LIB=netblock.so

# Source files
EXEC_SRC=test.c
LIB_SRC=netblock.c

# Default target
all: $(EXEC) $(LIB)

# Link the network_test executable
$(EXEC): $(EXEC_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(RESOLV_LIB)

# Compile the block_connect shared library
$(LIB): $(LIB_SRC)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

# Clean up
clean:
	rm -f $(EXEC) $(LIB)

# Phony targets
.PHONY: all clean
