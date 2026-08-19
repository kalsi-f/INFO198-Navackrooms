CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
 
SRC = $(shell find src -name '*.cpp')
TARGET = sistope
 
all: $(TARGET)
 
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
 
run: $(TARGET)
	./$(TARGET)
 
clean:
	rm -f $(TARGET)
 
.PHONY: all run clean
