# Target executable
TARGET = minesweeper.exe

# Source files
SRCS = *.cpp

# Paths for SFML library and includes
INCLUDE_DIR = "C:\\Vinh\\C\\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\\SFML-2.5.1\\include"
LIB_DIR = "C:\\Vinh\\C\\SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit\\SFML-2.5.1\\lib"

# Compiler and flags
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET)

.PHONY: all run clean
