# Zmienne z nazwami plików
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Kompilator i opcje kompilacji
CC = g++
CFLAGS = -Wall -I$(INC_DIR)

# Cel główny
main: $(OBJS) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $@ $^

# Reguła do kompilacji plików źródłowych do obiektowych
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Czyszczenie plików tymczasowych
clean:
	rm -f $(OBJ_DIR)/*.o main