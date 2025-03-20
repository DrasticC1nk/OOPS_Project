//INCLUDING DEPENDENCIES
#include "../src/include/maze/maze.hpp"
#include "../src/include/maze/renderer.hpp"

#include <iostream>
#include <SDL2/SDL.h>

//DECLARING THE SCOPE RESOLUTION OPERATOR
using namespace std;

//DRIVER
//'SDL_main' IS BEING USED AS THE ENTRY POINT FOR OUR PROGRAM. WE DON'T KNOW THE EXACT REASON WHY THIS IS BEING USED BUT THAT'S WHAT LETS
//OUR PROGRAM TO BUILD. WE FOUND THIS FIX ON A SUB-REDDIT. BUT WHAT WE FOUND IN THE DOCUMENTAITON IS THAT THIS IS REQUIRED ON WINDOWS AND 
//MACOS IN SOME CASES IF WANT TO PASS THE COMMAND LINE ARGUMENTS AND ALSO INITIALIZE SDL.
int SDL_main(int argc, char* argv[]) 
{
    //CHECKING IF IF HAVE SUFFICIENT ARGUMENTS BEING PASSED
    if(argc < 3) 
    {
        //'argv[0]' BEIGN THE EXECUTABLE NAME
        cerr << "Use it with executable followed by maze file and path file name with extensions >>> " << argv[0] << " <maze file> <path file>" << endl;

        return 1;
    }

    //RETRIVING THE FILE NAMES
    string mazeFile = argv[1]; //'argv[1]' BEING THE MAZE FILE NAME
    string pathFile = argv[2]; //'argv[2]' BEING THE PATH FILE NAME

    //CREATES AN INSTANCE OF 'Maze2D' AND 'Path1D' CLASSES AND PASSES THE FILE NAMES TO THEM
    Maze2D maze(mazeFile);
    Path1D path(pathFile);

    //CREATING A VECTOR OF TYPE 'Point2D' TO STORE 2D COORIDNATES THAT MAKE THE PATH
    vector<Point2D> pathTrace; //'Point2D' STRUCTURE IS DEFINED IN 'maze.hpp' 

    //STORES THE RESULT OF 'isValidPath' THAT RETURNS A BOOL
    bool valid = path.isValidPath(maze, pathTrace); //DETAILED EXPLANATION ABOUT WHAT IT DOES IN 'maze.hpp'

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

