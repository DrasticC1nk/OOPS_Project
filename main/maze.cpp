//INCLUDING DEPENDENCIES
#include "../src/include/maze/maze.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

using namespace std;

//METHOD TO CHECK IF THE FILE FORMAT IS VALID OR NOT
bool isValidFileFormat(const string& filename) 
{   
    //WE MAKE SURE THAT THE NAME OF THE FILE IS ATLEAST 5 CHARACTERS LONG(eg. -> 'x.txt') SO THAT WE CAN ACCOMODATE THE '.txt' OR '.dat'
    //EXTENSIONS WHILE PREVENTING OUT-OF-BOUNDS ERRORS WHILE USING 'substr()'
    return (filename.size() > 4 && (filename.substr(filename.size() - 4) == ".txt" || filename.substr(filename.size() - 4) == ".dat"));
}

Maze2D::Maze2D(const string& filename) 
{
    if(!isValidFileFormat(filename)) 
    {
        cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

        //THIS RETURNS THE EXIT STATUS TO THE OS WHILE TERMINATING THE APPLICATION 
        //'$LASTEXITCODE' CAN BE USED TO RETRIEVE THE EXIT STATUS IN POWERSHELL(COMMAND DIFFERS FOR OTHER SHELLS)
        exit(1);
    }

    //EXPLAINED THIS IN THE 'Path1D' CONSTRUCTOR 
    ifstream file("load/" + filename, ios::binary); 

    if(!file) 
    {
        cerr << "Maze file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    //CREATING A STRING 'line' TO STORE EACH LINE OF THE MAZE FILE TO MAKE IT EASY TO PROCESS THE FILE
    string line;

    //CREATING A TEMPORARY 3D VECTOR TO STROE THE MAZE
    vector<vector<int>> tempGrid;

    //WE ARE ABOUT TO READ THE FILE LINE BY LINE AND THEN CHARACTER BY CHARACTER TO POPULATE THE 'grid' VECTOR. THE 'getline()' FUNCTION
    //READS THE FILE LINE BY LINE AND STORING IT AS A STRING IN 'line'. IT READS THE LINE UNTIL IT ENCOUNTERS A NEW LINE CHARACTER '\n'
    //OR UNTIL IT REACHES EOF.  IT KEEPS RETURNING TRUE TILL THERE ARE LINES AND KEEPS THE LOOP RUNNING. IT ITERATES OVER THE LINES 
    //AUTOMATICALLY UNTILL EOF.
    while(getline(file, line)) 
    {
        //CREATING A VECTOR 'row' TO STORE THE ROWS OF THE MAZE AFTER READING
        vector<int> row;

        //USING THIS TO TREAT THE STRING 'line' AS AN INPUT STREA(AS IF IT WAS GIVEN AS AN INPUT VIA KEAYBOARD) TO MAKE IT EASY FOR US
        //TO READ IT CHARACTER BY CHARACTER. 
        istringstream iss(line);
        
        //EVERY CHARACTER IN THE STRING 'line' IS READ AND STORED IN 'cell' AND THEN CONVERTED TO AN INTEGER AND STORED IN 'row'
        char cell;

        //THIS LOOPS READS EVERY CHARACTER IN THE STRING 'line' WHILE AUTOMATICALLY SKIPPING THE WHITESPACES AND NEW LINE CHARACTERS
        while(iss >> cell) 
        {   
            //MAKING SURE THAT THE FILE DOESN'T CONTAIN ANYTHING OTHER THAN '0' AND '1'. WE THROW AN ERROR OTHERWISE
            if(cell != '0' && cell != '1') 
            {
                cerr << "Invalid character '" << cell << "' in maze file. Only '0' and '1' are allowed.\n";

                exit(1);
            }

            //HERE WE CONVERT THE CHAR VALUE WE HAVE IN CELL TO AN INTEGER BACAUSE THEY ARE EASY TO TREAT WHEN WE WANT TO DO ARITHMATIC
            //OBVIOUSLY. ALSO OUR RENDERER REQUIRES INTEGERS SO WE SAVE SOME TIME AND SPACE BY CONVERTING IT HERE. TO CONVERT, WE ARE 
            //SUBSTRACTING THE ASCII VALUE OF CAHRACTER '0' FROM THE ASCII VALUE OF THE CHARACTER IN 'cell' TO GET THE INTEGER VALUE.
            //LET'S SAY 'cell' HOLDS CHAR '1' WHICH HAS AN ASCII VALUE OF 49, AND THE ASCII OF CHAR '0' IS 48 SO 49 - 48 = 1. SO '1' 
            //IS CONVERTED TO 1. THIS CAN BE DONE WITH ANY NUMERIC CHARACTER TO CONVERT IT TO AN INTEGER NOT JUST '1' AND '0'.
            //WE POPULATE THE 'row' VECTOR
            row.push_back(cell - '0');  
        }

        //WE CHECK IF THE ROW IS NOT EMPTY AND THEN PUSH IT TO THE 'tempGrid' VECTOR
        if(!row.empty()) 
        {
            tempGrid.push_back(row);
        }
    }

    //ONCE EVERYTHING IS DONE, WE CLOSE THE FILE
    file.close();

    //CHECKING IF THE FILE IS EMPTY OR IMPROPERLY FORMATTED AND THROWING AN ERROR IF IT IS
    if(tempGrid.empty()) 
    {
        cerr << "Error >>> Maze file is empty or improperly formatted.\n";

        exit(1);
    }

    //WE GET THE SIZE OF THE ROWS AND COLLUMS OF THE MAZE
    rows = tempGrid.size();
    cols = tempGrid[0].size();

    //CHECKING IF THE ROWS ARE NOT OF THE SAME LENGTH AND THROWING AN ERROR IF THEY ARE NOT
    for(const auto& row : tempGrid) 
    {
        if(row.size() != cols) 
        {
            cerr << "Error >>> Inconsistent row lengths in maze file.\n";

            exit(1);
        }
    }

    //CHECKING IF THE MAZE DOESN'T START AT (0,0) AND THROWING AN ERROR IF IT DOESN'T
    if(tempGrid[0][0] == 0) 
    {
        cerr << "Error >>> Maze must start at (0,0) with '1' (open path).\n";

        exit(1);
    }

    //HERE WE MOVE THE DATA FROM 'tempGrid' TO 'grid' USING MOVE SEMANTICS TO AVOID COPYING THE DATA. THE MOVE SEMANTICS WORKS BY 
    //TRANSFERRING THE OWNERSHIP OF THE DATA FROM ONE OBJECT/CONTAINER/VARIABLE TO ANOTHER WITHOUT COPYING IT SAVIN TIME AND MEMORY.
    //IT CONVERTS THE 'tempGrid' TO A R-VALUE(TEMPORARY VALUES/EXPRESSIONS/OBJECTS THAT DO NOT PRESIST AFTER. FOR EXAMPLE LET'S
    //TAKE AN 'int x = 5;' HERE 'x' IS AN NAMED OBJECT OR AN LVALUE. NO LET'S TAKE 'int y = x + 5'. HERE EXPRESSION 'x + 5' IS AN
    //RVALUE REFFERENCE OR A TEMPORARY OBJECT i.e IT WON'T PRESIST AFTER THE EXPRESSION IS EVALUATED. BUT 'y' REMAINS AN LVALUE) AND
    //THEN MOVES IT TO 'grid'. SO NOW 'tempGrid' EXISTS BUT IS EMPTY AND 'grid' IS POPULATED. ALSO, 'grid' IS A MEMBER VARIABLE OF
    //'Maze2D' CLASS SO IT WILL BE PRESISTENT THROUGHOUT THE LIFETIME OF THE OBJECT AND WAS CREATED WITH THE OBJECT
    grid = move(tempGrid);
}

//THIS FUNCTION CHECKS IF THE MOVE IS WITHIN THE BOUNDS OF THE MAZE AND IF IT IS A VALID MOVE
bool Maze2D::isValidMove(int x, int y) const 
{
    return (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 1);
}

//THESE FUNCTIONS ARE USED TO GET THE ROWS AND COLLUMS OF THE MAZE 
int Maze2D::getRows() const { return rows; }
int Maze2D::getCols() const { return cols; }

//CUNSTROCTOR FOR 'Path1D' CLASS
//THIS READS THE FILE WHEN CALLED IN 'main.cpp' WHILE ACCEPTING THE FILE NAME AS A PARAMETER. IT PERFORMS ALL THE VALIDITY CHECKS AND THEN
//READS THE PATH AND POPULATES THE 'moves' VECTOR 
Path1D::Path1D(const string& filename) 
{
    //CASE WHERE THE USER PROVIDES THE PATH FILE WITH AN EXTENSION OTHER THAN '.txt' OR '.dat'
    if(!isValidFileFormat(filename)) 
    {
        cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

        exit(1);
    }

    //OPENS AN INPUT FILE STREAM TO READ THE PATH FILE FROM THE 'load' DIRECTORY. EVEN THOUGH ACCORDING TO US, THE RELATIVE PATH SHOULD
    //BE '../load/' BUT IT DOSNE'T WORK WHEN WE DO THAT SO THIS IS THE WORKAROUND. ASLO WE ARE TREAITNG THIS AS A BINARY FILE BECAUSE
    //RATHER THAN A TEXT FILE TO AVOID DEALING WITH END OF LINE CHARACTERS AND OTHER UNWANTED CHARACTERS IF THERE ARE ANY
    ifstream file("load/" + filename, ios::binary); 

    //IF FOR ANY REASON THE FILE COUDN'T BE OPENED LIKE IF IT DOESN'T EXIST OR THE PATH IS WRONG, WE THROW AN ERROR
    if(!file) 
    {
        cerr << "Path file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    int move;

    //IF EVERYTHING GOES SMOOTHLY, WE READ THE FILE AND POPULATE THE 'moves' VECTOR WITH THE MOVES
    //EVEN THOUGH WE ARE NOT READING THE FILE AS A TEXT FILE BUT WE SURE ARE TREATING IT AS ONE. '>>' THIS READS ALL THE CHARACTERS 
    //ONE BY ONE AND CONVERTS THEM TO INTEGERS AND STORES THEM IN 'move'. THIS AUTOMATICALLY SCIPS THE WHITESPACES
    while(file >> move) 
    {
        //MAKING SURE THAT THE FILE DOESN'T CONTAIN ANYTHING OTHER THAN NUMBERS WE WANT. WE ARE NOT INCLUDING ZERO IN THAT LIST BECAUSE 
        //THAT FEELS REDUNDANT. WE THROW AN ERROR OTHERWISE
        if(move < 1 || move > 4) 
        {
            cerr << "Invalid move '" << move << "' in path file. Allowed values: 1 (up), 2 (right), 3 (down), 4 (left).\n";

            exit(1);
        }

        moves.push_back(move);
    }

    //THROWING AN ERROR IF THE FILE CONTAINS NON-NUMERIC CHARACTERS AND MAKING SURE IT IS NOT THE END OF FILE 
    if(file.fail() && !file.eof()) 
    {
        cerr << "Error >>> Path file contains non-numeric characters.\n";

        exit(1);
    }

    //CLOSING THE FILE
    file.close();
}

//THIS FUNCTION CHECK THE VALIDITY OF THE PROVIDED PATH IN THE PATHFILE AGAINST THE MAZE. THE PROVIDED PATH WAS STORED IN THE 
//'moves' VECTOR FOR CONVENIENCE AND EFFICIENCY. THIS FUNCTION ALSO POPULATES THE 'pathTrace' VECTOR WITH THE POINTS OF THE PATH
bool Path1D::isValidPath(const Maze2D& maze, vector<Point2D>& pathTrace) const //PASSING BY REFFRENCE TO OPTIMIZE MEMORY
{
    //CREATING A 'pOINT2D' OBJECT TO STORE THE CURRENT POSITION OF THE PATH. IT IS DEFINED IN THE 'maze.hpp' FILE AND EXPLAINED THERE
    Point2D pos;

    pathTrace.push_back(pos); //PUSHING THE STARTING POINT OF THE PATH i.e (0,0) WHICH IS THE DEFAULT VALUE OF 'Point2D'

    //ITERATING THROUGH 'moves'
    for(int move : moves) 
    {
        //ASSIGNING OUR CURRENT POSITION TO 'newX' AND 'newY'
        int newX = pos.x, newY = pos.y;
        
        //MAIN LOGIC
        if(move == 1)
        {
            newX--;  
        }   
        else if(move == 2) 
        {
            newY++;
        }
        else if(move == 3) 
        {
            newX++;
        } 
        else if(move == 4) 
        {
            newY--; 
        } 

        //EDGE CASE IN CASE OF AN INVALID INPUT WITHIN THE PATH FILE
        if(!maze.isValidMove(newX, newY)) 
        {
            cerr << "Error >>> Path hits a wall or goes out of bounds at (" << newX << ", " << newY << ").\n";

            return false;
        }
        
        //UPDATING THE CURRENT POSITION
        pos.x = newX;
        pos.y = newY;
        
        //PUSHING THE UPDATED POSITION TO 'pathTrace'
        pathTrace.push_back(pos);
    }

    //CASE WHERE MAZE DOESN'T END AT THE LAST CELL i.e (rows - 1, cols - 1)
    if(pos.x != maze.getRows() - 1 || pos.y != maze.getCols() - 1) 
    {
        cerr << "Error >>> Path does not end at (" << maze.getRows() - 1 << ", " << maze.getCols() - 1 << ").\n";

        return false;
    }

    //IF CONTROL FLOW REACHES HERE, IT MEANS THAT THE PATH IS VALID
    return true;
}
