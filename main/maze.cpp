#include "../src/include/maze/maze.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

using namespace std;

bool isValidFileFormat(const string& filename) 
{
    return (filename.size() > 4 && (filename.substr(filename.size() - 4) == ".txt" || filename.substr(filename.size() - 4) == ".dat"));
}

Maze2D::Maze2D(const string& filename) 
{
    if(!isValidFileFormat(filename)) 
    {
        cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

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

void Maze2D::printMaze() const 
{
    for(const auto& row : grid) 
    {
        for(int cell : row) 
        {
            cout << (cell ? "1 " : "0 ");
        }

        cout << endl;
    }
}

Path1D::Path1D(const string& filename) 
{
    if(!isValidFileFormat(filename)) 
    {
        cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

        exit(1);
    }

    ifstream file("load/" + filename, ios::binary); 

    if(!file) 
    {
        cerr << "Path file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    int move;

    while(file >> move) 
    {
        if(move < 1 || move > 4) 
        {
            cerr << "Invalid move '" << move << "' in path file. Allowed values: 1 (up), 2 (right), 3 (down), 4 (left).\n";

            exit(1);
        }

        moves.push_back(move);
    }

    if(file.fail() && !file.eof()) 
    {
        cerr << "Error >>> Path file contains non-numeric characters.\n";

        exit(1);
    }

    file.close();
}

bool Path1D::isValidPath(const Maze2D& maze, vector<Point2D>& pathTrace) const 
{
    Point2D pos(0, 0);

    pathTrace.push_back(pos);

    for(int move : moves) 
    {
        int newX = pos.x, newY = pos.y;

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

        if(!maze.isValidMove(newX, newY)) 
        {
            cerr << "Error >>> Path hits a wall or goes out of bounds at (" << newX << ", " << newY << ").\n";

            return false;
        }

        pos.x = newX;
        pos.y = newY;

        pathTrace.push_back(pos);
    }

    if(pos.x != maze.getRows() - 1 || pos.y != maze.getCols() - 1) 
    {
        cerr << "Error >>> Path does not end at (" << maze.getRows() - 1 << ", " << maze.getCols() - 1 << ").\n";

        return false;
    }

    return true;
}
