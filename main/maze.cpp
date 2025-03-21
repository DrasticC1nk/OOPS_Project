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

    ifstream file("load/" + filename, ios::binary); 

    if(!file) 
    {
        cerr << "Maze file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    string line;
    vector<vector<int>> tempGrid;

    while(getline(file, line)) 
    {
        vector<int> row;
        istringstream iss(line);

        char cell;

        while(iss >> cell) 
        {
            if (cell != '0' && cell != '1') 
            {
                cerr << "Invalid character '" << cell << "' in maze file. Only '0' and '1' are allowed.\n";

                exit(1);
            }

            row.push_back(cell - '0');  
        }

        if(!row.empty()) 
        {
            tempGrid.push_back(row);
        }
    }

    file.close();

    if(tempGrid.empty()) 
    {
        cerr << "Error >>> Maze file is empty or improperly formatted.\n";

        exit(1);
    }

    rows = tempGrid.size();
    cols = tempGrid[0].size();

    for(const auto& row : tempGrid) 
    {
        if(row.size() != cols) 
        {
            cerr << "Error >>> Inconsistent row lengths in maze file.\n";

            exit(1);
        }
    }

    if(tempGrid[0][0] == 0) 
    {
        cerr << "Error >>> Maze must start at (0,0) with '1' (open path).\n";

        exit(1);
    }

    grid = move(tempGrid);
}

bool Maze2D::isValidMove(int x, int y) const 
{
    return (x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 1);
}

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

    return true;
}
