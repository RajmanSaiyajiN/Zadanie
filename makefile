#Variables with folder names
MAIN_SRC_DIR = src
MAIN_INC_DIR = include
MAIN_OBJ_DIR = obj

MEDIATOR_SRC_DIR = srcmediator
MEDIATOR_INC_DIR = includemediator
MEDIATOR_OBJ_DIR = objmediator

MAIN_SRCS = $(wildcard $(MAIN_SRC_DIR)/*.cpp)
MAIN_OBJS = $(patsubst $(MAIN_SRC_DIR)/%.cpp,$(MAIN_OBJ_DIR)/%.o,$(MAIN_SRCS))
MAIN_HEADERS = $(wildcard $(MAIN_INC_DIR)/*.h)

MEDIATOR_SRCS = $(wildcard $(MEDIATOR_SRC_DIR)/*.cpp)
MEDIATOR_OBJS = $(patsubst $(MEDIATOR_SRC_DIR)/%.cpp,$(MEDIATOR_OBJ_DIR)/%.o,$(MEDIATOR_SRCS))
MEDIATOR_HEADERS = $(wildcard $(MEDIATOR_INC_DIR)/*.h)

#Compilation options
CC = g++
CFLAGS = -Wall -I$(MAIN_INC_DIR) -I$(MEDIATOR_INC_DIR) -g
LDFLAGS = -pthread 

main: $(MAIN_OBJS) $(MAIN_OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

mediator: $(MEDIATOR_OBJS) $(MEDIATOR_OBJ_DIR)/mediator.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

#.cpp to .o for main
$(MAIN_OBJ_DIR)/%.o: $(MAIN_SRC_DIR)/%.cpp $(MAIN_INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ_DIR)/main.o: main.cpp $(MAIN_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

#.cpp to .o for mediator
$(MEDIATOR_OBJ_DIR)/%.o: $(MEDIATOR_SRC_DIR)/%.cpp $(MEDIATOR_INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MEDIATOR_OBJ_DIR)/mediator.o: mediator.cpp $(MEDIATOR_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(MAIN_OBJ_DIR)/*.o main
	rm -f $(MEDIATOR_OBJ_DIR)/*.o mediator
