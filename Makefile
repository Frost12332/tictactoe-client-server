CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

CLIENT_DIR = client
SERVER_DIR = server
SHARED_DIR = shared

# Sources
SRC_CLIENT = $(wildcard $(SRC_DIR)/$(CLIENT_DIR)/*.c)
SRC_SERVER = $(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.c)
SRC_SHARED = $(wildcard $(SRC_DIR)/$(SHARED_DIR)/*.c)

# Objects
OBJ_CLIENT = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_CLIENT))
OBJ_SERVER = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_SERVER))
OBJ_SHARED = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_SHARED))

# Binaries
CLIENT_BIN = $(BIN_DIR)/$(CLIENT_DIR)/client
SERVER_BIN = $(BIN_DIR)/$(SERVER_DIR)/server

.PHONY: all clean client server dirs

all: dirs client server

dirs:
	mkdir -p $(BUILD_DIR)/$(CLIENT_DIR)
	mkdir -p $(BUILD_DIR)/$(SERVER_DIR)
	mkdir -p $(BUILD_DIR)/$(SHARED_DIR)
	mkdir -p $(BIN_DIR)/$(CLIENT_DIR)
	mkdir -p $(BIN_DIR)/$(SERVER_DIR)

# Compile rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

client: $(CLIENT_BIN)

server: $(SERVER_BIN)

$(CLIENT_BIN): $(OBJ_CLIENT) $(OBJ_SHARED)
	$(CC) $^ -o $@

$(SERVER_BIN): $(OBJ_SERVER) $(OBJ_SHARED)
	$(CC) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
