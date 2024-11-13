TARGET = main

CXX = g++ -std=c++20

BUILD_DIR = build
LIB_DIR = lib

SRCS = $(wildcard $(LIB_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)
BUILD_OBJS = $(patsubst $(LIB_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(BUILD_OBJS) $(TARGET).cpp
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp $(LIB_DIR)/%.h
	$(CXX) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*
