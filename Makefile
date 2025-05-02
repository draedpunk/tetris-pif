# Nome do executável final
EXEC = tetris

# Pastas
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Arquivos-fonte
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compilador e flags
CC = gcc
CFLAGS = -I$(INCLUDE_DIR) -Wall -Wextra

# Regra padrão
all: $(EXEC)

# Como criar o executável
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(BUILD_DIR)/$(EXEC)

# Como compilar os .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(EXEC)
