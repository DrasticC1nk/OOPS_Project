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
//MACOS IN SOME CASES IF WANT TO PASS THE COMMAND LINE ARGUMENTS AND ALSO INITIALIZE SDL. THE SLD3 LIBRARY IMPROVED ON THIS AND DOESN'T
//REQUIRE THIS EXPLICIT 'SDL_main' ENTRY POINT AND MANAGES THIS THROUGH A HEADER. BUT I STAYED WITH SDL2 BECAUSE I WAS FAMILIAR WITH IT
int SDL_main(int argc, char* argv[]) 
{
    //CHECKING IF IF HAVE SUFFICIENT ARGUMENTS BEING PASSED
    if(argc < 3) 
    {
        //'argv[0]' BEIGN THE EXECUTABLE NAME
        //USING 'cerr' WHICH IS CONSOLE ERROR BECAUSE IT IS UNBUFFERED AND PRINTS THE ERROR IMMEDIATELY RATHER THAN WAITING FOR 'flush()'
        //OR A NEW LINE CHARACTER '\n'
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

    //WE MAKE A VECTOR OF PAIRS OF INTEGERS TO STORE THE PATHTRACE WE CALCULATED ERALIER. WE NEED THIS SO THAT I CAN SEND THE PATH TO 
    //OUR RENDERER TO DRAW THE PATH ON THE SCREEN. IT WAS NOT COMPILING WITH 'Point2D' SO I HAD TO CONVERT IT TO PAIRS OF INTEGERS. HENCE
    //THE CONVERSION
    vector<pair<int, int>> convertedPath;
    
    //POPULATING THE 'convertedPath' VECTOR
    for(const auto& point : pathTrace) 
    {
        convertedPath.push_back({point.x, point.y});
    }

    //PRINTING THE RESULT WE CALCULATED EARLIER
    cout << (valid ? "Valid Path!" : "Invalid Path!") << endl;

    Renderer renderer(640, 480);

    renderer.drawMaze(maze);
    
    renderer.animatePath(convertedPath, valid, maze);
    
    return 0; 
}

