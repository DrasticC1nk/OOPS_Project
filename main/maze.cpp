#include "../src/include/maze/maze.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>  

Maze2D::Maze2D(const std::string& filename) 
{
    std::ifstream file("load/" + filename);  

    if(!file) 
    {
        std::cerr << "Error opening maze file: " << filename << "\n";

        exit(1);
    }

    file >> rows >> cols;

    grid.resize(rows, std::vector<int>(cols));

    for(size_t i = 0; i < rows; ++i)
    {
        for(size_t j = 0; j < cols; ++j)
        {
            file >> grid[i][j];
        }
    }

    file.close();
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
    std::ifstream file("load/" + filename); 

    if(!file) 
    {
        std::cerr << "Error opening path file: " << filename << "\n";

        exit(1);
    }
    
    int move;

    while(file >> move)
    {
        moves.push_back(move);
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
            return false; 
        }

        pos.x = newX;
        pos.y = newY;
        
        pathTrace.push_back(pos);
    }

    return (pos.x == maze.getRows() - 1 && pos.y == maze.getCols() - 1);
}
