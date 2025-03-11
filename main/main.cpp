#include "maze.hpp"
#include "../src/include/maze/renderer.hpp"

#include <iostream>
#include <SDL2/SDL.h>

int SDL_main(int argc, char* argv[]) 
{
    Maze2D maze("maze.txt");
    Path1D path("path.txt");

    std::vector<Point2D> pathTrace;

    bool valid = path.isValidPath(maze, pathTrace);

    std::vector<std::pair<int, int>> convertedPath;

    for(const auto& point : pathTrace) 
    {
        convertedPath.push_back({point.x, point.y});
    }

    std::cout << (valid ? "Valid Path!" : "Invalid Path!") << std::endl;

    Renderer renderer(640, 480);

    renderer.drawMaze(maze);
    renderer.animatePath(convertedPath, valid, maze);
    
    return 0; 
}
