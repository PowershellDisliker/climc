CC=clang

APP_NAME=minesweeper

INC_DIR=./inc/
SRC_DIR=./src/
OBJ_DIR=./obj/
BIN_DIR=./bin/

SRC_NAMES=app game visual common

SRCS=$(SRC_NAMES:%=$(SRC_DIR)%.c)
OBJS=$(SRC_NAMES:%=$(OBJ_DIR)%.o)


$(APP_NAME): $(OBJ_DIR) $(BIN_DIR) $(OBJS)
	$(CC) -I$(INC_DIR) -g $(OBJ_DIR)* -o $(BIN_DIR)$(APP_NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -r $(OBJ_DIR) $(BIN_DIR)
