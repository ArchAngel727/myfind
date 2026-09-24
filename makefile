# Makefile
CXX      := clang++
CXXFLAGS := -std=c++23 -g -O0 -Wall -Wextra -Wpedantic -Wsign-conversion -Iheaders

SRC_DIR  := src
OBJ_DIR  := build

TARGET   := myfind

# App sources/objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all build run clean dirs

all: build

# --- build ---
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# --- run ---
run: build
	./$(TARGET)

# --- compile rules ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(OBJ_DIR)

# --- clean ---
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
