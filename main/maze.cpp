#include "../src/include/maze/maze.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

bool isValidFileFormat(const std::string& filename) 
{
    return (filename.size() > 4 && (filename.substr(filename.size() - 4) == ".txt" || filename.substr(filename.size() - 4) == ".dat"));
}

Maze2D::Maze2D(const std::string& filename) 
{
    if(!isValidFileFormat(filename)) 
    {
        std::cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

        exit(1);
    }

    std::ifstream file("load/" + filename, std::ios::binary); 

    if(!file) 
    {
        std::cerr << "Maze file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    std::string line;
    std::vector<std::vector<int>> tempGrid;

    while(std::getline(file, line)) 
    {
        std::vector<int> row;
        std::istringstream iss(line);

        char cell;

        while(iss >> cell) 
        {
            if (cell != '0' && cell != '1') 
            {
                std::cerr << "Invalid character '" << cell << "' in maze file. Only '0' and '1' are allowed.\n";

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
        std::cerr << "Error >>> Maze file is empty or improperly formatted.\n";

        exit(1);
    }

    rows = tempGrid.size();
    cols = tempGrid[0].size();

    for(const auto& row : tempGrid) 
    {
        if(row.size() != cols) 
        {
            std::cerr << "Error >>> Inconsistent row lengths in maze file.\n";

            exit(1);
        }
    }

    if(tempGrid[0][0] == 0) 
    {
        std::cerr << "Error >>> Maze must start at (0,0) with '1' (open path).\n";

        exit(1);
    }

    grid = std::move(tempGrid);
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
            std::cout << (cell ? "1 " : "0 ");
        }

        std::cout << std::endl;
    }
}

Path1D::Path1D(const std::string& filename) 
{
    if(!isValidFileFormat(filename)) 
    {
        std::cerr << "Error >>> Invalid file format for '" << filename << "'. Only .txt or .dat files are allowed.\n";

        exit(1);
    }

    std::ifstream file("load/" + filename, std::ios::binary); 

    if(!file) 
    {
        std::cerr << "Path file couldn't be oppened >>> " << filename << "\n";

        exit(1);
    }

    int move;

    while(file >> move) 
    {
        if(move < 1 || move > 4) 
        {
            std::cerr << "Invalid move '" << move << "' in path file. Allowed values: 1 (up), 2 (right), 3 (down), 4 (left).\n";

            exit(1);
        }

        moves.push_back(move);
    }

    if(file.fail() && !file.eof()) 
    {
        std::cerr << "Error >>> Path file contains non-numeric characters.\n";

        exit(1);
    }

    file.close();
}

bool Path1D::isValidPath(const Maze2D& maze, std::vector<Point2D>& pathTrace) const 
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
            std::cerr << "Error >>> Path hits a wall or goes out of bounds at (" << newX << ", " << newY << ").\n";

            return false;
        }

        pos.x = newX;
        pos.y = newY;

        pathTrace.push_back(pos);
    }

    if(pos.x != maze.getRows() - 1 || pos.y != maze.getCols() - 1) 
    {
        std::cerr << "Error >>> Path does not end at (" << maze.getRows() - 1 << ", " << maze.getCols() - 1 << ").\n";

        return false;
    }

    return true;
}
