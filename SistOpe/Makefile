CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

COMMON_SRC = $(shell find src -name '*.cpp' ! -name 'main.cpp' ! -name 'admin_main.cpp')
TARGET = sistope
ADMIN_TARGET = adminUsuariosPerfiles

all: $(TARGET) $(ADMIN_TARGET)

$(TARGET): main.o $(COMMON_SRC)
	$(CXX) $(CXXFLAGS) main.o $(COMMON_SRC) -o $(TARGET)

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

$(ADMIN_TARGET): admin_main.o $(COMMON_SRC)
	$(CXX) $(CXXFLAGS) admin_main.o $(COMMON_SRC) -o $(ADMIN_TARGET)

admin_main.o: src/admin_main.cpp
	$(CXX) $(CXXFLAGS) -c src/admin_main.cpp -o admin_main.o

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(ADMIN_TARGET) main.o admin_main.o

.PHONY: all run clean
