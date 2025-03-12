#include "maze.hpp"
#include "../src/include/maze/renderer.hpp"

#include <iostream>
#include <SDL2/SDL.h>

int SDL_main(int argc, char* argv[]) 
{
    if(argc < 3) 
    {
        std::cerr << "Use it with executable followed by maze file and path file name with extensions >>> " << argv[0] << " <maze file> <path file>" << std::endl;

        return 1;
    }

    std::string mazeFile = argv[1];
    std::string pathFile = argv[2];

    Maze2D maze(mazeFile);
    Path1D path(pathFile);

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

