# cc and flags
CC = gcc
CXXFLAGS = -g -Wall


# folders
INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./
MAIN_FOLDER = ./main/
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

# all sources, objs, and header files
TARGET1 = server
TARGET2 = client

SRC = $(wildcard $(SRC_FOLDER)*.c)
OBJ = $(patsubst $(SRC_FOLDER)%.c, $(OBJ_FOLDER)%.o, $(SRC))


all: createObj $(OBJ)
	$(CC) $(CXXFLAGS) $(MAIN_FOLDER)$(TARGET1).c $(OBJ) -o $(TARGET1) -I$(INCLUDE_FOLDER)
	$(CC) $(CXXFLAGS) $(MAIN_FOLDER)$(TARGET2).c $(OBJ) -o $(TARGET2) -I$(INCLUDE_FOLDER)

createObj:
	@ [ -d "./obj" ] && echo "'./obj' already exist" || mkdir obj

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.c
	$(CC) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

clean:
	@rm -rf $(OBJ_FOLDER)* 
	@rmdir obj
	@rm client server
