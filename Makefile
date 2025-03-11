CXX = g++
CXXFLAGS = -I./src/include -I./src/include/SDL2 -I./src/include/maze -D_REENTRANT -std=c++17
LDFLAGS = -L./src/lib -Wl,-rpath=./src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

all: src/bin/maze.exe

src/bin/maze.exe: main/main.o main/maze.o main/renderer.o
	$(CXX) $^ -o $@ $(LDFLAGS)

main/%.o: main/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q main\*.o src\bin\maze.exe 2>nul || exit 0
