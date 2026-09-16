CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

LIB = $(shell find lib -name '*.cpp')
MAIN_SRC = src/main.cpp
ADMIN_SRC = src/util/admin.cpp
COUNT_SRC = src/util/count.cpp

TARGET = target/sistope
ADMIN_TARGET = target/util/admin
COUNT_TARGET = target/util/count


all: compile

mkdir:
	mkdir -p target/util 

compile: $(TARGET) $(ADMIN_TARGET) $(COUNT_TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(LIB) -o $(TARGET)

$(ADMIN_TARGET):
	$(CXX) $(CXXFLAGS) $(ADMIN_SRC) $(LIB) -o $(ADMIN_TARGET)

$(COUNT_TARGET):
	$(CXX) $(CXXFLAGS) $(COUNT_SRC) $(LIB) -o $(COUNT_TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(ADMIN_TARGET) $(COUNT_TARGET)

.PHONY: all run clean
