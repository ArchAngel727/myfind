# Makefile
CXX      := clang++
CXXFLAGS := -std=c++23 -g -O0 -Wall -Wextra -Iheaders

SRC_DIR      := src
TEST_DIR     := tests
OBJ_DIR      := build
TEST_OBJ_DIR := build/tests

TARGET      := myfind
TEST_TARGET := test_runner

# App sources/objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Test sources/objects
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Everything except main.o — tests provide their own main via gtest_main
LIB_OBJS := $(filter-out $(OBJ_DIR)/main.o,$(OBJS))

.PHONY: all build run test clean dirs

all: build

# --- build ---
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# --- run ---
run: build
	./$(TARGET)

# --- test ---
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS)

# --- compile rules ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(OBJ_DIR) $(TEST_OBJ_DIR)

# --- clean ---
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)
