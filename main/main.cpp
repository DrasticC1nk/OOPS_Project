#include "maze.hpp"
#include "../src/include/maze/renderer.hpp"

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

int SDL_main(int argc, char* argv[]) 
{
    if(argc < 3) 
    {
        cerr << "Use it with executable followed by maze file and path file name with extensions >>> " << argv[0] << " <maze file> <path file>" << endl;

        return 1;
    }

    string mazeFile = argv[1];
    string pathFile = argv[2];

    Maze2D maze(mazeFile);
    Path1D path(pathFile);

    vector<Point2D> pathTrace;

    bool valid = path.isValidPath(maze, pathTrace);

    vector<pair<int, int>> convertedPath;

    for(const auto& point : pathTrace) 
    {
        convertedPath.push_back({point.x, point.y});
    }

    cout << (valid ? "Valid Path!" : "Invalid Path!") << endl;

    Renderer renderer(640, 480);

    renderer.drawMaze(maze);
    
    renderer.animatePath(convertedPath, valid, maze);
    
    return 0; 
}

