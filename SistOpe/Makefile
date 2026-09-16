CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

COMMON_SRC = src/Auth.cpp src/Profile.cpp src/User.cpp src/config/EnvConfig.cpp
MAIN_SRC = src/main.cpp src/Menu.cpp src/AdminMenu.cpp
ADMIN_SRC = src/admin_main.cpp src/AdminMenu.cpp src/Menu.cpp
COUNT_SRC = src/count_main.cpp src/CountSystem.cpp

TARGET = sistope
ADMIN_TARGET = adminUsuariosPerfiles
COUNT_TARGET = Count

all: $(TARGET) $(ADMIN_TARGET) $(COUNT_TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(COMMON_SRC) -o $(TARGET)

$(ADMIN_TARGET):
	$(CXX) $(CXXFLAGS) $(ADMIN_SRC) $(COMMON_SRC) -o $(ADMIN_TARGET)

$(COUNT_TARGET):
	$(CXX) $(CXXFLAGS) $(COUNT_SRC) -o $(COUNT_TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(ADMIN_TARGET) $(COUNT_TARGET)

.PHONY: all run clean
