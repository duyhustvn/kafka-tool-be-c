#  $@ The name of the target file (the one before the colon)
#  $< The name of the first (or only) prerequisite file (the first one after the colon)
#  $^ The names of all the prerequisite files (space-separated)
#  $* The stem (the bit which matches the `%` wildcard in the rule definition.
# Compiler flags
# CC = gcc -std=gnu17
CC = clang -std=gnu17
CFLAGS = -Wall -g

# make DEBUG=1 to enable DEBUG
ifdef DEBUG
	CFLAGS += -DDEBUG
endif

# Project files
LIBS = -lmemcached -lrdkafka

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = build

# Output executable
TARGET = kafka-tool-be-c

# Find all source files (.c) in the src/ directory and subdirectories
SRC = $(shell find $(SRC_DIR) -name '*.c')

# Create corresponding object files (.o) in the obj/ directory
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Find all header files (.h) in the src/ directory and subdirectories
DEPS = $(shell find $(SRC_DIR) -name '*.h')

# Default target
all: $(BIN_DIR)/$(TARGET)

# Link object files into the final executable
$(BIN_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN_DIR)/$(TARGET) $(LIBS)

# Rule to compile .c files to .o files, with header dependencies
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@) # Create the directory for the object file if it doesn't exist
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@ $(LIBS)

# Create object directory if it doesn't exist
#$(OBJ_DIR):
#	mkdir -p $(OBJ_DIR) $(LIBS)

run:
	KAFKA_BROKERS=172.17.0.1:9095 KAFKA_TOPIC=suricata KAFKA_GROUP_ID=test_client KAFKA_POOL_TIMEOUT_MS=100 ./build/kafka-tool-be-c

# Clean up object files and executable
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

valgrind-check:
	# valgrind -s --leak-check=yes --track-origins=yes ${BIN_DIR}/${TARGET}
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ${BIN_DIR}/${TARGET}
